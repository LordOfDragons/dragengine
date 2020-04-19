precision highp float;
precision highp int;

uniform vec4 pPosToOC; // scaleS, scaleT, offsetS, offsetT
uniform vec3 pLightPos; // light position in object space
uniform vec3 pLightColor; // light color
uniform vec3 pLightColorAmbient; // light ambient color
uniform vec2 pLightAtten; // light attenuation: att-coef, cut-off
uniform vec2 pLightDamp; // light dampening: threshold, factor
uniform vec2 pPosTransform; // position transform
uniform mat4 pShadowMatrix; // shadow matrix

uniform HIGHP sampler2D texDepth;
uniform lowp sampler2D texDiffuse;
uniform lowp sampler2D texNormal;
uniform lowp sampler2D texSpecular;
uniform lowp sampler2D texOcclusion;
uniform lowp samplerCube texShadowTranspColor;

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

#ifdef GEOM_ENCODED_DEPTH
const vec3 unpackGeomDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif

const vec2 blur = vec2( 0.25 );



void main( void ){
	vec2 vTCOcclusion = gl_FragCoord.xy * pPosToOC.xy + pPosToOC.zw;
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
	vec3 lightDir = pLightPos - position;
	float dist = length( lightDir );
	
	if( dist == 0.0 || dist > pLightAtten.y ) discard;
	lightDir /= dist;
	
	// calculate shadow
	vec2 shadow = texture( texOcclusion, vTCOcclusion ).rg; // * blur;
	//shadow = textureOffset( texOcclusion, vTCOcclusion, ivec2( 1, 0 ) ).rg * blur + shadow;
	//shadow = textureOffset( texOcclusion, vTCOcclusion, ivec2( 0, 1 ) ).rg * blur + shadow;
	//shadow = textureOffset( texOcclusion, vTCOcclusion, ivec2( 1, 1 ) ).rg * blur + shadow;
	
#ifdef NO_AMBIENT
	// if we are fully in shadows and there is no ambient light contribution
	// we can bail out here. helps since entire patches of pixels tend to be
	// in shadows not just single pixels
	if( shadow.r < epsilon ) discard;
#endif
	
	// if we have also transparent depth/color textures take them into account
#ifdef TRANSPARENT_SHADOW
	vec3 shapos = ( pShadowMatrix * vec4( position, 1.0 ) ).xyz;
	vec4 shadowColor = texture( texShadowTranspColor, shapos );
	vec3 lightColor = mix( pLightColor, shadowColor.rgb, shadowColor.a * shadow.g );
#endif // TRANSPARENT_SHADOW
	
	// diffuse component
	vec4 diffuse = texelFetch( texDiffuse, tc, 0 );
	
#ifdef TRANSPARENT_SHADOW
	vec3 finalColor = diffuse.rgb * lightColor;
#else
	vec3 finalColor = diffuse.rgb * pLightColor;
#endif
	
	// specular component and view direction attenuation
	vec3 normal = texelFetch( texNormal, tc, 0 ).rgb;
	vec3 specularity = texelFetch( texSpecular, tc, 0 ).rgb;
	
	if( dot( normal, normal ) > epsilon ){
		float specExp = specularity.g * 255.0 + 0.001;
		
#ifdef TRANSPARENT_SHADOW
		finalColor += lightColor * pow( max( dot( reflect( lightDir, normal ), normalize( position ) ), 0.0 ), specExp ) * specularity.r;
#else
		finalColor += pLightColor * pow( max( dot( reflect( lightDir, normal ), normalize( position ) ), 0.0 ), specExp ) * specularity.r;
#endif
		
		float dotval = max( dot( normal, lightDir ) * 0.5 + 0.5, 0.0 );
		finalColor *= vec3( ( dotval * dotval ) * 0.8 + 0.2 );
		//finalColor *= max( dot( normal, lightDir ), useNormal );
	}
	
	// shadow attenuation
	finalColor *= shadow.r;
	
	// ambient component
	finalColor += diffuse.rgb * pLightColorAmbient;
	
	// distance and spot attenuation of all components
	float attenuation = max( pLightDamp.x / ( 1.0 + pLightAtten.x * dist * dist ) + pLightDamp.y, 0.0 ); // att + damp
	attenuation *= specularity.b;
	
	// final light contribution
	outColor = vec4( finalColor * attenuation, diffuse.a );
}
