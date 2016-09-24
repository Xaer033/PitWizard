attribute highp vec4    	inVert;    // Vertices    
attribute mediump vec3   	inNorm; // Normals
attribute mediump vec2   	inUV0; // UV Stream 0
attribute vec4 				inColor;

uniform mediump mat4    	inPMVMat;

varying vec2	uv;
varying vec4	vertColor;

void main() 
{ 
	gl_Position =	inPMVMat * inVert; 

	uv          = inUV0; 
    vertColor   = inColor;
} 
