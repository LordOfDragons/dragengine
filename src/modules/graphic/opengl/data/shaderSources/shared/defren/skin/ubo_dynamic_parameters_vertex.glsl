/*
	<!-- shared/defren/skin/ubo_dynamic_parameters_vertex.glsl -->
	<define>DYNAMIC_OUTLINE_THICKNESS</define>
*/

#ifdef DYNAMIC_OUTLINE_THICKNESS
	#define pOutlineThickness pInstOutlineThickness
#else
	#define pOutlineThickness pTexOutlineThickness
#endif
