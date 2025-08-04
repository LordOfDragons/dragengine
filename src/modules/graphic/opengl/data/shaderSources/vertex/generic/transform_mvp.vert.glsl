#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(0) uniform mat4x4 pMatrixMVP; // matrix model-view-projection

layout(location=0) in vec3 inPosition;

#include "shared/interface/2d_vertex.glsl"

void main( void ){
	vertexShaderDefaultOutputs();
	gl_Position = pMatrixMVP * vec4( inPosition, 1 );
}
