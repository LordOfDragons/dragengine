#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"

uniform HIGHP sampler2D texGIOffset;

uniform mat4 pMatrixMVP;
uniform ivec3 pGIGridCoordShift; // grid coordinate shift (wrapping around)

in vec2 inPosition;

out vec3 vColor;

ivec3 giIndexToCoord( in int index ){
	int stride = pGIGridProbeCount.x * pGIGridProbeCount.z;
	return ivec3( index % pGIGridProbeCount.x, index / stride, ( index % stride ) / pGIGridProbeCount.x );
}

ivec3 giGridShiftToLocal( in ivec3 shifted ){
	return ( shifted + pGIGridCoordShift ) % pGIGridProbeCount;
}

void main( void ){
	ivec3 probeCoord = giIndexToCoord( gl_InstanceID );
	vec3 probePosition = pGIGridProbeSpacing * vec3( probeCoord );
	
	probeCoord = giGridShiftToLocal( probeCoord );
	ivec2 texCoord = ivec2( pGIGridProbeCount.x * probeCoord.y + probeCoord.x, probeCoord.z );
	vec3 probeOffset = texelFetch( texGIOffset, texCoord, 0 ).rgb;
	
	// first vertex is (-1,1), second is (1,1)
	if( inPosition.x < 0.0 ){
		vColor = vec3( 0.0, 0.0, 1.0 );
		gl_Position = pMatrixMVP * vec4( probePosition, 1.0 );
		
	}else{
		vColor = vec3( 1.0, 0.0, 0.0 );
		gl_Position = pMatrixMVP * vec4( probePosition + probeOffset, 1.0 );
	}
}
