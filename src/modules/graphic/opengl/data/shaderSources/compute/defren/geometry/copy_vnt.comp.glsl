#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/geometry/model_data.glsl"

UBOLAYOUT_BIND(0) readonly buffer ModelData {
	sModelData pModelData[];
};

UBOLAYOUT_BIND(1) writeonly restrict buffer TransformedData {
	sTransformedData pTransformedData[];
};

UBOLAYOUT_BIND(1) writeonly restrict buffer TransformedDataTcs1 {
	sTransformedDataTcs1 pTransformedDataTcs1[];
};

UBOLAYOUT_BIND(1) writeonly restrict buffer TransformedDataTcs2 {
	sTransformedDataTcs2 pTransformedDataTcs2[];
};

layout( local_size_x=64 ) in;

UNIFORM_BIND(0) uniform uvec2 pParams; // x=first, y=count

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
