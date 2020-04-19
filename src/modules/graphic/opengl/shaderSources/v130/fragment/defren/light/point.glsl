precision highp float;
precision highp int;

uniform vec3 pLightPos; // light position in object space
uniform vec3 pLightColor; // light color
uniform vec3 pLightColorAmbient; // light ambient color
uniform vec2 pLightAtten; // light attenuation: att-coef, cut-off
uniform vec2 pLightDamp; // light dampening: threshold, factor
uniform vec2 pPosTransform; // position transform
#ifdef USE_SHADOW
uniform vec3 pShaStaParams; // scale, offset, pcfSize
uniform vec3 pShaDynParams; // scale, offset, pcfSize
uniform vec2 pShaOpts2; // scale, offset, pcfSize, n/a
uniform mat4 pShadowMatrix; // shadow matrix
#endif // USE_SHADOW

uniform HIGHP sampler2D texDepth;
uniform lowp sampler2D texDiffuse;
uniform lowp sampler2D texNormal;
uniform lowp sampler2D texSpecular;
#ifdef USE_SHADOW
uniform mediump samplerCube texShadowLow;
uniform mediump samplerCube texShadowSolid;
uniform mediump samplerCube texShadowTransp;
uniform mediump samplerCube texShadowTranspColor;
#endif // USE_SHADOW
uniform lowp sampler2D texNoise2D;

in vec3 vLightVolumePos;

out vec4 outColor;

#define epsilon 0.0001

// A light which illuminates objects at 80 lum/sr should be drawn
// at 3183 lumens/meter^2/steradian, which equates to a multiplier
// of 39.78 per lumen.
//const float specularMultiplier = 39.78f;
const float specularMultiplier = 2.0; //9.945f; // 19.98

// texDiffuse: color.r, color.g, color.b, alpha
// texNormal: normal.x, normal.y, normal.z, n/a
// texSpecular: specular.int, specular.exp, n/a, n/a

#ifdef USE_SHADOW
//const vec4 unpackCubeDepth = vec4( 1.0 / 16777216.0, 1.0 / 65536.0, 1.0 / 256.0, 1.0 );
//const vec4 unpackCubeDepth = vec4( 1.0, 1.0 / 256.0, 1.0 / 65536.0, 1.0 / 16777216.0 );

const vec2 c1 = vec2( 1.0, 0.0 );
const vec2 c2 = vec2( -1.0, 1.0 );

#ifdef USE_ENCODED_DEPTH
const vec3 unpackCubeDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif // USE_ENCODED_DEPTH

#endif // USE_SHADOW

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


#ifdef USE_SHADOW

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

#endif // USE_SHADOW



#ifdef TRANSPARENT_SHADOW
	#define LIGHT_COLOR lightColor.rgb
#else
	#define LIGHT_COLOR pLightColor
#endif // TRANSPARENT_SHADOW

