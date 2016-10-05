
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
		_tempTexture = new Texture();
		_tempTexture->loadFromFile("./resources/environments/forest_road/textures/brick_albedo.png");
		_tempTexture->uploadToGPU();
		
		_roughnessTex = new Texture();
		_roughnessTex->loadFromFile("./resources/environments/forest_road/textures/brick_spec.png");
		_roughnessTex->uploadToGPU();

		_normalTex = new Texture();
		_normalTex->loadFromFile("./resources/environments/forest_road/textures/brick_norm.png");
		_normalTex->uploadToGPU();

		loadTempShader();
	}

	RenderFactory::~RenderFactory()
	{
		if(_tempTexture != nullptr)
			delete _tempTexture;

		if(_roughnessTex != nullptr)
			delete _roughnessTex;

		if(_normalTex != nullptr)
			delete _normalTex;
	}

	void RenderFactory::loadTempShader()
	{
		if(!initShader(_tempShader, "unlit"))
		{
			TRACE_ERROR("Could not load temp shader!");
			return;
		}
	}

	void RenderFactory::addCommand(BaseMaterial * mat, Model * geo, const Matrix4 & worldMatrix )
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

		RenderState::getInstance()->setViewMatrix(camera->getViewMatrix());
		RenderState::getInstance()->setProjectionMatrix(camera->getProjectionMatrix());

		_render3DList(camera);
	}

// -------------------------------------------------------------------------------

	void RenderFactory::_render3DList(const Camera * camera)
	{
		RenderState * rs = RenderState::getInstance();
		BaseMaterial *	currentMat		= nullptr;

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

			BaseMaterial * mat = command->material;
			if(mat != nullptr && mat != currentMat)
			{
				_setMaterial(mat);
				currentMat = mat;
			}

			_tempShader->setParameter("inMVP", viewProjection * command->modelMatrix);
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
		RenderState::getInstance()->setViewport(viewport);
	}

	void RenderFactory:: _clearRenderBuffer( 
		const Vector4 & clearColor, 
		uint clearMode ) const
	{
		if( clearMode != RenderState::ClearMode::CM_NONE )
		{
			RenderState::getInstance()->setClearColor(clearColor);
			RenderState::getInstance()->clearRenderBuffer(clearMode);
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
	
	void RenderFactory::_setMaterial(BaseMaterial * material)
	{
		if(material == nullptr)
		{
			TRACE_ERROR("Material is null!");
			return;
		}

		RenderState * rs = RenderState::getInstance();
		rs->setBlendmode(material->materialBlock.blendMode);
		rs->setCullMode(material->materialBlock.cullMode);
		rs->setDepthTesting(material->materialBlock.isDepthTesting);
		rs->setDepthRange(
			material->materialBlock.depthRange.x,
			material->materialBlock.depthRange.y
		);

		_tempShader->bind();

		_tempShader->setParameter("tintColor", Vector4(1.0f));
		_tempShader->setParameter("albedoMap", 0, _tempTexture->getId());
		_tempShader->setParameter("roughnessMap", 1, _roughnessTex->getId());
		_tempShader->setParameter("normalMap", 2, _normalTex->getId());

		//material->bindMaterialToShader(_tempShader);
	}
}