#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform int pWidth;
uniform vec4 pSrcTCTransform;
uniform vec4 pDestTCTransform;

layout(location=0) in vec2 inPosition;

VARYING_BIND(0) out vec2 vTexCoord;
out vec2 vDestTC;

const vec2 tcTransform = vec2( 0.5 );

void main( void ){
	gl_Position = vec4( inPosition, 0.0, 1.0 );
	vTexCoord = inPosition * pSrcTCTransform.xy + pSrcTCTransform.zw;
	vDestTC = vec2( ivec2( gl_InstanceID % pWidth, gl_InstanceID / pWidth ) ) * pDestTCTransform.xy + pDestTCTransform.zw;
}
