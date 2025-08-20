#include "shared/preamble.glsl"

layout( triangles ) in;
layout( triangle_strip, max_vertices=3 ) out;

#include "shared/interface/2d/geometry.glsl"

void main( void ){
	int i;
	for( i=0; i<3; i++ ){
		gl_Position = gl_in[ i ].gl_Position;
		geometryShaderDefaultOutputs(i);
		EmitVertex();
	}
	EndPrimitive();
}
