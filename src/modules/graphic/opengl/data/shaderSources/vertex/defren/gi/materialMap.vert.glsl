#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(0) uniform vec4 pQuadParams; // scaleX, scaleY, offsetX, offsetY

layout(location=0) in vec2 inPosition;

#include "shared/interface/2d_vertex.glsl"

void main( void ){
	vertexShaderDefaultOutputs();
	gl_Position = vec4( inPosition * pQuadParams.xy + pQuadParams.zw, 0, 1 );
	vTexCoord = inPosition * vec2( 0.5 ) + vec2( 0.5 );
}
