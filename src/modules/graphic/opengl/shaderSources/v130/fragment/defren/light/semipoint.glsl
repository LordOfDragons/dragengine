/////////////////////////////////////////////////////////////////////////////
//                             UNIFORMS                                    //
/////////////////////////////////////////////////////////////////////////////

precision highp float;
precision highp int;

uniform vec2 pPosTransform; // position transform
uniform vec4 pLightPos; // light position in object space + light-cutoff-distance
uniform vec3 pLightColor; // light color
uniform vec4 pLightAtten; // light attenuation: att-c1, att-c2, damp-c1, damp-c2
#ifdef USE_SHADOW
uniform mat4 pShadowMatrix; // shadow matrix
uniform vec2 pShaOpts1; // scale, offset, soffu, soffv
uniform vec3 pShaOpts2; // shadow base, shadow scale, cut-off, ter-pcf-size
uniform vec4 pShaOpts3; // shadow base, shadow scale, cut-off, ter-pcf-size
#ifdef USE_AMBIENT_MAP
uniform vec2 pPosToATC;
#endif
#endif // USE_SHADOW

uniform HIGHP sampler2D texDepth;
uniform lowp sampler2D texDiffuse;
uniform lowp sampler2D texNormal;
uniform lowp sampler2D texSpecular;
#ifdef USE_SHADOW
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
		uniform lowp sampler2D texShadowTraClr;
	#endif
	uniform lowp sampler2D texNoise2D;
	
	#ifdef USE_AMBIENT_MAP
	uniform lowp sampler2D texAmbientMap;
	#endif
#endif // USE_SHADOW

in vec3 vLightVolumePos;

out vec4 outColor;



/////////////////////////////////////////////////////////////////////////////
//                            CONSTANTS                                    //
/////////////////////////////////////////////////////////////////////////////

const float epsilon = 0.00001;

// A light which illuminates objects at 80 lum/sr should be drawn
// at 3183 lumens/meter^2/steradian, which equates to a multiplier
// of 39.78 per lumen.
//const float specularMultiplier = 39.78f;
const float specularMultiplier = 2.0; //9.945f; // 19.98

// texDiffuse: color.r, color.g, color.b, alpha
// texNormal: normal.x, normal.y, normal.z, n/a
// texSpecular: specular.int, specular.exp, n/a, n/a

// #define epsilon 0.00001

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



/////////////////////////////////////////////////////////////////////////////
//                         SHADOW FUNCTIONS                                //
/////////////////////////////////////////////////////////////////////////////

#ifdef PYRAMID_MAPPING
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
	
	// mögliche verbesserungen.
	// in der ersten linie ist immer shapos.x und shapos.y fix. variable is
	// wo shapos.z dazu oder abgezählt wird:
	//   y+ dom => y - z
	//   x+ dom => x - z
	//   y- dom => y + z
	//   x- dom => x + z
	// 
	// auf der zweiten zeile ist immer shapos.z vorhanden:
	//   y+ dom => z + y
	//   x+ dom => z + x
	//   y- dom => z - y
	//   x- dom => z - x
	// 
	// auf der dritten zeile ist der erste factor wie:
	//   y+/y- = vec2( 0.5, 0.25 )
	//   x+/x- = vec2( 0.25, 0.5 )
}
#endif

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
	
	/*
	shaoff = options.xyxy * scot2; // offU, offV, -offU, -offV
	rnoise = textureOffset( texNoise2D, fc, ivec2( 1, 1 ) ).rg * ntfS + ntfO;
	collect.x = texture( shamap, vec3( shapos.st + shaoff.xy + rnoise * options.zz, stc.p ) ); // (u,v)
	rnoise = textureOffset( texNoise2D, fc, ivec2( 1, -1 ) ).rg * ntfS + ntfO;
	collect.y = texture( shamap, vec3( shapos.st + shaoff.xw + rnoise * options.zz, stc.p ) ); // (u,-v)
	rnoise = textureOffset( texNoise2D, fc, ivec2( -1, 1 ) ).rg * ntfS + ntfO;
	collect.z = texture( shamap, vec3( shapos.st + shaoff.zy + rnoise * options.zz, stc.p ) ); // (-u,v)
	rnoise = textureOffset( texNoise2D, fc, ivec2( -1, -1 ) ).rg * ntfS + ntfO;
	collect.w = texture( shamap, vec3( shapos.st + shaoff.zw + rnoise * options.zz, stc.p ) ); // (-u,-v)
	shadow += dot( collect, pcfWeight2 );
	*/
	
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

