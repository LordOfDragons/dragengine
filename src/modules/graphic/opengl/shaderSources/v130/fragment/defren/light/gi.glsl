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

uniform HIGHP sampler2D texDepth;
uniform lowp sampler2D texDiffuse;
uniform lowp sampler2D texNormal;
uniform lowp sampler2D texAOSolidity;

uniform lowp sampler2D texGIIrradiance;
uniform HIGHP sampler2D texGIDistance;


// inputs
///////////

in vec2 vScreenCoord;


// outputs
////////////

out vec4 outColor;
out float outLuminance;


// constants
//////////////

#ifdef DECODE_IN_DEPTH
	const vec3 unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif

const vec3 lumiFactors = vec3( 0.2125, 0.7154, 0.0721 );


#include "v130/shared/defren/light/normal_from_depth.glsl"


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
vec3 giIlluminate( in vec3 position, in vec3 normal ){
	vec3 unclampedPosition = vec3( pGIMatrix * vec4( position, 1.0 ) );
	position = clamp( unclampedPosition, vec3( 0.0 ), pGIPositionClamp );
	
	normal = normal * pGIMatrixNormal; // reverse order does transpose()
	
	ivec3 baseCoord = clamp( ivec3( pGIProbeSpacingInv * position ), ivec3( 0 ), pGIProbeClamp );
	
	vec3 basePosition = pGIProbeSpacing * vec3( baseCoord );
	vec3 sumIrradiance = vec3( 0.0 );
	float sumWeight = 0.0;
	
	// distance from floor(currentVertex) position
	vec3 alpha = clamp( pGIProbeSpacingInv * ( position - basePosition ), vec3( 0.0 ), vec3( 1.0 ) );
	
	// iterate over adjacent probe cage
	int i;
	for( i=0; i<8; i++ ){
		// offset = 0 or 1 along each axis
		ivec3 offset = ivec3( i, i >> 1, i >> 2 ) & ivec3( 1 );
		ivec3 probeCoord = clamp( baseCoord + offset, ivec3( 0 ), pGIProbeClamp );
		vec3 probePosition = pGIProbeSpacing * vec3( probeCoord );
		
		probeCoord = giGridShiftToLocal( probeCoord );
		
		vec3 viewVector = probePosition - position;
		vec3 viewDir = normalize( viewVector );
		
		// ws_o = rayOrigin. where is this coming from? we are shading here scene fragments.
		// rayOrigin belongs to probe shading
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
		
		vec2 texCoord = giTCFromDirection( normal, probeCoord, pGIIrradianceMapScale, pGIIrradianceMapSize );
		
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
	if( diffuse.a == 0.0 ){
		discard;
	}
	
	// determine position of fragment to light
	#ifdef DECODE_IN_DEPTH
		float depth = dot( texelFetch( texDepth, tc, 0 ).rgb, unpackDepth );
	#else
		float depth = texelFetch( texDepth, tc, 0 ).r;
	#endif
	vec3 position = vec3( depth );
	position.z = pPosTransform.x / ( pPosTransform.y - position.z );
	position.xy = vScreenCoord * pPosTransform.zw * position.zz;
	
	// fetch normal. we can not use gbuffer normal here since it is bend potentially causing
	// troubles. derive instead the normal from the depth buffer
	/*
	#ifdef MATERIAL_NORMAL_INTBASIC
		vec3 normal = texelFetch( texNormal, tc, 0 ).rgb * vec3( 2.0 ) + vec3( -1.0 ); // IF USING FLOAT TEXTURE
		//vec3 normal = texelFetch( texNormal, tc, 0 ).rgb * vec3( 1.9921569 ) + vec3( -0.9921722 ); // IF USING INT TEXTURE
	#elif defined MATERIAL_NORMAL_SPHEREMAP
		vec2 fenc = texelFetch( texNormal, tc, 0 ).rgb * vec2( 4.0 ) - vec2( 2.0 );
		float f = dot( fenc, fenc );
		float g = sqrt( 1.0 - f * 0.25 );
		vec3 normal = vec3( fenc.xy * vec2( g ), f * 0.5 - 1.0 );
	#else
		vec3 normal = texelFetch( texNormal, tc, 0 ).rgb;
	#endif
	
	if( dot( normal, normal ) < 0.0001 ){
		normal = lightDir; // 0-normal means always point towards light source
		
	}else{
		normal = clamp( normal, vec3( -1.0 ), vec3( 1.0 ) ); // some shader writes broken normals (or missing clear?). temporary fix
		normal = normalize( normal );
	}
	*/
	vec3 normal = normalFromDepth( tc, depth, position );
	
	// merge the texture-ao with the ssao. use the minimum of the two to avoid over-occluding
	// if both are used. the result is stored in aoSolidity.g . this way aoSolidity.r contains
	// the pure texture-ao and aoSolidity.gb the combined ao
	vec3 aoSolidity = texelFetch( texAOSolidity, tc, 0 ).rgb;
	
	aoSolidity.g = min( aoSolidity.r, aoSolidity.g );
	diffuse.a *= aoSolidity.b;
	
	
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
	vec3 finalColor = giIlluminate( position, normal );
	
	finalColor *= vec3( aoSolidity.g ); // texture AO and SSAO
	
	outLuminance = dot( finalColor, lumiFactors );
	outColor = vec4( finalColor * diffuse.rgb, diffuse.a );
}
