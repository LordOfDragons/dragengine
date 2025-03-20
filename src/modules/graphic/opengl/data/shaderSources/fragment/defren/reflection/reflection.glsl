precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

UBOLAYOUT uniform EnvMapParameters{
	vec2 pBlendFactors; // x=multiply, y=add
	float pReflEnvMapLodLevel;
	int pReflLayerCount;
	vec4 pReflEnvMapPosLayer[ 100 ]; // xyz=position, w=layer
};


uniform HIGHP sampler2DArray texDepth;
uniform lowp sampler2DArray texDiffuse;
uniform lowp sampler2DArray texNormal;
uniform lowp sampler2DArray texReflectivity;
uniform lowp sampler2DArray texRoughness;
uniform lowp sampler2DArray texAOSolidity;
uniform mediump sampler2DArray texEnvMapArray;
uniform mediump sampler2D texEnvMapSky;


in vec2 vTexCoord;
in vec2 vScreenCoord;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out vec4 outColor;



// Constants
//////////////

#ifdef ENVMAP_EQUI
	const vec4 cemefac = vec4( 0.5, 1.0, -0.1591549, -0.3183099 ); // 0.5, 1.0, -1/2pi, -1/pi
#endif

#include "shared/normal_texture.glsl"
#include "shared/defren/depth_to_position.glsl"


/** Calculate the reflections parameters. */
void calculateReflectionParameters( in ivec3 tc, in vec3 position, out vec3 normal,
out vec3 reflectivity, out float roughness, out vec3 reflectDir ){
	// fetch reflectivity
	reflectivity = texelFetch( texReflectivity, tc, 0 ).rgb;
	
	// fetch roughness and ambient occlusion
	roughness = texelFetch( texRoughness, tc, 0 ).r;
	vec3 aoSolidity = texelFetch( texAOSolidity, tc, 0 ).rgb;
	#define ao aoSolidity.r
	#define solidity aoSolidity.b
	
	// calculate fresnel reflection
	vec3 fragmentDirection = normalize( position );
	normal = sanitizeNormal( normalLoadMaterial( texNormal, tc ) );
	float reflectDot = min( abs( dot( -fragmentDirection, normal ) ), 1.0 );
	reflectDir = reflect( fragmentDirection, normal );
	
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
}



/**
 * Calculates the reflection color based on blending the closest two environment maps together.
 * Using only the closest environment map would be faster but results also in sharp transitions.
 */

