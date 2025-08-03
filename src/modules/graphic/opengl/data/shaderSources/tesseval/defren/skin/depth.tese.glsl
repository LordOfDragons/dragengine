//#extension GL_ARB_tessellation_shader : enable

#include "shared/preamble.glsl"

layout(triangles, equal_spacing, ccw) in;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/defren/skin/ubo_texture_parameters.glsl"
#include "shared/defren/skin/ubo_instance_parameters.glsl"
#include "shared/defren/skin/ubo_dynamic_parameters_tese.glsl"

uniform HIGHP sampler2D texHeight;

in vec2 vTESTCColor[];
in float vTESHTMask[];
in vec3 vTESNormal[];
in vec3 vTESTangent[];
in vec3 vTESBitangent[];

#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED || defined GS_RENDER_STEREO
	#define PASS_ON_NEXT_STAGE 1
	
	out vec2 vGSTCColor;
	out float vGSHTMask;
	out vec3 vGSNormal;
	out vec3 vGSTangent;
	out vec3 vGSBitangent;
	flat out int vGSDoubleSided;
	flat out int vGSSPBIndex;
	flat out int vGSSPBFlags;
	
	#define vTCColor vGSTCColor
	#define vHTMask vGSHTMask
	#define vNormal vGSNormal
	#define vTangent vGSTangent
	#define vBitangent vGSBitangent
	#define vDoubleSided vGSDoubleSided
	#define vSPBIndex vGSSPBIndex
	
#else
	out vec2 vTCColor;
	out vec3 vClipCoord;
	out vec3 vSkinClipCoord;
	out vec3 vPosition;
	out float vHTMask;
	out vec3 vNormal;
	out vec3 vTangent;
	out vec3 vBitangent;
	out vec3 vReflectDir;
	out float vFadeZ;
	flat out int vSPBIndex;
#endif

#include "shared/tessellation.glsl"

void main(){
	// tessellate varying parameters
	TESS_VAR_LINEAR_TRI(vTCColor, vTESTCColor);
	TESS_VAR_LINEAR_TRI(vHTMask, vTESHTMask);
	TESS_VAR_LINEAR_TRI(vNormal, vTESNormal);
	TESS_VAR_LINEAR_TRI(vTangent, vTESTangent);
	TESS_VAR_LINEAR_TRI(vBitangent, vTESBitangent);
	
	// tessellate position
	tessLinearTri(gl_Position, gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
	
	#ifdef TEXTURE_HEIGHT
		vec3 displacement = normalize(vNormal) * vec3(texture(texHeight, vTCColor).r * pHeightRemap.x + pHeightRemap.y);
		gl_Position.xyz += displacement;
	#endif
	
	// normalize normals again. required since for displacement
	// the normals have to be in the proper coordinate system
	vNormal = normalize(vNormal * pMatrixVn[0]);
	vTangent = normalize(vTangent * pMatrixVn[0]);
	vBitangent = normalize(vBitangent * pMatrixVn[0]);
	
	#ifdef PASS_ON_NEXT_STAGE
		vDoubleSided = pDoubleSided ? 1 : 0;
		
	#else
		#ifdef BILLBOARD
			vPosition = vec3(gl_Position);
			vReflectDir = vec3(gl_Position);
			vClipCoord = vec3(gl_Position);
			vFadeZ = gl_Position.z;
		#else
			vPosition = pMatrixV[0] * gl_Position;
			vReflectDir = pMatrixV[0] * gl_Position;
			vClipCoord = pMatrixV[0] * gl_Position;
			vFadeZ = (pMatrixV[0] * gl_Position).z;
		#endif
		vSkinClipCoord = vec3(gl_Position);
		
		#ifdef BILLBOARD
			gl_Position = pMatrixP[0] * gl_Position;
		#else
			gl_Position = pMatrixVP[0] * gl_Position;
		#endif
	#endif
}
