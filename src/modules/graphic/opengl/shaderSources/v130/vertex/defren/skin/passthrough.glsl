// request high precision if the graphic card supports this
#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif



// Uniforms
/////////////

#ifdef SHARED_SPB
	#include "v130/shared/defren/skin/shared_spb_index.glsl"
#endif



// Inputs
///////////

#ifdef HEIGHT_MAP
	in vec2 inPosition;
	in float inHeight;
	in float inNormal;
#else
	in vec3 inPosition;
	in vec3 inNormal;
	#ifdef TEXTURE_NORMAL
		in vec4 inTangent;
	#endif
	in vec2 inTexCoord;
#endif

#ifdef NODE_VERTEX_INPUTS
NODE_VERTEX_INPUTS
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
	#ifdef TEXTURE_NORMAL
		out vec4 vInTangent;
	#endif
	out vec2 vInTexCoord;
#endif
#ifdef SHARED_SPB
	flat out int vSPBIndex;
#endif
#ifdef NODE_VERTEX_INPUTS
NODE_VERTEX_INPUTS
#endif



// Main Function
//////////////////

void main( void ){
	#include "v130/shared/defren/skin/shared_spb_index2.glsl"
	
	vInPosition = inPosition;
	vInNormal = inNormal;
	#ifdef HEIGHT_MAP
		vInHeight = inHeight;
	#else
		#ifdef TEXTURE_NORMAL
			vInTangent = inTangent;
		#endif
		vInTexCoord = inTexCoord;
	#endif
	
	#ifdef SHARED_SPB
	vSPBIndex = spbIndex;
	#endif
	
	#ifdef NODE_VERTEX_MAIN
	NODE_VERTEX_MAIN
	#endif
}
