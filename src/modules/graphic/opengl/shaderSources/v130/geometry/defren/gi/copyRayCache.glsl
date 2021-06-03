layout( triangles ) in;
layout( triangle_strip, max_vertices=3 ) out;

uniform int pCascade;

void main( void ){
	int i;
	for( i=0; i<3; i++ ){
		gl_Position = gl_in[ i ].gl_Position;
		gl_Layer = pCascade;
		gl_PrimitiveID = gl_PrimitiveIDIn;
		EmitVertex();
	}
	EndPrimitive();
}
