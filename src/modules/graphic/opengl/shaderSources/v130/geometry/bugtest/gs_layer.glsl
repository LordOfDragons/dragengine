precision highp float;
precision highp int;

// version with max vertices
layout( triangles ) in;
layout( triangle_strip, max_vertices=18 ) out;

out vec3 vColor;

void main( void ){
	int i;
	
	gl_PrimitiveID = gl_PrimitiveIDIn;
	
	for( i=0; i<6; i++ ){
		vColor = vec3( float( 40 + 40 * i ) / 255.0 );
		gl_Layer = i;
		
		gl_Position = gl_in[ 0 ].gl_Position;
		EmitVertex();
		
		gl_Position = gl_in[ 1 ].gl_Position;
		EmitVertex();
		
		gl_Position = gl_in[ 2 ].gl_Position;
		EmitVertex();
		
		EndPrimitive();
	}
}

/*
// version with invocation
#extension GL_ARB_gpu_shader5 : require

precision highp float;
precision highp int;

layout( triangles, invocations=6 ) in;
layout( triangle_strip, max_vertices=3 ) out;

out vec3 vColor;

void main( void ){
	gl_PrimitiveID = gl_PrimitiveIDIn;
	vColor = vec3( float( 40 + 40 * gl_InvocationID ) / 255.0 );
	
	gl_Position = gl_in[ 0 ].gl_Position;
	gl_Layer = gl_InvocationID;
	EmitVertex();
	
	gl_Position = gl_in[ 1 ].gl_Position;
	gl_Layer = gl_InvocationID;
	EmitVertex();
	
	gl_Position = gl_in[ 2 ].gl_Position;
	gl_Layer = gl_InvocationID;
	EmitVertex();
	
	EndPrimitive();
}
*/
