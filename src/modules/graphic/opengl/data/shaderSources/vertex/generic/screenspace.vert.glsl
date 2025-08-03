#include "shared/preamble.glsl"

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
#if defined FULLSCREENQUAD && FULLSCREENQUAD_SCTRANSFORM
	uniform vec4 pSCTransform; // scaleX, scaleY, offsetX, offsetY
#endif

#ifndef NO_TEXCOORD
	#ifdef FULLSCREENQUAD
		#include "shared/ubo_defines.glsl"
		#include "shared/defren/ubo_render_parameters.glsl"
	#endif
#endif

layout(location=0) in vec2 inPosition;

#ifdef VS_RENDER_STEREO
	layout(location=1) in int inLayer;
#else
	const int inLayer = 0;
#endif

#ifndef NO_TEXCOORD
	#ifdef GEOMETRY_SHADER
		out vec2 vGSTexCoord;
		#define vTexCoord vGSTexCoord
		
		#ifdef FULLSCREENQUAD
			out vec2 vGSScreenCoord;
			#define vScreenCoord vGSScreenCoord
		#endif
		
	#else
		out vec2 vTexCoord;
		
		#ifdef FULLSCREENQUAD
			out vec2 vScreenCoord;
		#endif
	#endif
#endif

#ifdef VS_RENDER_STEREO
	flat out int vLayer;
#endif

void main( void ){
	vec2 position = inPosition;
	
	#ifndef NO_POSTRANSFORM
		position = position * pPosTransform.xy + pPosTransform.zw;
	#endif
	
	gl_Position = vec4( position, 0, 1 );
	
	#ifndef NO_TEXCOORD
		vec2 texCoord;
		
		#ifdef NO_TCTRANSFORM
			texCoord = inPosition;
			
		#elif defined FULLSCREENQUAD
			texCoord = fsquadScreenCoordToTexCoord( inPosition );
			
			#ifdef FULLSCREENQUAD_TCTRANSFORM
				texCoord = texCoord * pTCTransform.xy + pTCTransform.zw;
			#endif
			
			#ifdef TEXCOORD_FLIP_Y
				texCoord.y = 1 - texCoord.y;
			#endif
			
			vec2 screenCoord = inPosition;
			
			#ifdef FULLSCREENQUAD_SCTRANSFORM
				screenCoord = screenCoord * pSCTransform.xy + pSCTransform.zw;
			#endif
			
			vScreenCoord = screenCoord;
			
		#else
			texCoord = inPosition * pTCTransform.xy + pTCTransform.zw;
		#endif
		
		vTexCoord = texCoord;
	#endif
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
