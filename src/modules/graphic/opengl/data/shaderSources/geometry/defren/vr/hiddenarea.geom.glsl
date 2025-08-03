#include "shared/preamble.glsl"

layout( triangles ) in;
layout( triangle_strip, max_vertices=3 ) out;

flat out int vLayer;

void main( void ){
	int i;
	for( i=0; i<3; i++ ){
		gl_Position = gl_in[ i ].gl_Position;
		vLayer = RENDER_TO_LAYER;
		gl_Layer = RENDER_TO_LAYER;
		gl_PrimitiveID = gl_PrimitiveIDIn;
		EmitVertex();
	}
	EndPrimitive();
}
