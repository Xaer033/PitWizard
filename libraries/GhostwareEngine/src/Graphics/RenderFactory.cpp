
#include "RenderFactory.h"
#include "Camera.h"
#include "Mesh.h"

#include <fstream>
#include <string>

#include <GG/Core/Vector.h>
#include <GG/Core/Rect.h>
#include <GG/Core/Log.h>
#include <GG/Core/MathDebug.h>
#include <GG/Graphics/IVertexBuffer.h>
#include <GG/Resources/ResourceManager.h>

namespace GG
{
	std::string loadFile(const std::string & filename)
	{
		std::ifstream in(filename, std::ios::in | std::ios::binary);

		std::string contents;

		if(in)
		{
			in.seekg(0, std::ios::end);
			contents.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
		}

		return contents;
	}

	//
	// Initialize the shader and program object
	//
	bool initShader(GG::Shader *& shader, const std::string & shaderName)
	{

		std::string directory = "./resources/shaders/";

		std::string vShaderStr	= loadFile((directory + shaderName + std::string(".vs")));
		std::string fShaderStr	= loadFile((directory + shaderName + std::string(".ps")));

		if(shader != nullptr)
			delete shader;

		shader = new Shader();
		shader->bindAttribute(VertexTags::Position,		"inVert");
		shader->bindAttribute(VertexTags::Uv0,			"inUV0");
		shader->bindAttribute(VertexTags::Normal,		"inNormal");
		shader->bindAttribute(VertexTags::Tangent,		"inTangent");
		shader->bindAttribute(VertexTags::Bitangent,	"inBitangent");

		//shader->bindAttribute(VertexTags::Color,	"inColor");

		shader->compile(vShaderStr, fShaderStr);

		return true;
	}


	RenderFactory::RenderFactory() : _tempShader(nullptr)
	{
		ResourceManager * rm = ResourceManager::GetInstance();
		_tempTexture	= rm->getResource<Texture2D>(STRING_ID("brick_albedo"));
		_roughnessTex	= rm->getResource<Texture2D>(STRING_ID("brick_spec"));
		_normalTex		= rm->getResource<Texture2D>(STRING_ID("brick_norm"));


		loadTempShader();
	}

	RenderFactory::~RenderFactory()
	{

	}

	void RenderFactory::loadTempShader()
	{
		if(!initShader(_tempShader, "unlit"))
		{
			TRACE_ERROR("Could not load temp shader!");
			return;
		}
	}

	void RenderFactory::addCommand(Material * mat, Model * geo, const Matrix4 & worldMatrix )
	{
		_renderCommand3DList.push_back( 
			RenderCommand3D { mat, geo, worldMatrix } 
		);
	}

	void RenderFactory::clearAllCommands()
	{
		_renderCommand3DList.clear();
	}

	void RenderFactory::renderAll( const Camera * camera )
	{
		if( camera == nullptr )
		{
			TRACE_ERROR( "Camera Node does not contain Camera!" );
			return;
		}

		_setViewport( camera->getViewport() );
		_clearRenderBuffer( camera->getClearColor(), camera->getClearMode() );

		RenderState::GetInstance()->setViewMatrix(camera->getViewMatrix());
		RenderState::GetInstance()->setProjectionMatrix(camera->getProjectionMatrix());

		_render3DList(camera);
	}

// -------------------------------------------------------------------------------

	void RenderFactory::_render3DList(const Camera * camera)
	{
		RenderState * rs = RenderState::GetInstance();
		Material *	currentMat		= nullptr;

		Matrix4 viewProjection = rs->getProjectionMatrix() * rs->getViewMatrix();

		for( auto it = _renderCommand3DList.begin(); 
			 it != _renderCommand3DList.end(); 
			 ++it 
		)
		{
			const RenderCommand3D * command = &( *it );
			if( command == nullptr )
			{
				TRACE_ERROR( "Render Command is null!" );
				continue;
			}

			rs->setModelMatrix(command->modelMatrix);

			Material * mat = command->material;
			if(mat != nullptr && mat != currentMat)
			{
				_setMaterial(mat);
				currentMat = mat;
			}
		
			_tempShader->setParameter("inMVP", viewProjection * command->modelMatrix);
			_tempShader->setParameter("inViewMat", rs->getViewMatrix());
			_tempShader->setParameter("inModelMat", command->modelMatrix);
			_tempShader->setParameter("inEyePos", camera->getEntity()->getSceneNode()->getWorldPosition());
			

			if( command->geometry != nullptr )
			{ 
				command->geometry->bind();
				command->geometry->render(DrawMode::TRIANGLE_LIST);
			}
		}
	}

