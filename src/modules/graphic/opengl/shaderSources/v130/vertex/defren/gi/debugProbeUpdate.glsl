uniform vec4 pPosTransform; // scaleX, scaleY, offsetX, offsetY
uniform vec4 pTCTransform;

#ifdef PASS2
	#include "v130/shared/ubo_defines.glsl"
	#include "v130/shared/defren/gi/constants.glsl"
	#include "v130/shared/defren/gi/trace_probe.glsl"
	#include "v130/shared/defren/light/ubo_gi.glsl"
	
	uniform ivec3 pParams; // probeSize, spaceSize, groupSpaceSize
	uniform int pGIDebugCascade;
#endif

in vec2 inPosition;

#ifdef PASS2
	flat out ivec3 vProbeCoord;
	
#else
	out vec2 vTC;
#endif

#ifdef PASS2
ivec3 probeIndexToGridCoord( in int index ){
	return ivec3( index % pGIParams[pGIDebugCascade].probeCount.x,
		index / ( pGIParams[pGIDebugCascade].probeCount.x * pGIParams[pGIDebugCascade].probeCount.z ),
		( index % ( pGIParams[pGIDebugCascade].probeCount.x * pGIParams[pGIDebugCascade].probeCount.z ) )
			/ pGIParams[pGIDebugCascade].probeCount.x );
}

ivec3 giGridLocalToShift( in ivec3 local ){
	return ( local + pGIParams[pGIDebugCascade].gridCoordUnshift ) % pGIParams[pGIDebugCascade].probeCount;
}
#endif

void main( void ){
	#ifdef PASS2
		int probeIndex = giTraceProbeProbeIndex( gl_InstanceID );
		vProbeCoord = giGridLocalToShift( probeIndexToGridCoord( probeIndex ) );
		
		const int probeStride = pParams.x + pParams.y;
		const int groupSize = pParams.x * pGIParams[pGIDebugCascade].probeCount.z
			+ pParams.y * ( pGIParams[pGIDebugCascade].probeCount.z - 1 );
		const int groupStride = groupSize + pParams.z;
		
		vec2 tc = vec2( groupStride * vProbeCoord.y + probeStride * vProbeCoord.x,
			probeStride * ( ( pGIParams[pGIDebugCascade].probeCount.z - 1 ) - vProbeCoord.z ) );
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
