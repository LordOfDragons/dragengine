#include "v130/shared/defren/sample_depth.glsl"

// calculate z from depth value. requires sampleDepth to be called to get depth value
// call is protected against "depth == zfar".
float depthToZ( in float depth, in int layer ){
	#ifdef GS_RENDER_STEREO
		float divisor = pDepthToPosition[ layer ].y - depth;
		return divisor != 0 ? pDepthToPosition[ layer ].x / divisor : pCameraViewDistance;
	#else
		float divisor = pDepthToPosition.y - depth;
		return divisor != 0 ? pDepthToPosition.x / divisor : pCameraViewDistance;
	#endif
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
	#ifdef GS_RENDER_STEREO
		position.xy = ( screenCoord + pDepthToPosition2[ layer ] ) * pDepthToPosition[ layer ].zw * position.zz;
	#else
		position.xy = ( screenCoord + pDepthToPosition2 ) * pDepthToPosition.zw * position.zz;
	#endif
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

// calculate z from pixel to shader. call is protected against "depth == zfar".
float depthToZ( sampler2DArray samplerDepth, in int layer ){
	return depthToZ( sampleDepth( samplerDepth, ivec3( gl_FragCoord.xy, layer ) ), layer );
}

// calculate position from pixel to shader. call is protected against "depth == zfar".
vec3 depthToPosition( sampler2DArray samplerDepth, in vec2 screenCoord, in int layer ){
	return depthToPosition( sampleDepth( samplerDepth, ivec3( gl_FragCoord.xy, layer ) ), screenCoord, layer );
}

// depth is z-far. use this test to avoid div-by-zero when calling depthToPosition
bool depthIsZFar( in float depth, in int layer ){
	#ifdef GS_RENDER_STEREO
		return depth == pDepthToPosition[ layer ].y;
	#else
		return depth == pDepthToPosition.y;
	#endif
}
