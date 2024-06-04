precision highp float;
precision highp int;

// version with max vertices
layout( triangles ) in;
layout( triangle_strip, max_vertices=18 ) out;

out vec3 vColor;


void emitCorner( in int layer, in vec4 position, in vec3 color ){
	vColor = color;
	gl_Position = position;
	gl_Layer = layer;
	gl_PrimitiveID = gl_PrimitiveIDIn;
	EmitVertex();
}

void main( void ){
	int i;
	
	// according to opengl website gl_Layer has to be set for each emit vertex
	// or else it becomes undefined. and since opengl requires all vertices to
	// have the same layer this can cause problems
	
	for( i=0; i<6; i++ ){
		vec3 color = vec3( float( 40 + 40 * i ) / 255.0 );
		emitCorner( i, gl_in[ 0 ].gl_Position, color );
		emitCorner( i, gl_in[ 1 ].gl_Position, color );
		emitCorner( i, gl_in[ 2 ].gl_Position, color );
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
	vec3 color = vec3( float( 40 + 40 * gl_InvocationID ) / 255.0 );
	emitCorner( gl_InvocationID, gl_in[ 0 ].gl_Position, color );
	emitCorner( gl_InvocationID, gl_in[ 1 ].gl_Position, color );
	emitCorner( gl_InvocationID, gl_in[ 2 ].gl_Position, color );
	EndPrimitive();
}
*/
