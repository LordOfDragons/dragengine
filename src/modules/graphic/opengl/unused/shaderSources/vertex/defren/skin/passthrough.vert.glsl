#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;



// Uniforms
/////////////

#include "shared/ubo_defines.glsl"

#ifdef SHARED_SPB
	#include "shared/defren/skin/shared_spb_index.glsl"
#endif
#include "shared/defren/skin/ubo_special_parameters.glsl"



// Inputs
///////////

// GeometryMode == GeometryModeHeightMap
layout(location=0) in vec2 inHMPosition;
layout(location=1) in float inHMHeight;
layout(location=2) in float inHMNormal;

// GeometryMode != GeometryModeHeightMap
layout(location=0) in vec3 inPosition;
layout(location=1) in vec3 inRealNormal;
layout(location=2) in vec3 inNormal;
layout(location=3) in vec4 inTangent;
layout(location=4) in vec2 inTexCoord;


// Outputs
////////////

#include "shared/interface/skin/vertex.glsl"


// Main Function
//////////////////

void main(void){
	#include "shared/defren/skin/shared_spb_index2.glsl"
	
	// if(GeometryMode == GeometryModeHeightMap){
		// vInHeightMap = vec4(inHMPosition, inHMHeight, inHMNormal);
		
	// }else{
		vInPosition = inPosition;
		vInNormal = inNormal;
		vInTangent = inTangent;
		vInTexCoord = inTexCoord;
	}
	
	vSPBIndex = spbIndex;
	vSPBFlags = spbFlags;
	vLayer = 0;
	
	if(VSRenderLayer){
		vLayer = getInLayer();
		#ifdef SUPPORTS_VSDRAWPARAM
		gl_Layer = vLayer;
		#endif
	}
}
