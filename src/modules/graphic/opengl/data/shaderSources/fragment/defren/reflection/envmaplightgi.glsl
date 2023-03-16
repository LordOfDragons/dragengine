precision highp float;
precision highp int;

#ifdef WITH_GI
	#include "shared/ubo_defines.glsl"
	#include "shared/defren/ubo_render_parameters.glsl"
	#include "shared/defren/light/ubo_gi.glsl"
	#include "shared/defren/gi/probe_flags.glsl"
	#include "shared/octahedral.glsl"
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
	#include "shared/defren/gi/probe_offset.glsl"
#endif

flat in int vLayer;

out vec4 outColor;


#ifdef WITH_GI
	#include "shared/defren/light/gi_illuminate.glsl"
	#include "shared/normal.glsl"
#endif


void main( void ){
	#ifdef ENVMAP_EQUI
		ivec2 tc = ivec3( gl_FragCoord.xy );
	#else
		ivec3 tc = ivec3( gl_FragCoord.xy, vLayer );
	#endif
	
	outColor = texelFetch( texEmissive, tc, 0 );
	
	#ifdef WITH_GI
		// if normal is 0 length pixel value has not been written to
		vec3 position = texelFetch( texPosition, tc, 0 ).xyz;
		vec3 diffuse = texelFetch( texDiffuse, tc, 0 ).rgb;
		vec3 normal = normalLoadShiftedInt( texNormal, tc );
		
		if( length( normal ) > 0 ){
			outColor.rgb += giIlluminate( position, normal, normal ) * diffuse;
		}
	#endif
}
