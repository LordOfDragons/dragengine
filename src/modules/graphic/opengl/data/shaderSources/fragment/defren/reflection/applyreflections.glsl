precision highp float;
precision highp int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

UBOLAYOUT uniform EnvironmentMaps{
	mat4x3 pEnvMapMatrixInfluence[ 8 ];
	mat4x3 pEnvMapMatrixReflBox[ 8 ];
	vec3 pEnvMapInfluenceBorder[ 8 ];
	vec3 pEnvMapInfluenceScale[ 8 ];
	vec3 pEnvMapPosition[ 8 ];
	float pEnvMapRoughness[ 8 ];
	int pEnvMapPriority[ 8 ];
	int pEnvMapCount;
};

uniform lowp sampler2DArray texDiffuse;
uniform HIGHP sampler2DArray texDepth;
uniform lowp sampler2DArray texReflection;
uniform lowp sampler2DArray texNormal;
uniform lowp sampler2DArray texReflectivity;
uniform lowp sampler2DArray texRoughness;
uniform lowp sampler2DArray texAOSolidity;
uniform mediump sampler2DArray texColor;
#ifdef ENVMAP_EQUI
	#define SAMPLER_ENVMAP mediump sampler2D
#else
	#define SAMPLER_ENVMAP mediump samplerCube
#endif
uniform SAMPLER_ENVMAP texEnvMap;
#if defined ENVMAP_BOX_PROJECTION || ! defined ENVMAP_SINGLE
uniform SAMPLER_ENVMAP texEnvMap2;
uniform SAMPLER_ENVMAP texEnvMap3;
uniform SAMPLER_ENVMAP texEnvMap4;
uniform SAMPLER_ENVMAP texEnvMap5;
uniform SAMPLER_ENVMAP texEnvMap6;
uniform SAMPLER_ENVMAP texEnvMap7;
uniform SAMPLER_ENVMAP texEnvMap8;
#endif

in vec2 vTexCoord;
in vec2 vScreenCoord;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out vec3 outColor;



#ifdef ENVMAP_EQUI
	const vec4 cemefac = vec4( 0.5, 1.0, -0.1591549, -0.3183099 ); // 0.5, 1.0, -1/2pi, -1/pi
#endif
const float roughnessToBlur = 3.14159265; // pi | 1.570796; // pi/2


#include "shared/normal_texture.glsl"
#include "shared/defren/depth_to_position.glsl"


