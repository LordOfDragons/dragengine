#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

// DepthDifferenceWeighting
UNIFORM_BIND(3) uniform vec4 pTCTransformAlt; // xy=scale, zw=offset

UNIFORM_BIND(4) uniform vec4 pOffsets1; // offset1.st, -offset1.st
// TapCount > 3
UNIFORM_BIND(5) uniform vec4 pOffsets2; // offset2.st, -offset2.st
// TapCount > 5
UNIFORM_BIND(6) uniform vec4 pOffsets3; // offset3.st, -offset3.st
// TapCount > 7
UNIFORM_BIND(7) uniform vec4 pOffsets4; // offset4.st, -offset4.st
// TapCount > 9
UNIFORM_BIND(8) uniform vec4 pOffsets5; // offset5.st, -offset5.st
// TapCount > 11
UNIFORM_BIND(9) uniform vec4 pOffsets6; // offset6.st, -offset6.st
// TapCount > 13
UNIFORM_BIND(10) uniform vec4 pOffsets7; // offset7.st, -offset7.st

UNIFORM_BIND(11) uniform vec4 pWeights1; // weightCenter, weight1, weight2, weight3
// TapCount > 7
UNIFORM_BIND(12) uniform vec4 pWeights2; // weight4, weight5, weight6, weight7

UNIFORM_BIND(13) uniform vec2 pClamp; // clamp.s, clamp.t

// DepthDifferenceWeighting
UNIFORM_BIND(14) uniform float pDepthDifferenceThreshold;


// !InputArrayTextures
layout(binding=0) uniform mediump sampler2D texData;

// InputArrayTextures
layout(binding=0) uniform mediump sampler2DArray texDataArray;

// DepthDifferenceWeighting
layout(binding=1) uniform HIGHP sampler2DArray texDepth;

#include "shared/interface/2d/fragment.glsl"

layout(location=0) out vec4 outData;


vec4 sampleData(vec2 tc){
	vec4 data;
	if(InputArrayTextures){
		data = textureLod(texDataArray, vec3(tc, vLayer), 0.0);
	}else{
		data = textureLod(texData, tc, 0.0);
	}
	
	if(TexDataSwizzle == 1){
		return data.gggg;
	}else{
		return data;
	}
}


#include "shared/defren/depth_to_position.glsl"

// DepthDifferenceWeighting
#include "shared/defren/depth_to_position_fragment.glsl"


void processPixel( in vec2 tc, in float weight, in float refdepth, inout vec4 accum, inout float weightSum ){
	if(DepthDifferenceWeighting){
		float depth = depthToZ( texDepth, vec3( tc * pTCTransformAlt.xy + pTCTransformAlt.zw, vLayer ), vLayer );
		weight *= max( 0.0, 1.0 - pDepthDifferenceThreshold * abs( depth - refdepth ) );
		accum += sampleData( tc ) * vec4( weight );
		weightSum += weight;
		
	}else{
		accum += sampleData( tc ) * vec4( weight );
	}
}


void main( void ){
	vec4 accum = sampleData( vTexCoord ) * vec4( pWeights1.x );
	float refdepth = 0.0;
	float weightSum = 1.0;
	
	if(DepthDifferenceWeighting){
		refdepth = depthToZ( texDepth, vec3( vTexCoord * pTCTransformAlt.xy + pTCTransformAlt.zw, vLayer ), vLayer );
		weightSum = pWeights1.x;
	}
	
	vec4 tc = min( vTexCoord.stst + pOffsets1, pClamp.stst );
	processPixel( tc.xy, pWeights1.y, refdepth, accum, weightSum );
	processPixel( tc.zw, pWeights1.y, refdepth, accum, weightSum );
	
	if(TapCount > 3){
		tc = min( vTexCoord.stst + pOffsets2, pClamp.stst );
		processPixel( tc.xy, pWeights1.z, refdepth, accum, weightSum );
		processPixel( tc.zw, pWeights1.z, refdepth, accum, weightSum );
	}
	
	if(TapCount > 5){
		tc = min( vTexCoord.stst + pOffsets3, pClamp.stst );
		processPixel( tc.xy, pWeights1.w, refdepth, accum, weightSum );
		processPixel( tc.zw, pWeights1.w, refdepth, accum, weightSum );
	}
	
	if(TapCount > 7){
		tc = min( vTexCoord.stst + pOffsets4, pClamp.stst );
		processPixel( tc.xy, pWeights2.x, refdepth, accum, weightSum );
		processPixel( tc.zw, pWeights2.x, refdepth, accum, weightSum );
	}
	
	if(TapCount > 9){
		tc = min( vTexCoord.stst + pOffsets5, pClamp.stst );
		processPixel( tc.xy, pWeights2.y, refdepth, accum, weightSum );
		processPixel( tc.zw, pWeights2.y, refdepth, accum, weightSum );
	}
	
	if(TapCount > 11){
		tc = min( vTexCoord.stst + pOffsets6, pClamp.stst );
		processPixel( tc.xy, pWeights2.z, refdepth, accum, weightSum );
		processPixel( tc.zw, pWeights2.z, refdepth, accum, weightSum );
	}
	
	if(TapCount > 13){
		tc = min( vTexCoord.stst + pOffsets7, pClamp.stst );
		processPixel( tc.xy, pWeights2.w, refdepth, accum, weightSum );
		processPixel( tc.zw, pWeights2.w, refdepth, accum, weightSum );
	}
	
	if(DepthDifferenceWeighting){
		accum /= vec4(weightSum);
	}
	
	if(OutDataSwizzle == 1){
		accum = accum.gggg;
	}
	
	if(OutDataSize == 1){
		outData.x = accum.x;
	}else if(OutDataSize == 2){
		outData.xy = vec2(accum);
	}else if(OutDataSize == 3){
		outData.xyz = vec3(accum);
	}else{
		outData = accum;
	}
}
