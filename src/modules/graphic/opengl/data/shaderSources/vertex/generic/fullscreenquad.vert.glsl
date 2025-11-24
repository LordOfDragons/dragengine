#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

// DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED //
//                                                        //
//             use screenspace.glsl instead               //
//    using defines: FULLSCREENQUAD, NO_POSTRANSFORM      //
//                                                        //
// DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED //

// !NoTexCoord
UNIFORM_BIND(0) uniform vec4 pQuadParams; // scaleX, scaleY, offsetX, offsetY

layout(location=0) in vec2 inPosition;

// VSRenderLayer
layout(location=1) in int inLayer;

#include "shared/interface/2d/vertex.glsl"

void main( void ){
	vertexShaderDefaultOutputs();
	
	gl_Position = vec4( inPosition, 0, 1 );
	// !NoTexCoord
	vTexCoord = inPosition * pQuadParams.xy + pQuadParams.zw;
	
	if(VSRenderLayer){
		vLayer = inLayer;
		#if defined SUPPORTS_VSLAYER && ! defined OPENGLES
		gl_Layer = vLayer;
		#endif
	}
}
