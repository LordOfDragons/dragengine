//#extension GL_ARB_tessellation_shader : enable
#include "shared/defren/skin/macros_geometry.glsl"

//layout( quads, equal_spacing, cw ) in;
layout( triangles, equal_spacing, ccw ) in;



// Uniform Parameters
///////////////////////

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/defren/skin/ubo_texture_parameters.glsl"
#include "shared/defren/skin/ubo_instance_parameters.glsl"



// Samplers
/////////////

#ifdef TEXTURE_HEIGHT
	uniform HIGHP sampler2D texHeight;
#endif



// Inputs
///////////

in vec2 vTESTCColor[];
#ifdef TEXTURE_NORMAL
	in vec2 vTESTCNormal[];
#endif
#ifdef TEXTURE_REFLECTIVITY
	in vec2 vTESTCReflectivity[];
#endif
#ifdef WITH_EMISSIVITY
	in vec2 vTESTCEmissivity[];
#endif
#ifdef TEXTURE_REFRACTION_DISTORT
	in vec2 vTESTCRefractionDistort[];
#endif
#ifdef TEXTURE_AO
	in vec2 vTESTCAO[];
#endif

in vec3 vTESNormal[];
#ifdef WITH_TANGENT
	in vec3 vTESTangent[];
#endif
#ifdef WITH_BITANGENT
	in vec3 vTESBitangent[];
#endif

#ifdef HEIGHT_MAP
	in float vTESHTMask[];
#endif

/*
#ifdef PROP_FIELD
	in float vTESRenderCondition[];
#endif
*/


// Outputs
////////////

#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED || defined GS_RENDER_STEREO
	#define PASS_ON_NEXT_STAGE 1
	out vec2 vGSTCColor;
	#define vTCColor vGSTCColor
	#ifdef TEXTURE_NORMAL
		out vec2 vGSTCNormal;
		#define vTCNormal vGSTCNormal
	#endif
	#ifdef TEXTURE_REFLECTIVITY
		out vec2 vGSTCReflectivity;
		#define vTCReflectivity vGSTCReflectivity
	#endif
	#ifdef WITH_EMISSIVITY
		out vec2 vGSTCEmissivity;
		#define vTCEmissivity vGSTCEmissivity
	#endif
	#ifdef TEXTURE_REFRACTION_DISTORT
		out vec2 vGSTCRefractionDistort;
		#define vTCRefractionDistort vGSTCRefractionDistort
	#endif
	#ifdef TEXTURE_AO
		out vec2 vGSTCAO;
		#define vTCAO vGSTCAO
	#endif
	
	out vec3 vGSNormal;
	#define vNormal vGSNormal
	#ifdef WITH_TANGENT
		out vec3 vGSTangent;
		#define vTangent vGSTangent
	#endif
	#ifdef WITH_BITANGENT
		out vec3 vGSBitangent;
		#define vBitangent vGSBitangent
	#endif
	
	#ifdef HEIGHT_MAP
		out float vGSHTMask;
		#define vHTMask vGSHTMask
	#endif
	
	/*
	#ifdef PROP_FIELD
		out float vGSRenderCondition;
	#endif
	*/
	
#else
	out vec2 vTCColor;
	#ifdef TEXTURE_NORMAL
		out vec2 vTCNormal;
	#endif
	#ifdef TEXTURE_REFLECTIVITY
		out vec2 vTCReflectivity;
	#endif
	#ifdef WITH_EMISSIVITY
		out vec2 vTCEmissivity;
	#endif
	#ifdef TEXTURE_REFRACTION_DISTORT
		out vec2 vTCRefractionDistort;
	#endif
	#ifdef TEXTURE_AO
		out vec2 vTCAO;
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
#endif



// Main Function
//////////////////

#include "shared/tessellation.glsl"
#include "shared/defren/sanitize_position.glsl"

void main(){
	// tessellate varying parameters
	TESS_VAR_LINEAR_TRI( vTCColor, vTESTCColor );
	#ifdef TEXTURE_NORMAL
		TESS_VAR_LINEAR_TRI( vTCNormal, vTESTCNormal );
	#endif
	#ifdef TEXTURE_REFLECTIVITY
		TESS_VAR_LINEAR_TRI( vTCReflectivity, vTESTCReflectivity );
	#endif
	#ifdef WITH_EMISSIVITY
		TESS_VAR_LINEAR_TRI( vTCEmissivity, vTESTCEmissivity );
	#endif
	#ifdef TEXTURE_REFRACTION_DISTORT
		TESS_VAR_LINEAR_TRI( vTCRefractionDistort, vTESTCRefractionDistort );
	#endif
	#ifdef TEXTURE_AO
		TESS_VAR_LINEAR_TRI( vTCAO, vTESTCAO );
	#endif
	
	TESS_VAR_LINEAR_TRI( vNormal, vTESNormal );
	#ifdef WITH_TANGENT
		TESS_VAR_LINEAR_TRI( vTangent, vTESTangent );
	#endif
	#ifdef WITH_BITANGENT
		TESS_VAR_LINEAR_TRI( vBitangent, vTESBitangent );
	#endif
	
	#ifdef HEIGHT_MAP
		TESS_VAR_LINEAR_TRI( vHTMask, vTESHTMask );
	#endif
	
	/*
	#ifdef PROP_FIELD
		TESS_VAR_LINEAR_TRI( vRenderCondition, vTESRenderCondition );
	#endif
	*/
	
	// tessellate position
	tessLinearTri( gl_Position, gl_in[ 0 ].gl_Position, gl_in[ 1 ].gl_Position, gl_in[ 2 ].gl_Position );
	
	#ifdef TEXTURE_HEIGHT
		vec3 displacement = normalize( vNormal ) * vec3( texture( texHeight, vTCColor ).r * pHeightRemap.x + pHeightRemap.y );
		gl_Position.xyz += displacement;
	#endif
	
	#ifndef PASS_ON_NEXT_STAGE
		// normalize normals. this has to be done after position adjustment since otherwise the coordinate system is wrong
		vNormal = normalize( vNormal * pMatrixVn[ 0 ] );
		#ifdef WITH_TANGENT
			vTangent = normalize( vTangent * pMatrixVn[ 0 ] );
		#endif
		#ifdef WITH_BITANGENT
			vBitangent = normalize( vBitangent * pMatrixVn[ 0 ] );
		#endif
		
		#ifndef BILLBOARD
			#ifdef WITH_REFLECT_DIR
				#ifdef BILLBOARD
					vReflectDir = gl_Position.xyz;
				#else
					vReflectDir = pMatrixV[ 0 ] * gl_Position;
				#endif
			#endif
			
			#ifdef FADEOUT_RANGE
				#ifdef BILLBOARD
					vFadeZ = gl_Position.z;
				#else
					vFadeZ = ( pMatrixV[ 0 ] * gl_Position ).z;
				#endif
			#endif
		#endif
		
		#ifdef BILLBOARD
			gl_Position = pMatrixP[ 0 ] * gl_Position;
		#else
			gl_Position = pMatrixVP[ 0 ] * gl_Position;
		#endif
	#endif
}
