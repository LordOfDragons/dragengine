layout( points ) in;
layout( points, max_vertices=1 ) out;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"

in vec4 vGSOffset[ 1 ];

flat out vec4 fbOffset; // offset(xyz), flags(w)

void main( void ){
	gl_Position = gl_in[ 0 ].gl_Position;
	gl_Layer = pGICascade;
	gl_PrimitiveID = gl_PrimitiveIDIn;
	fbOffset = vGSOffset[ 0 ];
	EmitVertex();
	EndPrimitive();
}
