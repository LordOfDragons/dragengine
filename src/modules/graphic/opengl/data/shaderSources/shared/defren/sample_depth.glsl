
// sample depth value
float sampleDepth( ARG_SAMP_HIGHP sampler2DArray samplerDepth, in ivec3 texCoord, in int level ){
	return texelFetch( samplerDepth, texCoord, level ).r;
}

float sampleDepth( ARG_SAMP_HIGHP sampler2DArray samplerDepth, in ivec3 texCoord ){
	return sampleDepth( samplerDepth, texCoord, 0 );
}

float sampleDepth( ARG_SAMP_HIGHP sampler2DArray samplerDepth, in vec3 texCoord, in float level ){
	return textureLod( samplerDepth, texCoord, level ).r;
}

float sampleDepth( ARG_SAMP_HIGHP sampler2DArray samplerDepth, in vec3 texCoord ){
	return sampleDepth( samplerDepth, texCoord, 0.0 );
}
