precision HIGHP float;
precision HIGHP int;

uniform vec4 pColor;

layout(location=0) out vec4 outColor;

#ifdef WITH_DEPTH
	#include "shared/ubo_defines.glsl"
	#include "shared/defren/ubo_render_parameters.glsl"
	
	uniform HIGHP sampler2DArray texDepth;
	
	#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
		flat in int vLayer;
	#else
		const int vLayer = 0;
	#endif
	
	#include "shared/defren/sample_depth.glsl"
#endif

void main( void ){
	#ifdef WITH_DEPTH
		float depth = sampleDepth( texDepth, ivec3( gl_FragCoord.xy, vLayer ) );
		#ifdef INVERSE_DEPTH
			if( gl_FragCoord.z < depth ) discard;
		#else
			if( gl_FragCoord.z > depth ) discard;
		#endif
	#endif
	
	outColor = pColor;
}
