#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(0) uniform vec4 pQuadParams; // scaleX, scaleY, offsetX, offsetY

in vec2 inPosition;

#include "shared/interface/2d_vertex.glsl"

void main( void ){
	vertexShaderDefaultOutputs();
	gl_Position = vec4( inPosition, 0, 1 );
	vScreenCoord = inPosition;
	vTexCoord = inPosition * pQuadParams.xy + pQuadParams.zw;
}
