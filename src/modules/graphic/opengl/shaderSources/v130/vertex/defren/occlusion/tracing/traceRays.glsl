precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/occtracing.glsl"

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
	vec2 realOffset = realSize * group * vec2( 2.0 ) + realSize + vec2( -1.0 );
	
	gl_Position = vec4( vec3( inPosition.xy * realSize + realOffset, 0.0 ), 1.0 );
}
