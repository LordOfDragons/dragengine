#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#if LAYERED_RENDERING_STEREO
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

#include "shared/interface/light/geometry.glsl"

void main(void){
	int eye;
	#ifdef GS_INSTANCING
	eye = gl_InvocationID;
	#else
	for(eye=0; eye<2; eye++){
	#endif
		
		int corner;
		for(corner=0; corner<3; corner++){
			lightGeometryShaderDefaultOutputs(corner, eye);
			
			vec4 position = gl_in[corner].gl_Position;
			if(FullScreenQuad){
				gl_Position = position;
				vScreenCoord = position.xy;
				
			}else{
				gl_Position = pMatrixMVP[eye] * position;
				vLightVolumePos = pMatrixMV[eye] * position;
			}
			
			EmitVertex();
		}
		
		EndPrimitive();
		
	#ifndef GS_INSTANCING
	}
	#endif
}
