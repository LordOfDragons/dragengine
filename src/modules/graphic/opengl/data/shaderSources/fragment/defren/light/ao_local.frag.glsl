precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

uniform HIGHP sampler2DArray texDepth;
uniform lowp sampler2DArray texDiffuse;
uniform lowp sampler2DArray texNormal;

in vec2 vTexCoord;
in vec2 vScreenCoord;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

layout(location=0) out vec3 outAO; // ao, ssao, solidity



// Calculate the screen space ambient occlusion
/////////////////////////////////////////////////

#include "shared/normal_texture.glsl"
#include "shared/defren/depth_to_position.glsl"
#include "shared/defren/depth_to_position_fragment.glsl"

#define pSSAOSelfOcclusion pSSAOParams1.x
#define pSSAOEpsilon pSSAOParams1.y
#define pSSAOScale pSSAOParams1.z
#define pSSAORandomAngleConstant pSSAOParams1.w

#define pTapCount int( pSSAOParams2.x )
#define pTapRadius pSSAOParams2.y
#define pTapRadiusInfluence pSSAOParams2.z
#define pTapRadiusLimit pSSAOParams2.w

#define pSSAORadiusFactor pSSAOParams3.x
#define pSSAOMipMapBase pSSAOParams3.y
#define pSSAOMipMapMaxLevel pSSAOParams3.z

float occlusion( in vec2 tc, in float level, in vec3 position, in vec3 normal ){
	tc = clamp( tc, pViewportMin, pViewportMax );
	
	float depth = sampleDepth( texDepth, vec3( tc, vLayer ), level );
	vec3 spos = depthToPosition( depth, fsquadTexCoordToScreenCoord( tc ), vLayer ) - position;
	
	float slen = max( length( spos ), pSSAOEpsilon );
	
	return clamp( mix( 0.0, ( dot( normal, spos ) / slen ) - pSSAOSelfOcclusion, slen < pTapRadiusInfluence ), 0.0, 1.0 );
}

float screenSpaceAO( in vec2 tc, in vec3 position, in vec3 normal, in float radius, in int tapCount ){
	vec2 factor1 = vec2( 1, 0.5 ) / vec2( tapCount );
	ivec2 tcint = ivec2( tc / pScreenSpacePixelSize );
	
	// this version is from the internet somewhere. for some reason it tends to create
	// a strange large scale circular banding originating from corners which is bad
	//float c1 = float( tcint.x ^ tcint.y ) * 20 + float( tcint.x ) * float( tcint.y ) * 10;
	
	// I picked up the idea from the broken version above but used instead a
	// modulus on the texture coordinates to get repeating coordinates to swirl
	// around. the main problem with this approach is a clearly visible pattern.
	// this version here I found by experimenting around with values in renderdoc.
	// the combination of these prime numbers produces a result that hides the
	// visible pattern quite well. it looks quite good compared to using a random
	// texture and has the advantage of not requiring texture sampling
	float c1 = dot( vec2( tcint % ivec2( 23 ) ), vec2( 21, 19 ) ) * 3.0;
	
	// these are random functions from the internet. the result is though inferior
	//float c1 = fract(sin(dot(tc, vec2(12.9898, 78.233))) * 43758.5453);
	
	float occaccum = 0.0;
	float v1, v2;
	float level;
	vec2 tcoff;
	int i;
	
	for( i=0; i<tapCount; i++ ){
		v1 = factor1.x * float( i ) + factor1.y;
		v2 = pSSAORandomAngleConstant * v1 + c1;
		
		tcoff = vec2( radius * v1 ) * vec2( cos( v2 ), sin( v2 ) );
		
		level = min( floor( log2( length( tcoff * pRenderSize.xy ) ) - pSSAOMipMapBase ), pSSAOMipMapMaxLevel );
		
		occaccum += occlusion( tc + tcoff, level, position, normal );
	}
	
	return clamp( 1.0 - occaccum * pSSAOScale / float( tapCount ), 0.0, 1.0 );
	//return clamp( 1.0 - occaccum * pSSAOScale / ( float( tapCount ) * pSSAOSelfOcclusion ), 0, 1 );
	// // this version scales by the self occlusion since with larger self occlusion values images get brighter
}



// Main Function
//////////////////

void main( void ){
	vec3 tc = vec3( vTexCoord, vLayer );
	
	outAO = vec3( 1 );
	
	// discard not inizalized fragments (set to 1)
	if( textureLod( texDiffuse, tc, 0.0 ).a == 0.0 ){
		//discard;
		return;
	}
	
	vec3 position = depthToPosition( texDepth, tc, vScreenCoord, vLayer );
	
	// calculate the parameters
#if 1
	vec3 normal = sanitizeNormal( normalLoadMaterial( texNormal, tc ) );
#else
	vec3 normal = normalize( cross( dFdy( position ), dFdx( position ) ) );
#endif
	
	float radius = min( pSSAORadiusFactor * pTapRadius / position.z, pTapRadiusLimit );
	
	// calculate the screen space ambient occlusion
	#ifndef SSAO_RESOLUTION_COUNT
		#define SSAO_RESOLUTION_COUNT 1
	#endif
	
	#if SSAO_RESOLUTION_COUNT == 1
		float ao = screenSpaceAO( vTexCoord, position, normal, radius, pTapCount );
		
	#elif SSAO_RESOLUTION_COUNT == 2
		float ao = screenSpaceAO( vTexCoord, position, normal, radius * 0.35, pTapCount ); // or weight 0.35
		ao = min( ao, screenSpaceAO( vTexCoord, position, normal, radius, pTapCount ) ); // or weight 0.65
		
	#elif SSAO_RESOLUTION_COUNT == 3
		float ao = screenSpaceAO( vTexCoord, position, normal, radius * 0.25, pTapCount ); // or weight 0.1
		ao = min( ao, screenSpaceAO( vTexCoord, position, normal, radius * 0.5, pTapCount ) ); // or weight 0.3
		ao = min( ao, screenSpaceAO( vTexCoord, position, normal, radius, pTapCount ) ); // or weight 0.6
		
	#else
		float ao = screenSpaceAO( vTexCoord, position, normal, radius * 0.15, pTapCount ); // or weight 0.32
		ao = min( ao, screenSpaceAO( vTexCoord, position, normal, radius * 0.25, pTapCount ) ); // or weight 0.27
		ao = min( ao, screenSpaceAO( vTexCoord, position, normal, radius * 0.5, pTapCount ) ); // or weight 0.23
		ao = min( ao, screenSpaceAO( vTexCoord, position, normal, radius, pTapCount ) ); // or weight 0.18
	#endif
	
	outAO = vec3( ao );
}
