#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(0) uniform mat3 pMatrixCamera;
UNIFORM_BIND(1) uniform mat3 pMatrixLayer;
UNIFORM_BIND(2) uniform vec3 pLayerPosition;
UNIFORM_BIND(3) uniform vec4 pLayerColor;
UNIFORM_BIND(4) uniform vec4 pParams; // x=1/projMat.a11, y=1/projMat.a22, -projMat.a13, -projMat.a23
UNIFORM_BIND(5) uniform vec4 pMaterialGamma;
UNIFORM_BIND(6) uniform vec4 pSkyBgColor; // skybg.rgb, skybgBlendFactor

layout(binding=0) uniform mediump samplerCube texSky;

#include "shared/interface/2d/fragment.glsl"

layout(location=0) out vec4 outColor;

void main( void ){
	//vec3 position = pMatrixLayer * normalize( vec3( vTexCoord, 1.0 ) * pParams );
	//vec3 position = normalize( pMatrixLayer * vec4( vec3( vTexCoord, 1.0 ) * pParams, 1.0 ) );
	float p, q, D, l;
	vec3 normal;
	
// 	normal = pMatrixCamera * normalize( vec3( vTexCoord, 1.0 ) * pParams );
	normal = pMatrixCamera * normalize( vec3( ( vTexCoord + pParams.zw ) * pParams.xy, 1.0 ) );
	
	p = 2.0 * dot( pLayerPosition, normal );
	q = dot( pLayerPosition, pLayerPosition ) - 1.0;
	D = p * p - 4.0 * q;
	l = 0.0;
	if( D >= 0.0 ){
		D = sqrt( D );
		l = 0.5 * max( -p + D, -p - D );
	}
	
	normal = pMatrixLayer * normalize( pLayerPosition + normal * l );
	
	outColor = pow( texture( texSky, normal.xyz ), pMaterialGamma ) * pLayerColor;
	
	// mix layer color with sky color. the sky color is either <rgb,1> if this is the first
	// layer or <black,0> if not. the color part has to be mixed using the layer transparency
	// and the alpha values summed up
	outColor.rgb = mix( pSkyBgColor.rgb, outColor.rgb, outColor.a );
	outColor.a = min( outColor.a + pSkyBgColor.a, 1.0 );
}
