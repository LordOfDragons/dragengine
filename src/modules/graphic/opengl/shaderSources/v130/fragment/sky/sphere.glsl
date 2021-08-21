precision highp float;
precision highp int;

uniform mat3 pMatrixCamera;
uniform mat3 pMatrixLayer;
uniform vec3 pLayerPosition;
uniform vec4 pLayerColor;
uniform vec4 pParams; // x=1/projMat.a11, y=1/projMat.a22, -projMat.a13, -projMat.a23
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
	// pParams = {tanf(fov / 2) * znear, tanf(fov * fovRatio / 2) * znear / aspectRatio, znear}
	//         = {znear / projMat.a11, znear / projMat.a22, znear}
	// pMatrixCamera = rot(inv(camMatrix))
	normal = pMatrixCamera * normalize( vec3( vTexCoord, 1.0 ) * pParams )
	
	// aligning the two normals (without pMatrixCamera) yields
	normalize( vec3(vTexCoord, 1) * pParams ) = normalize( (vScreenCoord + pPosTransform2) * pPosTransform.zw, 1 )
	
	// where pParams' = pParams / znear = {1 / projMat.a11, 1 / projMat.a22, 1}
	normalize( vTexCoord * pParams'.xy, 1 ) = normalize( (vScreenCoord + pPosTransform2) * pPosTransform.zw, 1 )
	
	// which means 1=1 and pParams'.xy = pPosTransform.zw . leaves us with aligning
	vTexCoord = vScreenCoord + pPosTransform2
	
	// hence we can simply add pPosTransform2 and get the right result
	*/
	
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
	
	tc.s = atan( normal.x, normal.z ) * INV_TWO_PI + 0.5;
	tc.t = 0.5 - asin( normal.y ) * INV_PI;
	
	outColor = pow( texture( texSky, tc ), pMaterialGamma ) * pLayerColor;
	
	// mix layer color with sky color. the sky color is either <rgb,1> if this is the first
	// layer or <black,0> if not. the color part has to be mixed using the layer transparency
	// and the alpha values summed up
	outColor.rgb = mix( pSkyBgColor.rgb, outColor.rgb, outColor.a );
	outColor.a = min( outColor.a + pSkyBgColor.a, 1.0 );
}
