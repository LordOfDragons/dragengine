uniform int pLayer;

layout( triangles ) in;
layout( triangle_strip, max_vertices=3 ) out;

in vec2 vGSTexCoord[ 3 ];

out vec2 vTexCoord;

void main( void ){
	int i;
	for( i=0; i<3; i++ ){
		gl_Position = gl_in[ i ].gl_Position;
		vTexCoord = vGSTexCoord[ i ];
		gl_Layer = pLayer;
		gl_PrimitiveID = gl_PrimitiveIDIn;
		EmitVertex();
	}
	
	EndPrimitive();
}
