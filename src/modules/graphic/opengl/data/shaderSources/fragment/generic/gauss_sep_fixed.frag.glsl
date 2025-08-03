#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

#ifdef DEPTH_DIFFERENCE_WEIGHTING
uniform vec4 pTCTransformAlt; // xy=scale, zw=offset
#endif

uniform vec4 pOffsets1; // offset1.st, -offset1.st
#if TAP_COUNT > 3
uniform vec4 pOffsets2; // offset2.st, -offset2.st
#endif
#if TAP_COUNT > 5
uniform vec4 pOffsets3; // offset3.st, -offset3.st
#endif
#if TAP_COUNT > 7
uniform vec4 pOffsets4; // offset4.st, -offset4.st
#endif
#if TAP_COUNT > 9
uniform vec4 pOffsets5; // offset5.st, -offset5.st
#endif
#if TAP_COUNT > 11
uniform vec4 pOffsets6; // offset6.st, -offset6.st
#endif
#if TAP_COUNT > 13
uniform vec4 pOffsets7; // offset7.st, -offset7.st
#endif

uniform vec4 pWeights1; // weightCenter, weight1, weight2, weight3
#if TAP_COUNT > 7
uniform vec4 pWeights2; // weight4, weight5, weight6, weight7
#endif

uniform vec2 pClamp; // clamp.s, clamp.t
#ifdef DEPTH_DIFFERENCE_WEIGHTING
uniform float pDepthDifferenceThreshold;
#endif

#ifdef INPUT_ARRAY_TEXTURES
	#define TEX_DATA_SAMPLER sampler2DArray
#else
	#define TEX_DATA_SAMPLER sampler2D;
#endif
uniform mediump TEX_DATA_SAMPLER texData;

#ifdef DEPTH_DIFFERENCE_WEIGHTING
	uniform HIGHP sampler2DArray texDepth;
#endif

in vec2 vTexCoord;

#ifdef INPUT_ARRAY_TEXTURES
	#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
		flat in int vLayer;
	#else
		const int vLayer = 0;
	#endif
#endif


#ifndef OUT_DATA_SIZE
	#define OUT_DATA_SIZE 4
#endif

#if OUT_DATA_SIZE == 1
	layout(location=0) out float outData;
#elif OUT_DATA_SIZE == 2
	layout(location=0) out vec2 outData;
#elif OUT_DATA_SIZE == 3
	layout(location=0) out vec3 outData;
#else
	layout(location=0) out vec4 outData;
#endif

#ifdef OUT_DATA_SWIZZLE
	#define OUT_DATA outData . OUT_DATA_SWIZZLE
#else
	#define OUT_DATA outData
#endif



#ifndef TEX_DATA_SIZE
	#define TEX_DATA_SIZE OUT_DATA_SIZE
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

#ifdef INPUT_ARRAY_TEXTURES
TEX_DATA_TYPE sampleData( vec2 tc ){
	return textureLod( texData, vec3( tc, vLayer ), 0.0 ) . TEX_DATA_SWIZZLE;
}
#else
TEX_DATA_TYPE sampleData( vec2 tc ){
	return textureLod( texData, tc, 0.0 ) . TEX_DATA_SWIZZLE;
}
#endif


#include "shared/defren/depth_to_position.glsl"
#ifdef DEPTH_DIFFERENCE_WEIGHTING
	#include "shared/defren/depth_to_position_fragment.glsl"
#endif


#ifdef DEPTH_DIFFERENCE_WEIGHTING
void processPixel( in vec2 tc, in float weight, in float refdepth, inout TEX_DATA_TYPE accum, inout float weightSum ){
	float depth = depthToZ( texDepth, vec3( tc * pTCTransformAlt.xy + pTCTransformAlt.zw, vLayer ), vLayer );
	weight *= max( 0.0, 1.0 - pDepthDifferenceThreshold * abs( depth - refdepth ) );
	accum += sampleData( tc ) * TEX_DATA_TYPE( weight );
	weightSum += weight;
}

#else
void processPixel( in vec2 tc, in float weight, in float refdepth, inout TEX_DATA_TYPE accum, inout float weightSum ){
	accum += sampleData( tc ) * TEX_DATA_TYPE( weight );
}
#endif



void main( void ){
	TEX_DATA_TYPE accum = sampleData( vTexCoord ) * TEX_DATA_TYPE( pWeights1.x );
	float refdepth = 0.0;
	float weightSum = 1.0;
	
	#ifdef DEPTH_DIFFERENCE_WEIGHTING
		refdepth = depthToZ( texDepth, vec3( vTexCoord * pTCTransformAlt.xy + pTCTransformAlt.zw, vLayer ), vLayer );
		weightSum = pWeights1.x;
	#endif
	
	vec4 tc = min( vTexCoord.stst + pOffsets1, pClamp.stst );
	processPixel( tc.xy, pWeights1.y, refdepth, accum, weightSum );
	processPixel( tc.zw, pWeights1.y, refdepth, accum, weightSum );
	
	#if TAP_COUNT > 3
		tc = min( vTexCoord.stst + pOffsets2, pClamp.stst );
		processPixel( tc.xy, pWeights1.z, refdepth, accum, weightSum );
		processPixel( tc.zw, pWeights1.z, refdepth, accum, weightSum );
	#endif
	
	#if TAP_COUNT > 5
		tc = min( vTexCoord.stst + pOffsets3, pClamp.stst );
		processPixel( tc.xy, pWeights1.w, refdepth, accum, weightSum );
		processPixel( tc.zw, pWeights1.w, refdepth, accum, weightSum );
	#endif
	
	#if TAP_COUNT > 7
		tc = min( vTexCoord.stst + pOffsets4, pClamp.stst );
		processPixel( tc.xy, pWeights2.x, refdepth, accum, weightSum );
		processPixel( tc.zw, pWeights2.x, refdepth, accum, weightSum );
	#endif
	
	#if TAP_COUNT > 9
		tc = min( vTexCoord.stst + pOffsets5, pClamp.stst );
		processPixel( tc.xy, pWeights2.y, refdepth, accum, weightSum );
		processPixel( tc.zw, pWeights2.y, refdepth, accum, weightSum );
	#endif
	
	#if TAP_COUNT > 11
		tc = min( vTexCoord.stst + pOffsets6, pClamp.stst );
		processPixel( tc.xy, pWeights2.z, refdepth, accum, weightSum );
		processPixel( tc.zw, pWeights2.z, refdepth, accum, weightSum );
	#endif
	
	#if TAP_COUNT > 13
		tc = min( vTexCoord.stst + pOffsets7, pClamp.stst );
		processPixel( tc.xy, pWeights2.w, refdepth, accum, weightSum );
		processPixel( tc.zw, pWeights2.w, refdepth, accum, weightSum );
	#endif
	
	#ifdef DEPTH_DIFFERENCE_WEIGHTING
		OUT_DATA = accum / TEX_DATA_TYPE( weightSum );
	#else
		OUT_DATA = accum;
	#endif
}
