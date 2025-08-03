#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

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

UBOLAYOUT_BIND(1) restrict buffer TransformedData {
	sTransformedData pTransformedData[];
};

UBOLAYOUT_BIND(2) readonly buffer WeightsMatrix {
	mat4x3 pWeightMatrix[];
};

layout( local_size_x=64 ) in;

UNIFORM_BIND(0) uniform uvec2 pParams; // x=first, y=count

#define GET_DATA(i,m) (TransformInPlace ? pTransformedData[i].m : pModelData[i].m)


void main( void ){
	// skip outside of parameter space
	if( gl_GlobalInvocationID.x >= pParams.y ){
		return;
	}
	
	// get parameters
	uint from = pParams.x + gl_GlobalInvocationID.x;
	uint to = gl_GlobalInvocationID.x;
	uint fetchFrom = TransformInPlace ? to : from;
	
	int weights = pModelData[ from ].weights;
	
	// if there is no weight write out all positions untransformed
	if( weights == -1 ){
		if(!TransformInPlace){
			pTransformedData[ to ].position = pModelData[ from ].position;
			pTransformedData[ to ].realNormal = pModelData[ from ].realNormal;
			pTransformedData[ to ].normal = pModelData[ from ].normal;
			pTransformedData[ to ].tangent = pModelData[ from ].tangent;
		}
		return;
	}
	
	mat4x3 matrix = pWeightMatrix[ weights ];
	
	// transform the position. this is correct and accurate
	pTransformedData[to].position = matrix * vec4(GET_DATA(fetchFrom, position), 1);
	
	// transform the normal and tangent. this is not correct and only an approximation
	// 
	// NOTE a quick solution is using if(v==vec3(0)) to check for degenerated case. this is though
	//      dangerous since the normalization is not done in this shader but inside another shader
	//      using the value written to VBO. This can result in values close to zero to become zero
	//      after the transformation. Or some calculation in the other shaders can be suspectible
	//      to values close to zero. Using if(dot(v,v)<0.001) is safer and allows to use a
	//      threshold value to protect against dangerously small vectors.
	mat3 matrixNormal = mat3( matrix );
	
	vec3 v = matrixNormal * GET_DATA(fetchFrom, realNormal);
	pTransformedData[ to ].realNormal = dot( v, v ) > 0.00001 ? v : vec3( 0, 1, 0 );
	
	v = matrixNormal * GET_DATA(fetchFrom, normal);
	pTransformedData[ to ].normal = dot( v, v ) > 0.00001 ? v : vec3( 0, 1, 0 );
	
	v = matrixNormal * vec3( GET_DATA(fetchFrom, tangent ) );
	pTransformedData[ to ].tangent.xyz = dot( v, v ) > 0.00001 ? v : vec3( 1, 0, 0 );
	if(!TransformInPlace){
		pTransformedData[ to ].tangent.w = pModelData[ fetchFrom ].tangent.w;
	}
}
