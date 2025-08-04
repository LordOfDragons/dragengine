#include "shared/preamble.glsl"

precision mediump float;
precision mediump int;

#ifndef NO_POSTRANSFORM
	UNIFORM_BIND(0) uniform vec4 pPosTransform; // scaleX, scaleY, offsetX, offsetY
#endif
#ifndef NO_TCTRANSFORM
	UNIFORM_BIND(1) uniform vec4 pTCTransform; // scaleX, scaleY, offsetX, offsetY
#endif
#if defined FULLSCREENQUAD && FULLSCREENQUAD_SCTRANSFORM
	UNIFORM_BIND(2) uniform vec4 pSCTransform; // scaleX, scaleY, offsetX, offsetY
#endif

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

layout(location=0) in vec2 inPosition;
layout(location=1) in int inLayer;

#include "shared/interface/2d_vertex.glsl"

void main( void ){
	vertexShaderDefaultOutputs();
	
	vec2 position = inPosition;
	
	#ifndef NO_POSTRANSFORM
		position = position * pPosTransform.xy + pPosTransform.zw;
	#endif
	
	gl_Position = vec4( position, 0, 1 );
	
	if(!NoTexCoord){
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
	}
	
	if(VSRenderStereo){
		vLayer = inLayer;
		#ifdef SUPPORTS_VSLAYER
		gl_Layer = vLayer;
		#endif
	}
}
