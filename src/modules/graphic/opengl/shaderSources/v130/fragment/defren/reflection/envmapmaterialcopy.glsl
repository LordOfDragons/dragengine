#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

uniform vec4 pPosTransform;
uniform vec2 pPosTransform2;
uniform mat4x3 pMatrixPosition;
uniform mat3 pMatrixNormal;

uniform HIGHP sampler2D texDepth;
uniform lowp sampler2D texDiffuse;
uniform lowp sampler2D texNormal;

in vec2 vScreenCoord;
in vec2 vTexCoord;

out vec3 outPosition;
out vec3 outDiffuse;
out vec3 outNormal;

#include "v130/shared/normal.glsl"

void main( void ){
	// not all pixels in the material buffer are possibly written to. this happens for
	// sky pixels for example. in this case material can not be written. using the same
	// discard rule as lighting shader is using
	vec4 diffuse = textureLod( texDiffuse, vTexCoord, 0 );
	
	if( diffuse.a == 0 ){
		discard;
	}
	
	// position is in camera space. transform to target space
	vec3 position = vec3( textureLod( texDepth, vTexCoord, 0 ).r );
	position.z = pPosTransform.x / ( pPosTransform.y - position.z );
	position.xy = ( vScreenCoord + pPosTransform2 ) * pPosTransform.zw * position.zz;
	
	outPosition = vec3( pMatrixPosition * vec4( position, /*1*/ textureLod( texDepth, vTexCoord, 0 ).r ) );
	
	outDiffuse = vec3( diffuse );
	
	// normal is in camera space. we need it though in world space.
	// reverse multiplication order does transpose()
	outNormal = normalEncodeShiftedInt( normalize( normalLoadMaterial( texNormal, vTexCoord ) * pMatrixNormal ) );
}
