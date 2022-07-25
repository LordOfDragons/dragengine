#include "v130/shared/defren/sample_depth.glsl"

// calculate position from depth value. requires sampleDepth to be called to get depth value
vec3 depthToPosition( in float depth, in vec2 screenCoord, in int layer ){
	vec3 position = vec3( depth );
	#ifdef GS_RENDER_STEREO
		position.z = pDepthToPosition[ layer ].x / ( pDepthToPosition[ layer ].y - position.z );
		position.xy = ( screenCoord + pDepthToPosition2[ layer ] ) * pDepthToPosition[ layer ].zw * position.zz;
	#else
		position.z = pDepthToPosition.x / ( pDepthToPosition.y - position.z );
		position.xy = ( screenCoord + pDepthToPosition2 ) * pDepthToPosition.zw * position.zz;
	#endif
	return position;
}

vec3 depthToPositionVolume( in float depth, in vec3 volumePosition, in int layer ){
	vec3 position = vec3( depth );
	#ifdef GS_RENDER_STEREO
		position.z = pDepthToPosition[ layer ].x / ( pDepthToPosition[ layer ].y - position.z );
	#else
		position.z = pDepthToPosition.x / ( pDepthToPosition.y - position.z );
	#endif
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

// calculate position from pixel to shader
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
