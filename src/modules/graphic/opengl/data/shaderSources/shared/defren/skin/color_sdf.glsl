/*
	<!-- shared/defren/skin/color_sdf.glsl -->
	<define>TEXTURE_COLOR_SDF</define>
*/


struct SdfParams{
	vec4 fillColor;
	vec4 borderColor;
	float borderSize;
	float pixelRange;
	float dilation;
};


/** SDF distance in the range [0, 1]. */
float sdfDistance(const in vec3 sdf){
	return max(min(sdf.r, sdf.g), min(max(sdf.r, sdf.g), sdf.b));
}

/** Coverage of a single SDF in the range [0, 1]. */
float sdfCoverage(const in float sdf, const in float pixelRange, const in float dilation){
	float threshold = 0.5 - dilation;
	return clamp(pixelRange * (sdf - threshold) + 0.5, 0.0, 1.0);
}

/** Apply SDF border. */
vec4 sdfApplyBorder(const in vec4 fillColor, const in float fillCoverage, const in float fillSdf, const in SdfParams params){
	float borderCoverage = sdfCoverage(fillSdf, params.pixelRange, params.dilation + params.borderSize);
	
	vec4 borderColor = params.borderColor;
	borderColor.a *= borderCoverage;
	
	return mix(fillColor, borderColor, max(borderCoverage - fillCoverage, 0.0));
}


/** Signed distance field color. */
vec4 getSdfColor(const in sampler2D sdfTex, const in vec2 tc, const in SdfParams params){
	float sdf = sdfDistance(textureLod(sdfTex, tc, 0.0).rgb);
	float coverage = sdfCoverage(sdf, params.pixelRange, params.dilation);
	
	vec4 color = params.fillColor;
	color.a *= coverage;
	
	return sdfApplyBorder(color, coverage, sdf, params);
}

/**
 * Composite signed distance field color layers over base color. Each texture layer represents
 * one colorized SDF layer. Distance is sampled using MSDF approach. The layer colors are defined
 * by the SDF layer color texture. Pixel with coordinate U defines colors of layer U.
 */
vec4 getSdfColorArray(const in sampler2DArray sdfTex, const in vec2 tc, const in SdfParams params){
	int clampTcCols = textureSize(texColorSdfColors, 0).x - 1;
	float pixelRange = params.pixelRange;
	float dilation = params.dilation;
	float fillSdf = 0.0, fillCoverage = 0.0;
	int i, layerCount = textureSize(sdfTex, 0).z;
	vec4 color = vec4(0);
	
	for(i=0; i<layerCount; i++){
		float sdf = sdfDistance(textureLod(sdfTex, vec3(tc, i), 0.0).rgb);
		float coverage = sdfCoverage(sdf, pixelRange, dilation);
		vec4 layerColor = texelFetch(texColorSdfColors, ivec2(min(i, clampTcCols), 0), 0);
		
		color = mix(color, vec4(layerColor.rgb, 1), layerColor.a * coverage);
		
		fillCoverage = max(fillCoverage, coverage);
		fillSdf = max(fillSdf, sdf);
	}
	
	return sdfApplyBorder(color, fillCoverage, fillSdf, params);
}
