precision highp float;
precision highp int;

uniform vec2 pPosTransform; // position transform
uniform vec4 pPosToTC; // scaleS, scaleT, offsetS, offsetT
uniform vec4 pLightPos; // light position in object space + light-cutoff-distance
uniform mat4 pShadowMatrix; // shadow matrix
uniform vec2 pShaOpts1; // scale, offset, soffu, soffv
uniform vec3 pShaOpts2; // shadow base, shadow scale, cut-off, ter-pcf-size
uniform vec4 pShaOpts3; // shadow base, shadow scale, cut-off, ter-pcf-size

uniform HIGHP sampler2D texDepth;
#ifdef USE_HW_COMPARE
	uniform lowp sampler2DShadow texShadowObj;
	uniform lowp sampler2DShadow texShadowTer;
#else // USE_HW_COMPARE
	uniform mediump sampler2D texShadowObj;
	uniform mediump sampler2D texShadowTer;
#endif // USE_HW_COMPARE

#ifdef TRANSP_SHADOW
	#ifdef USE_HW_COMPARE
		uniform lowp sampler2DShadow texShadowTra;
	#else // USE_HW_COMPARE
		uniform mediump sampler2D texShadowTra;
	#endif // USE_HW_COMPARE
#endif
uniform lowp sampler2D texNoise2D;

in vec3 vLightVolumePos;

out vec4 outOcclusion;



const float epsilon = 0.00001;

const vec2 sct1 = vec2( 0.5, 0.5 );
const vec2 sct2 = vec2( 0.5, 0.5 );
const vec4 scot1 = vec4( 1.0, 1.0, 0.0, 0.0 );
const vec4 scot2 = vec4( 1.0, 1.0, -1.0, -1.0 );

#ifdef USE_HW_COMPARE
#ifndef PCF_9TAP
const vec4 scot3 = vec4( 0.5, 0.5, -0.5, -0.5 );
const vec4 pcfHackWeight = vec4( 0.25 );
#endif
#endif // USE_HW_COMPARE

#if defined( PCF_9TAP ) || defined( PCF_NOISETAP )
const float pcfWeight = 1.0 / 9.0;
const vec4 pcfWeight2 = vec4( 1.0 / 9.0 );
#endif

#ifdef GEOM_ENCODED_DEPTH
const vec3 unpackGeomDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif

#ifdef PCF_NOISETAP
const vec2 ntfS = vec2( 1.0 );
const vec2 ntfO = vec2( -0.5 );
#endif



void pyramidShadowCoords( out vec3 stc, in vec3 shapos, in vec2 options ){
	float shadist = length( shapos ) * options.x + options.y;
	
	if( shapos.y >= abs( shapos.x ) ){
		stc = vec3( shapos.x, shapos.y - shapos.z, shadist );
		stc.st *= vec2( 1.0 / ( shapos.y + shapos.z ) );
		stc.st = stc.st * vec2( 0.5, 0.25 ) + vec2( 0.5, 0.75 );
		
	}else if( shapos.x >= abs( shapos.y ) ){
		stc = vec3( shapos.x - shapos.z, shapos.y, shadist );
		stc.st *= vec2( 1.0 / ( shapos.x + shapos.z ) );
		stc.st = stc.st * vec2( 0.25, 0.5 ) + vec2( 0.75, 0.5 );
		
	}else if( -shapos.y >= abs( shapos.x ) ){
		stc = vec3( shapos.x, shapos.y + shapos.z, shadist );
		stc.st *= vec2( 1.0 / ( shapos.z - shapos.y ) );
		stc.st = stc.st * vec2( 0.5, 0.25 ) + vec2( 0.5, 0.25 );
		
	}else{ // -shapos.x >= abs( shapos.y )
		stc = vec3( shapos.x + shapos.z, shapos.y, shadist );
		stc.st *= vec2( 1.0 / ( shapos.z - shapos.x ) );
		stc.st = stc.st * vec2( 0.25, 0.5 ) + vec2( 0.25, 0.5 );
	}
}

