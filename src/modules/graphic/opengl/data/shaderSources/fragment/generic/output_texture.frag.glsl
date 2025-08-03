#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform vec4 pColor;
uniform vec4 pGamma;
#ifdef TEXTURELEVEL
uniform float pLevel;
#endif
#ifdef ARRAYTEXTURE
uniform float pLayer;
#endif

#ifdef ARRAYTEXTURE
uniform mediump sampler2DArray texTexture;
#else
uniform mediump sampler2D texTexture;
#endif

in vec2 vTexCoord;



#ifndef OUT_COLOR_SIZE
	#define OUT_COLOR_SIZE 4
#endif

#if OUT_COLOR_SIZE == 1
	layout(location=0) out float outColor;
#elif OUT_COLOR_SIZE == 2
	layout(location=0) out vec2 outColor;
#elif OUT_COLOR_SIZE == 3
	layout(location=0) out vec3 outColor;
#else
	layout(location=0) out vec4 outColor;
#endif

#ifdef OUT_COLOR_SWIZZLE
	#define OUT_COLOR outColor . OUT_COLOR_SWIZZLE
#else
	#define OUT_COLOR outColor
#endif



#ifndef TEX_DATA_SIZE
	#define TEX_DATA_SIZE OUT_COLOR_SIZE
#endif

#if TEX_DATA_SIZE == 1
	#define TEX_DATA_TYPE float
	#ifndef TEX_DATA_SWIZZLE
		#define TEX_DATA_SWIZZLE r
	#endif
	
#elif TEX_DATA_SIZE == 2
	#define TEX_DATA_TYPE vec2
	#ifndef TEX_DATA_SWIZZLE
		#define TEX_DATA_SWIZZLE rg
	#endif
	
#elif TEX_DATA_SIZE == 3
	#define TEX_DATA_TYPE vec3
	#ifndef TEX_DATA_SWIZZLE
		#define TEX_DATA_SWIZZLE rgb
	#endif
	
#else
	#define TEX_DATA_TYPE vec4
	#ifndef TEX_DATA_SWIZZLE
		#define TEX_DATA_SWIZZLE rgba
	#endif
#endif



void main( void ){
	TEX_DATA_TYPE color;
	
	#ifdef ARRAYTEXTURE
		#ifdef TEXTURELEVEL
			color = textureLod( texTexture, vec3( vTexCoord, pLayer ), pLevel ) . TEX_DATA_SWIZZLE;
		#else
			color = textureLod( texTexture, vec3( vTexCoord, pLayer ), 0.0 ) . TEX_DATA_SWIZZLE;
		#endif
		
	#else
		#ifdef TEXTURELEVEL
			color = textureLod( texTexture, vTexCoord, pLevel ) . TEX_DATA_SWIZZLE;
		#else
			color = textureLod( texTexture, vTexCoord, 0.0 ) . TEX_DATA_SWIZZLE;
		#endif
	#endif
	
	OUT_COLOR = pow( color * TEX_DATA_TYPE( pColor ), TEX_DATA_TYPE( pGamma ) );
}
