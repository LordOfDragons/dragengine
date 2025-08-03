#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/image_buffer.glsl"

UNIFORM_BIND(0) uniform vec4 pTCDataToDepth; // xy=scale, zw=offset
UNIFORM_BIND(1) uniform ivec2 pOffsetRead;
UNIFORM_BIND(2) uniform ivec2 pOffsetWrite;
UNIFORM_BIND(3) uniform int pClamp;
UNIFORM_BIND(4) uniform float pDepthDifferenceThreshold;

uniform HIGHP sampler2DArray texDepth;

#ifdef ANDROID
	layout(binding=0, IMG_R8_FMT) uniform readonly lowp IMG_R8_2DARR texData_load;
	layout(binding=0, IMG_R8_FMT) uniform writeonly lowp IMG_R8_2DARR texData_store;
#else
	layout(binding=0, IMG_R8_FMT) uniform lowp IMG_R8_2DARR texData;
	#define texData_load texData
	#define texData_store texData
#endif

layout(local_size_x=64) in;

shared float vData[ 72 ];
shared float vDepth[ 72 ];

// gauss coefficients: gauss(i,s) = exp(-i*i/(2*s*s)), gaussNorm(s) = 1/(2*pi*s*s)
// py ['{:e}'.format(gauss(x,0.9)/gaussNorm(0.9)) for x in [0,1,2,3,4]]
// s typically 1.
// 
// normalization is not used here since the shader normalizes. this is required
// since depth thresholding changes the actual weights used.
// 
// here used similar to ['{:e}'.format(gauss(x,1.1)*0.1963806) for x in [0,1,2,3,4]]:
// [1.963806e-1, 1.299086e-1, 3.760594e-2, 4.763803e-3, 2.640767e-4]
// 
// alternate version:
// s=1.0: [1.0, 6.065307e-1, 1.353353e-1, 1.110900e-2, 3.354626e-4]
// s=1.5: [1.0, 8.007374e-1, 4.111123e-1, 1.353353e-1, 2.856550e-2]
// s=1.2: [1.0, 7.066483e-1, 2.493522e-1, 4.393693e-2, 3.865920e-3]
// s=1.1: [1.0, 6.615147e-1, 1.914952e-1, 2.425801e-2, 1.344719e-3]
// 
// for reduced version s=1 is used to sharpen the AO a bit since during upscaling
// the sharpness will be reduced a bit. furthermore the values are modified to smear
// out the ssao a bit to combine better with texture ao
// 
const float cGaussWeightCenter = 1.0;
const vec4 cGaussWeights = vec4( 6.065307e-1, 1.353353e-1, 1.110900e-2, 3.354626e-4 );

const float cGaussWeightCenterModifier = 0.8;
const vec4 cGaussWeightModifier = vec4( 0.85, 0.9, 0.95, 1 );


#include "shared/defren/depth_to_position.glsl"


const int cBlurCoord = BlurPass2 ? 1 : 0;


void main( void ){
	// constants that glsl does not allow to be global constants
	float cWeightCenter = cGaussWeightCenter * cGaussWeightCenterModifier;
	vec4 cWeights = cGaussWeights * cGaussWeightModifier;
	
	ivec3 tcCenter = ivec3(gl_GlobalInvocationID);
	if(BlurPass2){
		tcCenter.xy = tcCenter.yx;
	}
	
	// cooperative read data
	ivec3 tc = tcCenter;
	tc[ cBlurCoord ] = clamp( tcCenter[ cBlurCoord ] - 4, 0, pClamp );
	
	uint index = gl_LocalInvocationIndex;
	
	vData[index] = float(IMG_R8_LOAD(imageLoad(texData_load, ivec3(tc.xy + pOffsetRead, tc.z))));
	vDepth[ index ] = depthToZ( texDepth,
		vec3( vec2( tc.xy ) * pTCDataToDepth.xy + pTCDataToDepth.zw, tc.z ), tc.z );
	
	if( gl_LocalInvocationIndex < uint( 8 ) ){
		tc[ cBlurCoord ] = min( tcCenter[ cBlurCoord ] + 60, pClamp );
		index += uint( 64 );
		
		vData[index] = float(IMG_R8_LOAD(imageLoad(texData_load, ivec3(tc.xy + pOffsetRead, tc.z))));
		vDepth[ index ] = depthToZ( texDepth,
			vec3( vec2( tc.xy ) * pTCDataToDepth.xy + pTCDataToDepth.zw, tc.z ), tc.z );
	}
	
	barrier();
	
	
	// per invocation processing
	if(gl_GlobalInvocationID.x > uint(pClamp)){
		// skipping the invocation has to be done after the cooperative read phase since
		// otherwise parts of the shared data is not properly read causing errors
		return;
	}
	
	index = gl_LocalInvocationIndex + uint( 4 );
	
	float accum = vData[ index ] * cWeightCenter;
	float refdepth = vDepth[ index ];
	float weightSum = cWeightCenter;
	
	
	// negative side
	uvec4 i = uvec4( index ) - uvec4( 1, 2, 3, 4 );
	
	vec4 depth = vec4( vDepth[ i.x ], vDepth[ i.y ], vDepth[ i.z ], vDepth[ i.w ] );
	
	vec4 weights = cWeights * max( vec4( 0 ), vec4( 1 )
		- vec4( pDepthDifferenceThreshold ) * abs( depth - vec4( refdepth ) ) );
	
	vec4 data = vec4( vData[ i.x ], vData[ i.y ], vData[ i.z ], vData[ i.w ] );
	
	accum += dot( data, weights );
	weightSum += dot( weights, vec4( 1 ) );
	
	
	// positive side
	i = uvec4( index ) + uvec4( 1, 2, 3, 4 );
	
	depth = vec4( vDepth[ i.x ], vDepth[ i.y ], vDepth[ i.z ], vDepth[ i.w ] );
	
	weights = cWeights * max( vec4( 0 ), vec4( 1 )
		- vec4( pDepthDifferenceThreshold ) * abs( depth - vec4( refdepth ) ) );
	
	data = vec4( vData[ i.x ], vData[ i.y ], vData[ i.z ], vData[ i.w ] );
	
	accum += dot( data, weights );
	weightSum += dot( weights, vec4( 1 ) );
	
	
	// finish
	imageStore(texData_store, ivec3(tcCenter.xy + pOffsetWrite, tcCenter.z), IMG_R8_STORE(accum / weightSum));
}
