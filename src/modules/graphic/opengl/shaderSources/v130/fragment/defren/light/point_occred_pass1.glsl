precision highp float;
precision highp int;

uniform vec3 pLightPos; // light position in object space
uniform vec2 pPosTransform; // position transform
uniform vec2 pLightAtten; // light attenuation: att-coef, cut-off
uniform mat4 pShadowMatrix; // shadow matrix
uniform vec3 pShaStaParam; // scale, offset, pcfSize
uniform vec3 pShaDynParam; // scale, offset, pcfSize
uniform vec2 pShaOpts2;

uniform HIGHP sampler2D texDepth;
uniform mediump samplerCube texShadowLow;
uniform mediump samplerCube texShadowSolid;
uniform mediump samplerCube texShadowTransp;
uniform lowp sampler2D texNoise2D;

in vec3 vLightVolumePos;

out vec4 outOcclusion;

#define epsilon 0.0001

const vec2 c1 = vec2( 1.0, 0.0 );
const vec2 c2 = vec2( -1.0, 1.0 );

#ifdef USE_ENCODED_DEPTH
const vec3 unpackCubeDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif // USE_ENCODED_DEPTH

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



#ifdef USE_ENCODED_DEPTH
#define TCM( cm, tc, td )		step( td, dot( texture( cm, tc ).rgb, unpackCubeDepth ) )
#else // USE_ENCODED_DEPTH
#define TCM( cm, tc, td )		step( td, texture( cm, tc ).r )
#endif // USE_ENCODED_DEPTH
//#define TCM( cm, tc, td )		texture( cm, vec4( tc, td ) ).r

