#include "v130/shared/defren/skin/macros_geometry.glsl"

// layout definitions
layout( triangles ) in;
layout( triangle_strip, max_vertices=3 ) out;



// Inputs
///////////

in vec2 vGSTCColor[ 3 ];
#ifdef TEXTURE_COLOR_TINT_MASK
	in vec2 vGSTCColorTintMask[ 3 ];
#endif
#ifdef TEXTURE_NORMAL
	in vec2 vGSTCNormal[ 3 ];
#endif
#ifdef WITH_REFLECTIVITY
	in vec2 vGSTCReflectivity[ 3 ];
#endif
#ifdef WITH_EMISSIVITY
	in vec2 vGSTCEmissivity[ 3 ];
#endif
#ifdef TEXTURE_REFRACTION_DISTORT
	in vec2 vGSTCRefractionDistort[ 3 ];
#endif
#ifdef TEXTURE_AO
	in vec2 vGSTCAO[ 3 ];
#endif

in vec3 vGSNormal[ 3 ];
#ifdef WITH_TANGENT
	in vec3 vGSTangent[ 3 ];
#endif
#ifdef WITH_BITANGENT
	in vec3 vGSBitangent[ 3 ];
#endif
#ifdef WITH_REFLECT_DIR
	in vec3 vGSReflectDir[ 3 ];
#endif
#ifdef HEIGHT_MAP
	in float vGSHTMask[ 3 ];
#endif
#ifdef FADEOUT_RANGE
	in float vGSFadeZ[ 3 ];
#endif

in float vGSRenderCondition[ 3 ];

#ifdef SHARED_SPB
	flat in int vGSSPBIndex[ 3 ];
	#define spbIndex vGSSPBIndex[0]
	
	#ifdef GS_RENDER_CUBE
		flat in int vGSSPBFlags[ 3 ];
		#define spbFlags vGSSPBFlags[0]
	#endif
	
	#include "v130/shared/defren/skin/shared_spb_redirect.glsl"
#endif



// Outputs
////////////

in vec2 vTCColor;
#ifdef TEXTURE_COLOR_TINT_MASK
	in vec2 vTCColorTintMask;
#endif
#ifdef TEXTURE_NORMAL
	in vec2 vTCNormal;
#endif
#ifdef WITH_REFLECTIVITY
	in vec2 vTCReflectivity;
#endif
#ifdef WITH_EMISSIVITY
	in vec2 vTCEmissivity;
#endif
#ifdef TEXTURE_REFRACTION_DISTORT
	in vec2 vTCRefractionDistort;
#endif
#ifdef TEXTURE_AO
	in vec2 vTCAO;
#endif

out vec3 vNormal;
#ifdef WITH_TANGENT
	out vec3 vTangent;
#endif
#ifdef WITH_BITANGENT
	out vec3 vBitangent;
#endif
#ifdef WITH_REFLECT_DIR
	out vec3 vReflectDir;
#endif
#ifdef HEIGHT_MAP
	out float vHTMask;
#endif
#ifdef FADEOUT_RANGE
	out float vFadeZ;
#endif

#ifdef SHARED_SPB
	flat out int vSPBIndex;
#endif



// Main Function
//////////////////

void main( void ){
	if( vGSRenderCondition[ 0 ] >= 5.0 ){
		return;
	}
	
	int i;
	
	// pass through all parameters
	gl_PrimitiveID = gl_PrimitiveIDIn;
	gl_Layer = 0;
	
	#ifdef SHARED_SPB
	vSPBIndex = spbIndex;
	#endif
	
	for( i=0; i<3; i++ ){
		gl_Position = gl_in[ i ].gl_Position;
		
		vTCColor = vGSTCColor[ i ];
		#ifdef TEXTURE_COLOR_TINT_MASK
			vTCColorTintMask = vGSTCColorTintMask[ i ];
		#endif
		#ifdef TEXTURE_NORMAL
			vTCNormal = vGSTCNormal[ i ];
		#endif
		#ifdef WITH_REFLECTIVITY
			vTCReflectivity = vGSTCReflectivity[ i ];
		#endif
		#ifdef WITH_EMISSIVITY
			vTCEmissivity = vGSTCEmissivity[ i ];
		#endif
		#ifdef TEXTURE_REFRACTION_DISTORT
			vTCRefractionDistort = vGSTCRefractionDistort[ i ];
		#endif
		#ifdef TEXTURE_AO
			vTCAO = vGSTCAO[ i ];
		#endif
		
		vNormal = vGSNormal[ i ];
		#ifdef WITH_TANGENT
			vTangent = vGSTangent[ i ];
		#endif
		#ifdef WITH_BITANGENT
			vBitangent = vGSBitangent[ i ];
		#endif
		#ifdef WITH_REFLECT_DIR
			vReflectDir = vGSReflectDir[ i ];
		#endif
		#ifdef HEIGHT_MAP
			vHTMask = vGSHTMask[ i ];
		#endif
		#ifdef FADEOUT_RANGE
			vFadeZ = vGSFadeZ[ i ];
		#endif
		
		EmitVertex();
	}
	
	EndPrimitive();
}