	void RenderFactory::_setViewport( const Vector4 & viewport ) const
	{
		RenderState::GetInstance()->setViewport(viewport);
	}

	void RenderFactory:: _clearRenderBuffer( 
		const Vector4 & clearColor, 
		uint clearMode ) const
	{
		if( clearMode != RenderState::ClearMode::CM_NONE )
		{
			RenderState::GetInstance()->setClearColor(clearColor);
			RenderState::GetInstance()->clearRenderBuffer(clearMode);
		}
	}

	void RenderFactory::_opaqueSort( const Vector3 & cameraEye )
	{
		auto opaqueSort = 
			[ &cameraEye ]( RenderCommand3D a, RenderCommand3D b )
		{
			if( a.material == nullptr || b.material == nullptr )
				return false;

			/*int16 aAlphaMode = ( int16 )a.material->GetAlphaMode();
			int16 bAlphaMode = ( int16 )b.material->GetAlphaMode();

			bool alphaCompare		=	aAlphaMode < bAlphaMode;
			bool userFlagCompare	=	aAlphaMode == bAlphaMode  && 
										a.material->GetUserFlags() < 
										b.material->GetUserFlags();

			return	alphaCompare || userFlagCompare;*/
			return true;
		};

		std::sort( 
			_renderCommand3DList.begin(), 
			_renderCommand3DList.end(), 
			opaqueSort 
		);
	}

	void RenderFactory::_transparentSort( const Vector3 & cameraEye )
	{
		auto transparentSort = 
			[ &cameraEye ]( RenderCommand3D a, RenderCommand3D b )
		{
			if( a.material == nullptr || b.material == nullptr )
				return false;

		/*	int16 aAlphaMode = ( int16 )a.material->GetAlphaMode();
			int16 bAlphaMode = ( int16 )b.material->GetAlphaMode();
			
			bool alphaCompare		=	aAlphaMode < bAlphaMode;
			bool userFlagCompare	=	aAlphaMode == bAlphaMode  &&
				a.material->GetUserFlags() <
				b.material->GetUserFlags();

			return	alphaCompare || userFlagCompare;*/
			return true;
		};

		std::sort( 
			_renderCommand3DList.begin(), 
			_renderCommand3DList.end(), 
			transparentSort 
		);
	}
	
	void RenderFactory::_setMaterial(Material * material)
	{
		if(material == nullptr)
		{
			TRACE_ERROR("Material is null!");
			return;
		}

		RenderState * rs = RenderState::GetInstance();
		rs->setBlendmode(material->renderStateBlock.blendMode);
		rs->setCullMode(material->renderStateBlock.cullMode);
		rs->setDepthTesting(material->renderStateBlock.isDepthTesting);
		rs->setWindingMode(material->renderStateBlock.windingMode);
		rs->setDepthRange(
			material->renderStateBlock.depthRange.x,
			material->renderStateBlock.depthRange.y
		);

		_tempShader->bind();
		static float t = 0.0f;
		t += 0.016f;
		_tempShader->setParameter("time", t);

		_tempShader->setParameter("tintColor", Vector4(1.0f));
		_tempShader->setParameter("albedoMap", 0, _tempTexture.get()->getId());
		_tempShader->setParameter("roughnessMap", 1, _roughnessTex.get()->getId());
		_tempShader->setParameter("normalMap", 2, _normalTex.get()->getId());

		_tempShader->setParameter("lightList[0].position", Vector3(1.0f, 3.0f, 5.0f));
		_tempShader->setParameter("lightList[0].color", Vector4(.8, 0.749, 0.681, 6.0f));
		_tempShader->setParameter("lightList[0].radius", 20.0f);
		_tempShader->setParameter("lightList[0].type", 1);

		_tempShader->setParameter("lightList[1].position", Vector3((sinf(t) * 10.0f) - 5.0f, 4.0f, -8.0f));
		_tempShader->setParameter("lightList[1].color", Vector4(.3, 0.849, 0.481, 3.5f));
		_tempShader->setParameter("lightList[1].radius", 20.0f);
		_tempShader->setParameter("lightList[1].type", 1);

		_tempShader->setParameter("lightList[2].position", Vector3(10.0f, 3.0f, (cosf(t* 3.0f) * 10.0f) - 5.0f));
		_tempShader->setParameter("lightList[2].color", Vector4(.8f, 0.849, 0.21, 1.5f));
		_tempShader->setParameter("lightList[2].radius", 7.0f);
		_tempShader->setParameter("lightList[2].type", 1);

		//material->bindMaterialToShader(_tempShader);
	}
}