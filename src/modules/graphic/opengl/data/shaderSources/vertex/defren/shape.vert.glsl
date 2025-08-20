#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

UNIFORM_BIND(0) uniform mat4x3 pMatrixModel; // matrix model-view-projection

// WithSelector
UNIFORM_BIND(1) uniform mat4x3 pMatrixModel2;

layout(location=0) in vec3 inPosition;

// WithSelector
layout(location=1) in float inSelector;

// VSRenderLayer
UNIFORM_BIND(2) uniform int pDrawIDOffset;

#include "shared/interface/2d/vertex.glsl"

void main( void ){
	vLayer = 0;
	if(VSRenderLayer){
		#ifdef SUPPORTS_VSDRAWPARAM
		vLayer = gl_DrawID + pDrawIDOffset;
		#endif
	}
	
	vec3 position;
	if(WithSelector){
		if( bool( inSelector ) ){
			position = pMatrixModel2 * vec4( inPosition, 1 );
			
		}else{
			position = pMatrixModel * vec4( inPosition, 1 );
		}
		
	}else{
		position = pMatrixModel * vec4( inPosition, 1 );
	}
	
	if(LayeredRendering != LayeredRenderingNone && !VSRenderLayer){
		gl_Position = vec4(position, 1.0);
		
	}else{
		gl_Position = pMatrixVP[vLayer] * vec4(position, 1.0);
		gl_Position.z = gl_Position.z * pDebugDepthScale + pDebugDepthOffset;
	}
	
	#ifdef SUPPORTS_VSLAYER
	if(VSRenderLayer){
		gl_Layer = vLayer;
	}
	#endif
}
