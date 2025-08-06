#include "shared/preamble.glsl"

precision mediump float;
precision mediump int;

#ifdef MIPMAP
	uniform float pMipMapLevel;
#endif
#ifdef TCCLAMP
	#ifdef MIPMAP
		uniform vec2 pTCClamp;
	#else
		uniform ivec2 pTCClamp;
	#endif
#endif

#ifdef INPUT_ARRAY_TEXTURE
	layout(binding=0) uniform mediump sampler2DArray texColor;
	#define TEX_COLOR(tc) textureLod( texColor, vec3( tc, vLayer ), pMipMapLevel )
	#define TEX_FETCH_COLOR(tc) texelFetch( texColor, ivec3( tc, vLayer ), 0 )
#else
	layout(binding=0) uniform mediump sampler2D texColor;
	#define TEX_COLOR(tc) textureLod( texColor, vec2( tc ), pMipMapLevel )
	#define TEX_FETCH_COLOR(tc) texelFetch( texColor, tc, 0 )
#endif

#include "shared/interface/2d_fragment.glsl"

layout(location=0) out mediump vec4 outColor;

void main( void ){
	#ifdef MIPMAP
		#ifdef TCCLAMP
			outColor = TEX_COLOR( min( vTexCoord, pTCClamp ) );
		#else
			outColor = TEX_COLOR( vTexCoord );
		#endif
	#else
		#ifdef TCCLAMP
			outColor = TEX_FETCH_COLOR( min( ivec2( gl_FragCoord.xy ), pTCClamp ) );
		#else
			outColor = TEX_FETCH_COLOR( ivec2( gl_FragCoord.xy ) );
		#endif
	#endif
}
