#ifdef GS_INSTANCING
	#ifndef OPENGLES
		#extension GL_ARB_gpu_shader5 : require
	#endif
#endif

#ifdef GS_RENDER_STEREO
	#ifdef GS_INSTANCING
		layout( triangles, invocations=2 ) in;
		layout( triangle_strip, max_vertices=3 ) out;
	#else
		layout( triangles ) in;
		layout( triangle_strip, max_vertices=6 ) out;
	#endif
#else
	layout( triangles ) in;
	layout( triangle_strip, max_vertices=3 ) out;
#endif


uniform mat4 pMatrixVP;
uniform mat4 pMatrixVP2;

in vec2 vGSTexCoord[ 3 ];

out vec2 vTexCoord;


#ifdef GS_RENDER_STEREO

void main( void ){
	int eye;
	
	#ifdef GS_INSTANCING
	eye = gl_InvocationID;
	#else
	for( eye=0; eye<2; eye++ ){
	#endif
		
		int i;
		for( i=0; i<3; i++ ){
			gl_Position = ( eye == 0 ? pMatrixVP : pMatrixVP2 ) * gl_in[ i ].gl_Position;
			gl_Layer = eye;
			gl_PrimitiveID = gl_PrimitiveIDIn;
			
			vTexCoord = vGSTexCoord[ i ];
			
			EmitVertex();
		}
		EndPrimitive();
		
	#ifndef GS_INSTANCING
	}
	#endif
}

#else

void main( void ){
	int i;
	for( i=0; i<3; i++ ){
		gl_Position = gl_in[ i ].gl_Position;
		gl_Layer = 0;
		gl_PrimitiveID = gl_PrimitiveIDIn;
		
		vTexCoord = vGSTexCoord[ i ];
		
		EmitVertex();
	}
	
	EndPrimitive();
}

#endif // GS_RENDER_STEREO
