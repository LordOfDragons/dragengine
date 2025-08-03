#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"

struct sVertexPositionSetData{
	vec3 position;
	uint vertex;
};

struct sTransformedData{
	vec3 position;
	vec3 realNormal;
	vec3 normal;
	vec4 tangent;
};

UBOLAYOUT_BIND(0) readonly buffer VertexPositionSetData {
	sVertexPositionSetData pVertexPositionSetData[];
};

UBOLAYOUT_BIND(1) restrict buffer TransformedData {
	sTransformedData pTransformedData[];
};

layout( local_size_x=64 ) in;

UNIFORM_BIND(0) uniform uvec2 pParams; // x=first, y=count
UNIFORM_BIND(1) uniform float pWeight;

void main( void ){
	if( gl_GlobalInvocationID.x >= pParams.y ){
		return;
	}
	
	uint vpsIndex = pParams.x + gl_GlobalInvocationID.x;
	uint vertexIndex = pVertexPositionSetData[ vpsIndex ].vertex;
	
	pTransformedData[ vertexIndex ].position += pVertexPositionSetData[ vpsIndex ].position * pWeight;
}
