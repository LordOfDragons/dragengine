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
	vec3 unclampedPosition = vec3( pGIMatrix * vec4( position, 1.0 ) );
	position = clamp( unclampedPosition, vec3( 0.0 ), pGIPositionClamp );
	
	normal = normalize( normal * pGIMatrixNormal ); // reverse order does transpose()
	bendNormal = normalize( bendNormal * pGIMatrixNormal ); // reverse order does transpose()
	
	// if the fragment happens to end up exactly on the grid hell can break loose. in this
	// situation neighbor fragments can choose different cages to sample from depending on
	// numerical inaccuricies. this results in flickering lightings on walls because probes
	// inside the wall tend to be disabled. if the cage inside the wall is chosen all probes
	// are disabled which contrasts with probes inside the room being lit. even without
	// disabled probes the problem persists since using a cage inside the wall has no chance
	// to pick up light from inside room probes even if the probe sits right on the wall.
	// 
	// as a solution the fragment position is slightly nudged away from the grid boundary.
	// the question is though in which direction to push. the most sane direction to nudge
	// along is the fragment normal. this raises the chance to end up in a cage with useful
	// probes to sample lighting from. both the normal and bendNormal can be used for this
	// 
	// nudging the position slightly is not wrong for positions far away from grid boundaries.
	// for this reason no condition or mix call is required to prevent nudging. instead all
	// positions are slightly nudges along their normals
// 	vec3 gridPosition = pGIProbeSpacingInv * position;
// 	vec3 nudgePosition = mix( vec3( 0.0 ), bendNormal * 0.01,
// 		lessThan( abs( gridPosition - floor( gridPosition + vec3( 0.5 ) ) ), vec3( 0.01 ) ) );
// 	gridPosition = pGIProbeSpacingInv * clamp( unclampedPosition + nudgePosition, vec3( 0.0 ), pGIPositionClamp );
	ivec3 baseCoord = clamp( ivec3( pGIProbeSpacingInv
		* ( unclampedPosition + bendNormal * 0.01 ) ), ivec3( 0 ), pGIProbeClamp );
	
	// from here on we should be located in a good cage
	vec3 basePosition = pGIProbeSpacing * vec3( baseCoord );
	vec3 sumIrradiance = vec3( 0.0 );
	float sumWeight = 0.0;
	
	// distance from floor(currentVertex) position
	//vec3 alpha = clamp( pGIProbeSpacingInv * ( position - basePosition ), vec3( 0.0 ), vec3( 1.0 ) );
	
	// here something can go wrong. in the paper all probes are used which causes leaking.
	// by disabling probes (inside walls) or samples thereof (rays hitting geometry from
	// the outside) annoying artifacts can happen. "alpha" value contains 0 if the position
	// aligns with the base probe coordinate in one or more axes. basically this means one
	// or more fragment coordinates fall on probe grid. if this happens "trilinear" calculated
	// in the loop can contain 0 in one or more components. this causes weight to drop to 0.
	// now what happens is that disabled probes have 0 weight but good trilinear value.
	// on the other hand the enabled probes which should be used end up with >0 weight but
	// trilinear contains 0 components killing off the weight. all probes end up with 0 weight
	// and a problem is born.
	// 
	// the main problem here is disabling probes or samples thereof. but this is required
	// to remove the annoying artifacts. to solve this the alpha value used for calculating
	// trilinear value is slightly nudged off the grid if it happens to be on it. this does
	// affect only the rare case of fragments ending up in the bad spot
	//alpha = mix( alpha, alpha + vec3( 0.001 ), lessThan( abs( alpha ), vec3( 0.001 ) ) );
	vec3 alpha = clamp( pGIProbeSpacingInv * ( position - basePosition ), vec3( 0.01 ), vec3( 0.99 ) );
	
	// iterate over adjacent probe cage
	int i;
	for( i=0; i<8; i++ ){
		// offset = 0 or 1 along each axis
		ivec3 offset = ivec3( i, i >> 1, i >> 2 ) & ivec3( 1 );
		ivec3 probeCoord = clamp( baseCoord + offset, ivec3( 0 ), pGIProbeClamp );
		vec3 probePosition = pGIProbeSpacing * vec3( probeCoord );
		
		probeCoord = giGridShiftToLocal( probeCoord );
		
		probePosition += gipoProbeOffset( probeCoord );
		
		vec3 viewVector = probePosition - position;
		vec3 viewDir = normalize( viewVector );
		
		// in the original paper code something really strange is going on. they use
		// ws_o (or w_o) to offset. the code calculates ws_o in front of the for-loop
		// as "rayOrigin - position", which would be "viewDir" but outside the for-loop
		// there exists no rayOrigin. which of the cage probes is the ray origin?
		// this term makes no sense
		vec3 probeToPoint = -viewVector + ( normal /*+ 3.0 * ws_o*/ ) * pGINormalBias;
		
		vec3 trilinear = mix( vec3( 1.0 ) - alpha, alpha, offset );
		float weight = 1.0;
		
		// smooth backface test
		{
		float value = max( 0.0001, ( dot( viewDir, normal ) + 1.0 ) * 0.5 );
		//weight *= value * value + 0.2;
		
		// deviating from the paper here. the 0.2 addition term creates additional
		// light leaks. removing this term reduces the light leaks in dark rooms
		weight *= value * value;
		}
		
		// moment visibility test
		{
		vec2 texCoord = giTCFromDirection( normalize( probeToPoint ), probeCoord, pGIDistanceMapScale, pGIDistanceMapSize );
		float distToProbe = length( probeToPoint );
		
		vec2 temp = texture( texGIDistance, texCoord ).ra; // RG16 in opengl has RRRG as swizzle
		float mean = temp.x;
		float variance = abs( mean * mean - temp.y );
		
		float chebyshevWeight = max( distToProbe - mean, 0.0 );
		chebyshevWeight = variance / ( variance + chebyshevWeight * chebyshevWeight );
		chebyshevWeight = max( chebyshevWeight * chebyshevWeight * chebyshevWeight, 0.0 );
		
		weight *= distToProbe <= mean ? 1.0 : chebyshevWeight;
		}
		
		// avoid zero weight
		weight = max( 0.000001, weight );
		
		vec2 texCoord = giTCFromDirection( bendNormal, probeCoord, pGIIrradianceMapScale, pGIIrradianceMapSize );
		
		vec3 probeIrradiance = texture( texGIIrradiance, texCoord ).rgb;
		
		const float crushThreshold = 0.2;
		if( weight < crushThreshold ){
			weight *= weight * weight / ( crushThreshold * crushThreshold );
		}
		
		// trilinear weights
		weight *= trilinear.x * trilinear.y * trilinear.z;
		sumIrradiance += probeIrradiance * weight;
		sumWeight += weight;
	}
	
	// outside the grid of probes the irradiance is not known. the clamping above extends the
	// outer most probe result. the blend factor below fades out into non-irradiated over a
	// short distance.
	// 
	// this can be later on improved by using probe cascades
	float blend = 1.0 / sumWeight;
	blend *= 1.0 - min( length( unclampedPosition - position ) / 2.0, 1.0 );
	blend *= pGIEnergyPreservation;
	
	return sumIrradiance * blend;
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
