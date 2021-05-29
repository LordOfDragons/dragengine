#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/light/ubo_render_parameters.glsl"
#include "v130/shared/defren/light/ubo_gi.glsl"
#include "v130/shared/octahedral.glsl"


// samplers
/////////////

#ifdef GI_RAY
	uniform HIGHP sampler2D texPosition;
	uniform lowp sampler2D texDiffuse;
	uniform lowp sampler2D texNormal;
	uniform lowp sampler2D texReflectivity; // reflectivity.rgb, roughness
	
	// NOTE transfer:
	// params = giRayCastMaterialParams(ray.material)
	// tc = giRayCastMaterialTC(params, ray.tc)
	// texDiffuse: vec3 giRayCastSampleColor(params.g, tc)
	// texReflectivity: giRayCastSampleReflectivityRoughness(params.b, tc, out reflectivity, out roughness)
	// 
	// hence these values have to be stored in the ray:
	// - int ray.material => requires R16UI
	// - vec2 ray.texCoord = giRayCastFaceTexCoord() => requires RG16F
	
#else
	uniform HIGHP sampler2D texDepth;
	uniform lowp sampler2D texDiffuse;
	uniform lowp sampler2D texNormal;
	uniform lowp sampler2D texReflectivity;
	uniform lowp sampler2D texRoughness;
	uniform lowp sampler2D texAOSolidity;
#endif

uniform lowp sampler2D texGIIrradiance;
uniform HIGHP sampler2D texGIDistance;


// includes to come after defining fixed position samplers
#define pGIGridProbeCount pGIProbeCount
#include "v130/shared/defren/gi/probe_offset.glsl"


// inputs
///////////

in vec2 vScreenCoord;


// outputs
////////////

out vec4 outColor;
out float outLuminance;
out vec4 outSubSurface;


// constants
//////////////

#ifdef DECODE_IN_DEPTH
	const vec3 unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif

const vec3 lumiFactors = vec3( 0.2125, 0.7154, 0.0721 );


#include "v130/shared/normal.glsl"
#ifndef GI_RAY
	#include "v130/shared/defren/light/normal_from_depth.glsl"
#endif


// dir is required to be normalized
vec2 giTCFromDirection( in vec3 dir, in ivec3 probeCoord, in vec2 mapScale, in int mapSize ){
	vec2 tc = octahedralEncode( dir ); // range [-1..1]
	tc = ( tc + vec2( 1.0 ) ) * 0.5; // range [0..1]
	tc *= vec2( mapSize ); // range [0..mapSize] (left border of left pixel to right border of right pixel)
	tc += vec2( 2 ); // offset by full map border and probe map border
	tc += vec2( pGIProbeCount.x * probeCoord.y + probeCoord.x, probeCoord.z ) * vec2( mapSize + 2 );
	return tc * mapScale;
}

// shifted grid coordinates to local grid coordinates. the pGIGridCoordShift value
// contains "probeCount - shift" to reduce the calculation
ivec3 giGridShiftToLocal( in ivec3 shifted ){
	return ( shifted + pGIGridCoordShift ) % pGIProbeCount;
}

