/*
	<!-- shared/defren/sample_depth.glsl -->
	<define>DECODE_IN_DEPTH</define>
*/

// DecodeInDepth
const vec3 _sampleDepth_unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );

// sample depth value
float sampleDepth( ARG_SAMP_HIGHP sampler2DArray samplerDepth, in ivec3 texCoord, in int level ){
	if(DecodeInDepth){
		return dot( texelFetch( samplerDepth, texCoord, level ).rgb, _sampleDepth_unpackDepth );
		
	}else{
		return texelFetch( samplerDepth, texCoord, level ).r;
	}
}

float sampleDepth( ARG_SAMP_HIGHP sampler2DArray samplerDepth, in ivec3 texCoord ){
	return sampleDepth( samplerDepth, texCoord, 0 );
}

float sampleDepth( ARG_SAMP_HIGHP sampler2DArray samplerDepth, in vec3 texCoord, in float level ){
	if(DecodeInDepth){
		return dot( textureLod( samplerDepth, texCoord, level ).rgb, _sampleDepth_unpackDepth );
		
	}else{
		return textureLod( samplerDepth, texCoord, level ).r;
	}
}

float sampleDepth( ARG_SAMP_HIGHP sampler2DArray samplerDepth, in vec3 texCoord ){
	return sampleDepth( samplerDepth, texCoord, 0.0 );
}
