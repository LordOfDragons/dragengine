#include "shared/preamble.glsl"

#ifdef GS_INSTANCING
	layout( triangles, invocations=6 ) in;
	layout( triangle_strip, max_vertices=3 ) out;
#else
	layout( triangles ) in;
	layout( triangle_strip, max_vertices=18 ) out;
#endif

flat out int vLayer;

void main( void ){
	int face;
	
	#ifdef GS_INSTANCING
	face = gl_InvocationID;
	#else
	for( face=0; face<6; face++ ){
	#endif
		
		int i;
		for( i=0; i<3; i++ ){
			gl_Position = gl_in[ i ].gl_Position;
			
			vLayer = face;
			
			gl_Layer = face;
			gl_PrimitiveID = gl_PrimitiveIDIn;
			EmitVertex();
		}
		EndPrimitive();
		
	#ifndef GS_INSTANCING
	}
	#endif
}
