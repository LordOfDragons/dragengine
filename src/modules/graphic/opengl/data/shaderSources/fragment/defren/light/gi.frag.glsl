#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/defren/light/ubo_gi.glsl"
#include "shared/defren/gi/probe_flags.glsl"
#include "shared/octahedral.glsl"


// samplers
/////////////

// GIRay
// vvvvv
layout(binding=0) uniform HIGHP sampler2D texGIPosition;
layout(binding=1) uniform lowp sampler2D texGIDiffuse;
layout(binding=2) uniform lowp sampler2D texGINormal;
layout(binding=3) uniform lowp sampler2D texGIReflectivity; // reflectivity.rgb, roughness

// NOTE transfer:
// params = giRayCastMaterialParams(ray.material)
// tc = giRayCastMaterialTC(params, ray.tc)
// texDiffuse: vec3 giRayCastSampleColor(params.g, tc)
// texReflectivity: giRayCastSampleReflectivityRoughness(params.b, tc, out reflectivity, out roughness)
// 
// hence these values have to be stored in the ray:
// - int ray.material => requires R16UI
// - vec2 ray.texCoord = giRayCastFaceTexCoord() => requires RG16F
// ^^^^^

// !GIRay
layout(binding=0) uniform HIGHP sampler2DArray texDepth;
layout(binding=1) uniform lowp sampler2DArray texDiffuse;
layout(binding=2) uniform lowp sampler2DArray texNormal;
layout(binding=3) uniform lowp sampler2DArray texReflectivity;
layout(binding=4) uniform lowp sampler2DArray texRoughness;
layout(binding=5) uniform lowp sampler2DArray texAOSolidity;

layout(binding=6) uniform lowp sampler2DArray texGIIrradiance;
layout(binding=7) uniform HIGHP sampler2DArray texGIDistance;


// includes to come after defining fixed position samplers
#define pGIGridProbeCount pGIParams[0].probeCount
#define TEX_GI_PROBE_OFFSET_BINDING 8
#include "shared/defren/gi/probe_offset.glsl"


#include "shared/interface/2d/fragment.glsl"


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


#include "shared/normal/texture.glsl"
// !GIRay
#include "shared/defren/depth_to_position.glsl"
#include "shared/defren/depth_to_position_fragment.glsl"
#include "shared/defren/light/normal_from_depth.glsl"

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
	ivec3 tc = ivec3(gl_FragCoord.xy, vLayer);
	ivec2 tcGI = ivec2(gl_FragCoord.xy);
	
	// discard not inizalized fragments or fragements that are not supposed to be lit
	vec4 diffuse;
	
	if(GIRay){
		diffuse = texelFetch(texGIDiffuse, tcGI, 0);
		
	}else{
		diffuse = texelFetch(texDiffuse, tc, 0);
		if(diffuse.a == 0.0){
			outputUnlit();
			return;
		}
	}
	
	// determine position of fragment to light
	vec3 position;
	float depth;
	
	if(GIRay){
		vec4 positionDistance = texelFetch(texGIPosition, tcGI, 0);
		if( positionDistance.a > 9999.0 ){
			// ray hits nothing
			outputUnlit();
			return;
		}
		position = vec3( pGIRayMatrix * vec4( positionDistance.rgb, 1 ) );
		
	}else{
		depth = sampleDepth( texDepth, tc );
		position = depthToPosition( depth, vScreenCoord, vLayer );
	}
	
	// fetch normal
	vec3 normal, bendNormal;
	
	if(GIRay){
		// requires matrix transpose done by reversed order
		normal = sanitizeNormal(normalLoadMaterial(texGINormal, tcGI) * pGIRayMatrixNormal);
		bendNormal = normal;
		
	}else{
		// we can not use gbuffer normal here since it is bend potentially causing
		// troubles. derive instead the normal from the depth buffer
		normal = normalFromDepth( tc, depth, position );
		bendNormal = sanitizeNormal( normalLoadMaterial( texNormal, tc ) );
	}
	
	// merge the texture-ao with the ssao. use the minimum of the two to avoid over-occluding
	// if both are used. the result is stored in aoSolidity.g . this way aoSolidity.r contains
	// the pure texture-ao and aoSolidity.gb the combined ao
	vec3 aoSolidity;
	
	if(!GIRay){
		aoSolidity = texelFetch( texAOSolidity, tc, 0 ).rgb;
		
		aoSolidity.g = min( aoSolidity.r, aoSolidity.g );
		diffuse.a *= aoSolidity.b;
	}
	
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
	
	if(!GIRay){
		finalColor *= vec3( aoSolidity.g ); // texture AO and SSAO
	}
	
	outLuminance = vec4( vec3( dot( finalColor, lumiFactors ) ), diffuse.a );
	outColor = vec4( finalColor * diffuse.rgb, diffuse.a );
	outSubSurface = vec4( 0, 0, 0, diffuse.a );
}
