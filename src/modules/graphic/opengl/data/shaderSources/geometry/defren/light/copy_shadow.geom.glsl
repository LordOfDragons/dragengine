#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

// special situation: can not use interface/2d_vertex.glsl
VARYING_BIND(0) in vec3 vGSTexCoord[3];
VARYING_BIND(1) flat in int vGSLayer[3];

VARYING_BIND(0) out vec3 vTexCoord;

void main(void){
	int i;
	for(i=0; i<3; i++){
		gl_Position = gl_in[i].gl_Position;
		vTexCoord = vGSTexCoord[i];
		gl_Layer = vGSLayer[i];
		gl_PrimitiveID = gl_PrimitiveIDIn;
		EmitVertex();
	}
	EndPrimitive();
}
