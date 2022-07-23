#ifdef GS_INSTANCING
	#extension GL_ARB_gpu_shader5 : require
#endif

#if defined GS_RENDER_STEREO
	#ifdef GS_INSTANCING
		layout( triangles, invocations=2 ) in;
		layout( triangle_strip, max_vertices=3 ) out;
	#else
		layout( triangles ) in;
		layout( triangle_strip, max_vertices=6 ) out;
	#endif
#endif

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
			
			vLayer = corner;
			
			gl_Layer = corner;
			gl_PrimitiveID = gl_PrimitiveIDIn;
			
			EmitVertex();
		}
		
		EndPrimitive();
		
	#ifndef GS_INSTANCING
	}
	#endif
}
