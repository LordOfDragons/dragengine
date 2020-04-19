precision highp float;
precision highp int;



uniform vec4 pPosTransform;
uniform vec4 pTCTransform;
uniform vec4 pTCClamp;
uniform float pRadiusFactor;

uniform vec4 pParamSSAO; // self-occlusion, epsilon, scale, randomAngleConstant
uniform vec4 pParamTap; // count, radius, radius-influence, radius-limit
uniform vec4 pMipMapParams; // tcScaleU, tcScaleV, logBase, maxLevel

uniform HIGHP sampler2D texDepth;
#ifdef USE_DEPTH_MIPMAP
uniform HIGHP sampler2D texDepthMinMax;
#endif
uniform lowp sampler2D texDiffuse;
uniform lowp sampler2D texNormal;

in vec2 vTexCoord;

out vec3 outAO; // ao, ssao, solidity



// Constants
//////////////

#ifdef DECODE_IN_DEPTH
	const vec3 unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif

#define TC_CLAMPED(tc) clamp(tc, ivec2(0,0), pTCClamp.zw)



// Calculate the screen space ambient occlusion
/////////////////////////////////////////////////

#define pSSAOSelfOcclusion pParamSSAO.x
#define pSSAOEpsilon pParamSSAO.y
#define pSSAOScale pParamSSAO.z
#define pSSAORandomAngleConstant pParamSSAO.w

#define pTapCount int( pParamTap.x )
#define pTapRadius pParamTap.y
#define pTapRadiusInfluence pParamTap.z
#define pTapRadiusLimit pParamTap.w

float occlusion( in vec2 tc, in float level, in vec3 position, in vec3 normal ){
	tc = clamp( tc, pTCClamp.xy, pTCClamp.zw );
	
	#ifdef DECODE_IN_DEPTH
		vec3 spos = vec3( dot( textureLod( texDepth, tc, level ).rgb, unpackDepth ) );
	#else
		vec3 spos = vec3( textureLod( texDepth, tc, level ).r );
	#endif
	spos.z = pPosTransform.x / ( pPosTransform.y - spos.z );
	spos.xy = tc * pTCTransform.xy + pTCTransform.zw;
	spos.xy *= pPosTransform.zw * spos.zz;
	
	spos -= position;
	
	float slen = max( length( spos ), pSSAOEpsilon );
	
	return clamp( mix( 0.0, ( dot( normal, spos ) / slen ) - pSSAOSelfOcclusion, slen < pTapRadiusInfluence ), 0.0, 1.0 );
}

float screenSpaceAO( in vec2 tc, in vec3 position, in vec3 normal, in float radius, in int tapCount ){
	vec2 factor1 = vec2( 1.0, 0.5 ) / vec2( tapCount );
	ivec2 tcint = ivec2( tc * pMipMapParams.xy );
	float c1 = 30.0 * float( tcint.x ^ tcint.y ) + 10.0 * float( tcint.x ) * float( tcint.y );
	
	float occaccum = 0.0;
	float v1, v2;
	float level;
	vec2 tcoff;
	int i;
	
	for( i=0; i<tapCount; i++ ){
		v1 = factor1.x * float( i ) + factor1.y;
		v2 = pSSAORandomAngleConstant * v1 + c1;
		
		tcoff = vec2( radius * v1 ) * vec2( cos( v2 ), sin( v2 ) );
		
		level = min( floor( log2( length( tcoff * pMipMapParams.xy ) ) - pMipMapParams.z ), pMipMapParams.w );
		
		occaccum += occlusion( tc + tcoff, level, position, normal );
	}
	
	return clamp( 1.0 - occaccum * pSSAOScale / float( tapCount ), 0.0, 1.0 );
	//return clamp( 1.0 - occaccum * pSSAOScale / ( float( tapCount ) * pSSAOSelfOcclusion ), 0.0, 1.0 );
	// // this version scales by the self occlusion since with larger self occlusion values images get brighter
}



// Main Function
//////////////////

void main( void ){
	ivec2 tc = ivec2( gl_FragCoord.xy );
	
	outAO = vec3( 1.0 );
	
	// discard not inizalized fragments (set to 1)
	if( texelFetch( texDiffuse, tc, 0 ).a == 0.0 ){
		//discard;
		return;
	}
	
	// determine depth and z-position of fragment
	#ifdef DECODE_IN_DEPTH
		vec3 position = vec3( dot( texelFetch( texDepth, tc, 0 ).rgb, unpackDepth ) );
	#else
		vec3 position = vec3( texelFetch( texDepth, tc, 0 ).r );
	#endif
	position.z = pPosTransform.x / ( pPosTransform.y - position.z );
	position.xy = vTexCoord * pTCTransform.xy + pTCTransform.zw;
	position.xy *= pPosTransform.zw * position.zz;
	
	// calculate the parameters
#if 1
	#ifdef MATERIAL_NORMAL_INTBASIC
		vec3 normal = texelFetch( texNormal, tc, 0 ).rgb * vec3( 2.0 ) + vec3( -1.0 ); // IF USING FLOAT TEXTURE
		//vec3 normal = texelFetch( texNormal, tc, 0 ).rgb * vec3( 1.9921569 ) + vec3( -0.9921722 ); // IF USING INT TEXTURE
	#elif defined( MATERIAL_NORMAL_SPHEREMAP )
		vec2 fenc = texelFetch( texNormal, tc, 0 ).rgb * vec2( 4.0 ) - vec2( 2.0 );
		float f = dot( fenc, fenc );
		float g = sqrt( 1.0 - f * 0.25 );
		vec3 normal = vec3( fenc.xy * vec2( g ), f * 0.5 - 1.0 );
	#else
		vec3 normal = texelFetch( texNormal, tc, 0 ).rgb;
	#endif
	normal = normalize( normal );
#else
	vec3 normal = normalize( cross( dFdy( position ), dFdx( position ) ) );
#endif
	
	float radius = min( pRadiusFactor * pTapRadius / position.z, pTapRadiusLimit );
	
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
