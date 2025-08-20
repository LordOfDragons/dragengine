#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(3) uniform vec4 pBlendWeights;
UNIFORM_BIND(4) uniform float pMipMapLevel;

layout(binding=0) uniform mediump sampler2D texEnvMap1;
layout(binding=0) uniform mediump samplerCube texEnvMap1Cube;

layout(binding=1) uniform mediump sampler2D texEnvMap2;
layout(binding=1) uniform mediump samplerCube texEnvMap2Cube;

layout(binding=2) uniform mediump sampler2D texEnvMap3;
layout(binding=2) uniform mediump samplerCube texEnvMap3Cube;

layout(binding=3) uniform mediump sampler2D texEnvMap4;
layout(binding=3) uniform mediump samplerCube texEnvMap4Cube;

#include "shared/interface/2d/fragment.glsl"

// TextureEnvMapEqui
layout(location=0) out vec4 outColor;

// !TextureEnvMapEqui
#define outFace1 outColor
layout(location=1) out vec4 outFace2;
layout(location=2) out vec4 outFace3;
layout(location=3) out vec4 outFace4;
layout(location=4) out vec4 outFace5;
layout(location=5) out vec4 outFace6;

const vec2 tcFaceTransform = vec2( 1.0, -1.0 );

void main( void ){
	if(TextureEnvMapEqui){
		outColor = textureLod( texEnvMap1, vTexCoord, pMipMapLevel ) * vec4( pBlendWeights.x );
		outColor += textureLod( texEnvMap2, vTexCoord, pMipMapLevel ) * vec4( pBlendWeights.y );
		outColor += textureLod( texEnvMap3, vTexCoord, pMipMapLevel ) * vec4( pBlendWeights.z );
		outColor += textureLod( texEnvMap4, vTexCoord, pMipMapLevel ) * vec4( pBlendWeights.w );
		
	}else{
		vec4 tc = vec4( vTexCoord, 1.0, -1.0 );
		
		// positive y = (s,1,t), transform (1,1) same as above
		outFace3 = textureLod( texEnvMap1Cube, tc.xzy, pMipMapLevel ) * vec4( pBlendWeights.x );
		outFace3 += textureLod( texEnvMap2Cube, tc.xzy, pMipMapLevel ) * vec4( pBlendWeights.y );
		outFace3 += textureLod( texEnvMap3Cube, tc.xzy, pMipMapLevel ) * vec4( pBlendWeights.z );
		outFace3 += textureLod( texEnvMap4Cube, tc.xzy, pMipMapLevel ) * vec4( pBlendWeights.w );
		
		// negative x = (-1,-t,s), transform (1,-1)
		tc.xy = vTexCoord * tcFaceTransform.xy;
		outFace2 = textureLod( texEnvMap1Cube, tc.wyx, pMipMapLevel ) * vec4( pBlendWeights.x );
		outFace2 += textureLod( texEnvMap2Cube, tc.wyx, pMipMapLevel ) * vec4( pBlendWeights.y );
		outFace2 += textureLod( texEnvMap3Cube, tc.wyx, pMipMapLevel ) * vec4( pBlendWeights.z );
		outFace2 += textureLod( texEnvMap4Cube, tc.wyx, pMipMapLevel ) * vec4( pBlendWeights.w );
		
		// negative y = (s,-1,-t), transform (1,-1) same as above
		outFace4 = textureLod( texEnvMap1Cube, tc.xwy, pMipMapLevel ) * vec4( pBlendWeights.x );
		outFace4 += textureLod( texEnvMap2Cube, tc.xwy, pMipMapLevel ) * vec4( pBlendWeights.y );
		outFace4 += textureLod( texEnvMap3Cube, tc.xwy, pMipMapLevel ) * vec4( pBlendWeights.z );
		outFace4 += textureLod( texEnvMap4Cube, tc.xwy, pMipMapLevel ) * vec4( pBlendWeights.w );
		
		// positive z = (s,-t,1), transform (1,-1) same as above
		outFace5 = textureLod( texEnvMap1Cube, tc.xyz, pMipMapLevel ) * vec4( pBlendWeights.x );
		outFace5 += textureLod( texEnvMap2Cube, tc.xyz, pMipMapLevel ) * vec4( pBlendWeights.y );
		outFace5 += textureLod( texEnvMap3Cube, tc.xyz, pMipMapLevel ) * vec4( pBlendWeights.z );
		outFace5 += textureLod( texEnvMap4Cube, tc.xyz, pMipMapLevel ) * vec4( pBlendWeights.w );
		
		// positive x = (1,-t,-s), transform (-1,-1)
		tc.xy = vTexCoord * tcFaceTransform.yy;
		outFace1 = textureLod( texEnvMap1Cube, tc.zyx, pMipMapLevel ) * vec4( pBlendWeights.x );
		outFace1 += textureLod( texEnvMap2Cube, tc.zyx, pMipMapLevel ) * vec4( pBlendWeights.y );
		outFace1 += textureLod( texEnvMap3Cube, tc.zyx, pMipMapLevel ) * vec4( pBlendWeights.z );
		outFace1 += textureLod( texEnvMap4Cube, tc.zyx, pMipMapLevel ) * vec4( pBlendWeights.w );
		
		// negative z = (-s,-t,-1), transform (-1,-1) same as above
		outFace6 = textureLod( texEnvMap1Cube, tc.xyw, pMipMapLevel ) * vec4( pBlendWeights.x );
		outFace6 += textureLod( texEnvMap2Cube, tc.xyw, pMipMapLevel ) * vec4( pBlendWeights.y );
		outFace6 += textureLod( texEnvMap3Cube, tc.xyw, pMipMapLevel ) * vec4( pBlendWeights.z );
		outFace6 += textureLod( texEnvMap4Cube, tc.xyw, pMipMapLevel ) * vec4( pBlendWeights.w );
	}
}
