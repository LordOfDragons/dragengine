#include "shared/preamble.glsl"

layout(location=0) in vec3 inPosition;

#include "shared/interface/2d/vertex.glsl"

void main( void ){
	vertexShaderDefaultOutputs();
	
	gl_Position = vec4( inPosition.xy * vec2( 2 ) - vec2( 1 ), 1, 1 );
	
	if(VSRenderLayer){
		#ifdef SUPPORTS_VSDRAWPARAM
		vLayer = gl_DrawID;
		#ifdef SUPPORTS_VSLAYER
		gl_Layer = vLayer;
		#endif
		#endif
	}
}