void calculateReflectionParameters( in ivec3 tc, in vec3 position,
out vec3 reflectivity, out float roughness, out vec3 reflectDir ){
	// fetch ao and solidity. allows for an early exit if the solidity is 0
	vec3 aoSolidity = texelFetch( texAOSolidity, tc, 0 ).rgb;
	#define ao aoSolidity.r
	#define solidity aoSolidity.b
	
// 	if( solidity < 0.01 ){
// 		discard;
// 	}
	
	// fetch reflectivity
	reflectivity = texelFetch( texReflectivity, tc, 0 ).rgb;
	
	// fetch roughness
	roughness = texelFetch( texRoughness, tc, 0 ).r;
	
	// calculate fresnel reflection
	vec3 fragmentDirection = normalize( position );
	vec3 normal = sanitizeNormal( normalLoadMaterial( texNormal, tc ) );
	float reflectDot = min( abs( dot( -fragmentDirection, normal ) ), 1.0 );
	reflectDir = reflect( fragmentDirection, normal );
	
	//float fresnelReduction = smoothstep( 0.5, 1.0, 1.0 - roughness );
	//float fresnelReduction = 1.0 / ( 1.0 + roughness * 3.0 );
	//	// CoD version... no less than 25% reflectivity at fully grazing angle
	//float fresnelReduction = 1.0 - roughness;
	
	// pow(5) fits betters but pow(4) is cheaper:
	//float fresnelReduction = pow( 1.0 - roughness, 5.0 );
//	vec3 fresnelReduction = vec3( pow( 1.0 - roughness, 5.0 ) * 0.9524 + 0.0476 );
	vec3 fresnelReduction = mix( reflectivity, vec3( 1.0 ), pow( 1.0 - roughness, 5.0 ) );
//	vec3 fresnelReduction = mix( mix( vec3( 0.056 ), vec3( 1.0 ), reflectivity ), vec3( 1.0 ), pow( 1.0 - roughness, 5.0 ) );
	
	// reduce reflectivity depending on AO and roughness angles
	//float aoAngle = acos( 1.0 - ao );
	//float roughnessAngle = roughness * 1.5707963; // * pi/2
	//float reflectDotAngle = acos( reflectDot )
	//float fresnelReduction = clamp( reflectDotAngle - aoAngle + roughnessAngle ) / max( 2.0 * roughnessAngle, 0.01 ), 0.0, 1.0 );
	//fresnelReduction *=  1.0 - smoothstep( aoAngle - roughness, aoAngle + roughness, acos( reflectDot ) );
	// WARNING! the mix function is very fragile and passing under 0 or over 1 by even just a small
	//          amount results in NaN values and other nasty stuff. clamping costs some cycles
	//          but it prevents the mix function from smuggling NaN values into the image
	//fresnelReduction *= clamp( ( reflectDot - 1.0 + acos( 1.0 - ao ) * 0.636620 + roughness ) / max( 2.0 * roughness, 0.1 ), 0.0, 1.0 );
	fresnelReduction *= vec3( clamp(
		( acos( 1.0 - ao ) + roughness * 1.5707963 - acos( reflectDot ) + 0.01 )
			/ max( roughness * 3.14159265, 0.01 ), 0.0, 1.0 ) );
	float fresnelFactor = clamp( pow( 1.0 - reflectDot, 5.0 ), 0.0, 1.0 );
	reflectivity = mix( reflectivity, vec3( 1.0 ), vec3( fresnelFactor ) ) * fresnelReduction;
	
	reflectivity *= vec3( solidity );
	
// 	if( dot( reflectivity, reflectivity ) < 0.0001 ){ // refl.length^2 < 0.01^2
// 		discard;
// 	}
}

void calculateBouncedReflectivity( in vec3 tc, in vec3 reflectDir,
out vec3 reflectivity, out float roughness, out vec3 reflectDirBounced ){
	// fetch reflectivity
	reflectivity = texture( texReflectivity, tc ).rgb;
	
	// fetch roughness and ambient occlusion
	roughness = texture( texRoughness, tc ).r;
	vec3 aoSolidity = texture( texAOSolidity, tc ).rgb;
	#define ao aoSolidity.r
	#define solidity aoSolidity.b
	
	// calculate fresnel reflection
	vec3 normal = sanitizeNormal( normalLoadMaterial( texNormal, tc ) );
	float reflectDot = min( abs( dot( -reflectDir, normal ) ), 1.0 );
	reflectDirBounced = reflect( reflectDir, normal );
	
//	vec3 fresnelReduction = vec3( pow( 1.0 - roughness, 5.0 ) * 0.9524 + 0.0476 );
	vec3 fresnelReduction = mix( reflectivity, vec3( 1.0 ), pow( 1.0 - roughness, 5.0 ) );
//	vec3 fresnelReduction = mix( mix( vec3( 0.056 ), vec3( 1.0 ), reflectivity ), vec3( 1.0 ), pow( 1.0 - roughness, 5.0 ) );
	
	fresnelReduction *= vec3( clamp(
		( acos( 1.0 - ao ) + roughness * 1.5707963 - acos( reflectDot ) + 0.01 )
			/ max( roughness * 3.14159265, 0.01 ), 0.0, 1.0 ) );
	float fresnelFactor = clamp( pow( 1.0 - reflectDot, 5.0 ), 0.0, 1.0 );
	reflectivity = mix( reflectivity, vec3( 1.0 ), vec3( fresnelFactor ) ) * fresnelReduction;
	
	reflectivity *= vec3( solidity );
}