#ifdef USE_ENCODED_DEPTH
void evalShadowCube( out float shadow, in mediump samplerCube shacube, in vec3 options, in vec3 shapos ){
#else // USE_ENCODED_DEPTH
//void evalShadowCube( out float shadow, in mediump samplerCubeShadow shacube, in vec3 options, in vec3 shapos ){
void evalShadowCube( out float shadow, in mediump samplerCube shacube, in vec3 options, in vec3 shapos ){
#endif // USE_ENCODED_DEPTH
	float slen = length( shapos );
	float shadist = slen * options.x + options.y;
	
#if defined( PCF_9TAP ) || defined( PCF_VARTAP ) || defined( PCF_NOISETAP )
	float olen = slen * options.z;
	vec3 o1;// = vec3( 0.0 );
	vec3 o2;// = vec3( 0.0 );
	
	vec3 absshapos = abs( shapos );
	//vec3 r = step( absshapos.xyz, absshapos.yzx ) * step( absshapos.xyz, absshapos.zxy );
	//vec2 scale = vec2( dot( absshapos, r ) * pShaOpts1.z );
	//o1.xz = ( c2 * r.xx + c1 ) * scale;
	//o2.zy = ( c2 * r.yy + c1 ) * scale;
	if( absshapos.x > absshapos.y && absshapos.x > absshapos.z ){ // x is largest
		o1 = vec3( 0.0, 0.0, olen );
		o2 = vec3( 0.0, olen, 0.0 );
		
	}else if( absshapos.y > absshapos.x && absshapos.y > absshapos.z ){ // y is largest
		o1 = vec3( olen, 0.0, 0.0 );
		o2 = vec3( 0.0, 0.0, olen );
		
	}else{ // z is largest
		o1 = vec3( olen, 0.0, 0.0 );
		o2 = vec3( 0.0, olen, 0.0 );
	}
#endif
	
#ifdef PCF_VARTAP
	float shascale = clamp( shadist * 10.0, 0.5, 10.0 ); // clamp( shadist * 5.0, 0.5, 5.0 );
	vec3 offy;
	int x, y;
	
	o1 *= vec3( shascale );
	o2 *= vec3( shascale );
	shadow = 0.0;
	
	for( y=-2; y<2; y++ ){
		offy = shapos + o2 * vec3( float( y ) );
		
		for( x=-2; x<2; x++ ){
			shadow += TCM( shacube, offy + o1 * vec3( float( x ) ), shadist );
		}
	}
	
	shadow *= 0.04; // /= 25.0;
	
#elif defined( PCF_NOISETAP )
	vec2 fc = gl_FragCoord.xy / vec2( 32.0 );
	vec4 collect;
	vec2 rnoise;
	vec3 stc;
	
	rnoise = texture( texNoise2D, fc ).rg * ntfS + ntfO;
	stc = o1 * vec3( rnoise.x ) + shapos;
	stc = o2 * vec3( rnoise.y ) + stc;
	shadow = TCM( shacube, stc, shadist ) * pcfWeight;
	
	rnoise = textureOffset( texNoise2D, fc, ivec2( 1, 0 ) ).rg * ntfS + ntfO;
	stc = shapos + o1;
	stc = o1 * vec3( rnoise.x ) + stc;
	stc = o2 * vec3( rnoise.y ) + stc;
	collect.x = TCM( shacube, stc, shadist );
	
	rnoise = textureOffset( texNoise2D, fc, ivec2( -1, 0 ) ).rg * ntfS + ntfO;
	stc = shapos - o1;
	stc = o1 * vec3( rnoise.x ) + stc;
	stc = o2 * vec3( rnoise.y ) + stc;
	collect.y = TCM( shacube, stc, shadist );
	
	rnoise = textureOffset( texNoise2D, fc, ivec2( 0, 1 ) ).rg * ntfS + ntfO;
	stc = shapos + o2;
	stc = o1 * vec3( rnoise.x ) + stc;
	stc = o2 * vec3( rnoise.y ) + stc;
	collect.z = TCM( shacube, stc, shadist );
	
	rnoise = textureOffset( texNoise2D, fc, ivec2( 0, -1 ) ).rg * ntfS + ntfO;
	stc = shapos - o2;
	stc = o1 * vec3( rnoise.x ) + stc;
	stc = o2 * vec3( rnoise.y ) + stc;
	collect.w = TCM( shacube, stc, shadist );
	shadow += dot( collect, pcfWeight2 );
	
	rnoise = textureOffset( texNoise2D, fc, ivec2( 1, 1 ) ).rg * ntfS + ntfO;
	stc = shapos + o1 + o2;
	stc = o1 * vec3( rnoise.x ) + stc;
	stc = o2 * vec3( rnoise.y ) + stc;
	collect.x = TCM( shacube, stc, shadist );
	
	rnoise = textureOffset( texNoise2D, fc, ivec2( 1, -1 ) ).rg * ntfS + ntfO;
	stc = shapos + o1 - o2;
	stc = o1 * vec3( rnoise.x ) + stc;
	stc = o2 * vec3( rnoise.y ) + stc;
	collect.y = TCM( shacube, stc, shadist );
	
	rnoise = textureOffset( texNoise2D, fc, ivec2( -1, 1 ) ).rg * ntfS + ntfO;
	stc = shapos - o1 + o2;
	stc = o1 * vec3( rnoise.x ) + stc;
	stc = o2 * vec3( rnoise.y ) + stc;
	collect.z = TCM( shacube, stc, shadist );
	
	rnoise = textureOffset( texNoise2D, fc, ivec2( -1, -1 ) ).rg * ntfS + ntfO;
	stc = shapos - o1 - o2;
	stc = o1 * vec3( rnoise.x ) + stc;
	stc = o2 * vec3( rnoise.y ) + stc;
	collect.w = TCM( shacube, stc, shadist );
	shadow += dot( collect, pcfWeight2 );
	
#elif defined( PCF_9TAP )
	vec4 collect;
	
	shadow = TCM( shacube, shapos, shadist ) * pcfWeight;
	
	collect.x = TCM( shacube, shapos + o1, shadist );
	collect.y = TCM( shacube, shapos - o1, shadist );
	collect.z = TCM( shacube, shapos + o2, shadist );
	collect.w = TCM( shacube, shapos - o2, shadist );
	shadow += dot( collect, pcfWeight2 );
	
	collect.x = TCM( shacube, shapos + o1 + o2, shadist );
	collect.y = TCM( shacube, shapos + o1 - o2, shadist );
	collect.z = TCM( shacube, shapos - o1 + o2, shadist );
	collect.w = TCM( shacube, shapos - o1 - o2, shadist );
	shadow += dot( collect, pcfWeight2 );
	
#else // PCF_9TAP
	shadow = TCM( shacube, shapos, shadist );
#endif // PCF_9TAP
}



void main( void ){
	ivec2 tc = ivec2( gl_FragCoord.xy );
	
	// determine position of fragment to light
#ifdef GEOM_ENCODED_DEPTH
	vec3 position = vec3( dot( texelFetch( texDepth, tc, 0 ).rgb, unpackGeomDepth ) );
#else
	vec3 position = vec3( texelFetch( texDepth, tc, 0 ).r );
#endif
	position.z = pPosTransform.x / ( pPosTransform.y - position.z );
	position.xy = vLightVolumePos.xy * vec2( position.z ) / vec2( vLightVolumePos.z );
	
	// skip at a distance
	float dist = length( pLightPos - position );
	
	if( dist == 0.0 || dist > pLightAtten.y ) discard;
	
	// calculate solid occlusion
	vec3 shapos = ( pShadowMatrix * vec4( position, 1.0 ) ).xyz;
	float shadowLow, shadowHi;
	
	evalShadowCube( shadowLow, texShadowLow, pShaStaParam.xyz, shapos );
	evalShadowCube( shadowHi, texShadowSolid, pShaDynParam.xyz, shapos );
	//outOcclusion.r = shadowLow * ( shadowHi * pShaOpts2.y + pShaOpts2.x );
	outOcclusion.r = shadowLow * shadowHi;
	
	// calculate transparent occlusion if used
#ifdef TRANSPARENT_SHADOW
	float shadowTransp;
	
	//shadowHi *= shadowLow; // for transparency both shadow types are fully solid
	
	evalShadowCube( shadowTransp, texShadowTransp, pShaDynParam.xyz, shapos );
	
	outOcclusion.g = outOcclusion.r/*shadowHi*/ * ( 1.0 - shadowTransp );
#endif // TRANSPARENT_SHADOW
}
