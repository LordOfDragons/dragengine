UBOLAYOUT uniform InstanceParameters{
	mat4 pMatrixMVP[ 2 ];
	mat4x3 pMatrixMV[ 2 ];
	
	#ifdef PARTICLE_LIGHT
		vec4 pSamplesParams; // samplesScaleU, samplesOffsetU, samplesScaleV, samplesOffsetV
		float pBurstFactor;
		
	#else
		// light params in camera space
		vec3 pLightPosition[ 2 ];
		vec3 pLightView[ 2 ];
		//vec3 pLightUp[ 2 ]; // not used
		//vec3 pLightRight[ 2 ]; // not used
		float pDepthCompare; // 0=disable, 1=greater, -1=less
		
		mat4 pShadowMatrix1[ 2 ];
		mat4 pShadowMatrix2[ 2 ];
		#ifdef SKY_LIGHT
			mat4 pShadowMatrix3[ 2 ];
			mat4 pShadowMatrix4[ 2 ];
			vec4 pLayerBorder;
		#endif
		#if defined TEXTURE_COLOR_CUBEMAP || defined TEXTURE_COLOR_EQUIRECT
			mat4x3 pLightImageOmniMatrix[ 2 ];
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
