//#extension GL_ARB_tessellation_shader : enable
#include "v130/shared/defren/skin/macros_geometry.glsl"

layout( vertices=3 ) out;



// Inputs
///////////

#ifdef REQUIRES_TEX_COLOR
	in vec2 vTCSTCColor[];
#endif
#ifdef HEIGHT_MAP
	in float vTCSHTMask[];
#endif
#ifdef REQUIRES_NORMAL
	in vec3 vTCSNormal[];
	#ifdef WITH_TANGENT
		in vec3 vTCSTangent[];
	#endif
	#ifdef WITH_BITANGENT
		in vec3 vTCSBitangent[];
	#endif
#endif



// Outputs
////////////

#ifdef REQUIRES_TEX_COLOR
	out vec2 vTESTCColor[];
#endif
#ifdef HEIGHT_MAP
	out float vTESHTMask[];
#endif
#ifdef REQUIRES_NORMAL
	out vec3 vTESNormal[];
	#ifdef WITH_TANGENT
		out vec3 vTESTangent[];
	#endif
	#ifdef WITH_BITANGENT
		out vec3 vTESBitangent[];
	#endif
#endif



void main(void){
	if( gl_InvocationID == 0 ){
		vec3 edgeLen;
		edgeLen.x = length( gl_in[ 0 ].gl_Position.xyz - gl_in[ 1 ].gl_Position.xyz );
		edgeLen.y = length( gl_in[ 1 ].gl_Position.xyz - gl_in[ 2 ].gl_Position.xyz );
		edgeLen.z = length( gl_in[ 2 ].gl_Position.xyz - gl_in[ 0 ].gl_Position.xyz );
		
		vec3 splits = clamp( floor( edgeLen / vec3( 0.01 ) ), vec3( 1 ), vec3( 32 ) );
		
		gl_TessLevelOuter[ 0 ] = splits.x;
		gl_TessLevelOuter[ 1 ] = splits.y;
		gl_TessLevelOuter[ 2 ] = splits.z;
		
		gl_TessLevelInner[ 0 ] = floor( dot( splits, vec3( 1.0 / 3.0 ) ) );
	}
	
	gl_out[ gl_InvocationID ].gl_Position = gl_in[ gl_InvocationID ].gl_Position;
	
	#ifdef REQUIRES_TEX_COLOR
		vTESTCColor[ gl_InvocationID ] = vTCSTCColor[ gl_InvocationID ];
	#endif
	#ifdef HEIGHT_MAP
		vTESHTMask[ gl_InvocationID ] = vTCCSHTMask[ gl_InvocationID ];
	#endif
	#ifdef REQUIRES_NORMAL
		vTESNormal[ gl_InvocationID ] = vTCSNormal[ gl_InvocationID ];
		#ifdef WITH_TANGENT
			vTESTangent[ gl_InvocationID ] = vTCSTangent[ gl_InvocationID ];
		#endif
		#ifdef WITH_BITANGENT
			vTESBitangent[ gl_InvocationID ] = vTCSBitangent[ gl_InvocationID ];
		#endif
	#endif
}
