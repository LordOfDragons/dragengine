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

void main( void ){
	// this is the same for all points
	vParticleLightPosition = vec3( gl_in[ 0 ].gl_Position );
	vParticleLightColor = vGSParticleLightColor[ 0 ];
	vParticleLightRange = vGSParticleLightRange[ 0 ];
	
	gl_PrimitiveID = gl_PrimitiveIDIn;
	gl_Layer = 0;
	
	// generate billboard
	vLightVolumePos = vParticleLightPosition;
	vLightVolumePos.xy -= vec2( vGSParticleLightRange[ 0 ] );
	gl_Position = pMatrixMVP * vec4( vLightVolumePos, 1.0 );
	EmitVertex();
	
	vLightVolumePos.y = vParticleLightPosition.y + vGSParticleLightRange[ 0 ];
	gl_Position = pMatrixMVP * vec4( vLightVolumePos, 1.0 );
	EmitVertex();
	
	vLightVolumePos.x = vParticleLightPosition.x + vGSParticleLightRange[ 0 ];
	vLightVolumePos.y = vParticleLightPosition.y - vGSParticleLightRange[ 0 ];
	gl_Position = pMatrixMVP * vec4( vLightVolumePos, 1.0 );
	EmitVertex();
	
	vLightVolumePos.y = vParticleLightPosition.y + vGSParticleLightRange[ 0 ];
	gl_Position = pMatrixMVP * vec4( vLightVolumePos, 1.0 );
	EmitVertex();
	
	EndPrimitive();
}
