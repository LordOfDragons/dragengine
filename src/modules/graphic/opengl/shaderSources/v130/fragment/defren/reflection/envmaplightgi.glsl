precision highp float;
precision highp int;

#ifdef WITH_GI
	int pGIHighestCascade; // index of highest cascade
	
	#include "v130/shared/ubo_defines.glsl"
// 	#include "v130/shared/defren/light/ubo_render_parameters.glsl"
	#include "v130/shared/defren/light/ubo_gi.glsl"
	#include "v130/shared/defren/gi/probe_flags.glsl"
	#include "v130/shared/octahedral.glsl"
#endif

#ifdef ENVMAP_EQUI
	#define SAMPLER_ENVMAP mediump sampler2D
#else
	#define SAMPLER_ENVMAP mediump sampler2DArray
#endif

uniform SAMPLER_ENVMAP texPosition;
uniform SAMPLER_ENVMAP texDiffuse;
uniform SAMPLER_ENVMAP texNormal;
uniform SAMPLER_ENVMAP texEmissive;

#ifdef WITH_GI
	uniform lowp sampler2DArray texGIIrradiance;
	uniform HIGHP sampler2DArray texGIDistance;
	
	// includes to come after defining fixed position samplers
	#define pGIGridProbeCount pGIParams[0].probeCount
	#include "v130/shared/defren/gi/probe_offset.glsl"
#endif

flat in int vLayer;

out vec4 outColor;


#ifdef WITH_GI
	#include "v130/shared/defren/light/gi_illuminate.glsl"
#endif


void main( void ){
	#ifdef ENVMAP_EQUI
		ivec2 tc = ivec3( gl_FragCoord.xy );
	#else
		ivec3 tc = ivec3( gl_FragCoord.xy, vLayer );
	#endif
	
	outColor = texelFetch( texEmissive, tc, 0 );
	
	#ifdef WITH_GI
		vec3 position = texelFetch( texPosition, tc, 0 ).xyz;
		vec3 diffuse = texelFetch( texDiffuse, tc, 0 ).rgb;
		vec3 normal = texelFetch( texNormal, tc, 0 ).rgb;
		outColor.rgb += giIlluminate( position, normal, normal ) * diffuse;
	#endif
}
