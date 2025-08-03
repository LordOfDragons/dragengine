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

in float vDepth;
in vec3 vPosition;
in vec3 vClipCoord;
flat in int vLayer;
flat in int vSPBIndex;

#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED || defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	#define vInLayer vLayer
#else
	const int vInLayer = 0;
#endif

void main(void){
	#ifdef USE_CLIP_PLANE
		if(dot(vClipCoord, pClipPlane[vInLayer].xyz) <= pClipPlane[vInLayer].w){
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
