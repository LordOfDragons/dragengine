precision highp float;
precision highp int;

uniform vec4 pPosTransform; // position transform
uniform vec2 pPosTransform2; // position transform 2
uniform mat4 pShadowMatrix; // shadow matrix
uniform vec3 pShaOpts1; // scale, offset, cut-off

uniform HIGHP sampler2D texDepth;
#ifdef USE_HW_COMPARE
uniform lowp sampler2DShadow texShadowMap;
#else // USE_HW_COMPARE
uniform mediump sampler2D texShadowMap;
#endif // USE_HW_COMPARE

in vec2 vTexCoord;
in vec2 vScreenCoord;

out float outAmbient;



#ifdef GEOM_ENCODED_DEPTH
const vec3 unpackGeomDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif



void main( void ){
#ifdef GEOM_ENCODED_DEPTH
	vec3 position = vec3( dot( texture( texDepth, vTexCoord ).rgb, unpackGeomDepth ) );
#else
	vec3 position = vec3( texture( texDepth, vScreenCoord ).r );
#endif
	position.z = pPosTransform.x / ( pPosTransform.y - position.z );
	position.xy = ( vScreenCoord + pPosTransform2 ) * pPosTransform.zw * position.zz;
	
	vec3 shapos = ( pShadowMatrix * vec4( position, 1.0 ) ).xyz;
	
	if( shapos.z < pShaOpts1.z ){
		outAmbient = 0.0; // over 180 degree
		
	}else{
		float shadist = length( shapos ) * pShaOpts1.x + pShaOpts1.y;
		vec3 stc;
		
		if( shapos.y >= abs( shapos.x ) ){
			stc = vec3( shapos.x, shapos.y - shapos.z, shadist );
			stc.st *= vec2( 1.0 / ( shapos.y + shapos.z ) );
			stc.st = stc.st * vec2( 0.5, 0.25 ) + vec2( 0.5, 0.75 );
			
		}else if( shapos.x >= abs( shapos.y ) ){
			stc = vec3( shapos.x - shapos.z, shapos.y, shadist );
			stc.st *= vec2( 1.0 / ( shapos.x + shapos.z ) );
			stc.st = stc.st * vec2( 0.25, 0.5 ) + vec2( 0.75, 0.5 );
			
		}else if( -shapos.y >= abs( shapos.x ) ){
			stc = vec3( shapos.x, shapos.y + shapos.z, shadist );
			stc.st *= vec2( 1.0 / ( shapos.z - shapos.y ) );
			stc.st = stc.st * vec2( 0.5, 0.25 ) + vec2( 0.5, 0.25 );
			
		}else{ // -shapos.x >= abs( shapos.y )
			stc = vec3( shapos.x + shapos.z, shapos.y, shadist );
			stc.st *= vec2( 1.0 / ( shapos.z - shapos.x ) );
			stc.st = stc.st * vec2( 0.25, 0.5 ) + vec2( 0.25, 0.5 );
		}
		//pyramidShadowCoords( shapos, shapos, pShaOpts1.xy );
		
#ifdef USE_HW_COMPARE
		outAmbient = texture( texShadowMap, stc );
#else // USE_HW_COMPARE
		outAmbient = step( stc.p, texture( texShadowMap, stc.st ).r );
#endif // USE_HW_COMPARE
	}
}
