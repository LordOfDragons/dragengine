uniform vec4 pPosTransform; // scaleX, scaleY, offsetX, offsetY
uniform vec4 pTCTransform;

#ifdef PASS2
	#include "v130/shared/ubo_defines.glsl"
	#include "v130/shared/defren/gi/ubo_gi.glsl"
	#include "v130/shared/defren/gi/trace_probe.glsl"

	uniform ivec3 pParams; // probeSize, spaceSize, groupSpaceSize
#endif

in vec2 inPosition;

#ifdef PASS2
	flat out ivec3 vProbeCoord;
	
#else
	out vec2 vTC;
#endif

#ifdef PASS2
ivec3 probeIndexToGridCoord( in int index ){
	return ivec3( index % pGIGridProbeCount.x, index / ( pGIGridProbeCount.x * pGIGridProbeCount.z ),
		( index % ( pGIGridProbeCount.x * pGIGridProbeCount.z ) ) / pGIGridProbeCount.x );
}

ivec3 giGridLocalToShift( in ivec3 local ){
	return ( local + pGIGridCoordUnshift ) % pGIGridProbeCount;
}
#endif

void main( void ){
	#ifdef PASS2
		int probeIndex = giTraceProbeProbeIndex( gl_InstanceID );
		vProbeCoord = giGridLocalToShift( probeIndexToGridCoord( probeIndex ) );
		
		const int probeStride = pParams.x + pParams.y;
		const int groupSize = pParams.x * pGIGridProbeCount.z + pParams.y * ( pGIGridProbeCount.z - 1 );
		const int groupStride = groupSize + pParams.z;
		
		vec2 tc = vec2( groupStride * vProbeCoord.y + probeStride * vProbeCoord.x,
			probeStride * ( ( pGIGridProbeCount.z - 1 ) - vProbeCoord.z ) );
			// ^= z coordinate starts at the bottom (-z axis) but y starts at the top.
			//    for this reason z coordinate has to be flipped
		
		const vec2 halfProbeStride = vec2( pParams.x * 0.5 );
		
		tc += inPosition * halfProbeStride + halfProbeStride;
		
		gl_Position = vec4( tc * pPosTransform.xy + pPosTransform.zw, 0.0, 1.0 );
		
	#else
		gl_Position = vec4( inPosition * pPosTransform.xy + pPosTransform.zw, 0.0, 1.0 );
		vTC = inPosition * pTCTransform.xy + pTCTransform.zw;
	#endif
}
