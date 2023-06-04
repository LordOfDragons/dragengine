#ifdef DECODE_IN_DEPTH
	const vec3 _sampleDepth_unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif

// sample depth value
float sampleDepth( sampler2DArray samplerDepth, in ivec3 texCoord, in int level ){
	#ifdef DECODE_IN_DEPTH
		return dot( texelFetch( samplerDepth, texCoord, level ).rgb, _sampleDepth_unpackDepth );
	#else
		return texelFetch( samplerDepth, texCoord, level ).r;
	#endif
}

float sampleDepth( sampler2DArray samplerDepth, in ivec3 texCoord ){
	return sampleDepth( samplerDepth, texCoord, 0 );
}

float sampleDepth( sampler2DArray samplerDepth, in vec3 texCoord, in float level ){
	#ifdef DECODE_IN_DEPTH
		return dot( textureLod( samplerDepth, texCoord, level ).rgb, _sampleDepth_unpackDepth );
	#else
		return textureLod( samplerDepth, texCoord, level ).r;
	#endif
}

float sampleDepth( sampler2DArray samplerDepth, in vec3 texCoord ){
	return sampleDepth( samplerDepth, texCoord, 0 );
}
