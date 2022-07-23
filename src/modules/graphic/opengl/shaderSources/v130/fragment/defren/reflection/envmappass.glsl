precision highp float;
precision highp int;



uniform vec4 pPosTransform;
uniform vec2 pPosTransform2;
uniform float pScaleDistance;
uniform vec2 pBlendFactors; // multiply, add
uniform mat3 pMatrixEnvMap;
uniform float pEnvMapLodLevel;

uniform vec3 pEnvMapPosition;
uniform int pEnvMapIndex;



uniform HIGHP sampler2DArray texDepth;
uniform lowp sampler2DArray texNormal;
uniform lowp sampler2DArray texReflectivity;
uniform lowp sampler2DArray texRoughness;
uniform lowp sampler2DArray texAOSolidity;
uniform lowp isampler2DArray texIndices;
uniform mediump sampler2DArray texDistance1;
uniform mediump sampler2DArray texDistance2;
uniform mediump samplerCube texEnvMap;

#ifdef FULLSCREENQUAD
	in vec4 vScreenCoord;
#else
	in vec3 vVolumePos;
#endif

#ifdef GS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out vec4 outColor;

#include "v130/shared/normal.glsl"


void main( void ){
	vec3 tcScreen( vScreenCoord.xy, vLayer );
	
	// get indices and discard if this envmap is not matched
	ivec2 indices = texture( texIndices, tcScreen ).rg;
	if( all( notEqual( indices, ivec2( pEnvMapIndex ) ) ) ) discard;
	
	// get distances and calculate the blend weight according to them
	vec2 distances;
	distances.x = texture( texDistance1, tcScreen ).r;
	distances.y = texture( texDistance2, tcScreen ).r;
	distances *= vec2( pScaleDistance );
	
	float weight = clamp( ( distances.y - distances.x ) * pBlendFactors.x + pBlendFactors.y, 0.0, 1.0 );
	
	if( indices.y == pEnvMapIndex ){
		weight = 1.0 - weight;
	}
	
	// determine position of fragment
	ivec3 tc = ivec3( gl_FragCoord.xy, vLayer );
	
	#ifdef DECODE_IN_DEPTH
		vec3 position = vec3( dot( texelFetch( texDepth, tc, 0 ).rgb, unpackDepth ) );
	#else
		vec3 position = vec3( texelFetch( texDepth, tc, 0 ).r );
	#endif
	position.z = pPosTransform.x / ( pPosTransform.y - position.z );
	#ifdef FULLSCREENQUAD
		position.xy = ( vScreenCoord.zw + pPosTransform2 ) * pPosTransform.zw * position.zz;
	#else
		position.xy = vVolumePos.xy * position.zz / vVolumePos.zz;
	#endif
	
	// fetch reflectivity
	vec3 reflectivity = texelFetch( texReflectivity, tc, 0 ).rgb;
	
	// fetch roughness and ambient occlusion
	roughness = texelFetch( texRoughness, tc, 0 ).r;
	vec3 aoSolidity = texelFetch( texAOSolidity, tc, 0 ).rgb;
	#define ao aoSolidity.r
	#define solidity aoSolidity.b
	
	// calculate fresnel reflection
	vec3 fragmentDirection = normalize( position );
	vec3 normal = normalize( normalLoadMaterial( texNormal, tc ) );
	float reflectDot = min( abs( dot( -fragmentDirection, normal ) ), 1.0 );
	vec3 envMapDir = pMatrixEnvMap * vec3( reflect( fragmentDirection, normal ) );
	
	//float fresnelReduction = smoothstep( 0.5, 1.0, 1.0 - roughness );
	//float fresnelReduction = 1.0 / ( 1.0 + roughness * 3.0 ); // CoD version... no less than 25% reflectivity at fully grazing angle
	//float fresnelReduction = 1.0 - roughness;
	
	// pow(5) fits betters but pow(4) is cheaper:
	//float fresnelReduction = pow( 1.0 - roughness, 5.0 );
//	float fresnelReduction = pow( 1.0 - roughness, 5.0 ) * 0.9524 + 0.0476;
	vec3 fresnelReduction = mix( reflectivity, vec3( 1.0 ), pow( 1.0 - roughness, 5.0 ) );
//	vec3 fresnelReduction = mix( mix( vec3( 0.056 ), vec3( 1.0 ), reflectivity ), vec3( 1.0 ), pow( 1.0 - roughness, 5.0 ) );
	
	// reduce reflectivity depending on AO and roughness angles
	//float aoAngle = 1.0 - acos( 1.0 - ao ) * 0.636620; // * 2/pi
	//fresnelReduction *=  1.0 - smoothstep( aoAngle - roughness, aoAngle + roughness, reflectDot );
	// WARNING! the mix function is very fragile and passing under 0 or over 1 by even just a small
	//          amount results in NaN values and other nasty stuff. clamping costs some cycles
	//          but it prevents the mix function from smuggling NaN values into the image
	fresnelReduction *= vec3( clamp( ( reflectDot - 1.0 + acos( 1.0 - ao ) * 0.636620 + roughness ) / max( 2.0 * roughness, 0.1 ), 0.0, 1.0 ) );
	vec3 finalFresnel = clamp( vec3( pow( 1.0 - reflectDot, 5.0 ) ) * fresnelReduction, vec3( 0.0 ), vec3( 1.0 ) );
	reflectivity = mix( reflectivity, vec3( 1.0 ), finalFresnel );
	
	reflectivity *= vec3( solidity );
	
	// calculate the reflected color
	float envMapLodLevel = log2( 1.0 + pEnvMapLodLevel * roughness );
	vec3 reflectedColor = textureLod( texEnvMap, envMapDir, envMapLodLevel ).rgb;
	
	// this should simulate to some degree a prefiltered environment map (cosine filter, phong lobe).
	//reflectedColor *= vec3( ( cos( roughness * 3.1416 ) * 0.5 + 0.5 ) * 0.38 + 0.62 );
	// this version is based on a normalized situation and should be somewhat better
//	reflectedColor *= vec3( 0.2 + 0.8 / ( 1.0 + 50.0 * roughness * roughness ) );
	
	// apply reflectivity
	reflectedColor *= reflectivity.rgb;
	
	// write out the color and the weight
	outColor = vec4( reflectedColor, weight );
}
