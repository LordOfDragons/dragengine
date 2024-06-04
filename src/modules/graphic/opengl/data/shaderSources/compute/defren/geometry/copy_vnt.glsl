precision highp float;
precision highp int;

#include "shared/ubo_defines.glsl"

struct sModelData{
	vec3 position;
	vec3 realNormal;
	vec3 normal;
	vec4 tangent;
	vec2 texCoord;
	int weights;
};

struct sTransformedData{
	vec3 position;
	vec3 realNormal;
	vec3 normal;
	vec4 tangent;
};

UBOLAYOUT_BIND(0) readonly buffer ModelData {
	sModelData pModelData[];
};

UBOLAYOUT_BIND(1) writeonly restrict buffer TransformedData {
	sTransformedData pTransformedData[];
};

layout( local_size_x=64 ) in;

uniform uvec2 pParams; // x=first, y=count

void main( void ){
	if( gl_GlobalInvocationID.x >= pParams.y ){
		return;
	}
	
	uint from = pParams.x + gl_GlobalInvocationID.x;
	uint to = gl_GlobalInvocationID.x;
	
	pTransformedData[ to ].position = pModelData[ from ].position;
	pTransformedData[ to ].realNormal = pModelData[ from ].realNormal;
	pTransformedData[ to ].normal = pModelData[ from ].normal;
	pTransformedData[ to ].tangent = pModelData[ from ].tangent;
}
