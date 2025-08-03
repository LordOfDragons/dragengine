#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#if defined GS_RENDER_STEREO
	#ifdef GS_INSTANCING
		layout(triangles, invocations=2) in;
		layout(triangle_strip, max_vertices=3) out;
	#else
		layout(triangles) in;
		layout(triangle_strip, max_vertices=6) out;
	#endif
#endif

#include "shared/ubo_defines.glsl"
#include "shared/defren/light/ubo_instance_parameters.glsl"

#ifdef FULLSCREENQUAD
	out vec2 vScreenCoord;
#else
	out vec3 vLightVolumePos;
#endif

flat out int vLayer;

void main(void){
	int eye;
	#ifdef GS_INSTANCING
	eye = gl_InvocationID;
	#else
	for(eye=0; eye<2; eye++){
	#endif
		
		int corner;
		for(corner=0; corner<3; corner++){
			vec4 position = gl_in[corner].gl_Position;
			
			#ifdef FULLSCREENQUAD
				gl_Position = position;
				vScreenCoord = position.xy;
			#else
				gl_Position = pMatrixMVP[eye] * position;
				vLightVolumePos = pMatrixMV[eye] * position;
			#endif
			
			vLayer = eye;
			
			gl_Layer = eye;
			gl_PrimitiveID = gl_PrimitiveIDIn;
			
			EmitVertex();
		}
		
		EndPrimitive();
		
	#ifndef GS_INSTANCING
	}
	#endif
}
