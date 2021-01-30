precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"

UBOLAYOUT uniform TracingParameters{
	vec2 pSampleImageScale;
	int pProbeCount;
	int pRaysPerProbe;
	int pProbesPerLine;
	int pBVHInstanceRootNode;
	ivec3 pGridProbeCount;
	vec3 pGridProbeSpacing;
	ivec4 pProbeIndex[ 64 ]; // max probe update count / 4
	vec3 pProbePosition[ 256 ]; // max probe update count
	vec3 pRayDirection[ 64 ]; // ray count
};

in vec3 inPosition;

out int vOffset;
out vec3 vProbePosition;

void main( void ){
	ivec2 group;
	group.y = gl_InstanceID / pProbesPerLine;
	group.x = gl_InstanceID - group.y * pProbesPerLine;
	
	vOffset = group.x * pRaysPerProbe;
	vProbePosition = pProbePosition[ gl_InstanceID ];
	
	vec2 realSize = vec2( pRaysPerProbe, 1.0 ) * pSampleImageScale;
	vec2 realOffset = realSize * vec2( 2.0 ) * group + vec2( -1.0 );
	realOffset.x += realSize.x;
	
	gl_Position = vec4( vec3( inPosition.xy * realSize + realOffset, 0.0 ), 1.0 );
}
