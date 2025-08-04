#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(0) uniform int pWidth;
UNIFORM_BIND(1) uniform vec4 pSrcTCTransform;
UNIFORM_BIND(2) uniform vec4 pDestTCTransform;

layout(location=0) in vec2 inPosition;

#include "shared/interface/2d_vertex.glsl"
VARYING_BIND(3) out vec2 vDestTC;

const vec2 tcTransform = vec2( 0.5 );

void main( void ){
	vertexShaderDefaultOutputs();
	gl_Position = vec4( inPosition, 0.0, 1.0 );
	vTexCoord = inPosition * pSrcTCTransform.xy + pSrcTCTransform.zw;
	vDestTC = vec2( ivec2( gl_InstanceID % pWidth, gl_InstanceID / pWidth ) ) * pDestTCTransform.xy + pDestTCTransform.zw;
}