/*
// version with improved quality. takes two mip-map levels lower than what the roughness requires and applies
// a stripped down 5x5 pixel gauss filter. this filter is done using 5 taps only in an appropriate pattern.
// this way 17 pixels of the total 25 of the filter can be blurred in only 5 taps. one mip-map level step
// averages 4 pixels. 2 mip-map levels thus average 16 pixels. this 17 pixel hack version is thus more or less
// equal to 2 level steps in the mip-map chain but with much better quality for just 4 additional taps.
envMapLodLevel -= 2.0;
vec2 offset = pow( vec2( 2.0 ), vec2( envMapLodLevel ) ) / vec2( 256.0 );

color = textureLod( texEnvMap, tcEnvMap, envMapLodLevel ).rgb * vec3( 0.7735849 );
color += textureLod( texEnvMap, tcEnvMap + offset * vec2( 0.37736, 1.20755 ), envMapLodLevel ).rgb;
color += textureLod( texEnvMap, tcEnvMap + offset * vec2( 1.20755, -0.37736 ), envMapLodLevel ).rgb;
color += textureLod( texEnvMap, tcEnvMap + offset * vec2( -0.37736, -1.20755 ), envMapLodLevel ).rgb;
color += textureLod( texEnvMap, tcEnvMap + offset * vec2( -1.20755, 0.37736 ), envMapLodLevel ).rgb;
color *= vec3( 0.209486 );
*/

#ifdef ENVMAP_BOX_PROJECTION

#define VERSION_ENVMAP 2

void envMapReflection( in int index, in SAMPLER_ENVMAP samplerEnvMap,
in vec3 position, in vec3 reflectDir, in float roughness,
out vec4 color, out vec2 weights, out float hitDistance, out float borderDistance ){
	vec3 localPosition = abs( pEnvMapMatrixInfluence[ index ] * vec4( position, 1.0 ) );
	vec3 hitPoint;
	
	borderDistance = length( ( localPosition - clamp( localPosition, vec3( 0.0 ), vec3( 1.0 ) ) )
		* pEnvMapInfluenceScale[ index ] );
	
	// the weights for the reflections are calculated like this:
	// 
	// the first order reflection weight is the product of the weight of the reflection point compared with
	// the influence box and the mask weight
	//    order1Weight = (reflectionPoint weighted influenceBox) * mask
	// 
	// the second order reflection weight is the product of the reflected point compared to the reflection
	// box and the mask weight. if the reflection point is inside the influence box the weight is 0
	//    order2Weight = (reflectedPoint weighted influenceBox) * mask
	//    if reflectionPoint inside influenceBox order2Weight = 0
	weights = vec2( 0.0, 1.0 );
	
	// weight of the reflection point compared to the influence box. use for both order weights
	if( all( lessThanEqual( localPosition, vec3( 1.0 ) ) ) ){
		localPosition = ( vec3( 1.0 ) - localPosition ) * pEnvMapInfluenceBorder[ index ];
		localPosition = clamp( localPosition, vec3( 0.0 ), vec3( 1.0 ) );
		weights.x = min( min( localPosition.x, localPosition.y ), localPosition.z );
		weights.y = 0.0;
	}
	
	// intersect the reflection ray with the environment map reflection box
	localPosition = pEnvMapMatrixReflBox[ index ] * vec4( position, 1.0 );
	vec3 localReflectDir = mat3( pEnvMapMatrixReflBox[ index ] ) * reflectDir;
	
#if VERSION_ENVMAP == 1
	vec4 absLocalPosition = vec4( abs( localPosition ), 1.0 );
	absLocalPosition.xy = max( absLocalPosition.xy, absLocalPosition.zw );
	localPosition /= max( absLocalPosition.x, absLocalPosition.y );
	
	vec3 reflectDirDist = mix( ( sign( localReflectDir ) - localPosition ) / localReflectDir,
		vec3( 1e6 ), lessThan( abs(localReflectDir), vec3( 1e-5 ) ) );
	hitDistance = min( reflectDirDist.x, min( reflectDirDist.y, reflectDirDist.z ) );
	
#elif VERSION_ENVMAP == 2
	vec3 reflectDirDist = mix( ( sign( localReflectDir ) - localPosition ) / localReflectDir,
		vec3( 1e6 ), lessThan( abs( localReflectDir ), vec3( 1e-5 ) ) );
	hitDistance = max( min( reflectDirDist.x, min( reflectDirDist.y, reflectDirDist.z ) ), 0.0 );
	
	if( any( greaterThan( abs( pEnvMapMatrixInfluence[ index ]
	* vec4( position + reflectDir * vec3( hitDistance ), 1.0 ) ), vec3( 1.0 ) ) ) ){
		weights.y = 0.0;
	}
	
	if( any( greaterThan( abs( localPosition + localReflectDir * vec3( hitDistance ) ), vec3( 1.0001 ) ) ) ){
		weights.y = 0.0;
		
		hitPoint = abs( localPosition );
		localPosition /= max( max( hitPoint.x, hitPoint.y ), hitPoint.z );
		
		reflectDirDist = mix( ( sign( localReflectDir ) - localPosition ) / localReflectDir,
			vec3( 1e6 ), lessThan( abs( localReflectDir ), vec3( 1e-5 ) ) );
		hitDistance = min( reflectDirDist.x, min( reflectDirDist.y, reflectDirDist.z ) ); // wrong, but let it be for the moment
		//hitDistance = 0.0;
	}
#endif
	
	hitPoint = position + reflectDir * vec3( hitDistance );
	
	// calculate the weight for the second order reflection. this weight is the product of
	// the reflected point compared to the reflection box and the mask weight
	//weights.y = 1.0; // for the time being this is set to 1 using the default value above
	
	// calculate texture coordinates for the sampling either as cube map or equirectangular map
	vec3 envMapDir = pMatrixEnvMap * ( hitPoint - pEnvMapPosition[ index ] );
	#ifdef ENVMAP_EQUI
		envMapDir = normalize( envMapDir );
		vec2 tcEnvMap = cemefac.xy + cemefac.zw * vec2( atan( envMapDir.x, envMapDir.z ), acos( envMapDir.y ) );
	#else
		#define tcEnvMap envMapDir
	#endif
	
	// sample environment map color. the roughness jhas to be adjusted depending on the distance
	// to the hit point. the approximation formula looks like this:
	//    distance = length( localReflectDir ) * hitDistance
	//    blurSize = distance * roughness * pi/2 * 2
	//    level = log2( blurSize * cubeMapPixelSize / 2 )
	// the distance has to be calculated in the local unit space to better calculate against
	// the pixel size. due to the different scaling of the reflection box this result is
	// incorrect but a good approximation as otherwise complicated calculations are required.
	// the blur distance should correctly use tan( distance * roughness * pi/2 ) but for small
	// angles the approximation is fine enough. for large angles there is considerable error
	// but nobody is going to notice if the large blur is correct or not. the multiplication
	// with 2 is required as otherwise half the blur size would be calculated. eventually
	// the /2 in the level calculation is the size of the unit box (from -1 to 1). all in all
	// this gives a simple aproximation of the bluriness of rough surfaces without too
	// complicated calculations float envMapLodLevel = log2( 1.0 + pEnvMapLodLevel * roughness );
	float envMapLodLevel = log2( pEnvMapRoughness[ index ]
		* roughness * hitDistance * length( localReflectDir ) );
	//float blurSize = length( mat3( pEnvMapMatrixReflBox[ index ] )
	//	* ( reflectDir * vec3( hitDistance * roughness * roughnessToBlur ) ) );
	//float envMapLodLevel = log2( pEnvMapRoughness[ index ] * blurSize );
	
	color = textureLod( samplerEnvMap, tcEnvMap, envMapLodLevel );
	weights *= vec2( color.a );
}

