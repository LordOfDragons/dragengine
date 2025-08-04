#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

UNIFORM_BIND(0) uniform mat4x3 pMatrixModel; // matrix model-view-projection
#ifdef WITH_SELECTOR
	UNIFORM_BIND(1) uniform mat4x3 pMatrixModel2;
#endif

layout(location=0) in vec3 inPosition;
#ifdef WITH_SELECTOR
	layout(location=1) in float inSelector;
#endif

// VSRenderStereo
UNIFORM_BIND(2) uniform int pDrawIDOffset;

#include "shared/interface/2d_vertex.glsl"

void main( void ){
	vLayer = 0;
	if(VSRenderStereo){
		#ifdef SUPPORTS_VSDRAWPARAM
		vLayer = gl_DrawID + pDrawIDOffset;
		#endif
	}
	
	vec3 position;
	#ifdef WITH_SELECTOR
		if( bool( inSelector ) ){
			position = pMatrixModel2 * vec4( inPosition, 1 );
		}else{
			position = pMatrixModel * vec4( inPosition, 1 );
		}
	#else
		position = pMatrixModel * vec4( inPosition, 1 );
	#endif
	
	#ifdef GS_RENDER_STEREO
		gl_Position = vec4( position, 1 );
		
	#else
		gl_Position = pMatrixVP[vLayer] * vec4( position, 1 );
		gl_Position.z = gl_Position.z * pDebugDepthScale + pDebugDepthOffset;
	#endif
	
	#ifdef SUPPORTS_VSLAYER
	if(VSRenderStereo){
		gl_Layer = vLayer;
	}
	#endif
}
