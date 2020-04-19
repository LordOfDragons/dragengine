// request high precision if the graphic card supports this
#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

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
#if defined TEXTURE_REFLECTIVITY || defined TEXTURE_ROUGHNESS
	in vec2 vGSTCReflectivity[ 3 ];
#endif
#ifdef TEXTURE_EMISSIVITY
	in vec2 vGSTCEmissivity[ 3 ];
#endif
#ifdef TEXTURE_REFRACTION_DISTORT
	in vec2 vGSTCRefractionDistort[ 3 ];
#endif
#ifdef TEXTURE_AO
	in vec2 vGSTCAO[ 3 ];
#endif

in vec3 vGSNormal[ 3 ];
#ifdef TEXTURE_NORMAL
	in vec3 vGSTangent[ 3 ];
	in vec3 vGSBitangent[ 3 ];
#endif
#ifdef TEXTURE_ENVMAP
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
#if defined TEXTURE_REFLECTIVITY || defined TEXTURE_ROUGHNESS
	in vec2 vTCReflectivity;
#endif
#ifdef TEXTURE_EMISSIVITY
	in vec2 vTCEmissivity;
#endif
#ifdef TEXTURE_REFRACTION_DISTORT
	in vec2 vTCRefractionDistort;
#endif
#ifdef TEXTURE_AO
	in vec2 vTCAO;
#endif

out vec3 vNormal;
#ifdef TEXTURE_NORMAL
	out vec3 vTangent;
	out vec3 vBitangent;
#endif
#ifdef TEXTURE_ENVMAP
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
		#if defined TEXTURE_REFLECTIVITY || defined TEXTURE_ROUGHNESS
			vTCReflectivity = vGSTCReflectivity[ i ];
		#endif
		#ifdef TEXTURE_EMISSIVITY
			vTCEmissivity = vGSTCEmissivity[ i ];
		#endif
		#ifdef TEXTURE_REFRACTION_DISTORT
			vTCRefractionDistort = vGSTCRefractionDistort[ i ];
		#endif
		#ifdef TEXTURE_AO
			vTCAO = vGSTCAO[ i ];
		#endif
		
		vNormal = vGSNormal[ i ];
		#ifdef TEXTURE_NORMAL
			vTangent = vGSTangent[ i ];
			vBitangent = vGSBitangent[ i ];
		#endif
		#ifdef TEXTURE_ENVMAP
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
