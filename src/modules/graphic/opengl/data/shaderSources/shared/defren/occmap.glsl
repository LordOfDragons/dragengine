/*
	<!-- shared/defren/occmap.glsl -->
	<define>SHARED_SPB</define>
*/

// WithShadowMap
#include "shared/defren/ubo_render_parameters.glsl"

// !WithShadowMap
UBOLAYOUT_BIND(0) uniform RenderParametersOccMap{
	mat4 pOMMatrixVP[6];
	mat4x3 pOMMatrixV[6]; // RenderParameters.pMatrixV
	vec4 pOMTransformZ[6];
	vec2 pOMZToDepth;
	vec4 pOMClipPlane[2]; // RenderParameters.pClipPlane
};

mat4 getMatrixVP(const in int index){
	return WithShadowMap ? pMatrixVP[index] : pOMMatrixVP[index];
}
mat4x3 getMatrixV(const in int index){
	return WithShadowMap ? pMatrixV[index] : pOMMatrixV[index];
}
vec4 getTransformZ(const in int index){
	return WithShadowMap ? vec4(0.0) : pOMTransformZ[index];
}
vec2 getZToDepth(){
	return WithShadowMap ? vec2(0.0) : pOMZToDepth;
}
vec4 getClipPlane(const in int index){
	return WithShadowMap ? vec4(0.0) : pOMClipPlane[index];
}
#define GET_DEPTH_OFFSET_VALUE(layer) (WithShadowMap ? pDepthOffset[layer] : vec4(0.0))


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
	
#else // SHARED_SPB
	UBOLAYOUT_BIND(2) uniform OcclusionMeshParameters{
		mat4x3 pMatrixModel;
	};
#endif
