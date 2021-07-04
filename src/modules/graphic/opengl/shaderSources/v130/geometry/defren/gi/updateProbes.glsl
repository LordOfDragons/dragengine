layout( triangles ) in;
layout( triangle_strip, max_vertices=3 ) out;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"

in ivec2 vGSRayOffset[ 3 ];
in ivec2 vGSOffset[ 3 ];
in float vGSBlendFactor[ 3 ];
in vec3 vGSProbePosition[ 3 ];

flat out ivec2 vRayOffset;
flat out ivec2 vOffset;
flat out float vBlendFactor;
flat out vec3 vProbePosition;

void main( void ){
	int i;
	for( i=0; i<3; i++ ){
		gl_Position = gl_in[ i ].gl_Position;
		gl_Layer = pGICascade;
		gl_PrimitiveID = gl_PrimitiveIDIn;
		vRayOffset = vGSRayOffset[ 0 ];
		vOffset = vGSOffset[ 0 ];
		vBlendFactor = vGSBlendFactor[ 0 ];
		vProbePosition = vGSProbePosition[ 0 ];
		EmitVertex();
	}
	EndPrimitive();
}
