precision highp float;
precision highp int;

#include "shared/ubo_defines.glsl"

#ifdef WITH_SHADOWMAP
	#include "shared/defren/ubo_render_parameters.glsl"
	
#else
	UBOLAYOUT uniform RenderParameters{
		mat4 pMatrixVP[ 6 ];
		mat4x3 pMatrixV[ 6 ];
		vec4 pTransformZ[ 6 ];
		vec2 pZToDepth;
		vec4 pClipPlane[ 2 ]; // normal.xyz, distance
	};
#endif

#ifdef PERSPECTIVE_TO_LINEAR
in float vDepth;
#endif
#ifdef DEPTH_DISTANCE
in vec3 vPosition;
#endif
#ifdef USE_CLIP_PLANE
in vec3 vClipCoord;
#endif

#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED || defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

void main( void ){
	#ifdef USE_CLIP_PLANE
	if( dot( vClipCoord, pClipPlane[ vLayer ].xyz ) <= pClipPlane[ vLayer ].w ) discard;
	#endif
	#ifdef PERSPECTIVE_TO_LINEAR
	gl_FragDepth = vDepth;
	#endif
	#ifdef DEPTH_DISTANCE
	gl_FragDepth = length( vPosition ) * pZToDepth.x + pZToDepth.y;
	#endif
}