#ifdef USE_HW_COMPARE
void evalShadowMap( out float shadow, in lowp sampler2DShadow shamap, in vec3 options, in vec3 shapos ){
	vec4 shaoff, collect;
	vec3 stc = shapos;
	
#ifdef PCF_VARTAP
	vec2 shascale = options.xy * vec2( clamp( shapos.p * 5.0, 0.5, 5.0 ) );
	int x, y;
	
	shadow = 0.0;
	
	for( y=-2; y<2; y++ ){
		stc.t = shascale.y * float( y ) + shapos.t;
		
		for( x=-2; x<2; x++ ){
			stc.s = shascale.x * float( x ) + shapos.s;
			shadow += texture( shamap, stc );
		}
	}
	
	shadow /= 25.0;
	
#elif defined( PCF_NOISETAP )
	vec2 fc = gl_FragCoord.xy / vec2( 32.0 );
	vec2 rnoise;
	
	rnoise = texture( texNoise2D, fc ).rg * ntfS + ntfO;
	shadow = texture( shamap, vec3( stc.st + rnoise * options.zz, stc.p ) ) * pcfWeight;
	
	shaoff = options.xyxy * scot1; // offU, offV, 0, 0
	rnoise = textureOffset( texNoise2D, fc, ivec2( 1, 0 ) ).rg * ntfS + ntfO;
	collect.x = texture( shamap, vec3( shapos.st + shaoff.xw + rnoise * options.zz, stc.p ) ); // (u,0)
	rnoise = textureOffset( texNoise2D, fc, ivec2( -1, 0 ) ).rg * ntfS + ntfO;
	collect.y = texture( shamap, vec3( shapos.st - shaoff.xw + rnoise * options.zz, stc.p ) ); // (-u,0)
	rnoise = textureOffset( texNoise2D, fc, ivec2( 0, 1 ) ).rg * ntfS + ntfO;
	collect.z = texture( shamap, vec3( shapos.st + shaoff.zy + rnoise * options.zz, stc.p ) ); // (0,v)
	rnoise = textureOffset( texNoise2D, fc, ivec2( 0, -1 ) ).rg * ntfS + ntfO;
	collect.w = texture( shamap, vec3( shapos.st - shaoff.zy + rnoise * options.zz, stc.p ) ); // (0,-v)
	shadow += dot( collect, pcfHackWeight/*pcfWeight2*/ );
	
#elif defined( PCF_9TAP )
	shadow = texture( shamap, stc ) * pcfWeight;
	
	shaoff = options.xyxy * scot1; // offU, offV, 0, 0
	collect.x = texture( shamap, vec3( shapos.st + shaoff.xw, stc.p ) ); // (u,0)
	collect.y = texture( shamap, vec3( shapos.st - shaoff.xw, stc.p ) ); // (-u,0)
	collect.z = texture( shamap, vec3( shapos.st + shaoff.zy, stc.p ) ); // (0,v)
	collect.w = texture( shamap, vec3( shapos.st - shaoff.zy, stc.p ) ); // (0,-v)
	shadow += dot( collect, pcfWeight2 );
	
	shaoff = options.xyxy * scot2; // offU, offV, -offU, -offV
	collect.x = texture( shamap, vec3( shapos.st + shaoff.xy, stc.p ) ); // (u,v)
	collect.y = texture( shamap, vec3( shapos.st + shaoff.xw, stc.p ) ); // (u,-v)
	collect.z = texture( shamap, vec3( shapos.st + shaoff.zy, stc.p ) ); // (-u,v)
	collect.w = texture( shamap, vec3( shapos.st + shaoff.zw, stc.p ) ); // (-u,-v)
	shadow += dot( collect, pcfWeight2 );
	
#else // PCF_9TAP
	shaoff = options.xyxy * scot3; // offU, offV, -offU, -offV
	stc.st = shapos.st + shaoff.xy;
	collect.x = texture( shamap, stc ); // (u,v)
	stc.st = shapos.st + shaoff.xw;
	collect.y = texture( shamap, stc ); // (u,-v)
	stc.st = shapos.st + shaoff.zy;
	collect.z = texture( shamap, stc ); // (-u,v)
	stc.st = shapos.st + shaoff.zw;
	collect.w = texture( shamap, stc ); // (-u,-v)
	shadow = dot( collect, pcfHackWeight );
#endif // PCF_9TAP
}

