#include "shared/preamble.glsl"

#if LAYERED_RENDERING == LAYERED_RENDERING_STEREO
	#ifdef GS_INSTANCING
		layout( triangles, invocations=2 ) in;
		layout( triangle_strip, max_vertices=3 ) out;
	#else
		layout( triangles ) in;
		layout( triangle_strip, max_vertices=6 ) out;
	#endif
#endif

in vec4 vGSScreenCoord[ 3 ];

out vec4 vScreenCoord;
flat out int vLayer;

void main( void ){
	int eye;
	#ifdef GS_INSTANCING
	eye = gl_InvocationID;
	#else
	for( eye=0; eye<2; eye++ ){
	#endif
		
		int corner;
		for( corner=0; corner<3; corner++ ){
			gl_Position = gl_in[ corner ].gl_Position;
			
			vScreenCoord = vGSScreenCoord[ corner ];
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
