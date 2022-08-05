#ifdef EXT_ARB_SHADER_VIEWPORT_LAYER_ARRAY
	#extension GL_ARB_shader_viewport_layer_array : require
#endif
#ifdef EXT_ARB_SHADER_DRAW_PARAMETERS
	#extension GL_ARB_shader_draw_parameters : require
#endif

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

#ifndef NO_TEXCOORD
	#ifdef FULLSCREENQUAD
		#include "v130/shared/ubo_defines.glsl"
		#include "v130/shared/defren/ubo_render_parameters.glsl"
	#endif
#endif

in vec2 inPosition;

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
	#define inLayer gl_DrawID
	out int vLayer;
#else
	const int inLayer = 0;
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
			vTexCoord = fsquadScreenCoordToTexCoord( inPosition );
			vScreenCoord = inPosition;
		#else
			vTexCoord = inPosition * pTCTransform.xy + pTCTransform.zw;
		#endif
	#endif
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