#else // USE_HW_COMPARE
void evalShadowMap( out float shadow, in mediump sampler2D shamap, in vec2 options, in vec3 stc ){
#ifdef PCF_9TAP
	vec4 shaoff, collect;
	
	shadow = step( stc.p, texture( shamap, stc.st ).r ) * pcfWeight;
	
	shaoff = options.xyxy * scot1; // offU, offV, 0, 0
	collect.x = step( stc.p, texture( shamap, stc.st + shaoff.xw ).r ); // (u,0)
	collect.y = step( stc.p, texture( shamap, stc.st - shaoff.xw ).r ); // (-u,0)
	collect.z = step( stc.p, texture( shamap, stc.st + shaoff.zy ).r ); // (0,v)
	collect.w = step( stc.p, texture( shamap, stc.st - shaoff.zy ).r ); // (0,-v)
	shadow += dot( collect, pcfWeight2 );
	
	shaoff = options.xyxy * scot2; // offU, offV, -offU, -offV
	collect.x = step( stc.p, texture( shamap, stc.st + shaoff.xy ).r ); // (u,v)
	collect.y = step( stc.p, texture( shamap, stc.st + shaoff.xw ).r ); // (u,-v)
	collect.z = step( stc.p, texture( shamap, stc.st + shaoff.zy ).r ); // (-u,v)
	collect.w = step( stc.p, texture( shamap, stc.st + shaoff.zw ).r ); // (-u,-v)
	shadow += dot( collect, pcfWeight2 );
	
#else // PCF_9TAP
	shadow = step( stc.p, texture( shamap, stc.st ).r );
#endif // PCF_9TAP
}
#endif // USE_HW_COMPARE



void main( void ){
	vec2 tc = gl_FragCoord.xy * pPosToTC.xy + pPosToTC.zw;
	
	// calculate position
#ifdef GEOM_ENCODED_DEPTH
	vec3 position = vec3( dot( texture( texDepth, tc ).rgb, unpackGeomDepth ) );
#else
	vec3 position = vec3( texture( texDepth, tc ).r );
#endif
	position.z = pPosTransform.x / ( pPosTransform.y - position.z );
	position.xy = vLightVolumePos.xy * vec2( position.z ) / vec2( vLightVolumePos.z );
	
	// calculate light direction and distance
//	vec3 closestLightPoint = clamp( position, pLightPos.xyz - vec3( 0.5, 0.0, 0.5 ), pLightPos.xyz + ( 0.5, 0.0, 0.5 ) );
	
	float dist = length( pLightPos.xyz - position );
	
	if( dist == 0.0 || dist > pLightPos.w ) discard; // outside light range
	
	// calculate occlusion values
	vec3 shapos = ( pShadowMatrix * vec4( position, 1.0 ) ).xyz;
	
	if( shapos.z < pShaOpts3.z ){ // over 180 degree
		outOcclusion.rg = vec2( 0.0 );
		
	}else{
		// calculate solid occlusion
		float shadow = 0.0;
		float shadowObj = 1.0;
		
		pyramidShadowCoords( shapos, shapos, pShaOpts1.xy );
		
		evalShadowMap( shadowObj, texShadowObj, pShaOpts2, shapos );
		evalShadowMap( shadow, texShadowTer, pShaOpts2, shapos );
		
		outOcclusion.r = shadow * ( shadowObj * pShaOpts3.y + pShaOpts3.x );
		
#ifdef TRANSP_SHADOW
		float shadowTransp;
		
		shadowObj *= shadow; // for transparency both shadow types are fully solid
		
		evalShadowMap( shadowTransp, texShadowTra, pShaOpts2, shapos );
		
		outOcclusion.g = shadowObj * ( 1.0 - shadowTransp );
#endif // TRANSP_SHADOW
	}
}
