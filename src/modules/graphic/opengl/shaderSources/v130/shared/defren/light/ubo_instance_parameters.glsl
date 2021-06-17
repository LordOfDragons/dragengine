UBOLAYOUT uniform InstanceParameters{
	mat4 pMatrixMVP;
	mat4x3 pMatrixMV;
	
	#ifdef PARTICLE_LIGHT
		vec4 pSamplesParams; // samplesScaleU, samplesOffsetU, samplesScaleV, samplesOffsetV
		float pBurstFactor;
		
	#else
		// light params in camera space
		vec3 pLightPosition;
		vec3 pLightView;
		//vec3 pLightUp; // not used
		//vec3 pLightRight; // not used
		float pDepthCompare; // 0=disable, 1=greater, -1=less
		
		mat4 pShadowMatrix1;
		mat4 pShadowMatrix2;
		#ifdef SKY_LIGHT
			mat4 pShadowMatrix3;
			mat4 pShadowMatrix4;
			vec4 pLayerBorder;
		#endif
		#if defined TEXTURE_COLOR_CUBEMAP || defined TEXTURE_COLOR_EQUIRECT
			mat4x3 pLightImageOmniMatrix;
		#endif
		
		vec3 pShadow1Solid; // x=pixelSizeU, y=pixelSizeV, z=noiseScale
		vec3 pShadow1Transparent; // x=pixelSizeU, y=pixelSizeV, z=noiseScale
		vec3 pShadow2Solid; // x=pixelSizeU, y=pixelSizeV, z=noiseScale
		vec3 pShadow2Transparent; // x=pixelSizeU, y=pixelSizeV, z=noiseScale
		
		vec2 pShadowDepthTransform;
		#ifdef SKY_LIGHT
			vec2 pShadowDepthTransform2;
		#endif
		
		#ifdef SKY_LIGHT
			mat4 pGIShadowMatrix;
			vec3 pGIShadowParams; // x=pixelSizeU, y=pixelSizeV, z=noiseScale
		#endif
	#endif
};