void envMapReflection( in vec3 position, in vec3 reflectDir, in vec3 reflectivity, in float roughness, out vec3 color ){
	// calculate the environment maps to blend together and the blend weight
	vec2 envMapLayers = vec2( -1.0 );
	float lowDistLimit = -1.0;
	float bestSquareDist;
	float curSquareDist;
	float blendWeight;
	vec2 distances;
	vec3 envMapDir;
	int bestLayer;
	int i, j;
	
	for( i=0; i<2; i++ ){
		bestSquareDist = 1000.0;
		
		//while( true ){
			bestLayer = -1;
			
			for( j=0; j<pReflLayerCount; j++ ){
				envMapDir = position - pReflEnvMapPosLayer[ j ].xyz;
				curSquareDist = dot( envMapDir, envMapDir );
				
				if( curSquareDist < bestSquareDist && curSquareDist > lowDistLimit ){
					bestSquareDist = curSquareDist;
					bestLayer = j;
				}
			}
			
			if( bestLayer == -1 ){
				envMapLayers[ i ] = -1.0;
				i == 2;
				break;
			}
			
			lowDistLimit = bestSquareDist;
			
			// test if the environment map is occluded. if not break the loop
			// if( ! occluded ){
				envMapLayers[ i ] = pReflEnvMapPosLayer[ bestLayer ].w;
				//break;
			// }
		//}
		
		distances[ i ] = bestSquareDist;
	}
	
	// tap the environment maps and blend the colors
	float envMapLodLevel = log2( 1.0 + pReflEnvMapLodLevel * roughness );
	envMapDir = pMatrixEnvMap * reflectDir;
	
	#ifdef ENVMAP_EQUI
		envMapDir = normalize( envMapDir );
	#endif
	
	if( envMapLayers.x < -0.5 ){
		#ifdef ENVMAP_EQUI
			vec2 tcEnvMap = cemefac.xy + cemefac.zw * vec2( atan( envMapDir.x, envMapDir.z ), acos( envMapDir.y ) );
			color = textureLod( texEnvMapSky, tcEnvMap, envMapLodLevel ).rgb;
		#else
			color = textureLod( texEnvMapSky, envMapDir, envMapLodLevel ).rgb;
		#endif
		
	}else if( envMapLayers.y < -0.5 ){
		#ifdef ENVMAP_EQUI
			vec3 tcEnvMap = vec3( cemefac.xy + cemefac.zw * vec2( atan( envMapDir.x, envMapDir.z ), acos( envMapDir.y ) ), envMapLayers.x );
			color = textureLod( texEnvMapArray, tcEnvMap, envMapLodLevel ).rgb;
		#else
			vec4 tcEnvMap = vec4( envMapDir, envMapLayers.x );
			color = textureLod( texEnvMapArray, tcEnvMap, envMapLodLevel ).rgb;
		#endif
		
	}else{
		distances = sqrt( distances );
		blendWeight = clamp( ( distances.x - distances.y ) * pBlendFactors.x + pBlendFactors.y, 0.0, 1.0 );
		
		#ifdef ENVMAP_EQUI
			vec3 tcEnvMap = vec3( cemefac.xy + cemefac.zw * vec2( atan( envMapDir.x, envMapDir.z ), acos( envMapDir.y ) ), envMapLayers.x );
			color = textureLod( texEnvMapArray, tcEnvMap, envMapLodLevel ).rgb;
			tcEnvMap.z = envMapLayers.y;
			color = mix( color, textureLod( texEnvMapArray, tcEnvMap, envMapLodLevel ).rgb, vec3( blendWeight ) );
		#else
			vec4 tcEnvMap = vec4( envMapDir, envMapLayers.x );
			color = textureLod( texEnvMapArray, tcEnvMap, envMapLodLevel ).rgb;
			tcEnvMap.w = envMapLayers.y;
			color = mix( color, textureLod( texEnvMapArray, tcEnvMap, envMapLodLevel ).rgb, vec3( blendWeight ) );
		#endif
	}
	
	// this should simulate to some degree a prefiltered environment map (cosine filter, phong lobe).
	// color *= vec3( ( cos( roughness * 3.1416 ) * 0.5 + 0.5 ) * 0.38 + 0.62 );
	// this version is based on a normalized situation and should be somewhat better
//	color *= vec3( 0.2 + 0.8 / ( 1.0 + 50.0 * roughness * roughness ) );
	
	// apply reflectivity
	color *= reflectivity;
}

#if 0
void envMapReflection( in vec3 position, in vec3 reflectDir, in vec3 reflectivity, in float roughness, out vec3 color ){
	float envMapLodLevel = log2( 1.0 + pReflEnvMapLodLevel * roughness );
	vec3 envMapDir = pMatrixEnvMap * reflectDir;
	
	envMapDir = normalize( envMapDir );
	
	if( pReflLayerCount < 2 ){
		vec2 tcEnvMap = cemefac.xy + cemefac.zw * vec2( atan( envMapDir.x, envMapDir.z ), acos( envMapDir.y ) );
		color = textureLod( texEnvMapSky, tcEnvMap, envMapLodLevel ).rgb;
		
	}else{
		vec3 tcEnvMap = vec3( cemefac.xy + cemefac.zw * vec2( atan( envMapDir.x, envMapDir.z ), acos( envMapDir.y ) ), 0.0 );
		color = textureLod( texEnvMapArray, tcEnvMap, envMapLodLevel ).rgb;
		tcEnvMap.z = 1.0;
		color = mix( color, textureLod( texEnvMapArray, tcEnvMap, envMapLodLevel ).rgb, vec3( 0.5 ) );
	}
	
	// this should simulate to some degree a prefiltered environment map (cosine filter, phong lobe).
	// color *= vec3( ( cos( roughness * 3.1416 ) * 0.5 + 0.5 ) * 0.38 + 0.62 );
	// this version is based on a normalized situation and should be somewhat better
//	color *= vec3( 0.2 + 0.8 / ( 1.0 + 50.0 * roughness * roughness ) );
	
	// apply reflectivity
	color *= reflectivity;
}
#endif


// Main Function
//////////////////

