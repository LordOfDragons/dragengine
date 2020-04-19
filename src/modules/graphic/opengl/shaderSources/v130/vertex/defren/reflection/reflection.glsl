precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"

UBOLAYOUT uniform RenderParameters{
	mat3 pMatrixEnvMap;
	vec4 pQuadTCTransform;
	vec4 pPosTransform;
	vec2 pBlendFactors; // x=multiply, y=add
	float pEnvMapLodLevel;
	int pLayerCount;
	vec4 pEnvMapPosLayer[ 100 ]; // xyz=position, w=layer
};

in vec3 inPosition;

out vec4 vScreenCoord;

void main( void ){
	gl_Position = vec4( inPosition, 1.0 );
	vScreenCoord = inPosition.xyxy;
	vScreenCoord.xy = vScreenCoord.xy * pQuadTCTransform.xy + pQuadTCTransform.zw;
}
