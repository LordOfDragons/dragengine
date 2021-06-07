layout( triangles ) in;
layout( triangle_strip, max_vertices=3 ) out;

uniform int pLayer;

in ivec2 vGSRayOffset[ 3 ];

flat out ivec2 vRayOffset;

void main( void ){
	int i;
	for( i=0; i<3; i++ ){
		gl_Position = gl_in[ i ].gl_Position;
		gl_Layer = pLayer;
		gl_PrimitiveID = gl_PrimitiveIDIn;
		vRayOffset = vGSRayOffset[ i ];
		EmitVertex();
	}
	EndPrimitive();
}