void main( void ){
	ivec2 tc = ivec2( gl_FragCoord.xy );
	
#ifdef GEOM_ENCODED_DEPTH
	vec3 position = vec3( dot( texelFetch( texDepth, tc, 0 ).rgb, unpackGeomDepth ) );
#else
	vec3 position = vec3( texelFetch( texDepth, tc, 0 ).r );
#endif
	position.z = pPosTransform.x / ( pPosTransform.y - position.z );
	position.xy = vLightVolumePos.xy * vec2( position.z ) / vec2( vLightVolumePos.z );
	
	vec4 diffuse = texelFetch( texDiffuse, tc, 0 );
	if( diffuse.a == 0.0 ) discard; // fragment that should not be rendered (not initialized or do-not-light)
	
	// calculate light direction and distance
	vec3 lightDir = pLightPos - position;
	float dist = length( lightDir );
	
	if( dist == 0.0 || dist > pLightAtten.y ) discard;
	lightDir /= dist;
	
	
	
	// calculate shadow
#ifdef USE_SHADOW
	vec3 shapos = ( pShadowMatrix * vec4( position, 1.0 ) ).xyz;
	float shadow, shadowLow, shadowHi;
	
	evalShadowCube( shadowLow, texShadowLow, pShaStaParams.xyz, shapos );
	evalShadowCube( shadowHi, texShadowSolid, pShaDynParams.xyz, shapos );
	//shadow = shadowLow * ( shadowHi * pShaOpts2.y + pShaOpts2.x );
	shadow = shadowLow * shadowHi;
	
#ifdef NO_AMBIENT
	// if we are fully in shadows and there is no ambient light contribution
	// we can bail out here. helps since entire patches of pixels tend to be
	// in shadows not just single pixels
	if( shadow < epsilon ) discard;
#endif
	
	// if we have also transparent depth/color textures take them into account
#ifdef TRANSPARENT_SHADOW
	float shadowTransp;
	
	//shadowHi *= shadowLow; // for transparency both shadow types are fully solid
	
	evalShadowCube( shadowTransp, texShadowTransp, pShaDynParams.xyz, shapos );
	
	vec4 shadowColor = texture( texShadowTranspColor, shapos );
	vec3 lightColor = mix( pLightColor, shadowColor.rgb, shadowColor.a * shadow/*Hi*/ * ( 1.0 - shadowTransp ) );
#endif // TRANSPARENT_SHADOW
	
#endif // USE_SHADOW
	
	// diffuse term = color_diff * color_light * max( dot( normal, lightDir ), 0 )
	// specular term = ( ( ap + 2 ) / 8 ) * pow( max( dot( normal, halfDir ), 0 ), ap )
	//                   * fresnel( color_reflectivity, lightDir, halfDir )
	//                   * color_light * max( dot( normal, lightDir ), 0 )
	// 
	// vec3 halfDir = normalize( lightDir - position );
	// vec3 lightTerm = colorLight * vec3( max( dot( normal, lightDir ), 0.0 ) );
	// float specNormTerm = ( ap + 2.0 ) / 8.0;
	// float specPowTerm = pow( max( dot( normal, halfDir ), 0.0 ), ap );
	// vec3 specFresnelTerm = mix( colorSpecular, vec3( 1.0 ), vec3( pow( 1.0 - dot( lightDir, halfDir ), 5.0 ) ) );
	// vec3 specularTerm = vec3( specNormTerm * specPowTerm ) * specFresnelTerm;
	// vec3 fragmentColor = ( colorDiffuse + specularTerm ) * lightTerm;
	// 
	// calculation of the ap term using a (pseudo) linear roughness value. roughness value is 0 for sharp reflection
	// all the way to 1 for fully diffuse reflection. roughness is considered to be 1 minus the beckmann roughness.
	// the original formula is ap = 2 / roughness**2 - 2. to avoid a division by zero a small max ap factor is used.
	// this factor clamps the ap value to a given maximum value. a value of 0.00002 for the max ap factor clamps the
	// ap to roughly 100000. this is enough for a fully sharp reflection. to avoid a near flat line result at the
	// fully diffuse end of the ap spectrum a min ap factor is used. the smallest physically reasonable ap values
	// are in the range from 0.1 to 2. a min ap factor of 2-0.5 = 1.5 keeps the smallest ap to 0.5 which looks nice.
	// 
	// roughness = clamp( texture-input( 0=sharp .. 1=diffuse ), 0.0, 1.0 )
	// float ap = 2.0 / ( specularity.g * specularity.g + maxAP ) - minAP
	
	vec3 normal = texelFetch( texNormal, tc, 0 ).rgb;
	vec3 specularity = texelFetch( texSpecular, tc, 0 ).rgb;
	vec3 colorSpecular = vec3( specularity.r );
	
	// diffuse term
	vec3 finalColor = diffuse.rgb;
	
	// specular term
	float roughness = specularity.g;
	float ap = 426.0 * ( 1.0 - roughness ) / ( 90.0 * roughness * roughness + roughness + 0.0001 ) + 1.0;
	vec3 halfDir = normalize( lightDir - normalize( position ) );
	float specNormTerm = ( ap + 2.0 ) / 8.0;
	float specPowTerm = pow( max( dot( normal, halfDir ), 0.0 ), ap );
	vec3 specFresnelTerm = mix( colorSpecular, vec3( 1.0 ), vec3( pow( 1.0 - dot( lightDir, halfDir ), 5.0 ) ) );
	
	float dotval = max( dot( normal, lightDir ), 0.0 );
	
	finalColor += vec3( specNormTerm * specPowTerm ) * specFresnelTerm;
	finalColor *= LIGHT_COLOR * vec3( dotval );
	
	// shadow attenuation
#ifdef USE_SHADOW
	finalColor *= shadow;
#endif // USE_SHADOW
	
	// ambient component
	finalColor += diffuse.rgb * pLightColorAmbient;
	
	// distance and spot attenuation of all components
	float attenuation = max( pLightDamp.x / ( 1.0 + pLightAtten.x * dist * dist ) + pLightDamp.y, 0.0 ); // att + damp
	attenuation *= specularity.b;
	
	// final light contribution
	outColor = vec4( finalColor * vec3( attenuation ), diffuse.a );
}
