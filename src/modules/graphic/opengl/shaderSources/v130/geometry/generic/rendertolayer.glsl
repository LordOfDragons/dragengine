uniform int pLayer;

layout( triangles ) in;
layout( triangle_strip, max_vertices=3 ) out;

in vec2 vGSTexCoord[ 3 ];

out vec2 vTexCoord;

void main( void ){
	gl_PrimitiveID = gl_PrimitiveIDIn;
	gl_Layer = pLayer;
	
	gl_Position = gl_in[ 0 ].gl_Position;
	vTexCoord = vGSTexCoord[ 0 ];
	EmitVertex();
	
	gl_Position = gl_in[ 1 ].gl_Position;
	vTexCoord = vGSTexCoord[ 1 ];
	EmitVertex();
	
	gl_Position = gl_in[ 2 ].gl_Position;
	vTexCoord = vGSTexCoord[ 2 ];
	EmitVertex();
	
	EndPrimitive();
}
