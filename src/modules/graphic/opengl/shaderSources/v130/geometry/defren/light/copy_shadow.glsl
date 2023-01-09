precision highp float;
precision highp int;

layout( triangles ) in;
layout( triangle_strip, max_vertices=3 ) out;

in vec3 vGSTexCoord[ 3 ];
in int vGSLayer[ 3 ];

out vec3 vTexCoord;

void main( void ){
	int i;
	for( i=0; i<3; i++ ){
		gl_Position = gl_in[ i ].gl_Position;
		vTexCoord = vGSTexCoord[ i ];
		gl_Layer = vGSLayer[ i ];
		gl_PrimitiveID = gl_PrimitiveIDIn;
		EmitVertex();
	}
	EndPrimitive();
}
