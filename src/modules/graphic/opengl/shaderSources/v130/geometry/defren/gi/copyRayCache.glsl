layout( triangles ) in;
layout( triangle_strip, max_vertices=3 ) out;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"

in ivec2 vGSRayOffset[ 3 ];

flat out ivec2 vRayOffset;

void main( void ){
	int i;
	for( i=0; i<3; i++ ){
		gl_Position = gl_in[ i ].gl_Position;
		gl_Layer = pGICascade;
		gl_PrimitiveID = gl_PrimitiveIDIn;
		vRayOffset = vGSRayOffset[ 0 ];
		EmitVertex();
	}
	EndPrimitive();
}
