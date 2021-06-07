precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/defren/gi/trace_probe.glsl"

uniform mediump sampler2DArray texDistance;

out vec3 fbMinExtend;
out vec3 fbMaxExtend;

void main( void ){
	gl_Position = vec4( 2, 2, 2, 1 ); // keep broken compilers happy
	fbMinExtend = pGIFieldSize;
	fbMaxExtend = -pGIFieldSize;
	
	ivec3 rayTC = ivec3( ( gl_InstanceID % pGIProbesPerLine ) * pGIRaysPerProbe,
		gl_InstanceID / pGIProbesPerLine, pGICascade );
	
	vec3 probePosition = pGIProbePosition[ gl_InstanceID ].xyz;
	int i;
	
	for( i=0; i<pGIRaysPerProbe; i++ ){
		vec3 position = probePosition + pGIRayDirection[ i ]
			* texelFetch( texDistance, rayTC + ivec3( i, 0, 0 ), 0 ).r;
		
		fbMinExtend = min( fbMinExtend, position );
		fbMaxExtend = max( fbMaxExtend, position );
	}
	
	fbMinExtend = max( fbMinExtend, -pGIFieldSize );
	fbMaxExtend = min( fbMaxExtend, pGIFieldSize );
}
