#include "shared/defren/sample_depth.glsl"

// calculate z from depth value. requires sampleDepth to be called to get depth value
// call is protected against "depth == zfar".
float depthToZ( in float depth, in int layer ){
	float divisor = pDepthToPosition[ layer ].y - depth;
	return divisor != 0 ? pDepthToPosition[ layer ].x / divisor : pCameraViewDistance;
}

float depthToZ( sampler2DArray samplerDepth, in ivec3 texCoord, in int layer ){
	return depthToZ( sampleDepth( samplerDepth, texCoord ), layer );
}

float depthToZ( sampler2DArray samplerDepth, in vec3 texCoord, in int layer ){
	return depthToZ( sampleDepth( samplerDepth, texCoord ), layer );
}

// calculate position from depth value. requires sampleDepth to be called to get depth value
// call is protected against "depth == zfar".
vec3 depthToPosition( in float depth, in vec2 screenCoord, in int layer ){
	vec3 position;
	position.z = depthToZ( depth, layer );
	position.xy = ( screenCoord + pDepthToPosition2[ layer ] ) * pDepthToPosition[ layer ].zw * position.zz;
	return position;
}

vec3 depthToPositionVolume( in float depth, in vec3 volumePosition, in int layer ){
	vec3 position;
	position.z = depthToZ( depth, layer );
	position.xy = volumePosition.xy * position.zz / volumePosition.zz;
	return position;
}

vec3 depthToPositionVolume( sampler2DArray samplerDepth, in ivec3 texCoord, in vec3 volumePosition, in int layer ){
	return depthToPositionVolume( sampleDepth( samplerDepth, texCoord ), volumePosition, layer );
}

vec3 depthToPosition( sampler2DArray samplerDepth, in ivec3 texCoord, in vec2 screenCoord, in int layer ){
	return depthToPosition( sampleDepth( samplerDepth, texCoord ), screenCoord, layer );
}

vec3 depthToPosition( sampler2DArray samplerDepth, in vec3 texCoord, in vec2 screenCoord, in int layer ){
	return depthToPosition( sampleDepth( samplerDepth, texCoord ), screenCoord, layer );
}

// depth is z-far. use this test to avoid div-by-zero when calling depthToPosition
bool depthIsZFar( in float depth, in int layer ){
	return depth == pDepthToPosition[ layer ].y;
}
