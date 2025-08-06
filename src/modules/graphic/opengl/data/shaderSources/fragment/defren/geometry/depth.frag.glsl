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
in vec3 vClipCoord;

// GSRenderStereo || VSRenderStereo
VARYING_BIND(1) flat in int vLayer;

#ifdef ENCODE_DEPTH
layout(location=0) out vec4 outDepth;
#endif

#ifdef ENCODE_DEPTH
const vec3 packShift = vec3( 1.0, 256.0, 65536.0 );
const vec3 packMask = vec3( 1.0 / 256.0, 1.0 / 256.0, 0.0 );
#ifdef CLIP_DEPTH
const vec3 unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif // CLIP_DEPTH
#endif // ENCODE_DEPTH

#ifdef CLIP_DEPTH
#define epsilon 0.0001
#endif

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
		#ifdef ENCODE_DEPTH
			if( gl_FragCoord.z <= dot( texelFetch( texDepth, ivec3( gl_FragCoord.xy, vLayer ), 0 ).rgb, unpackDepth ) ) discard;
		#else
			if( gl_FragCoord.z <= texelFetch( texDepth, ivec3( gl_FragCoord.xy, vLayer ), 0 ).r ) discard;
		#endif
		
	}else{
		#ifdef ENCODE_DEPTH
			if( gl_FragCoord.z >= dot( texelFetch( texDepth, ivec3( gl_FragCoord.xy, vLayer ), 0 ).rgb, unpackDepth ) ) discard;
		#else
			if( gl_FragCoord.z >= texelFetch( texDepth, ivec3( gl_FragCoord.xy, vLayer ), 0 ).r ) discard;
		#endif
	}
#endif

#ifdef ENCODE_DEPTH
	vec3 encoded = fract( packShift * vec3( gl_FragCoord.z ) );
	outDepth = vec4( encoded - ( encoded.yzz * packMask ), 1.0 ); // 1.0 in alpha indicates we wrote the pixel
#endif
}
