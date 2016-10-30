
attribute highp vec4    	inVert;    // Vertices    
attribute mediump vec2   	inUV0; // UV Stream 0
attribute mediump vec3		inNormal;
attribute mediump vec3		inTangent;
attribute mediump vec3		inBitangent;

//attribute vec4 				inColor;

uniform mediump mat4    	inMVP;
uniform mediump	mat4		inModelMat;
uniform mediump mat4		inViewMat;

varying vec2	uv;
varying vec3	normal;
varying vec4	worldPos;
varying mat3	tbnMat;
varying vec3	tangent;
varying vec3	bitangent;
varying vec4	viewSpace;

void main() 
{ 
	gl_Position =	inMVP * inVert;

	uv          = inUV0; 
	worldPos	= inModelMat * inVert;
	viewSpace	= inViewMat * inModelMat * inVert;

	normal		= (inModelMat * vec4(inNormal, 0)).xyz;
	tangent		= (inModelMat * vec4(inTangent, 0)).xyz;
	bitangent	= (inModelMat * vec4(inBitangent, 0)).xyz;

	tbnMat		= mat3(tangent, bitangent, normal);
} 
