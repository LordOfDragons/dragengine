//#extension GL_ARB_tessellation_shader : enable
#include "v130/shared/defren/skin/macros_geometry.glsl"

layout( triangles, equal_spacing, ccw ) in;

// Uniform Parameters
///////////////////////

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/ubo_render_parameters.glsl"
#include "v130/shared/defren/skin/ubo_texture_parameters.glsl"
#include "v130/shared/defren/skin/ubo_instance_parameters.glsl"
#include "v130/shared/defren/skin/ubo_dynamic_parameters.glsl"



// Samplers
/////////////

#ifdef TEXTURE_HEIGHT
	uniform mediump sampler2D texHeight;
#endif



// Inputs
///////////

#ifdef REQUIRES_TEX_COLOR
	in vec2 vTESTCColor[];
#endif
#ifdef HEIGHT_MAP
	in float vTESHTMask[];
#endif
#ifdef REQUIRES_NORMAL
	in vec3 vTESNormal[];
	#ifdef WITH_TANGENT
		in vec3 vTESTangent[];
	#endif
	#ifdef WITH_BITANGENT
		in vec3 vTESBitangent[];
	#endif
#endif



// Outputs
////////////

#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED || defined GS_RENDER_STEREO
	#define PASS_ON_NEXT_STAGE 1
	#ifdef REQUIRES_TEX_COLOR
		out vec2 vGSTCColor;
		#define vTCColor vGSTCColor
	#endif
	#ifdef HEIGHT_MAP
		out float vGSHTMask;
		#define vHTMask vGSHTMask
	#endif
	#ifdef REQUIRES_NORMAL
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
	#endif
	
	#ifdef SHARED_SPB
		flat out int vGSSPBIndex;
		#define vSPBIndex vGSSPBIndex
		
		#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED
			flat out int vGSSPBFlags;
		#endif
	#endif
	
#else
	#ifdef REQUIRES_TEX_COLOR
		out vec2 vTCColor;
	#endif
	#ifdef CLIP_PLANE
		out vec3 vClipCoord;
	#endif
	#ifdef DEPTH_DISTANCE
		out vec3 vPosition;
	#endif
	#ifdef HEIGHT_MAP
		out float vHTMask;
	#endif
	#ifdef REQUIRES_NORMAL
		out vec3 vNormal;
		#ifdef WITH_TANGENT
			out vec3 vTangent;
		#endif
		#ifdef WITH_BITANGENT
			out vec3 vBitangent;
		#endif
	#endif
	#ifdef WITH_REFLECT_DIR
		out vec3 vReflectDir;
	#endif
	#ifdef FADEOUT_RANGE
		out float vFadeZ;
	#endif
	
	#ifdef SHARED_SPB
		flat out int vSPBIndex;
	#endif
#endif



// Main Function
//////////////////

#include "v130/shared/tessellation.glsl"
#include "v130/shared/defren/sanitize_position.glsl"

void main(){
	// tessellate varying parameters
	#ifdef REQUIRES_TEX_COLOR
		TESS_VAR_LINEAR_TRI( vTCColor, vTESTCColor );
	#endif
	#ifdef HEIGHT_MAP
		TESS_VAR_LINEAR_TRI( vHTMask, vTESHTMask );
	#endif
	#ifdef REQUIRES_NORMAL
		TESS_VAR_LINEAR_TRI( vNormal, vTESNormal );
		#ifdef WITH_TANGENT
			TESS_VAR_LINEAR_TRI( vTangent, vTESTangent );
		#endif
		#ifdef WITH_BITANGENT
			TESS_VAR_LINEAR_TRI( vBitangent, vTESBitangent );
		#endif
	#endif
	
	// tessellate position
	tessLinearTri( gl_Position, gl_in[ 0 ].gl_Position, gl_in[ 1 ].gl_Position, gl_in[ 2 ].gl_Position );
	
	#ifdef REQUIRES_NORMAL
		vec3 displacement = normalize( vNormal ) * vec3( texture( texHeight, vTCColor ).r * pHeightRemap.x + pHeightRemap.y );
		gl_Position.xyz += displacement;
		
		// normalize normals again. required since for displacement
		// the normals have to be in the proper coordinate system
		vNormal = normalize( vNormal * pMatrixVn[ 0 ] );
		#ifdef WITH_TANGENT
			vTangent = normalize( vTangent * pMatrixVn[ 0 ] );
		#endif
		#ifdef WITH_BITANGENT
			vBitangent = normalize( vBitangent * pMatrixVn[ 0 ] );
		#endif
	#endif
	
	#ifndef PASS_ON_NEXT_STAGE
		#ifdef REQUIRES_NORMAL
			#ifdef DEPTH_DISTANCE
				#ifdef BILLBOARD
					vPosition = gl_Position.xyz;
				#else
					vPosition = pMatrixV[ 0 ] * gl_Position;
				#endif
			#endif
			#ifdef WITH_REFLECT_DIR
				#ifdef BILLBOARD
					vReflectDir = gl_Position.xyz;
				#else
					vReflectDir = pMatrixV[ 0 ] * gl_Position;
				#endif
			#endif
			#ifdef CLIP_PLANE
				#ifdef BILLBOARD
					vClipCoord = gl_Position.xyz;
				#else
					vClipCoord = pMatrixV[ 0 ] * gl_Position;
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
