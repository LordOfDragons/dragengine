#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

// DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED //
//                                                        //
//             use screenspace.glsl instead               //
//    using defines: FULLSCREENQUAD, NO_POSTRANSFORM      //
//                                                        //
// DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED //

#ifndef GEOMETRY_SHADER
	#ifdef GS_RENDER_STEREO
		#define GEOMETRY_SHADER 1
	#endif
#endif

#ifndef NO_TEXCOORD
	uniform vec4 pQuadParams; // scaleX, scaleY, offsetX, offsetY
#endif

layout(location=0) in vec2 inPosition;

#ifdef VS_RENDER_STEREO
	layout(location=1) in int inLayer;
#else
	const int inLayer = 0;
#endif

#ifndef NO_TEXCOORD
	#ifdef GEOMETRY_SHADER
		out vec2 vGSTexCoord;
		#define vTexCoord vGSTexCoord
	#else
		out vec2 vTexCoord;
	#endif
#endif

#ifdef VS_RENDER_STEREO
	flat out int vLayer;
#endif

void main( void ){
	gl_Position = vec4( inPosition, 0, 1 );
	#ifndef NO_TEXCOORD
		vTexCoord = inPosition * pQuadParams.xy + pQuadParams.zw;
	#endif
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
