precision highp float;
precision highp int;

uniform mat3 pMatrixCamera;
uniform mat3 pMatrixLayer;
uniform vec3 pLayerPosition;
uniform vec4 pLayerColor;
uniform vec3 pParams;
uniform vec4 pMaterialGamma;
uniform vec4 pSkyBgColor;

uniform mediump sampler2D texSky;

in vec2 vTexCoord;

out vec4 outColor;

const float PI = 3.14159265;
const float INV_TWO_PI = 1.0 / ( PI * 2.0 );
const float INV_HALF_PI = 1.0 / ( PI * 0.5 );
const float INV_PI = 1.0 / PI;

void main( void ){
	//vec3 position = pMatrixLayer * normalize( vec3( vTexCoord, 1.0 ) * pParams );
	//vec3 position = normalize( pMatrixLayer * vec4( vec3( vTexCoord, 1.0 ) * pParams, 1.0 ) );
	float p, q, D, l;
	vec3 normal;
	vec2 tc;
	
	normal = pMatrixCamera * normalize( vec3( vTexCoord, 1.0 ) * pParams );
	
	p = 2.0 * dot( pLayerPosition, normal );
	q = dot( pLayerPosition, pLayerPosition ) - 1.0;
	D = p * p - 4.0 * q;
	l = 0.0;
	if( D >= 0.0 ){
		D = sqrt( D );
		l = 0.5 * max( -p + D, -p - D );
	}
	
	normal = pMatrixLayer * normalize( pLayerPosition + normal * l );
	
	tc.s = atan( normal.x, normal.z ) * INV_TWO_PI + 0.5;
	tc.t = 0.5 - asin( normal.y ) * INV_PI; //asin( normal.y ) * INV_PI + 0.5;
	
	outColor = pow( texture( texSky, tc ), pMaterialGamma ) * pLayerColor;
	
	// mix layer color with sky color. the sky color is either <rgb,1> if this is the first
	// layer or <black,0> if not. the color part has to be mixed using the layer transparency
	// and the alpha values summed up
	outColor.rgb = mix( pSkyBgColor.rgb, outColor.rgb, outColor.a );
	outColor.a = min( outColor.a + pSkyBgColor.a, 1.0 );
}
