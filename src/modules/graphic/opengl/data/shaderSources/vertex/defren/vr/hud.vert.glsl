#include "shared/preamble.glsl"

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

layout(location=0) in vec2 inPosition;
layout(location=1) in vec2 inTexCoord;
layout(location=2) in int inEye; // 0=left, 1=right

VARYING_BIND(0) out mediump vec2 vTexCoord;

void main( void ){
	// map inPosition to angle: [-1,1] -> [-fov/2, fov/2]
	vec2 angle = inPosition * (pVRHudFov / 2.0);
	
	// flat projection at z=1 (curvature=0)
	vec3 posFlat = vec3(tan(angle.x), tan(angle.y), 1);
	
	// sphere projection (curvature=1)
	vec3 posCurved = vec3(sin(angle.x), sin(angle.y), cos(angle.x) * cos(angle.y));
	
	// final position
	gl_Position = pMatrixP[inEye] * vec4(mix(posFlat, posCurved, pVRHudCurvature), 1);
	vTexCoord = inTexCoord;
}
