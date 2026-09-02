#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

layout(binding=0) uniform HIGHP sampler2D texDepth;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/interface/2d/fragment.glsl"

void main(void){
	float depth = texelFetch(texDepth, ivec2(gl_FragCoord.xy), 0).r;
	float z = pVRDepthTransform.x / (depth + max(pVRDepthTransform.y, pVRDepthClamp));
	gl_FragDepth = (z * pVRDepthTransform.z + pVRDepthTransform.w) / z;
}