/*
void evalShadowMapStatic( out float shadow, in mediump sampler2DShadow shamap, in vec2 shascale, in vec3 shapos ){
//	vec2 shascale = options.xy * vec2( min( 5.0, shadist / 2.5 ) );
	vec3 stc = shapos;
	int x, y;
	
	shadow = 0.0;
	
	for( y=-2; y<2; y++ ){
		stc.t = shascale.y * float( y ) + shapos.t;
		
		for( x=-2; x<2; x++ ){
			stc.s = shascale.x * float( x ) + shapos.s;
			shadow += texture( shamap, stc ).r;
		}
	}
	
	shadow /= 25.0;
}
*/

/*
void evalShadowCube( inout float shadow, in mediump samplerCube shacube, in vec2 options, in float pcfsize, in vec3 shapos ){
	float shadist = length( shapos ) * options.x + options.y;
	
//#ifdef PCF_9TAP
	vec3 o1;// = vec3( 0.0 );
	vec3 o2;// = vec3( 0.0 );
	vec3 absshapos;
	vec4 collect;
	
	absshapos = abs( shapos );
	//vec3 r = step( absshapos.xyz, absshapos.yzx ) * step( absshapos.xyz, absshapos.zxy );
	//vec2 scale = vec2( dot( absshapos, r ) * pShadowOptions.z );
	//o1.xz = ( c2 * r.xx + c1 ) * scale;
	//o2.zy = ( c2 * r.yy + c1 ) * scale;
	if( absshapos.x > absshapos.y && absshapos.x > absshapos.z ){ // x is largest
		o1 = vec3( 0.0, 0.0, pcfsize );
		o2 = vec3( 0.0, pcfsize, 0.0 );
		
	}else if( absshapos.y > absshapos.x && absshapos.y > absshapos.z ){ // y is largest
		o1 = vec3( pcfsize, 0.0, 0.0 );
		o2 = vec3( 0.0, 0.0, pcfsize );
		
	}else{ // z is largest
		o1 = vec3( pcfsize, 0.0, 0.0 );
		o2 = vec3( 0.0, pcfsize, 0.0 );
	}
	
	shadow = step( shadist, textureCube( shacube, shapos ).r ) * pcfWeight;
	
	collect.x = step( shadist, textureCube( shacube, shapos + o1 ).r );
	collect.y = step( shadist, textureCube( shacube, shapos - o1 ).r );
	collect.z = step( shadist, textureCube( shacube, shapos + o2 ).r );
	collect.w = step( shadist, textureCube( shacube, shapos - o2 ).r );
	shadow += dot( collect, pcfWeight2 );
	
	collect.x = step( shadist, textureCube( shacube, shapos + o1 + o2 ).r );
	collect.y = step( shadist, textureCube( shacube, shapos + o1 - o2 ).r );
	collect.z = step( shadist, textureCube( shacube, shapos - o1 + o2 ).r );
	collect.w = step( shadist, textureCube( shacube, shapos - o1 - o2 ).r );
	shadow += dot( collect, pcfWeight2 );
	
//#else // PCF_9TAP
//	shadow = step( shadist, textureCube( shacube, shapos ).r );
//#endif // PCF_9TAP
}
*/


//#define ESM evalShadowMap
//#define ESM evalShadowMapStatic

#ifdef TRANSP_SHADOW
	#define LIGHT_COLOR lightColor.rgb
#else
	#define LIGHT_COLOR pLightColor
#endif // TRANSP_SHADOW

/////////////////////////////////////////////////////////////////////////////
//                           MAIN FUNCTION                                 //
/////////////////////////////////////////////////////////////////////////////