void envMapProcessAt( in int index, in SAMPLER_ENVMAP samplerEnvMap,
in vec3 position, in vec3 reflectDir, in float roughness,
inout float order1WeightSum, inout float order1Mask, inout vec3 order1Color, inout int order1Priority,
inout float order2WeightSum, inout vec3 order2Color, inout float order2Distance,
inout float closestBorderDistance, inout vec3 closestColor ){
	if( index >= pEnvMapCount ){
		return;
	}
	
	float borderDistance;
	vec2 envMapWeights;
	float hitDistance;
	vec4 envMapColor;
	
	envMapReflection( index, ARG_SAMP_MEDP samplerEnvMap, position, reflectDir, roughness,
		envMapColor, envMapWeights, hitDistance, borderDistance );
	
	// first order reflection
	if( envMapWeights.x > 0.0 ){
		if( order1WeightSum == 0.0 ){
			order1Priority = pEnvMapPriority[ index ];
		}
		
		if( pEnvMapPriority[ index ] < order1Priority ){
			envMapWeights.x = max( min( envMapWeights.x, 1.0 - order1WeightSum ), 0.0 );
		}
		
		order1Color += envMapColor.rgb * vec3( envMapWeights.x );
		order1WeightSum += envMapWeights.x;
		order1Mask = max( order1Mask, envMapColor.a );
		
	}else{
		// second order reflection
		if( envMapWeights.y > 0.0 ){
			if( hitDistance < order2Distance ){
				float scale = 1.0;
				
				if( order2WeightSum > 0.001 ){
					scale = max( min( order2WeightSum, 1.0 - envMapWeights.y ), 0.0 ) / order2WeightSum;
				}
				
				order2Color *= vec3( scale );
				order2WeightSum *= scale;
				
				order2Distance = hitDistance;
				
			}else{
				envMapWeights.y = max( min( envMapWeights.y, 1.0 - order2WeightSum ), 0.0 );
			}
			envMapWeights.y = max( 1.0 - order2WeightSum, 0.0 ); // temporary hack, needs sorting for smallest hitDistance
			order2Color += envMapColor.rgb * vec3( envMapWeights.y );
			order2WeightSum += envMapWeights.y;
		}
	}
	
	// third order reflection
	if( borderDistance < closestBorderDistance ){
		closestColor = envMapColor.rgb;
		closestBorderDistance = borderDistance;
	}
}

