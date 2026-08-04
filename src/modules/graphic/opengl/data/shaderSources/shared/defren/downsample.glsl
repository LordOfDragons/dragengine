// functions for downsampling

// performance measurements in renderdoc on 2440x1585:
// 13-tap: 117ys
// 4-tap: 113ys
// 1-tap: 113ys

// NPOT color 13-tap downsampling filter based on CoD presentation at Siggraph 2014
//
// tc: texture coordinate of current pixel (range 0..1)
// pixelSize: size of pixel in texture coordinates (1/textureSize)
// layer: layer of texture array to sample from
// level: mipmap level to sample from
vec4 downsampleColorNpot13Tap(in mediump sampler2DArray samplerColor, in vec2 tc, in vec2 pixelSize,
in vec2 tcClamp, in int layer, in float level){
	float psx = pixelSize.x;
	float psy = pixelSize.y;
	
	// take 13 samples around current texel (e):
	// a - b - c
	// - j - k -
	// d - e - f
	// - l - m -
	// g - h - i
	vec4 s2 = textureLod(samplerColor, vec3(min(vec2(tc.x - 2.0 * psx, tc.y + 2.0 * psy), tcClamp), layer), level); // a
	vec4 s3 = textureLod(samplerColor, vec3(min(vec2(tc.x, tc.y + 2.0 * psy), tcClamp), layer), level); // b
	s2 += textureLod(samplerColor, vec3(min(vec2(tc.x + 2.0 * psx, tc.y + 2.0 * psy), tcClamp), layer), level); // c
	
	s3 += textureLod(samplerColor, vec3(min(vec2(tc.x - 2.0 * psx, tc.y), tcClamp), layer), level); // d
	vec4 s1 = textureLod(samplerColor, vec3(min(vec2(tc.x, tc.y), tcClamp), layer), level); // e
	s3 += textureLod(samplerColor, vec3(min(vec2(tc.x + 2.0 * psx, tc.y), tcClamp), layer), level); // f
	
	s2 += textureLod(samplerColor, vec3(min(vec2(tc.x - 2.0 * psx, tc.y - 2.0 * psy), tcClamp), layer), level); // g
	s3 += textureLod(samplerColor, vec3(min(vec2(tc.x, tc.y - 2.0 * psy), tcClamp), layer), level); // h
	s2 += textureLod(samplerColor, vec3(min(vec2(tc.x + 2.0 * psx, tc.y - 2.0 * psy), tcClamp), layer), level); // i
	
	vec4 s4 = textureLod(samplerColor, vec3(min(vec2(tc.x - psx, tc.y + psy), tcClamp), layer), level); // j
	s4 += textureLod(samplerColor, vec3(min(vec2(tc.x + psx, tc.y + psy), tcClamp), layer), level); // k
	s4 += textureLod(samplerColor, vec3(min(vec2(tc.x - psx, tc.y - psy), tcClamp), layer), level); // l
	s4 += textureLod(samplerColor, vec3(min(vec2(tc.x + psx, tc.y - psy), tcClamp), layer), level); // m
	
	return s1 * 0.125 + s2 * 0.03125 + s3 * 0.0625 + s4 * 0.125;
}

// NPOT color 13-tap downsampling filter based on CoD presentation at Siggraph 2014 with sharpening
//
// tc: texture coordinate of current pixel (range 0..1)
// pixelSize: size of pixel in texture coordinates (1/textureSize)
// layer: layer of texture array to sample from
// level: mipmap level to sample from
vec4 downsampleColorNpot13TapSharpen(in mediump sampler2DArray samplerColor, in vec2 tc, in vec2 pixelSize,
in vec2 tcClamp, in int layer, in float level, float sharpen){
	float psx = pixelSize.x;
	float psy = pixelSize.y;
	
	// take 13 samples around current texel (e):
	// a - b - c
	// - j - k -
	// d - e - f
	// - l - m -
	// g - h - i
	vec4 s2 = textureLod(samplerColor, vec3(min(vec2(tc.x - 2.0 * psx, tc.y + 2.0 * psy), tcClamp), layer), level); // a
	vec4 s3 = textureLod(samplerColor, vec3(min(vec2(tc.x, tc.y + 2.0 * psy), tcClamp), layer), level); // b
	s2 += textureLod(samplerColor, vec3(min(vec2(tc.x + 2.0 * psx, tc.y + 2.0 * psy), tcClamp), layer), level); // c
	
	s3 += textureLod(samplerColor, vec3(min(vec2(tc.x - 2.0 * psx, tc.y), tcClamp), layer), level); // d
	vec4 s1 = textureLod(samplerColor, vec3(min(vec2(tc.x, tc.y), tcClamp), layer), level); // e
	s3 += textureLod(samplerColor, vec3(min(vec2(tc.x + 2.0 * psx, tc.y), tcClamp), layer), level); // f
	
	s2 += textureLod(samplerColor, vec3(min(vec2(tc.x - 2.0 * psx, tc.y - 2.0 * psy), tcClamp), layer), level); // g
	s3 += textureLod(samplerColor, vec3(min(vec2(tc.x, tc.y - 2.0 * psy), tcClamp), layer), level); // h
	s2 += textureLod(samplerColor, vec3(min(vec2(tc.x + 2.0 * psx, tc.y - 2.0 * psy), tcClamp), layer), level); // i
	
	vec4 s4 = textureLod(samplerColor, vec3(min(vec2(tc.x - psx, tc.y + psy), tcClamp), layer), level); // j
	s4 += textureLod(samplerColor, vec3(min(vec2(tc.x + psx, tc.y + psy), tcClamp), layer), level); // k
	s4 += textureLod(samplerColor, vec3(min(vec2(tc.x - psx, tc.y - psy), tcClamp), layer), level); // l
	s4 += textureLod(samplerColor, vec3(min(vec2(tc.x + psx, tc.y - psy), tcClamp), layer), level); // m
	
	// weighting
	float w1Base = 0.125;
	float w2Base = 0.03125;
	float w3Base = 0.0625;
	float w4Base = 0.125;
	
	float w2 = w2Base - (0.04 * sharpen);
	float w3 = w3Base - (0.05 * sharpen);
	
	float w1 = w1Base + (0.12 * sharpen);
	float w4 = w4Base + (0.06 * sharpen);
	
	return s1 * w1 + s2 * w2 + s3 * w3 + s4 * w4;
}



