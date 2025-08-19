#include "shared/preamble.glsl"

UNIFORM_BIND(0) uniform vec4 pPosTransform; // scaleX, scaleY, offsetX, offsetY
UNIFORM_BIND(1) uniform vec4 pTCTransform;

// RenderPass == 1
#include "shared/ubo_defines.glsl"
#include "shared/defren/gi/trace_probe.glsl"
#include "shared/defren/light/ubo_gi.glsl"

// RenderPass == 1
UNIFORM_BIND(2) uniform ivec3 pParams; // probeSize, spaceSize, groupSpaceSize
UNIFORM_BIND(3) uniform int pGIDebugCascade;

layout(location=0) in vec2 inPosition;

// RenderPass == 0
VARYING_BIND(0) out vec2 vTC;

// RenderPass == 1
VARYING_BIND(1) flat out ivec3 vProbeCoord;


// RenderPass == 1
ivec3 probeIndexToGridCoord( in int index ){
	return ivec3( index % pGIParams[pGIDebugCascade].probeCount.x,
		index / ( pGIParams[pGIDebugCascade].probeCount.x * pGIParams[pGIDebugCascade].probeCount.z ),
		( index % ( pGIParams[pGIDebugCascade].probeCount.x * pGIParams[pGIDebugCascade].probeCount.z ) )
			/ pGIParams[pGIDebugCascade].probeCount.x );
}

// RenderPass == 1
ivec3 giGridLocalToShift( in ivec3 local ){
	return ( local + pGIParams[pGIDebugCascade].gridCoordUnshift ) % pGIParams[pGIDebugCascade].probeCount;
}


void main( void ){
	if(RenderPass == 1){
		int probeIndex = giTraceProbeProbeIndex( gl_InstanceID );
		vProbeCoord = giGridLocalToShift( probeIndexToGridCoord( probeIndex ) );
		
		UFCONST int probeStride = pParams.x + pParams.y;
		UFCONST int groupSize = pParams.x * pGIParams[pGIDebugCascade].probeCount.z
			+ pParams.y * ( pGIParams[pGIDebugCascade].probeCount.z - 1 );
		UFCONST int groupStride = groupSize + pParams.z;
		
		vec2 tc = vec2( groupStride * vProbeCoord.y + probeStride * vProbeCoord.x,
			probeStride * ( ( pGIParams[pGIDebugCascade].probeCount.z - 1 ) - vProbeCoord.z ) );
			// ^= z coordinate starts at the bottom (-z axis) but y starts at the top.
			//    for this reason z coordinate has to be flipped
		
		UFCONST vec2 halfProbeStride = vec2(float(pParams.x) * 0.5);
		
		tc += inPosition * halfProbeStride + halfProbeStride;
		
		gl_Position = vec4( tc * pPosTransform.xy + pPosTransform.zw, 0.0, 1.0 );
		
	}else{
		gl_Position = vec4( inPosition * pPosTransform.xy + pPosTransform.zw, 0.0, 1.0 );
		vTC = inPosition * pTCTransform.xy + pTCTransform.zw;
	}
}