void colorEnvMapReflection( in vec3 position, in vec3 reflectDir, in float roughness, out vec3 color ){
	float order1WeightSum = 0.0;
	float order1Mask = 0.0;
	vec3 order1Color = vec3( 0.0 );
	int order1Priority = 0;
	
	float order2WeightSum = 0.0;
	vec3 order2Color = vec3( 0.0 );
	float order2Distance = 1e5;
	
	float closestBorderDistance = 1e5;
	vec3 closestColor = vec3( 0.0 );
	
	envMapProcessAt( 0, texEnvMap, position, reflectDir, roughness,
		order1WeightSum, order1Mask, order1Color, order1Priority,
		order2WeightSum, order2Color, order2Distance, closestBorderDistance, closestColor );
	
	envMapProcessAt( 1, texEnvMap2, position, reflectDir, roughness,
		order1WeightSum, order1Mask, order1Color, order1Priority,
		order2WeightSum, order2Color, order2Distance, closestBorderDistance, closestColor );
	
	envMapProcessAt( 2, texEnvMap3, position, reflectDir, roughness,
		order1WeightSum, order1Mask, order1Color, order1Priority,
		order2WeightSum, order2Color, order2Distance, closestBorderDistance, closestColor );
	
	envMapProcessAt( 3, texEnvMap4, position, reflectDir, roughness,
		order1WeightSum, order1Mask, order1Color, order1Priority,
		order2WeightSum, order2Color, order2Distance, closestBorderDistance, closestColor );
	
	envMapProcessAt( 4, texEnvMap5, position, reflectDir, roughness,
		order1WeightSum, order1Mask, order1Color, order1Priority,
		order2WeightSum, order2Color, order2Distance, closestBorderDistance, closestColor );
	
	envMapProcessAt( 5, texEnvMap6, position, reflectDir, roughness,
		order1WeightSum, order1Mask, order1Color, order1Priority,
		order2WeightSum, order2Color, order2Distance, closestBorderDistance, closestColor );
	
	envMapProcessAt( 6, texEnvMap7, position, reflectDir, roughness,
		order1WeightSum, order1Mask, order1Color, order1Priority,
		order2WeightSum, order2Color, order2Distance, closestBorderDistance, closestColor );
	
	envMapProcessAt( 7, texEnvMap8, position, reflectDir, roughness,
		order1WeightSum, order1Mask, order1Color, order1Priority,
		order2WeightSum, order2Color, order2Distance, closestBorderDistance, closestColor );
	
	//color = vec3( order1Mask, 0, 0); return;
	if( order1WeightSum < 1.0 && order1Mask < 1.0 ){
		float scale = 1.0;
		
		if( order1WeightSum + order2WeightSum > 1.0 ){
			scale = max( min( order2WeightSum, 1.0 - order1WeightSum ), 0.0 ) / max( order2WeightSum, 0.001 );
		}
		
		order1Color += order2Color * vec3( scale );
		order1WeightSum += order2WeightSum * scale;
	}
	
	if( order1WeightSum > 0.001 ){
		color = order1Color / vec3( order1WeightSum );
		
	}else{
		//color = vec3(1,0,0);
		color = closestColor;
	}
	//color = vec3( clamp(order1WeightSum,0,1), max(order1WeightSum-1,0), 0);
	//color = vec3( order1WeightSum, order1Mask, 0 );
}

