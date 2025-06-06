precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/defren/light/ubo_gi.glsl"
#include "shared/defren/gi/probe_flags.glsl"
#include "shared/octahedral.glsl"


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
	uniform HIGHP sampler2DArray texDepth;
	uniform lowp sampler2DArray texDiffuse;
	uniform lowp sampler2DArray texNormal;
	uniform lowp sampler2DArray texReflectivity;
	uniform lowp sampler2DArray texRoughness;
	uniform lowp sampler2DArray texAOSolidity;
#endif

uniform lowp sampler2DArray texGIIrradiance;
uniform HIGHP sampler2DArray texGIDistance;


// includes to come after defining fixed position samplers
#define pGIGridProbeCount pGIParams[0].probeCount
#include "shared/defren/gi/probe_offset.glsl"


// inputs
///////////

in vec2 vTexCoord; // not used but present in generic vertex shader source
in vec2 vScreenCoord;

#ifndef GI_RAY
	#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
		flat in int vLayer;
	#else
		const int vLayer = 0;
	#endif
#endif


// outputs
////////////

layout(location=0) out vec4 outColor;
layout(location=1) out vec4 outLuminance;
	// vec4 not float since blending is required. if float is used
	// GPU randomly writes NaN values breaking rendering
layout(location=2) out vec4 outSubSurface;


// constants
//////////////

const vec3 lumiFactors = vec3( 0.2125, 0.7154, 0.0721 );


#include "shared/normal_texture.glsl"
#ifndef GI_RAY
	#include "shared/defren/depth_to_position.glsl"
	#include "shared/defren/depth_to_position_fragment.glsl"
	#include "shared/defren/light/normal_from_depth.glsl"
#endif

#include "shared/defren/light/gi_illuminate.glsl"



// Discard fragment without disabling early-z
///////////////////////////////////////////////

void outputUnlit(){
	outColor = vec4( 0 );
	outLuminance = vec4( 0 );
	outSubSurface = vec4( 0 );
}


// Main Function
//////////////////

void main( void ){
	#ifdef GI_RAY
		ivec2 tc = ivec2( gl_FragCoord.xy );
	#else
		ivec3 tc = ivec3( gl_FragCoord.xy, vLayer );
	#endif
	
	// discard not inizalized fragments or fragements that are not supposed to be lit
	vec4 diffuse = texelFetch( texDiffuse, tc, 0 );
	#ifndef GI_RAY
		if( diffuse.a == 0.0 ){
			outputUnlit();
			return;
		}
	#endif
	
	// determine position of fragment to light
	#ifdef GI_RAY
		vec4 positionDistance = texelFetch( texPosition, tc, 0 );
		if( positionDistance.a > 9999.0 ){
			// ray hits nothing
			outputUnlit();
			return;
		}
		vec3 position = vec3( pGIRayMatrix * vec4( positionDistance.rgb, 1 ) );
	#else
		float depth = sampleDepth( texDepth, tc );
		vec3 position = depthToPosition( depth, vScreenCoord, vLayer );
	#endif
	
	// fetch normal
	#ifdef GI_RAY
		// requires matrix transpose done by reversed order
		vec3 normal = sanitizeNormal( normalLoadMaterial( texNormal, tc ) * pGIRayMatrixNormal );
		#define bendNormal normal
		
	#else
		// we can not use gbuffer normal here since it is bend potentially causing
		// troubles. derive instead the normal from the depth buffer
		vec3 normal = normalFromDepth( tc, depth, position );
		vec3 bendNormal = sanitizeNormal( normalLoadMaterial( texNormal, tc ) );
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
	
	outLuminance = vec4( vec3( dot( finalColor, lumiFactors ) ), diffuse.a );
	outColor = vec4( finalColor * diffuse.rgb, diffuse.a );
	outSubSurface = vec4( 0, 0, 0, diffuse.a );
}
