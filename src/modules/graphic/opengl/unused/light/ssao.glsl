precision highp float;
precision highp int;

layout(binding=0) uniform HIGHP sampler2DArray texDepth;

layout(binding=0, rgba8) uniform readonly image2DArray texDiffuse;
layout(binding=1, rgba16f) uniform readonly image2DArray texNormal;

layout(binding=2, r8) uniform writeonly restrict image2DArray texAO;

layout( local_size_x=64 ) in;


#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

#include "shared/normal/image.glsl"
#include "shared/defren/depth_to_position.glsl"

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


float occlusion( in vec2 tc, in int layer, in float level, in vec3 position, in vec3 normal ){
	tc = clamp( tc, pViewportMin, pViewportMax );
	
	float depth = sampleDepth( texDepth, vec3( tc, layer ), level );
	vec3 spos = depthToPosition( depth, fsquadTexCoordToScreenCoord( tc ), layer ) - position;
	
	float slen = max( length( spos ), pSSAOEpsilon );
	
	return clamp( mix( 0.0, ( dot( normal, spos ) / slen ) - pSSAOSelfOcclusion, slen < pTapRadiusInfluence ), 0.0, 1.0 );
}


float screenSpaceAO( in ivec3 fragCoord, in vec2 texCoord, in vec3 position, in vec3 normal, in float radius, in int tapCount ){
	//float c1 = float( fragCoord.x ^ fragCoord.y ) * 20 + float( fragCoord.x ) * float( fragCoord.y ) * 10;
	float c1 = dot( vec2( fragCoord % ivec2( 23 ) ), vec2( 21, 19 ) ) * 3;
	vec2 factor1 = vec2( 1, 0.5 ) / vec2( tapCount );
	
	float occaccum = 0.0;
	int i;
	
	for( i=0; i<tapCount; i++ ){
		float v1 = factor1.x * float( i ) + factor1.y;
		float v2 = pSSAORandomAngleConstant * v1 + c1;
		
		vec2 tcoff = vec2( radius * v1 ) * vec2( cos( v2 ), sin( v2 ) );
		
		float level = min( floor( log2( length( tcoff * pRenderSize ) ) - pSSAOMipMapBase ), pSSAOMipMapMaxLevel );
		
		occaccum += occlusion( texCoord + tcoff, fragCoord.z, level, position, normal );
	}
	
	return clamp( 1.0 - occaccum * pSSAOScale / float( tapCount ), 0.0, 1.0 );
	//return clamp( 1.0 - occaccum * pSSAOScale / ( float( tapCount ) * pSSAOSelfOcclusion ), 0, 1 );
	// // this version scales by the self occlusion since with larger self occlusion values images get brighter
}


void main( void ){
	// image is process in lines so y never can be outside boundary only x. same for z
	if( gl_GlobalInvocationID.x >= pRenderSizeCompute.x ){
		return;
	}
	
	ivec3 fragCoord = ivec3( gl_GlobalInvocationID );
	
	// discard not inizalized fragments (set to 1)
	if( imageLoad( texDiffuse, fragCoord ).a == 0 ){
		imageStore( texAO, fragCoord, vec4( 1 ) );
		return;
	}
	
	vec2 texCoord = fragCoordToTexCoord( fragCoord.xy );
	
	vec3 position = depthToPosition( sampleDepth( texDepth, fragCoord ),
		fragCoordToScreenCoord( fragCoord.xy ), fragCoord.z );
	
	// calculate the parameters
	vec3 normal = normalize( normalLoadMaterial( texNormal, fragCoord ) );
	
	float radius = min( pSSAORadiusFactor * pTapRadius / position.z, pTapRadiusLimit );
	
	// calculate the screen space ambient occlusion
	#ifndef SSAO_RESOLUTION_COUNT
		#define SSAO_RESOLUTION_COUNT 1
	#endif
	
	float ao;
	
	#if SSAO_RESOLUTION_COUNT == 1
		ao = screenSpaceAO( fragCoord, texCoord, position, normal, radius, pTapCount );
		
	#elif SSAO_RESOLUTION_COUNT == 2
		ao = screenSpaceAO( fragCoord, texCoord, position, normal, radius * 0.35, pTapCount ); // or weight 0.35
		ao = min( ao, screenSpaceAO( fragCoord, texCoord, position, normal, radius, pTapCount ) ); // or weight 0.65
		
	#elif SSAO_RESOLUTION_COUNT == 3
		ao = screenSpaceAO( fragCoord, texCoord, position, normal, radius * 0.25, pTapCount ); // or weight 0.1
		ao = min( ao, screenSpaceAO( fragCoord, texCoord, position, normal, radius * 0.5, pTapCount ) ); // or weight 0.3
		ao = min( ao, screenSpaceAO( fragCoord, texCoord, position, normal, radius, pTapCount ) ); // or weight 0.6
		
	#else
		ao = screenSpaceAO( fragCoord, texCoord, position, normal, radius * 0.15, pTapCount ); // or weight 0.32
		ao = min( ao, screenSpaceAO( fragCoord, texCoord, position, normal, radius * 0.25, pTapCount ) ); // or weight 0.27
		ao = min( ao, screenSpaceAO( fragCoord, texCoord, position, normal, radius * 0.5, pTapCount ) ); // or weight 0.23
		ao = min( ao, screenSpaceAO( fragCoord, texCoord, position, normal, radius, pTapCount ) ); // or weight 0.18
	#endif
	
	imageStore( texAO, fragCoord, vec4( ao ) );
}
