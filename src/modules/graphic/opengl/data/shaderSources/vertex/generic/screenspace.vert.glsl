#include "shared/preamble.glsl"

precision mediump float;
precision mediump int;

// !NoPosTransform
UNIFORM_BIND(0) uniform vec4 pPosTransform; // scaleX, scaleY, offsetX, offsetY

// !NoTCTransform
UNIFORM_BIND(1) uniform vec4 pTCTransform; // scaleX, scaleY, offsetX, offsetY

// FullScreenQuad && FullScreenQuadSCTransform
UNIFORM_BIND(2) uniform vec4 pSCTransform; // scaleX, scaleY, offsetX, offsetY

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

layout(location=0) in vec2 inPosition;
layout(location=1) in int inLayer;

#include "shared/interface/2d/vertex.glsl"

void main( void ){
	vertexShaderDefaultOutputs();
	
	vec2 position = inPosition;
	
	if(!NoPosTransform){
		position = position * pPosTransform.xy + pPosTransform.zw;
	}
	
	gl_Position = vec4( position, 0, 1 );
	
	if(!NoTexCoord){
		vec2 texCoord;
		
		if(NoTCTransform){
			texCoord = inPosition;
			
		}else if(FullScreenQuad){
			texCoord = fsquadScreenCoordToTexCoord( inPosition );
			
			if(FullScreenQuadTCTransform){
				texCoord = texCoord * pTCTransform.xy + pTCTransform.zw;
			}
			
			if(TexCoordFlipY){
				texCoord.y = 1 - texCoord.y;
			}
			
			vec2 screenCoord = inPosition;
			
			if(FullScreenQuadSCTransform){
				screenCoord = screenCoord * pSCTransform.xy + pSCTransform.zw;
			}
			
			vScreenCoord = screenCoord;
			
		}else{
			texCoord = inPosition * pTCTransform.xy + pTCTransform.zw;
		}
		
		vTexCoord = texCoord;
	}
	
	if(VSRenderLayer){
		vLayer = inLayer;
		#ifdef SUPPORTS_VSLAYER
		gl_Layer = vLayer;
		#endif
	}
}
