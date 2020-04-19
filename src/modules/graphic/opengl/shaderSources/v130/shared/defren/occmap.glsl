// contains the definition shared between vertex and geometry shaders for the
// occlusion map shader sources. the definition part is left out for the
// includers to define while keeping the rest here

#ifdef SHARED_SPB
	struct sOcclusionMeshParameters{
		mat4x3 pSPBMatrixModel;
		
		#ifdef SHARED_SPB_PADDING
		vec4 _pSPBPadding[ SHARED_SPB_PADDING ];
		#endif
	};
	
	#ifdef SHARED_SPB_USE_SSBO
		#ifdef ANDROID
			UBOLAYOUT_BIND(0) readonly buffer OcclusionMeshParametersSSBO
		#else
			UBOLAYOUT readonly buffer OcclusionMeshParametersSSBO
		#endif
	#else
		UBOLAYOUT uniform OcclusionMeshParameters
	#endif
	{
		#ifdef SHARED_SPB_ARRAY_SIZE
		sOcclusionMeshParameters pSharedSPB[ SHARED_SPB_ARRAY_SIZE ];
		#else
		sOcclusionMeshParameters pSharedSPB[];
		#endif
	};
	
#else  // SHARED_SPB
	UBOLAYOUT uniform OcclusionMeshParameters{
		mat4x3 pMatrixModel;
	};
#endif
