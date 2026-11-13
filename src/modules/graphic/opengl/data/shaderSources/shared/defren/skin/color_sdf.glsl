/*
	<!-- shared/defren/skin/color_sdf.glsl -->
	<define>TEXTURE_COLOR_SDF</define>
*/


struct SdfParams{
	vec4 fillColor;
	vec4 borderColor;
	vec4 tcClamp;
	vec2 tcToSdf;
	vec2 distanceRemap;
	float borderSize;
	float pixelRange;
	float dilation;
};


/**
 * Clamp texture coordinates to the SDF rectangle. Returns clamped coordinates with distance of
 * clamped coordinates to unclamped coordinates.
 */
vec3 sdfClampTexCoord(const in sampler2D sdfTex, const in vec2 tc, const in SdfParams params){
	vec2 tcClamped = clamp(tc, params.tcClamp.xy, params.tcClamp.zw);
	float sdfDist = length((tcClamped - tc) * params.tcToSdf);
	return vec3(tcClamped, sdfDist);
}

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
vec4 sdfApplyBorder(const in vec4 fillColor, const in float fillCoverage, const in float borderSdf, const in SdfParams params){
	float borderCoverage = sdfCoverage(borderSdf, params.pixelRange, params.dilation + params.borderSize);
	
	vec4 borderColor = params.borderColor;
	borderColor.a *= borderCoverage;
	
	return mix(fillColor, borderColor, max(borderCoverage - fillCoverage, 0.0));
}


/** Signed distance field color. */
vec4 getSdfColor(const in sampler2D sdfTex, const in vec2 tc, const in SdfParams params){
	vec3 tcClamped = sdfClampTexCoord(sdfTex, tc, params);
	vec4 sdf = textureLod(sdfTex, tcClamped.xy, 0.0);
	
	float coverage = sdfCoverage(sdfDistance(sdf.rgb), params.pixelRange, params.dilation);
	
	vec4 color = params.fillColor;
	color.a *= coverage;
	
	float borderSdf = sdf.a * params.distanceRemap.x + params.distanceRemap.y;
	return sdfApplyBorder(color, coverage, borderSdf - tcClamped.z, params);
}

/**
 * Composite signed distance field color layers over base color. Each texture layer represents
 * one colorized SDF layer. Distance is sampled using MSDF approach. The layer colors are defined
 * by the SDF layer color texture. Pixel with coordinate U defines colors of layer U.
 */
vec4 getSdfColorArray(const in sampler2DArray sdfTex, const in vec2 tc, const in SdfParams params){
	vec3 tcClamped = sdfClampTexCoord(texColorSdfColors, tc, params);
	int clampTcCols = textureSize(texColorSdfColors, 0).x - 1;
	float pixelRange = params.pixelRange;
	float dilation = params.dilation;
	float borderSdf = 0.0, fillCoverage = 0.0;
	int i, layerCount = textureSize(sdfTex, 0).z;
	vec4 color = vec4(0);
	
	for(i=0; i<layerCount; i++){
		vec4 sdf = textureLod(sdfTex, vec3(tcClamped.xy, i), 0.0);
		
		float coverage = sdfCoverage(sdfDistance(sdf.rgb), pixelRange, dilation);
		vec4 layerColor = texelFetch(texColorSdfColors, ivec2(min(i, clampTcCols), 0), 0);
		
		color = mix(color, vec4(layerColor.rgb, 1), layerColor.a * coverage);
		
		fillCoverage = max(fillCoverage, coverage);
		borderSdf = max(borderSdf, sdf.a);
	}
	
	borderSdf = borderSdf * params.distanceRemap.x + params.distanceRemap.y;
	return sdfApplyBorder(color, fillCoverage, borderSdf - tcClamped.z, params);
}
