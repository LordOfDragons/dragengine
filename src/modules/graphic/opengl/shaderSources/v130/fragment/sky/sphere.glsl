precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/ubo_render_parameters.glsl"

uniform mat3 pMatrixLayer;
uniform vec3 pLayerPosition;
uniform vec4 pLayerColor;
uniform vec4 pMaterialGamma;
uniform vec4 pSkyBgColor;

uniform mediump sampler2D texSky;

in vec2 vTexCoord;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out vec4 outColor;

const float PI = 3.14159265;
const float INV_TWO_PI = 1.0 / ( PI * 2.0 );
const float INV_HALF_PI = 1.0 / ( PI * 0.5 );
const float INV_PI = 1.0 / PI;

void main( void ){
	// x=1/projMat.a11, y=1/projMat.a22, -projMat.a13, -projMat.a23
	vec4 params = vec4( pDepthToPosition[ vLayer ].zw, pDepthToPosition2[ vLayer ] );
	
	//vec3 position = pMatrixLayer * normalize( vec3( vTexCoord, 1.0 ) * params );
	//vec3 position = normalize( pMatrixLayer * vec4( vec3( vTexCoord, 1.0 ) * params, 1.0 ) );
	float p, q, D, l;
	vec3 normal;
	vec2 tc;
	
	/*
	// vScreenCoord = {-1..1}
	// pPosTransform = {-znear, 0, 1 / projMat.a11, 1 / projMat.a22}
	// pPosTransform2 = {-projMat.a13, -projMat.a23}
	vec3 position = vec3(depth);
	position.z = pPosTransform.x / (pPosTransform.y - position.z);
	position.xy = (vScreenCoord + pPosTransform2) * pPosTransform.zw * position.zz;
	
	// with position.z = 1
	position = vec3( (vScreenCoord + pPosTransform2) * pPosTransform.zw, 1 );
	
	// with position.z = 1000
	position = vec3( (vScreenCoord + pPosTransform2) * pPosTransform.zw * vec2(1000), 1000 );
	
	// when normalized
	normal = normalize(position)
	       = normalize(position')  // where position' = position / 1000
	
	// so position.z = 1000 or position.z = 1 is the same for this calculation
	
	// vTexCoord = {-1..1}
	// params = {tanf(fov / 2) * znear, tanf(fov * fovRatio / 2) * znear / aspectRatio, znear}
	//         = {znear / projMat.a11, znear / projMat.a22, znear}
	// pMatrixVn[ vLayer ] = rot(inv(camMatrix))
	normal = pMatrixVn[ vLayer ] * normalize( vec3( vTexCoord, 1.0 ) * params )
	
	// aligning the two normals (without pMatrixVn[ vLayer ]) yields
	normalize( vec3(vTexCoord, 1) * params ) = normalize( (vScreenCoord + pPosTransform2) * pPosTransform.zw, 1 )
	
	// where params' = params / znear = {1 / projMat.a11, 1 / projMat.a22, 1}
	normalize( vTexCoord * params'.xy, 1 ) = normalize( (vScreenCoord + pPosTransform2) * pPosTransform.zw, 1 )
	
	// which means 1=1 and params'.xy = pPosTransform.zw . leaves us with aligning
	vTexCoord = vScreenCoord + pPosTransform2
	
	// hence we can simply add pPosTransform2 and get the right result
	*/
	
// 	normal = pMatrixVn[ vLayer ] * normalize( vec3( vTexCoord, 1.0 ) * params );
	normal = pMatrixVn[ vLayer ] * normalize( vec3( ( vTexCoord + params.zw ) * params.xy, 1.0 ) );
	
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
	tc.t = 0.5 - asin( normal.y ) * INV_PI;
	
	outColor = pow( texture( texSky, tc ), pMaterialGamma ) * pLayerColor;
	
	// mix layer color with sky color. the sky color is either <rgb,1> if this is the first
	// layer or <black,0> if not. the color part has to be mixed using the layer transparency
	// and the alpha values summed up
	outColor.rgb = mix( pSkyBgColor.rgb, outColor.rgb, outColor.a );
	outColor.a = min( outColor.a + pSkyBgColor.a, 1.0 );
}