// NPOT color 9-tap downsampling filter with sharpening
//
// tc: texture coordinate of current pixel (range 0..1)
// pixelSize: size of pixel in texture coordinates (1/textureSize)
// layer: layer of texture array to sample from
// level: mipmap level to sample from
vec4 downsampleColorNpot9TapSharpen(in mediump sampler2DArray samplerColor, in vec2 tc, in vec2 pixelSize,
in vec2 tcClamp, in int layer, in float level, in float sharpen){
	// sample 3x3 grid
	vec4 s3 = textureLod(samplerColor, vec3(min(vec2(tc.x - pixelSize.x, tc.y - pixelSize.y), tcClamp), layer), level);
	vec4 s2 = textureLod(samplerColor, vec3(min(vec2(tc.x, tc.y - pixelSize.y), tcClamp), layer), level);
	s3 += textureLod(samplerColor, vec3(min(vec2(tc.x + pixelSize.x, tc.y - pixelSize.y), tcClamp), layer), level);
	
	s2 += textureLod(samplerColor, vec3(min(vec2(tc.x - pixelSize.x, tc.y), tcClamp), layer), level);
	vec4 s1 = textureLod(samplerColor, vec3(min(tc, tcClamp), layer), level); // Center
	s2 += textureLod(samplerColor, vec3(min(vec2(tc.x + pixelSize.x, tc.y), tcClamp), layer), level);
	
	s3 += textureLod(samplerColor, vec3(min(vec2(tc.x - pixelSize.x, tc.y + pixelSize.y), tcClamp), layer), level);
	s2 += textureLod(samplerColor, vec3(min(vec2(tc.x, tc.y + pixelSize.y), tcClamp), layer), level);
	s3 += textureLod(samplerColor, vec3(min(vec2(tc.x + pixelSize.x, tc.y + pixelSize.y), tcClamp), layer), level);
	
	// weighting
	float weightEdge = -0.10 * sharpen;
	float weightCorner = -0.05 * sharpen;
	float weightCenter = 1.0 - (4.0 * weightEdge) - (4.0 * weightCorner);
	
	return max(vec4(0.0), s1 * weightCenter + s2 * weightEdge + s3 * weightCorner);
}

// NPOT depth 9-tap downsampling filter with sharpening
//
// tc: texture coordinate of current pixel (range 0..1)
// pixelSize: size of pixel in texture coordinates (1/textureSize)
// layer: layer of texture array to sample from
// level: mipmap level to sample from
float downsampleDepthNpot9Tap(in mediump sampler2DArray samplerDepth, in vec2 tc, in vec2 pixelSize,
in vec2 tcClamp, in int layer, in float level, in float sharpen){
	return downsampleColorNpot9TapSharpen(samplerDepth, tc, pixelSize, tcClamp, layer, level, sharpen).r;
}



// NPOT color 4-tap downsampling filter
//
// tc: texture coordinate of current pixel (range 0..1)
// pixelSize: size of pixel in texture coordinates (1/textureSize)
// layer: layer of texture array to sample from
// level: mipmap level to sample from
vec4 downsampleColorNpot4Tap(in mediump sampler2DArray samplerColor, in vec2 tc, in vec2 pixelSize,
in vec2 tcClamp, in int layer, in float level){
	float psx = pixelSize.x;
	float psy = pixelSize.y;
	
	vec4 s = textureLod(samplerColor, vec3(min(vec2(tc.x - psx, tc.y - psy), tcClamp), layer), level);
	s += textureLod(samplerColor, vec3(min(vec2(tc.x + psx, tc.y - psy), tcClamp), layer), level);
	s += textureLod(samplerColor, vec3(min(vec2(tc.x - psx, tc.y + psy), tcClamp), layer), level);
	s += textureLod(samplerColor, vec3(min(vec2(tc.x + psx, tc.y + psy), tcClamp), layer), level);
	
	return s / 4.0;
}
