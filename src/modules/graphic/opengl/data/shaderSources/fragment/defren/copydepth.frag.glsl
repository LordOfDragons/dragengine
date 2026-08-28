#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

// InputArrayTextures
layout(binding=0) uniform HIGHP sampler2DArray texDepthArray;

// !InputArrayTextures
layout(binding=0) uniform HIGHP sampler2D texDepth;

// CopyColor && InputArrayTextures
layout(binding=1) uniform mediump sampler2DArray texColorArray;

// CopyColor && !InputArrayTextures
layout(binding=1) uniform mediump sampler2D texColor;

#include "shared/interface/2d/fragment.glsl"

// CopyColor
layout(location=0) out vec4 outColor;

// EncodedDepth
layout(location=1) out vec4 outDepth;


void main( void ){
	ivec3 tc = ivec3(gl_FragCoord.xy, vLayer);
	
	if(EncodedDepth){
		if(InputArrayTextures){
			outDepth = texelFetch(texDepthArray, tc, 0);
		}else{
			outDepth = texelFetch(texDepth, tc.xy, 0);
		}
		
		if(DepthTest != DepthTestNone){
			/* if( outDepth.rgb == vec3( 0.0 ) ){
				outDepth.rgb = vec3( 1.0, 0.0, 0.0 );
			} */
			if( outDepth.rgb == vec3( 0.0 ) ){
				discard;
			}
		}
		
	}else{
		if(InputArrayTextures){
			gl_FragDepth = texelFetch(texDepthArray, tc, 0).r;
		}else{
			gl_FragDepth = texelFetch(texDepth, tc.xy, 0).r;
		}
		
		if(DepthTest != DepthTestNone){
			/* if( gl_FragDepth == 0.0 ){
				gl_FragDepth = 1.0;
			} */
			if(gl_FragDepth == (ShadowInverseDepth ? 1.0 : 0.0)){
				discard;
			}
		}
	}
	
	if(CopyColor){
		if(InputArrayTextures){
			outColor = texelFetch(texColorArray, tc, 0);
		}else{
			outColor = texelFetch(texColor, tc.xy, 0);
		}
	}
}
