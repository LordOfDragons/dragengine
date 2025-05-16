precision HIGHP float;
precision HIGHP int;

uniform vec4 pPosTransform;
uniform vec2 pPosTransform2;
uniform mat4x3 pMatrixPosition;
uniform mat3 pMatrixNormal;

uniform HIGHP sampler2DArray texDepth;
uniform lowp sampler2DArray texDiffuse;
uniform lowp sampler2DArray texNormal;

in vec2 vScreenCoord;
in vec2 vTexCoord;

layout(location=0) out vec3 outPosition;
layout(location=1) out vec3 outDiffuse;
layout(location=2) out vec3 outNormal;

#include "shared/normal_texture.glsl"


void main( void ){
	// not all pixels in the material buffer are possibly written to. this happens for
	// sky pixels for example. in this case material can not be written. using the same
	// discard rule as lighting shader is using
	vec3 tc = vec3( vTexCoord, 0 );
	
	vec4 diffuse = textureLod( texDiffuse, tc, 0.0 );
	
	if( diffuse.a == 0.0 ){
		discard;
	}
	
	// position is in camera space. transform to target space
	vec3 position = vec3( textureLod( texDepth, tc, 0.0 ).r );
	position.z = pPosTransform.x / ( pPosTransform.y - position.z );
	position.xy = ( vScreenCoord + pPosTransform2 ) * pPosTransform.zw * position.zz;
	
	outPosition = vec3( pMatrixPosition * vec4( position, /*1*/ textureLod( texDepth, tc, 0.0 ).r ) );
	
	outDiffuse = vec3( diffuse );
	
	// normal is in camera space (framebuffer normal encoding, usually float).
	// we need it though in world space. reverse multiplication order does transpose().
	// envmap normal is in shift-int format
	outNormal = normalEncodeShiftedInt( normalize( sanitizeNormal( normalLoadMaterial( texNormal, tc ) ) * pMatrixNormal ) );
}
