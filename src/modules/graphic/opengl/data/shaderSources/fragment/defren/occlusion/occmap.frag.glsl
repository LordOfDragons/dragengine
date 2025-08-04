#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"

#ifdef WITH_SHADOWMAP
	#include "shared/defren/ubo_render_parameters.glsl"
	
#else
	UBOLAYOUT_BIND(0) uniform RenderParameters{
		mat4 pMatrixVP[6];
		mat4x3 pMatrixV[6];
		vec4 pTransformZ[6];
		vec2 pZToDepth;
		vec4 pClipPlane[2]; // normal.xyz, distance
	};
#endif

VARYING_BIND(0) in float vDepth;
VARYING_BIND(1) in vec3 vPosition;
VARYING_BIND(2) in vec3 vClipCoord;
VARYING_BIND(3) flat in int vLayer;
VARYING_BIND(4) flat in int vSPBIndex;

void main(void){
	#ifdef USE_CLIP_PLANE
		if(dot(vClipCoord, pClipPlane[vLayer].xyz) <= pClipPlane[vLayer].w){
			discard;
		}
	#endif
	#ifdef PERSPECTIVE_TO_LINEAR
		gl_FragDepth = vDepth;
	#endif
	#ifdef DEPTH_DISTANCE
		gl_FragDepth = length(vPosition) * pZToDepth.x + pZToDepth.y;
	#endif
}
