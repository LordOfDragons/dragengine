//#extension GL_ARB_tessellation_shader : enable

// request high precision if the graphic card supports this
#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

layout( vertices=3 ) out;



// Inputs
///////////

in vec2 vTCSTCColor[];
#ifdef TEXTURE_NORMAL
	in vec2 vTCSTCNormal[];
#endif
#ifdef TEXTURE_REFLECTIVITY
	in vec2 vTCSTCReflectivity[];
#endif
#ifdef TEXTURE_EMISSIVITY
	in vec2 vTCSTCEmissivity[];
#endif
#ifdef TEXTURE_REFRACTION_DISTORT
	in vec2 vTCSTCRefractionDistort[];
#endif
#ifdef TEXTURE_AO
	in vec2 vTCSTCAO[];
#endif

in vec3 vTCSNormal[];
#ifdef TEXTURE_NORMAL
	in vec3 vTCSTangent[];
	in vec3 vTCSBitangent[];
#endif

#ifdef TEXTURE_ENVMAP
	in vec3 vTCSReflectDir[];
#endif

#ifdef HEIGHT_MAP
	in float vTCSHTMask[];
#endif

in float vTCSRenderCondition[];



// Outputs
////////////

out vec2 vTESTCColor[];
#ifdef TEXTURE_NORMAL
	out vec2 vTESTCNormal[];
#endif
#ifdef TEXTURE_REFLECTIVITY
	out vec2 vTESTCReflectivity[];
#endif
#ifdef TEXTURE_EMISSIVITY
	out vec2 vTESTCEmissivity[];
#endif
#ifdef TEXTURE_REFRACTION_DISTORT
	out vec2 vTESTCRefractionDistort[];
#endif
#ifdef TEXTURE_AO
	out vec2 vTESTCAO[];
#endif

out vec3 vTESNormal[];
#ifdef TEXTURE_NORMAL
	out vec3 vTESTangent[];
	out vec3 vTESBitangent[];
#endif

#ifdef TEXTURE_ENVMAP
	out vec3 vTESReflectDir[];
#endif

#ifdef HEIGHT_MAP
	out float vTESHTMask[];
#endif

#ifdef PROP_FIELD
	out float vTESRenderCondition[];
#endif




// Main Function
//////////////////

void main(void){
	if( gl_InvocationID == 0 ){
		vec3 edgeLen;
		edgeLen.x = length( gl_in[ 0 ].gl_Position.xyz - gl_in[ 1 ].gl_Position.xyz );
		edgeLen.y = length( gl_in[ 1 ].gl_Position.xyz - gl_in[ 2 ].gl_Position.xyz );
		edgeLen.z = length( gl_in[ 2 ].gl_Position.xyz - gl_in[ 0 ].gl_Position.xyz );
		
		vec3 splits = clamp( floor( edgeLen / vec3( 0.01 ) ), vec3( 1.0 ), vec3( 32.0 ) );
		
		gl_TessLevelOuter[ 0 ] = splits.x;
		gl_TessLevelOuter[ 1 ] = splits.y;
		gl_TessLevelOuter[ 2 ] = splits.z;
		
		gl_TessLevelInner[ 0 ] = floor( dot( splits, vec3( 1.0 / 3.0 ) ) );
	}
	
	gl_out[ gl_InvocationID ].gl_Position = gl_in[ gl_InvocationID ].gl_Position;
	
	vTESTCColor[ gl_InvocationID ] = vTCSTCColor[ gl_InvocationID ];
	#ifdef TEXTURE_NORMAL
		vTESTCNormal[ gl_InvocationID ] = vTCSTCNormal[ gl_InvocationID ];
	#endif
	#ifdef TEXTURE_REFLECTIVITY
		vTESTCReflectivity[ gl_InvocationID ] = vTCSTCReflectivity[ gl_InvocationID ];
	#endif
	#ifdef TEXTURE_EMISSIVITY
		vTESTCEmissivity[ gl_InvocationID ] = vTCSTCEmissivity[ gl_InvocationID ];
	#endif
	#ifdef TEXTURE_REFRACTION_DISTORT
		vTESTCRefractionDistort[ gl_InvocationID ] = vTCSTCRefractionDistort[ gl_InvocationID ];
	#endif
	#ifdef TEXTURE_AO
		vTESTCAO[ gl_InvocationID ] = vTCSTCAO[ gl_InvocationID ];
	#endif

	vTESNormal[ gl_InvocationID ] = vTCSNormal[ gl_InvocationID ];
	#ifdef TEXTURE_NORMAL
		vTESTangent[ gl_InvocationID ] = vTCSTangent[ gl_InvocationID ];
		vTESBitangent[ gl_InvocationID ] = vTCSBitangent[ gl_InvocationID ];
	#endif

	#ifdef TEXTURE_ENVMAP
		vTESReflectDir[ gl_InvocationID ] = vTCSReflectDir[ gl_InvocationID ];
	#endif

	#ifdef HEIGHT_MAP
		vTESHTMask[ gl_InvocationID ] = vTCSHTMask[ gl_InvocationID ];
	#endif

	#ifdef PROP_FIELD
		vTESRenderCondition[ gl_InvocationID ] = vTCSRenderCondition[ gl_InvocationID ];
	#endif
}
