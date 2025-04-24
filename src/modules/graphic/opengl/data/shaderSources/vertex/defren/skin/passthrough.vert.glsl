#ifdef EXT_ARB_SHADER_VIEWPORT_LAYER_ARRAY
	#extension GL_ARB_shader_viewport_layer_array : require
#endif
#ifdef EXT_AMD_VERTEX_SHADER_LAYER
	#extension GL_AMD_vertex_shader_layer : require
#endif
#ifdef EXT_ARB_SHADER_DRAW_PARAMETERS
	#extension GL_ARB_shader_draw_parameters : require
#endif

precision HIGHP float;
precision HIGHP int;



// Uniforms
/////////////

#ifdef SHARED_SPB
	#include "shared/defren/skin/shared_spb_index.glsl"
#endif
#include "shared/defren/skin/ubo_special_parameters.glsl"



// Inputs
///////////

#ifdef HEIGHT_MAP
	layout(location=0) in vec2 inPosition;
	layout(location=1) in float inHeight;
	layout(location=2) in float inNormal;
#else
	layout(location=0) in vec3 inPosition;
	layout(location=1) in vec3 inRealNormal;
	layout(location=2) in vec3 inNormal;
	layout(location=3) in vec4 inTangent;
	layout(location=4) in vec2 inTexCoord;
#endif



// Outputs
////////////

#ifdef HEIGHT_MAP
	out vec2 vInPosition;
	out float vInHeight;
	out float vInNormal;
#else
	out vec3 vInPosition;
	out vec3 vInNormal;
	out vec4 vInTangent;
	out vec2 vInTexCoord;
#endif

flat out int vSPBIndex;
flat out int vSPBFlags;

#ifdef VS_RENDER_STEREO
	uniform int pDrawIDOffset;
	#define inLayer (gl_DrawID + pDrawIDOffset)
	flat out int vLayer;
#else
	const int inLayer = 0;
#endif



// Main Function
//////////////////

void main(void){
	#include "shared/defren/skin/shared_spb_index2.glsl"
	
	vInPosition = inPosition;
	vInNormal = inNormal;
	#ifdef HEIGHT_MAP
		vInHeight = inHeight;
	#else
		vInTangent = inTangent;
		vInTexCoord = inTexCoord;
	#endif
	
	vSPBIndex = spbIndex;
	vSPBFlags = spbFlags;
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
