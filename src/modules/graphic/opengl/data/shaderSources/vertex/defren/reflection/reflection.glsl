precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"

UBOLAYOUT uniform RenderParameters{
	mat3 pMatrixEnvMap;
	vec4 pQuadTCTransform;
	vec4 pPosTransform;
	vec2 pPosTransform2;
	vec2 pBlendFactors; // x=multiply, y=add
	float pEnvMapLodLevel;
	int pLayerCount;
	vec4 pEnvMapPosLayer[ 100 ]; // xyz=position, w=layer
};

in vec2 inPosition;

out vec4 vScreenCoord;

void main( void ){
	gl_Position = vec4( inPosition, 0, 1 );
	vScreenCoord = vec4( inPosition * pQuadTCTransform.xy + pQuadTCTransform.zw, inPosition );
}
