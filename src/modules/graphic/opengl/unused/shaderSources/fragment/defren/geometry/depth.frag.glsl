#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform vec4 pClipPlane; // normal.xyz, distance

layout(binding=0) uniform lowp sampler2D texDiffuse;
layout(binding=1) uniform HIGHP sampler2DArray texDepth;

#if defined IGNORE_HOLES || defined CLIP_DEPTH
VARYING_BIND(0) in vec2 vTexCoord;
#endif

// UseClipPlane
VARYING_BIND(1) in vec3 vClipCoord;

// LayeredRendering != LayeredRenderingNone
VARYING_BIND(2) flat in int vLayer;

// EncodedDepth
layout(location=0) out vec4 outDepth;

// EncodedDepth
const vec3 packShift = vec3( 1.0, 256.0, 65536.0 );
const vec3 packMask = vec3( 1.0 / 256.0, 1.0 / 256.0, 0.0 );

// EncodedDepth && ClipDepth
const vec3 unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );

// ClipDepth
#define epsilon 0.0001

void main( void ){
	if(UseClipPlane){
		if( dot( vClipCoord, pClipPlane.xyz ) <= pClipPlane.w ) discard;
	}
	
#ifdef IGNORE_HOLES
	//if( texture( texDiffuse, vTexCoord ).a < 0.5 ) discard;
	if( texture( texDiffuse, vTexCoord ).a < 0.35 ) discard;
#endif
	
#ifdef CLIP_DEPTH
	// remove next line for deoglSkinShader version when using DepthTestBack on estDepth
	if( texture( texDiffuse, vTexCoord ).a < epsilon ) discard;
	
	if(InverseDepth){
		if(EncodedDepth){
			if( gl_FragCoord.z <= dot( texelFetch( texDepth, ivec3( gl_FragCoord.xy, vLayer ), 0 ).rgb, unpackDepth ) ) discard;
			
		}else{
			if( gl_FragCoord.z <= texelFetch( texDepth, ivec3( gl_FragCoord.xy, vLayer ), 0 ).r ) discard;
		}
		
	}else{
		if(EncodedDepth){
			if( gl_FragCoord.z >= dot( texelFetch( texDepth, ivec3( gl_FragCoord.xy, vLayer ), 0 ).rgb, unpackDepth ) ) discard;
			
		}else{
			if( gl_FragCoord.z >= texelFetch( texDepth, ivec3( gl_FragCoord.xy, vLayer ), 0 ).r ) discard;
		}
	}
#endif

	if(EncodedDepth){
		vec3 encoded = fract( packShift * vec3( gl_FragCoord.z ) );
		outDepth = vec4( encoded - ( encoded.yzz * packMask ), 1.0 ); // 1.0 in alpha indicates we wrote the pixel
	}
}