#else // ENVMAP_BOX_PROJECTION

#ifdef ENVMAP_SINGLE
void colorEnvMapReflection( in vec3 position, in vec3 reflectDir, in float roughness, out vec3 color ){
	float envMapLodLevel = log2( 1.0 + pEnvMapLodLevel * roughness );
	
	vec3 envMapDir = pMatrixEnvMap * reflectDir;
	#ifdef ENVMAP_EQUI
		envMapDir = normalize( envMapDir );
		vec2 tcEnvMap = cemefac.xy + cemefac.zw * vec2( atan( envMapDir.x, envMapDir.z ), acos( envMapDir.y ) );
	#else
		#define tcEnvMap envMapDir
	#endif
	
	color = textureLod( texEnvMap, tcEnvMap, envMapLodLevel );
}

#else // ENVMAP_SINGLE
void envMapReflection( in int index, in SAMPLER_ENVMAP samplerEnvMap,
in vec3 position, in vec3 reflectDir, in float roughness,
out vec3 color, out float weight, out float borderDistance ){
	vec3 localPosition = abs( pEnvMapMatrixInfluence[ index ] * vec4( position, 1.0 ) );
	
	borderDistance = length( ( localPosition - clamp( localPosition, vec3( 0.0 ), vec3( 1.0 ) ) )
		* pEnvMapInfluenceScale[ index ] );
	
	weight = 0.0;
	
	if( all( lessThanEqual( localPosition, vec3( 1.0 ) ) ) ){
		localPosition = ( vec3( 1.0 ) - localPosition ) * pEnvMapInfluenceBorder[ index ];
		localPosition = clamp( localPosition, vec3( 0.0 ), vec3( 1.0 ) );
		weight = min( min( localPosition.x, localPosition.y ), localPosition.z );
	}
	
	vec3 envMapDir = pMatrixEnvMap * reflectDir;
	#ifdef ENVMAP_EQUI
		envMapDir = normalize( envMapDir );
		vec2 tcEnvMap = cemefac.xy + cemefac.zw * vec2( atan( envMapDir.x, envMapDir.z ), acos( envMapDir.y ) );
	#else
		#define tcEnvMap envMapDir
	#endif
	
	float envMapLodLevel = log2( 1.0 + pEnvMapLodLevel * roughness );
	
	color = textureLod( samplerEnvMap, tcEnvMap, envMapLodLevel ).rgb;
}

void envMapProcessAt( in int index, in SAMPLER_ENVMAP samplerEnvMap, in vec3 position,
in vec3 reflectDir, in float roughness, inout float weightSum, inout vec3 color,
inout int priority, inout float closestBorderDistance, inout vec3 closestColor ){
	if( index >= pEnvMapCount ){
		return;
	}
	
	float borderDistance;
	float envMapWeight;
	vec3 envMapColor;
	
	envMapReflection( index, ARG_SAMP_MEDP samplerEnvMap, position, reflectDir, roughness, envMapColor, envMapWeight, borderDistance );
	
	if( envMapWeight > 0.0 ){
		if( weightSum == 0.0 ){
			priority = pEnvMapPriority[ index ];
		}
		
		if( pEnvMapPriority[ index ] < priority ){
			envMapWeight = max( min( envMapWeight, 1.0 - weightSum ), 0.0 );
		}
		
		color += envMapColor * vec3( envMapWeight );
		weightSum += envMapWeight;
	}
	
	if( borderDistance < closestBorderDistance ){
		closestColor = envMapColor;
		closestBorderDistance = borderDistance;
	}
}

