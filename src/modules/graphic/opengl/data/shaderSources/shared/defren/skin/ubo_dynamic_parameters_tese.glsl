/*
	<!-- shared/defren/skin/ubo_dynamic_parameters_tese.glsl -->
	<define>DYNAMIC_HEIGHT_REMAP</define>
*/

#ifdef DYNAMIC_HEIGHT_REMAP
	#define pHeightRemap pInstHeightRemap
#else
	#define pHeightRemap pTexHeightRemap
#endif
