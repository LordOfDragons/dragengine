#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(0) uniform vec4 pPosTransform; // scaleX, scaleY, offsetX, offsetY
UNIFORM_BIND(1) uniform vec4 pTCTransform; // scaleU, scaleV, offsetU, offsetV

layout(location=0) in vec2 inPosition;

#include "shared/interface/2d_vertex.glsl"

void main( void ){
	vertexShaderDefaultOutputs();
	gl_Position = vec4( vec3( inPosition * pPosTransform.xy + pPosTransform.zw, 0.0 ), 1.0 );
	vTexCoord = inPosition * pTCTransform.xy + pTCTransform.zw;
}