void colorEnvMapReflection( in vec3 position, in vec3 reflectDir, in float roughness, out vec3 color ){
	float closestBorderDistance = 1e5;
	vec3 closestColor = vec3( 0.0 );
	float weightSum = 0.0;
	color = vec3( 0.0 );
	int priority = 0;
	
	envMapProcessAt( 0, texEnvMap, position, reflectDir, roughness, weightSum, color,
		priority, closestBorderDistance, closestColor );
	envMapProcessAt( 1, texEnvMap2, position, reflectDir, roughness, weightSum, color,
		priority, closestBorderDistance, closestColor );
	envMapProcessAt( 2, texEnvMap3, position, reflectDir, roughness, weightSum, color,
		priority, closestBorderDistance, closestColor );
	envMapProcessAt( 3, texEnvMap4, position, reflectDir, roughness, weightSum, color,
		priority, closestBorderDistance, closestColor );
	envMapProcessAt( 4, texEnvMap5, position, reflectDir, roughness, weightSum, color,
		priority, closestBorderDistance, closestColor );
	envMapProcessAt( 5, texEnvMap6, position, reflectDir, roughness, weightSum, color,
		priority, closestBorderDistance, closestColor );
	envMapProcessAt( 6, texEnvMap7, position, reflectDir, roughness, weightSum, color,
		priority, closestBorderDistance, closestColor );
	envMapProcessAt( 7, texEnvMap8, position, reflectDir, roughness, weightSum, color,
		priority, closestBorderDistance, closestColor );
	
	if( weightSum > 0.0 ){
		color /= vec3( weightSum );
		
	}else{
		color = closestColor;
	}
}
#endif // ENVMAP_SINGLE

#endif // ENVMAP_BOX_PROJECTION

