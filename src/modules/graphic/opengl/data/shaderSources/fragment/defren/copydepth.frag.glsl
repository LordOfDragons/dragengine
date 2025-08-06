#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

layout(binding=0) uniform HIGHP sampler2DArray texDepth;

// CopyColor
layout(binding=1) uniform mediump sampler2DArray texColor;

#include "shared/interface/2d_fragment.glsl"

// CopyColor
layout(location=0) out vec4 outColor;

// EncodedDepth
layout(location=1) out vec4 outDepth;


void main( void ){
	ivec3 tc = ivec3( gl_FragCoord.xy, vLayer );
	
	if(EncodedDepth){
		outDepth = texelFetch( texDepth, tc, 0 );
		if(DepthTest){
			/* if( outDepth.rgb == vec3( 0.0 ) ){
				outDepth.rgb = vec3( 1.0, 0.0, 0.0 );
			} */
			if( outDepth.rgb == vec3( 0.0 ) ){
				discard;
			}
		}
		
	}else{
		gl_FragDepth = texelFetch( texDepth, tc, 0 ).r;
		if(DepthTest){
			/* if( gl_FragDepth == 0.0 ){
				gl_FragDepth = 1.0;
			} */
			if(gl_FragDepth == (ShadowInverseDepth ? 1.0 : 0.0)){
				discard;
			}
		}
	}
	
	if(CopyColor){
		outColor = texelFetch( texColor, tc, 0 );
	}
}
