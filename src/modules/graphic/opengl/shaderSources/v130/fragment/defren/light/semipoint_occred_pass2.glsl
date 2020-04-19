precision highp float;
precision highp int;

uniform vec2 pPosTransform; // position transform
uniform vec4 pPosToOC; // scaleS, scaleT, offsetS, offsetT
uniform vec4 pLightPos; // light position in object space + light-cutoff-distance
uniform vec3 pLightColor; // light color
uniform vec4 pLightAtten; // light attenuation: att-c1, att-c2, damp-c1, damp-c2
uniform mat4 pShadowMatrix; // shadow matrix
uniform vec2 pShaOpts1; // scale, offset, soffu, soffv

uniform HIGHP sampler2D texDepth;
uniform lowp sampler2D texDiffuse;
uniform lowp sampler2D texNormal;
uniform lowp sampler2D texSpecular;
uniform lowp sampler2D texOcclusion;
#ifdef TRANSP_SHADOW
uniform lowp sampler2D texShadowTraClr;
#endif

in vec3 vLightVolumePos;

out vec4 outColor;



const float epsilon = 0.00001;
const float specularMultiplier = 2.0;

#ifdef GEOM_ENCODED_DEPTH
const vec3 unpackGeomDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif

const vec2 blur = vec2( 0.25 );

#ifdef TRANSP_SHADOW
	#define LIGHT_COLOR lightColor
#else
	#define LIGHT_COLOR pLightColor
#endif // TRANSP_SHADOW



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



void main( void ){
	vec2 tcOcclusion = gl_FragCoord.xy * pPosToOC.xy + pPosToOC.zw;
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
	vec3 lightDir = pLightPos.xyz - position;
	float dist = length( lightDir );
	
	if( dist == 0.0 || dist > pLightPos.w ) discard; // outside light range
	lightDir /= dist;
	
	// calculate shadow
	vec2 shadow = texture( texOcclusion, tcOcclusion ).rg; // * blur;
	//shadow = textureOffset( texOcclusion, tcOcclusion, ivec2( 1, 0 ) ).rg * blur + shadow;
	//shadow = textureOffset( texOcclusion, tcOcclusion, ivec2( 0, 1 ) ).rg * blur + shadow;
	//shadow = textureOffset( texOcclusion, tcOcclusion, ivec2( 1, 1 ) ).rg * blur + shadow;
	
	// if we have also transparent depth/color textures take them into account
#ifdef TRANSP_SHADOW
	vec3 shapos = ( pShadowMatrix * vec4( position, 1.0 ) ).xyz;
	
	pyramidShadowCoords( shapos, shapos, pShaOpts1.xy );
	
	vec4 shadowColor = texture( texShadowTraClr, shapos.st );
	vec3 lightColor = mix( pLightColor, shadowColor.rgb, shadowColor.a * shadow.g );
#endif // TRANSP_SHADOW
	
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
	}
	
	// attenuations
	float attenuation = shadow.r * max( pLightAtten.z / ( 1.0 + pLightAtten.y * dist * dist ) + pLightAtten.w, 0.0 ); // att + damp
	attenuation *= specularity.b;
	
	// final light contribution
	outColor = vec4( finalColor * vec3( attenuation ), diffuse.a );
}
