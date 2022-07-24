precision mediump float;
precision mediump int;

#ifndef GEOMETRY_SHADER
	#ifdef GS_RENDER_STEREO
		#define GEOMETRY_SHADER 1
	#endif
#endif

#ifndef NO_POSTRANSFORM
	uniform vec4 pPosTransform; // scaleX, scaleY, offsetX, offsetY
#endif
#ifndef NO_TCTRANSFORM
	uniform vec4 pTCTransform; // scaleX, scaleY, offsetX, offsetY
#endif

#if ! defined NO_TEXCOORD && defined FULLSCREENQUAD
	#include "v130/shared/ubo_defines.glsl"
	#include "v130/shared/defren/skin/ubo_render_parameters.glsl"
#endif

in vec2 inPosition;

#ifndef NO_TEXCOORD
	#ifdef GEOMETRY_SHADER
		out vec2 vGSTexCoord;
		#define vTexCoord vGSTexCoord
	#else
		out vec2 vTexCoord;
	#endif
#endif

void main( void ){
	#ifdef NO_POSTRANSFORM
		gl_Position = vec4( inPosition, 0, 1 );
	#else
		gl_Position = vec4( vec3( inPosition * pPosTransform.xy + pPosTransform.zw, 0 ), 1 );
	#endif
	
	#ifndef NO_TEXCOORD
		#ifdef NO_TCTRANSFORM
			vTexCoord = inPosition;
		#elif defined FULLSCREENQUAD
			vTexCoord = inPosition * pFullScreenQuad.xy + pFullScreenQuad.zw;
		#else
			vTexCoord = inPosition * pTCTransform.xy + pTCTransform.zw;
		#endif
	#endif
}
