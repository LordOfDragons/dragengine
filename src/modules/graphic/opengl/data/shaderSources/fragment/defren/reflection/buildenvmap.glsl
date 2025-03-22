precision HIGHP float;
precision HIGHP int;

uniform vec4 pBlendWeights;
uniform float pMipMapLevel;

#ifdef ENVMAP_EQUI
	#define SAMPLER_ENVMAP sampler2D
#else
	#define SAMPLER_ENVMAP samplerCube
#endif
uniform mediump SAMPLER_ENVMAP texEnvMap1;
uniform mediump SAMPLER_ENVMAP texEnvMap2;
uniform mediump SAMPLER_ENVMAP texEnvMap3;
uniform mediump SAMPLER_ENVMAP texEnvMap4;

in vec2 vTexCoord;

#ifdef ENVMAP_EQUI
	out vec4 outColor;
#else
	out vec4 outFace1;
	out vec4 outFace2;
	out vec4 outFace3;
	out vec4 outFace4;
	out vec4 outFace5;
	out vec4 outFace6;
#endif

const vec2 tcFaceTransform = vec2( 1.0, -1.0 );

void main( void ){
	#ifdef ENVMAP_EQUI
		outColor = textureLod( texEnvMap1, vTexCoord, pMipMapLevel ) * vec4( pBlendWeights.x );
		outColor += textureLod( texEnvMap2, vTexCoord, pMipMapLevel ) * vec4( pBlendWeights.y );
		outColor += textureLod( texEnvMap3, vTexCoord, pMipMapLevel ) * vec4( pBlendWeights.z );
		outColor += textureLod( texEnvMap4, vTexCoord, pMipMapLevel ) * vec4( pBlendWeights.w );
	#else
		vec4 tc = vec4( vTexCoord, 1.0, -1.0 );
		
		// positive y = (s,1,t), transform (1,1) same as above
		outFace3 = textureLod( texEnvMap1, tc.xzy, pMipMapLevel ) * vec4( pBlendWeights.x );
		outFace3 += textureLod( texEnvMap2, tc.xzy, pMipMapLevel ) * vec4( pBlendWeights.y );
		outFace3 += textureLod( texEnvMap3, tc.xzy, pMipMapLevel ) * vec4( pBlendWeights.z );
		outFace3 += textureLod( texEnvMap4, tc.xzy, pMipMapLevel ) * vec4( pBlendWeights.w );
		
		// negative x = (-1,-t,s), transform (1,-1)
		tc.xy = vTexCoord * tcFaceTransform.xy;
		outFace2 = textureLod( texEnvMap1, tc.wyx, pMipMapLevel ) * vec4( pBlendWeights.x );
		outFace2 += textureLod( texEnvMap2, tc.wyx, pMipMapLevel ) * vec4( pBlendWeights.y );
		outFace2 += textureLod( texEnvMap3, tc.wyx, pMipMapLevel ) * vec4( pBlendWeights.z );
		outFace2 += textureLod( texEnvMap4, tc.wyx, pMipMapLevel ) * vec4( pBlendWeights.w );
		
		// negative y = (s,-1,-t), transform (1,-1) same as above
		outFace4 = textureLod( texEnvMap1, tc.xwy, pMipMapLevel ) * vec4( pBlendWeights.x );
		outFace4 += textureLod( texEnvMap2, tc.xwy, pMipMapLevel ) * vec4( pBlendWeights.y );
		outFace4 += textureLod( texEnvMap3, tc.xwy, pMipMapLevel ) * vec4( pBlendWeights.z );
		outFace4 += textureLod( texEnvMap4, tc.xwy, pMipMapLevel ) * vec4( pBlendWeights.w );
		
		// positive z = (s,-t,1), transform (1,-1) same as above
		outFace5 = textureLod( texEnvMap1, tc.xyz, pMipMapLevel ) * vec4( pBlendWeights.x );
		outFace5 += textureLod( texEnvMap2, tc.xyz, pMipMapLevel ) * vec4( pBlendWeights.y );
		outFace5 += textureLod( texEnvMap3, tc.xyz, pMipMapLevel ) * vec4( pBlendWeights.z );
		outFace5 += textureLod( texEnvMap4, tc.xyz, pMipMapLevel ) * vec4( pBlendWeights.w );
		
		// positive x = (1,-t,-s), transform (-1,-1)
		tc.xy = vTexCoord * tcFaceTransform.yy;
		outFace1 = textureLod( texEnvMap1, tc.zyx, pMipMapLevel ) * vec4( pBlendWeights.x );
		outFace1 += textureLod( texEnvMap2, tc.zyx, pMipMapLevel ) * vec4( pBlendWeights.y );
		outFace1 += textureLod( texEnvMap3, tc.zyx, pMipMapLevel ) * vec4( pBlendWeights.z );
		outFace1 += textureLod( texEnvMap4, tc.zyx, pMipMapLevel ) * vec4( pBlendWeights.w );
		
		// negative z = (-s,-t,-1), transform (-1,-1) same as above
		outFace6 = textureLod( texEnvMap1, tc.xyw, pMipMapLevel ) * vec4( pBlendWeights.x );
		outFace6 += textureLod( texEnvMap2, tc.xyw, pMipMapLevel ) * vec4( pBlendWeights.y );
		outFace6 += textureLod( texEnvMap3, tc.xyw, pMipMapLevel ) * vec4( pBlendWeights.z );
		outFace6 += textureLod( texEnvMap4, tc.xyw, pMipMapLevel ) * vec4( pBlendWeights.w );
	#endif
}
