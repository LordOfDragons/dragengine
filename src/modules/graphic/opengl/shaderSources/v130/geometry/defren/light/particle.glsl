#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

layout( points ) in;
layout( triangle_strip, max_vertices=4 ) out;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/light/ubo_instance_parameters.glsl"

in vec3 vGSParticleLightColor[ 1 ];
in float vGSParticleLightRange[ 1 ];

out vec3 vLightVolumePos;
out vec3 vParticleLightPosition;
out vec3 vParticleLightColor;
out float vParticleLightRange;


void emitCorner( in vec3 position ){
	vParticleLightPosition = gl_in[ 0 ].gl_Position.xyz;
	vParticleLightColor = vGSParticleLightColor[ 0 ];
	vParticleLightRange = vGSParticleLightRange[ 0 ];
	
	vLightVolumePos = position;
	
	gl_Position = pMatrixMVP * vec4( position, 1.0 );
	
	gl_Layer = 0;
	gl_PrimitiveID = gl_PrimitiveIDIn;
	
	EmitVertex();
}

void main( void ){
	vec3 position = gl_in[ 0 ].gl_Position.xyz;
	vec3 range = vec3( vGSParticleLightRange[ 0 ], -vGSParticleLightRange[ 0 ], 0.0 );
	
	emitCorner( position + range.yyz ); // -range, -range
	emitCorner( position + range.yxz ); // -range, +range
	emitCorner( position + range.xyz ); // +range, -range
	emitCorner( position + range.xxz ); // +range, +range
	
	EndPrimitive();
}