// calculate illumination to apply to fragment
vec3 giIlluminate( in vec3 position, in vec3 normal, in vec3 bendNormal ){
	// in the paper this calculation looks like this:
	//   BiasVector = (n * 0.2 + wo * 0.8 ) * ( 0.75 * minDistanceBetweenProbes ) * TurnableShadowBias
	// whereas n is the normal and wo the direction from fragment to camera.
	// 
	// we need the offset in GI space so the pGIMatrixNormal is used to transform it. since
	// position is the vector from camera to fragment w0 is the negation of the normalized
	// position. furthermore the GI state sets pGISelfShadowBias is set to 0.75 * selfShadowBias.
	vec3 offsetPosition = mix( normal, -normalize( position ), vec3( pGINormalBias ) ) * pGISelfShadowBias;
	offsetPosition = offsetPosition * pGIMatrixNormal; // reverse order does transpose()
	
	normal = normalize( normal * pGIMatrixNormal ); // reverse order does transpose()
	bendNormal = normalize( bendNormal * pGIMatrixNormal ); // reverse order does transpose()
	position = vec3( pGIMatrix * vec4( position, 1.0 ) );
	
	ivec3 baseCoord = clamp( ivec3( ( position + offsetPosition ) * pGIProbeSpacingInv ), ivec3( 0 ), pGIProbeClamp );
	vec3 basePosition = pGIProbeSpacing * vec3( baseCoord );
	
	vec3 sumIrradiance = vec3( 0.0 );
	float sumWeight = 0.0;
	
// 	vec3 alpha = clamp( pGIProbeSpacingInv * ( position + offsetPosition - basePosition ), vec3( 0.01 ), vec3( 0.99 ) ); // test
	vec3 alpha = clamp( pGIProbeSpacingInv * ( position + offsetPosition - basePosition ), vec3( 0.0 ), vec3( 1.0 ) ); // paper
	
	// iterate over adjacent probe cage
	int i;
	for( i=0; i<8; i++ ){
		// offset = 0 or 1 along each axis
		ivec3 offset = ivec3( i, i >> 1, i >> 2 ) & ivec3( 1 );
		ivec3 probeCoord = clamp( baseCoord + offset, ivec3( 0 ), pGIProbeClamp );
		vec3 probePosition = pGIProbeSpacing * vec3( probeCoord );
		
		probeCoord = giGridShiftToLocal( probeCoord );
		
		probePosition += gipoProbeOffset( probeCoord );
		
		vec3 viewDir = normalize( probePosition - position );
		
		vec3 trilinear = mix( vec3( 1.0 ) - alpha, alpha, offset );
		trilinear = max( vec3( 0.001 ), trilinear ); // paper
		float weight = 1.0;
		
		// smooth backface test
		{
// 		float value = max( 0.0001, ( dot( viewDir, normal ) + 1.0 ) * 0.5 ); // test
// 		weight *= value * value;
		
		float value = dot( viewDir, normal ) * 0.5 + 0.5;
		weight *= value * value + 0.2;
		}
		
		// moment visibility test
		{
		vec3 probeToPoint = position + offsetPosition - probePosition;
		float distToProbe = length( probeToPoint );
		
		vec2 texCoord = giTCFromDirection( probeToPoint / distToProbe,
			probeCoord, pGIDistanceMapScale, pGIDistanceMapSize );
		
		vec2 temp = texture( texGIDistance, texCoord ).ra; // RG16 in opengl has RRRG as swizzle
		float mean = temp.x;
		float variance = abs( mean * mean - temp.y );
		
		float chebyshevWeight = 1.0;
		if( distToProbe > mean ){
			// in shadow case according to paper
			chebyshevWeight = distToProbe - mean;
			chebyshevWeight = variance / ( variance + chebyshevWeight * chebyshevWeight );
			chebyshevWeight = max( chebyshevWeight * chebyshevWeight * chebyshevWeight, 0.05 );
		}
		
		weight *= chebyshevWeight;
		}
		
		// avoid zero weight
		weight = max( 0.000001, weight );
		
		vec2 texCoord = giTCFromDirection( bendNormal, probeCoord, pGIIrradianceMapScale, pGIIrradianceMapSize );
		
		const float crushThreshold = 0.2;
		if( weight < crushThreshold ){
			weight *= weight * weight / ( crushThreshold * crushThreshold );
		}
		
		// trilinear weights
		weight *= trilinear.x * trilinear.y * trilinear.z;
		
		vec3 probeIrradiance = texture( texGIIrradiance, texCoord ).rgb;
		
		// from source code. using some kind of gamma=2 curve (basically an sqrt) to
		// approximate sRGB blending. should help somehow with trilinear?
		probeIrradiance = pow( probeIrradiance, vec3( pGIIrradianceGamma * 0.5 ) );
		
		sumIrradiance += probeIrradiance * weight;
		sumWeight += weight;
	}
	
	// normalize
	sumIrradiance /= sumWeight;
	
	// from source code. convert back to linear irradiance (aka square root it)
	sumIrradiance *= sumIrradiance;
	
	// from source code: "was factored out of probes". no idea what this means.
	// if I leave this uncommented the irradiance in the scene keeps on increading
	// until all blows out
// 	const float pi = 3.1415926538;
// 	sumIrradiance *= 2.0 * pi;
	
	// outside the grid of probes the irradiance is not known. the clamping above
	// extends the outer most probe result. the blend factor below fades out into
	// non-irradiated over a short distance.
	// 
	// this can be later on improved by using probe cascades
	sumIrradiance *= 1.0 - min( length( position - clamp( position, vec3( 0.0 ), pGIPositionClamp ) ) / 2.0, 1.0 );
	
	// final result
	return sumIrradiance;
}


