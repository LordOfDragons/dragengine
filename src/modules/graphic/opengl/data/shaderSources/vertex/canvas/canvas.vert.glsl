#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

const bool RequiresTexture = WithTexture || WithRenderWorld;

UNIFORM_BIND(0) uniform mat3x2 pTransform;
// RequiresTexture
UNIFORM_BIND(1) uniform mat3x2 pTCTransform;

// WithMask
UNIFORM_BIND(2) uniform mat3x2 pTCTransformMask;

layout(location=0) in vec2 inPosition;

#include "shared/interface/2d_vertex.glsl"

VARYING_BIND(3) out vec2 vTexCoordMask;

void main( void ){
	vertexShaderDefaultOutputs();
	
	gl_Position = vec4( vec2( pTransform * vec3( inPosition, 1.0 ) ), 0.0, 1.0 );
	
	if(RequiresTexture){
		vTexCoord = vec2( pTCTransform * vec3( inPosition, 1.0 ) );
	}
	
	if(WithMask){
		//vTexCoordMask = vec2( pTCTransformMask * vec3( gl_Position.xy, 1.0 ) );
		vTexCoordMask = vec2( pTCTransformMask * vec3( inPosition, 1.0 ) );
	}
}
