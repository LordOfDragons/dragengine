precision highp float;
precision highp int;



uniform vec4 pPosTransform;
uniform float pScaleDistance;
uniform vec2 pBlendFactors; // multiply, add
uniform mat3 pMatrixEnvMap;
uniform float pEnvMapLodLevel;

uniform vec3 pEnvMapPosition;
uniform int pEnvMapIndex;



uniform HIGHP sampler2D texDepth;
uniform lowp sampler2D texNormal;
uniform lowp sampler2D texReflectivity;
uniform lowp sampler2D texRoughness;
uniform lowp sampler2D texAOSolidity;
uniform lowp isampler2D texIndices;
uniform mediump sampler2D texDistance1;
uniform mediump sampler2D texDistance2;
uniform mediump samplerCube texEnvMap;

#ifdef FULLSCREENQUAD
	in vec4 vScreenCoord;
#else
	in vec3 vVolumePos;
#endif

out vec4 outColor;



void main( void ){
	// get indices and discard if this envmap is not matched
	ivec2 indices = texture( texIndices, vScreenCoord.xy ).rg;
	if( all( notEqual( indices, ivec2( pEnvMapIndex ) ) ) ) discard;
	
	// get distances and calculate the blend weight according to them
	vec2 distances;
	distances.x = texture( texDistance1, vScreenCoord.xy ).r;
	distances.y = texture( texDistance2, vScreenCoord.xy ).r;
	distances *= vec2( pScaleDistance );
	
	float weight = clamp( ( distances.y - distances.x ) * pBlendFactors.x + pBlendFactors.y, 0.0, 1.0 );
	
	if( indices.y == pEnvMapIndex ){
		weight = 1.0 - weight;
	}
	
	// determine position of fragment
	ivec2 tc = ivec2( gl_FragCoord.xy );
	
	#ifdef DECODE_IN_DEPTH
		vec3 position = vec3( dot( texelFetch( texDepth, tc, 0 ).rgb, unpackDepth ) );
	#else
		vec3 position = vec3( texelFetch( texDepth, tc, 0 ).r );
	#endif
	position.z = pPosTransform.x / ( pPosTransform.y - position.z );
	#ifdef FULLSCREENQUAD
		position.xy = vScreenCoord.zw * pPosTransform.zw * position.zz;
	#else
		position.xy = vVolumePos.xy * position.zz / vVolumePos.zz;
	#endif
	
	// fetch normal
	#ifdef MATERIAL_NORMAL_INTBASIC
		vec3 normal = texelFetch( texNormal, tc, 0 ).rgb * vec3( 1.9921569 ) + vec3( -0.9921722 );
	#elif defined MATERIAL_NORMAL_FLOATBASIC
		vec3 normal = texelFetch( texNormal, tc, 0 ).rgb * vec3( 2.0 ) + vec3( -1.0 );
	#elif defined( MATERIAL_NORMAL_SPHEREMAP )
		vec2 fenc = texelFetch( texNormal, tc, 0 ).rgb * vec2( 4.0 ) - vec2( 2.0 );
		float f = dot( fenc, fenc );
		float g = sqrt( 1.0 - f * 0.25 );
		vec3 normal = vec3( fenc.xy * vec2( g ), f * 0.5 - 1.0 );
	#else
		vec3 normal = texelFetch( texNormal, tc, 0 ).rgb;
	#endif
	
	normal = normalize( normal );
	
	// fetch reflectivity
	vec3 reflectivity = texelFetch( texReflectivity, tc, 0 ).rgb;
	
	// fetch roughness and ambient occlusion
	roughness = texelFetch( texRoughness, tc, 0 ).r;
	vec3 aoSolidity = texelFetch( texAOSolidity, tc, 0 ).rgb;
	#define ao aoSolidity.r
	#define solidity aoSolidity.b
	
	// calculate fresnel reflection
	vec3 fragmentDirection = normalize( position );
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