// Main Function
//////////////////

void main( void ){
	ivec2 tc = ivec2( gl_FragCoord.xy );
	
	// discard not inizalized fragments or fragements that are not supposed to be lit
	vec4 diffuse = texelFetch( texDiffuse, tc, 0 );
	#ifndef GI_RAY
		if( diffuse.a == 0.0 ){
			discard;
		}
	#endif
	
	// determine position of fragment to light
	#ifdef GI_RAY
		vec4 positionDistance = texelFetch( texPosition, tc, 0 );
		if( positionDistance.a > 9999.0 ){
			discard; // ray hits nothing
		}
		vec3 position = vec3( pGIRayMatrix * vec4( positionDistance.rgb, 1.0 ) );
	#else
		#ifdef DECODE_IN_DEPTH
			float depth = dot( texelFetch( texDepth, tc, 0 ).rgb, unpackDepth );
		#else
			float depth = texelFetch( texDepth, tc, 0 ).r;
		#endif
		vec3 position = vec3( depth );
		position.z = pPosTransform.x / ( pPosTransform.y - position.z );
		position.xy = vScreenCoord * pPosTransform.zw * position.zz;
	#endif
	
	// fetch normal
	#ifdef GI_RAY
		// requires matrix transpose done by reversed order
		vec3 normal = normalLoadMaterial( texNormal, tc ) * pGIRayMatrixNormal;
		#define bendNormal normal
		
	#else
		// we can not use gbuffer normal here since it is bend potentially causing
		// troubles. derive instead the normal from the depth buffer
		vec3 normal = normalFromDepth( tc, depth, position );
		vec3 bendNormal = normalLoadMaterial( texNormal, tc );
	#endif
	
	// merge the texture-ao with the ssao. use the minimum of the two to avoid over-occluding
	// if both are used. the result is stored in aoSolidity.g . this way aoSolidity.r contains
	// the pure texture-ao and aoSolidity.gb the combined ao
	#ifndef GI_RAY
		vec3 aoSolidity = texelFetch( texAOSolidity, tc, 0 ).rgb;
		
		aoSolidity.g = min( aoSolidity.r, aoSolidity.g );
		diffuse.a *= aoSolidity.b;
	#endif
	
	// apply ambient occlusion to the direct lighting. this is done by comparing the ambient
	// occlusion angle with the lighting angle. the ambient occlusion angle can be calculated
	// using the relation ao = 1 - cos(angle). this works because the ao value can be considered
	// the ratio between a free sphere cap and the entire half sphere. since a sphere cap is
	// defined by an angle around the sphere direction the ao value can be directly related to
	// an angle with a simple calculation. this allows to add self-shadowing with next to no
	// extra cost
	// 
	// with SSAO the result is tricky and more wrong than right in some cases. due to this only
	// the texture-ao is used for self-shadowing since this one is guaranteed to be well
	// calculated and usually on a small scale
// 	lightColor *= vec3( clamp( ( ( acos( 1.0 - aoSolidity.r ) - acos( dotval ) ) * pAOSelfShadow.y ) + 1.0, pAOSelfShadow.x, 1.0 ) );
	
	// global illumination
	vec3 finalColor = giIlluminate( position, normal, bendNormal );
	
	#ifndef GI_RAY
		finalColor *= vec3( aoSolidity.g ); // texture AO and SSAO
	#endif
	
	outLuminance = dot( finalColor, lumiFactors );
	outColor = vec4( finalColor * diffuse.rgb, diffuse.a );
	outSubSurface = vec4( 0.0 );
}
