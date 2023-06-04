
// calculate z from pixel to shader. call is protected against "depth == zfar".
float depthToZ( sampler2DArray samplerDepth, in int layer ){
	return depthToZ( sampleDepth( samplerDepth, ivec3( gl_FragCoord.xy, layer ) ), layer );
}

// calculate position from pixel to shader. call is protected against "depth == zfar".
vec3 depthToPosition( sampler2DArray samplerDepth, in vec2 screenCoord, in int layer ){
	return depthToPosition( sampleDepth( samplerDepth, ivec3( gl_FragCoord.xy, layer ) ), screenCoord, layer );
}