void main( void ){
	ivec3 tc = ivec3( gl_FragCoord.xy, vLayer );
	
	// discard not inizalized fragments
	if( texelFetch( texDiffuse, tc, 0 ).a == 0.0 ){
		discard;
	}
	
	vec3 position = depthToPosition( texDepth, tc, vScreenCoord, vLayer );
	
	// calculate the reflection for the given point using the results found in the screen space reflection pass
	vec3 reflection = texelFetch( texReflection, tc, 0 ).rgb;
	vec3 reflectivity, reflectDir;
	float roughness;
	
	#ifdef HACK_NO_SSR
	reflection.z = 0.0;
	#endif
	
	calculateReflectionParameters( tc, position, reflectivity, roughness, reflectDir );
	
// TEST TEST TEST ================
	/*
	reflection *= vec3( 0.7735849 );
	vec4 TtcLocal = min( pScreenSpacePixelSize.xyxy * gl_FragCoord.xyxy + pScreenSpacePixelSize.xyxy
		* vec4( 0.37736, 1.20755, 1.20755, -0.37736 ), pViewportMax.xyxy );
	reflection += textureLod( texReflection, vec3( TtcLocal.xy, vLayer ), 0.0 ).rgb;
	reflection += textureLod( texReflection, vec3( TtcLocal.zw, vLayer ), 0.0 ).rgb;
	TtcLocal = min( pScreenSpacePixelSize.xyxy * gl_FragCoord.xyxy + pScreenSpacePixelSize.xyxy
		* vec4( -0.37736, -1.20755, -1.20755, 0.37736 ), pViewportMax.xyxy );
	reflection += textureLod( texReflection, vec3( TtcLocal.xy, vLayer ), 0.0 ).rgb;
	reflection += textureLod( texReflection, vec3( TtcLocal.zw, vLayer ), 0.0 ).rgb;
	reflection *= vec3( 0.209486 );
	*/
// TEST TEST TEST ================

	// get the local reflection
	// 
	// a note about roughness. the roughness is defined as the maximal spread angle across a sphere with
	// 1m radius. this linear definition allows us to properly modify the roughness depending on the
	// distance between the reflection point and the reflected point. in the real world the image becomes
	// more and more blurred with increasing distance. using the linear definition the roughness simply
	// scales linearly with the distance clamped to a maximum of 1 obviously.
	/*
	vec3 hitPosition = depthToPosition( texDepth, vScreenCoord, vLayer );
	
	vec2 mipMapLevel = vec2( distance( position, hitPosition )
		* tan( min( roughness, 0.5 ) * roughnessToAngle ) / position.z );
	mipMapLevel = log2( clamp( mipMapLevel * pMipMapPixelSize,
		vec2( 1.0 ), vec2( pMipMapMaxLevel ) ) ) - 2.0;
	
//mipMapLevel = reflection.z - 2.0; // roughnessTest* only
	mipMapLevel = vec2( -2.0 );
	//vec3 reflectionLocal = textureLod( texColor, vec3( reflection.xy, vLayer ), max( mipMapLevel.x, mipMapLevel.y ) ).rgb;
	
	*/
	/*
	float localMipMapLevel = log2( 1.0 + pMipMapMaxLevel * roughness ) - 2;
	vec2 offset = pow( vec2( 2.0 ), vec2( localMipMapLevel ) ) * pScreenSpacePixelSize;
	
	vec3 reflectionLocal = textureLod( texColor,
		vec3( min( reflection.xy, pViewportMax.xyxy ), vLayer ), localMipMapLevel ).rgb * vec3( 0.7735849 );
	vec4 tcLocal = min( reflection.xyxy + offset.xyxy
		* vec4( 0.37736, 1.20755, 1.20755, -0.37736 ), pViewportMax.xyxy );
	reflectionLocal += textureLod( texColor, vec3( tcLocal.xy, vLayer ), localMipMapLevel ).rgb;
	reflectionLocal += textureLod( texColor, vec3( tcLocal.zw, vLayer ), localMipMapLevel ).rgb;
	tcLocal = min( reflection.xyxy + offset.xyxy
		* vec4( -0.37736, -1.20755, -1.20755, 0.37736 ), pViewportMax.xyxy );
	reflectionLocal += textureLod( texColor, vec3( tcLocal.xy, vLayer ), localMipMapLevel ).rgb;
	reflectionLocal += textureLod( texColor, vec3( tcLocal.zw, vLayer ), localMipMapLevel ).rgb;
	reflectionLocal *= vec3( 0.209486 );
	*/
	vec3 reflectionLocal = textureLod( texColor, vec3( reflection.xy, vLayer ), 0.0 ).rgb;
	
	// check the reflectivity of the hit point. if the reflectivity is high we end up with near black
	// reflections which looks very odd. the proper solution would be to calculate additional bounces.
	// typically one additional bounce should be enough to get a proper solution without too high
	// reflection. for the time being though this is hacked by bouncing a ray into the env-map and
	// blending the result together. this lets the environment map jump in for these cases.
	vec3 envMapDir;
	#ifdef ENVMAP_EQUI
		vec2 tcEnvMap;
	#else
		#define tcEnvMap envMapDir
	#endif
	
	if( reflection.z > 0.0 ){
		// this check here is currently required to avoid trying to tap from the sky or another
		// special pixel not having written the reflectivity properly. later on this should be
		// handled by doing a proper clear pass that also clears the reflectivity
		vec3 tcRefl = vec3( reflection.xy, vLayer );
		
		if( texture( texDiffuse, tcRefl ).a > 0.0 ){
			vec3 bouncedReflectionColor;
			vec3 bouncedReflectivity;
			vec3 bouncedReflectDir;
			float bouncedRoughness;
			
			calculateBouncedReflectivity( tcRefl, reflectDir,
				bouncedReflectivity, bouncedRoughness, bouncedReflectDir );
			
			vec2 reflScreenCoord = ( reflection.xy / pViewportMax ) * vec2( 2 ) - vec2( 1 );
			vec3 hitPosition = depthToPosition( texDepth, tcRefl, reflScreenCoord, vLayer );
			
			colorEnvMapReflection( hitPosition, bouncedReflectDir, bouncedRoughness, bouncedReflectionColor );
			
			reflectionLocal = mix( reflectionLocal, bouncedReflectionColor, bouncedReflectivity );
		}
	}
	
	// get the global reflection
	vec3 reflectionGlobal;
	colorEnvMapReflection( position, reflectDir, roughness, reflectionGlobal );
	
	// mix the reflections
	outColor = mix( reflectionGlobal, reflectionLocal, reflection.z );
	
	// this should simulate to some degree a prefiltered environment map (cosine filter, phong lobe).
	//reflectedColor *= vec3( ( cos( roughness * 3.1416 ) * 0.5 + 0.5 ) * 0.38 + 0.62 );
	// this version is based on a normalized situation and should be somewhat better
//	outColor *= vec3( 0.2 + 0.8 / ( 1.0 + 50.0 * roughness * roughness ) );
	
	// apply reflectivity
	outColor *= reflectivity;
}
