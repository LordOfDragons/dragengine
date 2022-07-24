precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/skin/ubo_render_parameters.glsl"

uniform HIGHP sampler2DArray texDepth;
#ifdef USE_DEPTH_MIPMAP
uniform HIGHP sampler2DArray texDepthMinMax;
#endif
uniform lowp sampler2DArray texDiffuse;
uniform lowp sampler2DArray texNormal;

in vec2 vTexCoord;

#ifdef GS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out vec3 outAO; // ao, ssao, solidity



// Calculate the screen space ambient occlusion
/////////////////////////////////////////////////

#include "v130/shared/normal.glsl"
#include "v130/shared/defren/depth_to_position.glsl"

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
	tc = clamp( tc, pFSQuadTCClamp.xy, pFSQuadTCClamp.zw );
	
	float depth = sampleDepth( texDepth, vec3( tc, vLayer ), level );
	vec2 screenCoord = tc * pFSQuadTCTransform.xy + pFSQuadTCTransform.zw;
	vec3 spos = depthToPosition( depth, screenCoord, vLayer ) - position;
	
	float slen = max( length( spos ), pSSAOEpsilon );
	
	return clamp( mix( 0, ( dot( normal, spos ) / slen ) - pSSAOSelfOcclusion, slen < pTapRadiusInfluence ), 0, 1 );
}

float screenSpaceAO( in vec2 tc, in vec3 position, in vec3 normal, in float radius, in int tapCount ){
	vec2 factor1 = vec2( 1, 0.5 ) / vec2( tapCount );
	ivec2 tcint = ivec2( tc * pRenderSize.xy );
	float c1 = float( tcint.x ^ tcint.y ) * 20 + float( tcint.x ) * float( tcint.y ) * 10;
	
	float occaccum = 0;
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
	
	return clamp( 1.0 - occaccum * pSSAOScale / float( tapCount ), 0, 1 );
	//return clamp( 1.0 - occaccum * pSSAOScale / ( float( tapCount ) * pSSAOSelfOcclusion ), 0, 1 );
	// // this version scales by the self occlusion since with larger self occlusion values images get brighter
}



// Main Function
//////////////////

void main( void ){
	ivec3 tc = ivec3( gl_FragCoord.xy, vLayer );
	
	outAO = vec3( 1.0 );
	
	// discard not inizalized fragments (set to 1)
	if( texelFetch( texDiffuse, tc, 0 ).a == 0.0 ){
		//discard;
		return;
	}
	
	vec3 position = depthToPosition( texDepth, vTexCoord * pFSQuadTCTransform.xy + pFSQuadTCTransform.zw, vLayer );
	
	// calculate the parameters
#if 1
	vec3 normal = normalize( normalLoadMaterial( texNormal, tc ) );
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
