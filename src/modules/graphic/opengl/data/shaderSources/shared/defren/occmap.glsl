// contains the definition shared between vertex and geometry shaders for the
// occlusion map shader sources. the definition part is left out for the
// includers to define while keeping the rest here

/*
	<!-- shared/defren/occmap.glsl -->
	<define>SHARED_SPB</define>
*/

#ifdef SHARED_SPB
	struct sOcclusionMeshParameters{
		mat4x3 pSPBMatrixModel;
		
		#ifdef SHARED_SPB_PADDING
		vec4 _pSPBPadding[ SHARED_SPB_PADDING ];
		#endif
	};
	
	#ifdef SHARED_SPB_USE_SSBO
		UBOLAYOUT_BIND(0) readonly buffer OcclusionMeshParametersSSBO
	#else
		UBOLAYOUT_BIND(2) uniform OcclusionMeshParameters
	#endif
	{
		#ifdef SHARED_SPB_ARRAY_SIZE
		sOcclusionMeshParameters pSharedSPB[ SHARED_SPB_ARRAY_SIZE ];
		#else
		sOcclusionMeshParameters pSharedSPB[];
		#endif
	};
	
#else  // SHARED_SPB
	UBOLAYOUT_BIND(2) uniform OcclusionMeshParameters{
		mat4x3 pMatrixModel;
	};
#endif