void main( void ){
	ivec3 tc = ivec3( gl_FragCoord.xy, vLayer );
	
	// discard not inizalized fragments or fragements that are not supposed to be lit
	if( texelFetch( texDiffuse, tc, 0 ).a == 0.0 ){
		discard;
	}
	
	// determine position of fragment
	vec3 position = depthToPosition( texDepth, tc, vScreenCoord, vLayer );
	
	// calculate the reflection parameters. these are the same no matter which solution is used later on
	// to obtain the reflected color from
	vec3 normal, reflectivity, reflectDir;
	float roughness;
	
	calculateReflectionParameters( tc, position, normal, reflectivity, roughness, reflectDir );
	
	// blend together the closest two environment maps. using only the closest environment map would be
	// faster but results also in sharp transitions
	vec3 color;
	
	envMapReflection( position, reflectDir, reflectivity, roughness, color );
	
	outColor = vec4( color, 1.0 );
	
	
	/*
	float bestLayer1, bestSquareDist1, curSquareDist;
	float curSquareDist;
	vec3 envMapDir;
	
	
	vec3 envMapDir = position - pReflEnvMapPosLayer[ 0 ].xyz;
	float bestSquareDist = dot( envMapDir, envMapDir );
	float bestLayer = pReflEnvMapPosLayer[ 0 ].w;
	float curSquareDist;
	int i;
	
	for( i=1; i<pReflLayerCount; i++ ){
		envMapDir = position - pReflEnvMapPosLayer[ i ].xyz;
		curSquareDist = dot( envMapDir, envMapDir );
		if( curSquareDist < bestSquareDist ){
			bestSquareDist = curSquareDist;
			bestLayer = pReflEnvMapPosLayer[ i ].w;
		}
	}
	
	outColor = vec4( bestLayer / 255.0, sqrt( bestSquareDist ) / 10.0, 0.0, 1.0 );
	*/
	
	/*
	vec3 envMapDir = position - pReflEnvMapPosLayer[ 0 ].xyz;
	vec2 bestEnvMap = vec2( dot( envMapDir, envMapDir ), pReflEnvMapPosLayer[ 0 ].w );
	vec2 curEnvMap;
	int i;
	
	for( i=1; i<pReflLayerCount; i++ ){
		envMapDir = position - pReflEnvMapPosLayer[ i ].xyz;
		curEnvMap = vec2( dot( envMapDir, envMapDir ), pReflEnvMapPosLayer[ i ].w );
		bestEnvMap = mix( bestEnvMap, curEnvMap, curEnvMap.x < bestEnvMap.x );
	}
	
	outColor = vec4( bestEnvMap.y / 255.0, sqrt( bestEnvMap.x ) / 10.0, 0.0, 1.0 );
	*/
	
#if 0
	// OLD CODE using "vec4 vTexCoord" with
	//   vTexCoord.xy = inPosition.xy * pQuadTCTransform.xy + pQuadTCTransform.zw;
	
	// get indices and discard if this envmap is not matched
	ivec2 indices = texture( texIndices, vTexCoord.xy ).rg;
	if( all( notEqual( indices, ivec2( pEnvMapIndex ) ) ) ) discard;
	
	// get distances and calculate the blend weight according to them
	vec2 distances;
	distances.x = texture( texDistance1, vTexCoord.xy ).r;
	distances.y = texture( texDistance2, vTexCoord.xy ).r;
	distances *= vec2( pScaleDistance );
	
	float weight = clamp( ( distances.y - distances.x ) * pBlendFactors.x + pBlendFactors.y, 0.0, 1.0 );
	
	if( indices.y == pEnvMapIndex ){
		weight = 1.0 - weight;
	}
	
	// determine position of fragment
	ivec3 tc = ivec3( gl_FragCoord.xy, vLayer );
	
	#ifdef FULLSCREENQUAD
	vec3 position = depthToPosition( texDepth, tc, vScreenCoord, vLayer );
	#else
	vec3 position = depthToPositionVolume( texDepth, tc, vVolumePos, vLayer );
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
	vec3 normal = sanitizeNormal( normalLoadMaterial( texNormal, tc ) );
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
	float envMapLodLevel = log2( 1.0 + pReflEnvMapLodLevel * roughness );
	vec3 reflectedColor = textureLod( texEnvMap, envMapDir, envMapLodLevel ).rgb;
	
	// this should simulate to some degree a prefiltered environment map (cosine filter, phong lobe).
	//reflectedColor *= vec3( ( cos( roughness * 3.1416 ) * 0.5 + 0.5 ) * 0.38 + 0.62 );
	// this version is based on a normalized situation and should be somewhat better
//	reflectedColor *= vec3( 0.2 + 0.8 / ( 1.0 + 50.0 * roughness * roughness ) );
	
	// apply reflectivity
	reflectedColor *= reflectivity.rgb;
	
	// write out the color and the weight
	outColor = vec4( reflectedColor, weight );
#endif
}