// texDiffuse: color.r, color.g, color.b, alpha
// texNormal: normal.x, normal.y, normal.z, n/a
// texSpecular: specular.int, specular.exp, ao, n/a

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
	
	// calculate light direction and distance
//	vec3 closestLightPoint = clamp( position, pLightPos.xyz - vec3( 0.5, 0.0, 0.5 ), pLightPos.xyz + ( 0.5, 0.0, 0.5 ) );
	
	vec3 lightDir = /*closestLightPoint*/pLightPos.xyz - position;
	float dist = length( lightDir );
	
	if( dist == 0.0 || dist > pLightPos.w ) discard; // outside light range
	lightDir /= dist;
	
#ifdef USE_SHADOW
	vec3 shapos = ( pShadowMatrix * vec4( position, 1.0 ) ).xyz;
	
	if( shapos.z < pShaOpts3.z ) discard; // over 180 degree
	
	float shadow = 0.0;
	float shadowObj = 1.0;
#ifdef PYRAMID_MAPPING
	pyramidShadowCoords( shapos, shapos, pShaOpts1.xy );
#else // PYRAMID_MAPPING
	float shadist = length( shapos );
	shapos /= vec3( max( epsilon, shadist ) );
	
	shapos.st = ( shapos.xy / vec2( shapos.z + 1.0 ) ) * sct2 + sct1;
	shapos.p = shadist * pShaOpts1.x + pShaOpts1.y;
#endif
	evalShadowMap( shadowObj, texShadowObj, pShaOpts2, shapos );
	evalShadowMap( shadow, texShadowTer, pShaOpts2, shapos );
	
#ifdef USE_AMBIENT_MAP
	float ambient = texture( texAmbientMap, gl_FragCoord.xy * pPosToATC.xy ).r;
	shadow *= shadowObj;
	shadow = mix( shadow, shadow * pShaOpts3.y + pShaOpts3.x, ambient );
#else
	shadow *= shadowObj * pShaOpts3.y + pShaOpts3.x;
#endif
	
	if( shadow < epsilon ) discard; // in total shadow
	
#ifdef TRANSP_SHADOW
	vec3 lightColor = pLightColor;
	
	shadowObj *= shadow; // for transparency both shadow types are fully solid
	
	if( shadowObj > epsilon ){ // only evaluate transparency if not in solid shadow
		float shadowTra;
		
		evalShadowMap( shadowTra, texShadowTra, pShaOpts2, shapos );
		
		vec4 shadowColor = texture( texShadowTraClr, shapos.st );
		lightColor = mix( pLightColor, shadowColor.rgb, shadowColor.a * shadowObj * ( 1.0 - shadowTra ) );
	}
#endif // TRANSP_SHADOW
#endif // USE_SHADOW
	
	// diffuse component
	vec4 diffuse = texelFetch( texDiffuse, tc, 0 );
	vec3 finalColor = diffuse.rgb * LIGHT_COLOR;
	
	// specular component and view direction attenuation
	vec3 normal = texelFetch( texNormal, tc, 0 ).rgb;
	vec3 specularity = texelFetch( texSpecular, tc, 0 ).rgb;
	
	if( dot( normal, normal ) > epsilon ){
		float specExp = specularity.g * 255.0 + 0.001;
		finalColor += LIGHT_COLOR * pow( max( dot( reflect( lightDir, normal ), normalize( position ) ), 0.0 ), specExp ) * specularity.r;
		
		float dotval = max( dot( normal, lightDir ) * 0.5 + 0.5, 0.0 );
		finalColor *= vec3( ( dotval * dotval ) * 0.8 + 0.2 );
		//finalColor *= vec3( max( dot( normal, lightDir ), 0.0 ) );
	}
	
	// attenuations
	float attenuation = max( pLightAtten.z / ( 1.0 + pLightAtten.y * dist * dist ) + pLightAtten.w, 0.0 ); // att + damp
	attenuation *= specularity.b;
	
#ifdef USE_SHADOW
	attenuation *= shadow;
#endif
	
	// final light contribution
	outColor = vec4( finalColor * vec3( attenuation ), diffuse.a );
}
