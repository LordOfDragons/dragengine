#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(3) uniform vec4 pPosTransform;
UNIFORM_BIND(4) uniform mat3 pMatrixRotation;
UNIFORM_BIND(5) uniform vec3 pInitialMinValue;
UNIFORM_BIND(6) uniform vec3 pInitialMaxValue;
UNIFORM_BIND(7) uniform vec3 pClearMinValue;
UNIFORM_BIND(8) uniform vec3 pClearMaxValue;

UNIFORM_BIND(9) uniform int pMipMapLevel;

layout(binding=0) uniform HIGHP sampler2D texMin;
layout(binding=1) uniform HIGHP sampler2D texMax;

// DepthCubeMap
layout(binding=2) uniform HIGHP samplerCube texDepthCube;
layout(binding=3) uniform HIGHP samplerCube texAmbientCube;

// !DepthCubeMap
layout(binding=2) uniform HIGHP sampler2D texDepth;
layout(binding=3) uniform HIGHP sampler2D texAmbient;

#include "shared/interface/2d/fragment.glsl"

layout(location=0) out vec3 outMin;
layout(location=1) out vec3 outMax;

const vec3 unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );


void updateMinMaxFromDepth(in HIGHP samplerCube tex, const in vec3 tc,
const in ivec3 swizzle, const in ivec3 flipper, inout vec3 resMin, inout vec3 resMax){
	vec3 position = vec3( textureLod( tex, tc, 0.0 ).r );
	
	if(ShadowInverseDepth){
		// clear value is 0 causing inf value. nothing can be 0 except clear value.
		// skipping the value though would result in wrong results if all merged points
		// have are clear values. in this case the initial min/max value is returned
		// which results in a tiny bounding box around the light center. this is obviously
		// wrong. to fix this a clear-value is defined by the light source. this value
		// equals the maximum boundary box and is a safe value
		if(position.z == 0.0){
			resMin = min(resMin, pClearMinValue);
			resMax = max(resMax, pClearMaxValue);
			return;
		}
	}
	
	position.z = pPosTransform.x / ( pPosTransform.y - position.z );
	position.xy = vTexCoord * position.zz;
	position = vec3( position[ swizzle.x ], position[ swizzle.y ], position[ swizzle.z ] ) * vec3(flipper);
	position = pMatrixRotation * position;
	
	resMin = min(resMin, position);
	resMax = max(resMax, position);
}

void updateMinMaxFromDepth(in HIGHP sampler2D tex, const in ivec2 tc,
inout vec3 resMin, inout vec3 resMax){
	vec3 position = vec3( texelFetch( tex, tc, 0 ).r );
	
	if(ShadowInverseDepth){
		// clear value is 0 causing inf value. nothing can be 0 except clear value.
		// skipping the value though would result in wrong results if all merged points
		// have are clear values. in this case the initial min/max value is returned
		// which results in a tiny bounding box around the light center. this is obviously
		// wrong. to fix this a clear-value is defined by the light source. this value
		// equals the maximum boundary box and is a safe value
		if( position.z == 0.0 ){
			resMin = min(resMin, pClearMinValue);
			resMax = max(resMax, pClearMaxValue);
			return;
		}
	}
	
	position.z = pPosTransform.x / ( pPosTransform.y - position.z );
	position.xy = vTexCoord * pPosTransform.zw * position.zz;
	position = pMatrixRotation * position;
	
	resMin = min(resMin, position);
	resMax = max(resMax, position);
}


const vec2 tcFaceTransform = vec2( 1, -1 );

