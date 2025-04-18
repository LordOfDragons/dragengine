precision HIGHP float;
precision HIGHP int;

// version with max vertices
layout( triangles ) in;
layout( triangle_strip, max_vertices=18 ) out;

out vec3 vColor;


void emitCorner( in int layer, in vec4 position, in vec3 color ){
	vColor = color;
	gl_Position = position;
	gl_Layer = layer;
	gl_PrimitiveID = gl_PrimitiveIDIn;
	//EmitVertex();
}

void main( void ){
	// NOTE: quest requires EmitVertex to be called in main()
	
	int i, j;
	
	// according to opengl website gl_Layer has to be set for each emit vertex
	// or else it becomes undefined. and since opengl requires all vertices to
	// have the same layer this can cause problems
	
	for( i=0; i<6; i++ ){
		vec3 color = vec3( float( 40 + 40 * i ) / 255.0 );
		for(j=0; j<3; j++){
			emitCorner(i, gl_in[j].gl_Position, color);
			EmitVertex();
		}
		EndPrimitive();
	}
}

/*
// version with invocation
#ifndef OPENGLES
	#extension GL_ARB_gpu_shader5 : require
#endif

precision HIGHP float;
precision HIGHP int;

layout( triangles, invocations=6 ) in;
layout( triangle_strip, max_vertices=3 ) out;

out vec3 vColor;

void main( void ){
	vec3 color = vec3( float( 40 + 40 * gl_InvocationID ) / 255.0 );
	int i;
	for(i=0; i<3; i++){
		emitCorner(gl_InvocationID, gl_in[i].gl_Position, color);
		#ifdef QUEST_BUG_EMITVERTEX
		EmitVertex();
		#endif
	}
	EndPrimitive();
}
*/
