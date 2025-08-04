#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform vec4 pSCTransform; // scaleX, scaleY, offsetX, offsetY
uniform vec4 pTCTransform; // scaleX, scaleY, offsetX, offsetY

layout(location=0) in vec2 inPosition;

#include "shared/interface/2d_vertex.glsl"

void main( void ){
	vertexShaderDefaultOutputs();
	gl_Position = vec4( inPosition, 0.0, 1.0 );
	vScreenCoord = inPosition * pSCTransform.xy + pSCTransform.zw;
	vTexCoord = inPosition * pTCTransform.xy + pTCTransform.zw;
}
