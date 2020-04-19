precision highp float;
precision highp int;

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
uniform vec3 pDepthTransform; // transformZ1, transformZ2, depthThreshold
#endif

uniform mediump sampler2D texData;
#ifdef DEPTH_DIFFERENCE_WEIGHTING
uniform HIGHP sampler2D texDepth;
#endif

in vec2 vTexCoord;



#ifndef OUT_DATA_SIZE
	#define OUT_DATA_SIZE 4
#endif

#if OUT_DATA_SIZE == 1
	out float outData;
#elif OUT_DATA_SIZE == 2
	out vec2 outData;
#elif OUT_DATA_SIZE == 3
	out vec3 outData;
#else
	out vec4 outData;
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

#define TEX_DATA(tc,weights) ( textureLod( texData, tc, 0.0 ) . TEX_DATA_SWIZZLE * TEX_DATA_TYPE( weights ) )



#ifdef DECODE_IN_DEPTH
	const vec3 unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif

#ifdef DEPTH_DIFFERENCE_WEIGHTING
	#ifdef DECODE_IN_DEPTH
		#define TAP_DEPTH(tc) depth = pDepthTransform.x / ( pDepthTransform.y - dot( textureLod( texDepth, tc, 0.0 ).rgb, unpackDepth ) )
		//#define TAP_DEPTH(tc) depth = dot( textureLod( texDepth, tc, 0.0 ).rgb, unpackDepth )
	#else
		#define TAP_DEPTH(tc) depth = pDepthTransform.x / ( pDepthTransform.y - textureLod( texDepth, tc, 0.0 ).r )
		//#define TAP_DEPTH(tc) depth = textureLod( texDepth, tc, 0.0 ).r
	#endif
	#define DDW_CALC_WEIGHT(w) weight = w * max( 0.0, 1.0 - pDepthTransform.z * abs( depth - refdepth ) )
	#define EVAL_PIXEL(tc,w) TAP_DEPTH(tc); DDW_CALC_WEIGHT(w); accum += TEX_DATA(tc,weight); weightSum += weight
#else
	#define EVAL_PIXEL(tc,w) accum += TEX_DATA(tc,w)
#endif



void main( void ){
	TEX_DATA_TYPE accum = TEX_DATA( vTexCoord, pWeights1.x );
	#ifdef DEPTH_DIFFERENCE_WEIGHTING
		#ifdef DECODE_IN_DEPTH
			float refdepth = pDepthTransform.x / ( pDepthTransform.y - dot( texelFetch( texDepth, ivec2( gl_FragCoord.xy ), 0 ).rgb, unpackDepth ) );
			//float refdepth = dot( texelFetch( texDepth, ivec2( gl_FragCoord.xy ), 0 ).rgb, unpackDepth );
		#else
			float refdepth = pDepthTransform.x / ( pDepthTransform.y - texelFetch( texDepth, ivec2( gl_FragCoord.xy ), 0 ).r );
			//float refdepth = texelFetch( texDepth, ivec2( gl_FragCoord.xy ), 0 ).r;
		#endif
		float weightSum = pWeights1.x;
		float weight, depth;
	#endif
	
	vec4 tc = min( vTexCoord.stst + pOffsets1, pClamp.stst );
	EVAL_PIXEL( tc.xy, pWeights1.y );
	EVAL_PIXEL( tc.zw, pWeights1.y );
	
	#if TAP_COUNT > 3
		tc = min( vTexCoord.stst + pOffsets2, pClamp.stst );
		EVAL_PIXEL( tc.xy, pWeights1.z );
		EVAL_PIXEL( tc.zw, pWeights1.z );
	#endif
	
	#if TAP_COUNT > 5
		tc = min( vTexCoord.stst + pOffsets3, pClamp.stst );
		EVAL_PIXEL( tc.xy, pWeights1.w );
		EVAL_PIXEL( tc.zw, pWeights1.w );
	#endif
	
	#if TAP_COUNT > 7
		tc = min( vTexCoord.stst + pOffsets4, pClamp.stst );
		EVAL_PIXEL( tc.xy, pWeights2.x );
		EVAL_PIXEL( tc.zw, pWeights2.x );
	#endif
	
	#if TAP_COUNT > 9
		tc = min( vTexCoord.stst + pOffsets5, pClamp.stst );
		EVAL_PIXEL( tc.xy, pWeights2.y );
		EVAL_PIXEL( tc.zw, pWeights2.y );
	#endif
	
	#if TAP_COUNT > 11
		tc = min( vTexCoord.stst + pOffsets6, pClamp.stst );
		EVAL_PIXEL( tc.xy, pWeights2.z );
		EVAL_PIXEL( tc.zw, pWeights2.z );
	#endif
	
	#if TAP_COUNT > 13
		tc = min( vTexCoord.stst + pOffsets7, pClamp.stst );
		EVAL_PIXEL( tc.xy, pWeights2.w );
		EVAL_PIXEL( tc.zw, pWeights2.w );
	#endif
	
	#ifdef DEPTH_DIFFERENCE_WEIGHTING
		OUT_DATA = accum / TEX_DATA_TYPE( weightSum );
	#else
		OUT_DATA = accum;
	#endif
}
