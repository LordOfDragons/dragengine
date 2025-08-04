#include "shared/preamble.glsl"

layout( triangles ) in;
layout( triangle_strip, max_vertices=3 ) out;

#include "shared/ubo_defines.glsl"
#include "shared/defren/gi/ubo_gi.glsl"

in vec2 vGSTexCoord[ 3 ];

VARYING_BIND(0) out vec2 vTexCoord;

void main( void ){
	int i;
	for( i=0; i<3; i++ ){
		gl_Position = gl_in[ i ].gl_Position;
		vTexCoord = vGSTexCoord[ i ];
		gl_Layer = pGICascade;
		gl_PrimitiveID = gl_PrimitiveIDIn;
		EmitVertex();
	}
	
	EndPrimitive();
}