void main(void){
	// min-max of depth input
	if(DepthInput){
		outMin = pInitialMinValue;
		outMax = pInitialMaxValue;
		
		if(DepthCubeMap){
			vec4 tc = vec4( vTexCoord, 1, -1 );
			vec3 tcoff = vec3( pPosTransform.z ) * vec3( 1, -1, 0 );
			ivec3 swizzle, flipper;
			
			// positive y = (s,-1,t), transform (1,1) same as above
			swizzle = ivec3( 0, 2, 1 );
			flipper = ivec3( 1, -1, 1 );
			updateMinMaxFromDepth(texDepthCube, tc.xwy, swizzle, flipper, outMin, outMax);
			updateMinMaxFromDepth(texDepthCube, tc.xwy + tcoff.xzz, swizzle, flipper, outMin, outMax); // x=1, y=0
			updateMinMaxFromDepth(texDepthCube, tc.xwy + tcoff.zzx, swizzle, flipper, outMin, outMax); // x=0, y=1
			updateMinMaxFromDepth(texDepthCube, tc.xwy + tcoff.xzx, swizzle, flipper, outMin, outMax); // x=1, y=1
			
			if(AmbientMap){
				updateMinMaxFromDepth(texAmbientCube, tc.xwy, swizzle, flipper, outMin, outMax);
				updateMinMaxFromDepth(texAmbientCube, tc.xwy + tcoff.xzz, swizzle, flipper, outMin, outMax); // x=1, y=0
				updateMinMaxFromDepth(texAmbientCube, tc.xwy + tcoff.zzx, swizzle, flipper, outMin, outMax); // x=0, y=1
				updateMinMaxFromDepth(texAmbientCube, tc.xwy + tcoff.xzx, swizzle, flipper, outMin, outMax); // x=1, y=1
			}
			
			// negative x = (-1,t,s), transform (1,1) same as above
			swizzle = ivec3( 2, 1, 0 );
			flipper = ivec3( -1, 1, 1 );
			updateMinMaxFromDepth(texDepthCube, tc.wyx, swizzle, flipper, outMin, outMax);
			updateMinMaxFromDepth(texDepthCube, tc.wyx + tcoff.zzx, swizzle, flipper, outMin, outMax); // x=1, y=0
			updateMinMaxFromDepth(texDepthCube, tc.wyx + tcoff.zxz, swizzle, flipper, outMin, outMax); // x=0, y=1
			updateMinMaxFromDepth(texDepthCube, tc.wyx + tcoff.zxx, swizzle, flipper, outMin, outMax); // x=1, y=1
			
			if(AmbientMap){
				updateMinMaxFromDepth(texAmbientCube, tc.wyx, swizzle, flipper, outMin, outMax);
				updateMinMaxFromDepth(texAmbientCube, tc.wyx + tcoff.zzx, swizzle, flipper, outMin, outMax); // x=1, y=0
				updateMinMaxFromDepth(texAmbientCube, tc.wyx + tcoff.zxz, swizzle, flipper, outMin, outMax); // x=0, y=1
				updateMinMaxFromDepth(texAmbientCube, tc.wyx + tcoff.zxx, swizzle, flipper, outMin, outMax); // x=1, y=1
			}
			
			// positive z = (s,t,1), transform (1,1) same as above
			swizzle = ivec3( 0, 1, 2 );
			flipper = ivec3( 1, 1, 1 );
			updateMinMaxFromDepth(texDepthCube, tc.xyz, swizzle, flipper, outMin, outMax);
			updateMinMaxFromDepth(texDepthCube, tc.xyz + tcoff.xzz, swizzle, flipper, outMin, outMax); // x=1, y=0
			updateMinMaxFromDepth(texDepthCube, tc.xyz + tcoff.zxz, swizzle, flipper, outMin, outMax); // x=0, y=1
			updateMinMaxFromDepth(texDepthCube, tc.xyz + tcoff.xxz, swizzle, flipper, outMin, outMax); // x=1, y=1
			
			if(AmbientMap){
				updateMinMaxFromDepth(texAmbientCube, tc.xyz, swizzle, flipper, outMin, outMax);
				updateMinMaxFromDepth(texAmbientCube, tc.xyz + tcoff.xzz, swizzle, flipper, outMin, outMax); // x=1, y=0
				updateMinMaxFromDepth(texAmbientCube, tc.xyz + tcoff.zxz, swizzle, flipper, outMin, outMax); // x=0, y=1
				updateMinMaxFromDepth(texAmbientCube, tc.xyz + tcoff.xxz, swizzle, flipper, outMin, outMax); // x=1, y=1
			}
			
			// negative z = (-s,t,-1), transform (-1,1)
			swizzle = ivec3( 0, 1, 2 );
			flipper = ivec3( -1, 1, -1 );
			tc.xy = vTexCoord * tcFaceTransform.yx;
			updateMinMaxFromDepth(texDepthCube, tc.xyw, swizzle, flipper, outMin, outMax);
			updateMinMaxFromDepth(texDepthCube, tc.xyw + tcoff.yzz, swizzle, flipper, outMin, outMax); // x=1, y=0
			updateMinMaxFromDepth(texDepthCube, tc.xyw + tcoff.zxz, swizzle, flipper, outMin, outMax); // x=0, y=1
			updateMinMaxFromDepth(texDepthCube, tc.xyw + tcoff.yxz, swizzle, flipper, outMin, outMax); // x=1, y=1
			
			if(AmbientMap){
				updateMinMaxFromDepth(texAmbientCube, tc.xyw, swizzle, flipper, outMin, outMax);
				updateMinMaxFromDepth(texAmbientCube, tc.xyw + tcoff.yzz, swizzle, flipper, outMin, outMax); // x=1, y=0
				updateMinMaxFromDepth(texAmbientCube, tc.xyw + tcoff.zxz, swizzle, flipper, outMin, outMax); // x=0, y=1
				updateMinMaxFromDepth(texAmbientCube, tc.xyw + tcoff.yxz, swizzle, flipper, outMin, outMax); // x=1, y=1
			}
			
			// positive x = (1,t,-s), transform (1,1) same as above
			swizzle = ivec3( 2, 1, 0 );
			flipper = ivec3( 1, 1, -1 );
			updateMinMaxFromDepth(texDepthCube, tc.zyx, swizzle, flipper, outMin, outMax);
			updateMinMaxFromDepth(texDepthCube, tc.zyx + tcoff.zzy, swizzle, flipper, outMin, outMax); // x=1, y=0
			updateMinMaxFromDepth(texDepthCube, tc.zyx + tcoff.zxz, swizzle, flipper, outMin, outMax); // x=0, y=1
			updateMinMaxFromDepth(texDepthCube, tc.zyx + tcoff.zxy, swizzle, flipper, outMin, outMax); // x=1, y=1
			
			if(AmbientMap){
				updateMinMaxFromDepth(texAmbientCube, tc.zyx, swizzle, flipper, outMin, outMax);
				updateMinMaxFromDepth(texAmbientCube, tc.zyx + tcoff.zzy, swizzle, flipper, outMin, outMax); // x=1, y=0
				updateMinMaxFromDepth(texAmbientCube, tc.zyx + tcoff.zxz, swizzle, flipper, outMin, outMax); // x=0, y=1
				updateMinMaxFromDepth(texAmbientCube, tc.zyx + tcoff.zxy, swizzle, flipper, outMin, outMax); // x=1, y=1
			}
			
			// negative y = (s,1,-t), transform (-1,-1)
			swizzle = ivec3( 0, 2, 1 );
			flipper = ivec3( 1, 1, -1 );
			tc.xy = vTexCoord * tcFaceTransform.yy;
			updateMinMaxFromDepth(texDepthCube, tc.xzy, swizzle, flipper, outMin, outMax);
			updateMinMaxFromDepth(texDepthCube, tc.xzy + tcoff.xzz, swizzle, flipper, outMin, outMax); // x=1, y=0
			updateMinMaxFromDepth(texDepthCube, tc.xzy + tcoff.zzy, swizzle, flipper, outMin, outMax); // x=0, y=1
			updateMinMaxFromDepth(texDepthCube, tc.xzy + tcoff.xzy, swizzle, flipper, outMin, outMax); // x=1, y=1
			
			if(AmbientMap){
				updateMinMaxFromDepth(texAmbientCube, tc.xzy, swizzle, flipper, outMin, outMax);
				updateMinMaxFromDepth(texAmbientCube, tc.xzy + tcoff.xzz, swizzle, flipper, outMin, outMax); // x=1, y=0
				updateMinMaxFromDepth(texAmbientCube, tc.xzy + tcoff.zzy, swizzle, flipper, outMin, outMax); // x=0, y=1
				updateMinMaxFromDepth(texAmbientCube, tc.xzy + tcoff.xzy, swizzle, flipper, outMin, outMax); // x=1, y=1
			}
			
		}else{
			ivec2 tc = ivec2( gl_FragCoord.xy ) * ivec2( 2 );
			
			updateMinMaxFromDepth(texDepth, tc, outMin, outMax);
			updateMinMaxFromDepth(texDepth, tc + ivec2(1, 0), outMin, outMax);
			updateMinMaxFromDepth(texDepth, tc + ivec2(0, 1), outMin, outMax);
			updateMinMaxFromDepth(texDepth, tc + ivec2(1, 1), outMin, outMax);
			
			if(AmbientMap){
				updateMinMaxFromDepth(texAmbient, tc, outMin, outMax);
				updateMinMaxFromDepth(texAmbient, tc + ivec2(1, 0), outMin, outMax);
				updateMinMaxFromDepth(texAmbient, tc + ivec2(0, 1), outMin, outMax);
				updateMinMaxFromDepth(texAmbient, tc + ivec2(1, 1), outMin, outMax);
			}
		}
		
	}else{
		ivec2 tc = ivec2( gl_FragCoord.xy ) * ivec2( 2 );
		
		outMin = texelFetch( texMin, tc, pMipMapLevel ).rgb;
		outMin = min( outMin, texelFetchOffset( texMin, tc, pMipMapLevel, ivec2( 1, 0 ) ).rgb );
		outMin = min( outMin, texelFetchOffset( texMin, tc, pMipMapLevel, ivec2( 0, 1 ) ).rgb );
		outMin = min( outMin, texelFetchOffset( texMin, tc, pMipMapLevel, ivec2( 1, 1 ) ).rgb );
		
		outMax = texelFetch( texMax, tc, pMipMapLevel ).rgb;
		outMax = max( outMax, texelFetchOffset( texMax, tc, pMipMapLevel, ivec2( 1, 0 ) ).rgb );
		outMax = max( outMax, texelFetchOffset( texMax, tc, pMipMapLevel, ivec2( 0, 1 ) ).rgb );
		outMax = max( outMax, texelFetchOffset( texMax, tc, pMipMapLevel, ivec2( 1, 1 ) ).rgb );
	}
}
