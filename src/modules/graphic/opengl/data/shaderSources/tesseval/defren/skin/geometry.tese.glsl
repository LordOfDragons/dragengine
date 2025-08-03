//#extension GL_ARB_tessellation_shader : enable

#include "shared/preamble.glsl"

//layout(quads, equal_spacing, cw) in;
layout(triangles, equal_spacing, ccw) in;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/defren/skin/ubo_texture_parameters.glsl"
#include "shared/defren/skin/ubo_instance_parameters.glsl"
#include "shared/defren/skin/ubo_dynamic_parameters_tese.gls

uniform HIGHP sampler2D texHeight;

in vec2 vTESTCColor[];
in vec2 vTESTCNormal[];
in vec2 vTESTCReflectivity[];
in vec2 vTESTCEmissivity[];
in vec2 vTESTCRefractionDistort[];
in vec2 vTESTCAO[];
in vec3 vTESNormal[];
in vec3 vTESTangent[];
in vec3 vTESBitangent[];
in float vTESHTMask[];
// in float vTESRenderCondition[];

#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED || defined GS_RENDER_STEREO
	#define PASS_ON_NEXT_STAGE 1
	
	out vec2 vGSTCColor;
	out vec2 vGSTCNormal;
	out vec2 vGSTCReflectivity;
	out vec2 vGSTCEmissivity;
	out vec2 vGSTCRefractionDistort;
	out vec2 vGSTCAO;
	out vec3 vGSNormal;
	out vec3 vGSTangent;
	out vec3 vGSBitangent;
	out float vGSHTMask;
	// out float vGSRenderCondition;
	
	#define vTCColor vGSTCColor
	#define vTCNormal vGSTCNormal
	#define vTCReflectivity vGSTCReflectivity
	#define vTCEmissivity vGSTCEmissivity
	#define vTCRefractionDistort vGSTCRefractionDistort
	#define vTCAO vGSTCAO
	#define vNormal vGSNormal
	#define vTangent vGSTangent
	#define vBitangent vGSBitangent
	#define vHTMask vGSHTMask
	
#else
	out vec2 vTCColor;
	out vec2 vTCNormal;
	out vec2 vTCReflectivity;
	out vec2 vTCEmissivity;
	out vec2 vTCRefractionDistort;
	out vec2 vTCAO;
	out vec3 vNormal;
	out vec3 vTangent;
	out vec3 vBitangent;
	out vec3 vReflectDir;
	out float vHTMask;
#endif

#include "shared/tessellation.glsl"

void main(){
	// tessellate varying parameters
	TESS_VAR_LINEAR_TRI(vTCColor, vTESTCColor);
	TESS_VAR_LINEAR_TRI(vTCNormal, vTESTCNormal);
	TESS_VAR_LINEAR_TRI(vTCReflectivity, vTESTCReflectivity);
	TESS_VAR_LINEAR_TRI(vTCEmissivity, vTESTCEmissivity);
	TESS_VAR_LINEAR_TRI(vTCRefractionDistort, vTESTCRefractionDistort);
	TESS_VAR_LINEAR_TRI(vTCAO, vTESTCAO);
	TESS_VAR_LINEAR_TRI(vNormal, vTESNormal);
	TESS_VAR_LINEAR_TRI(vTangent, vTESTangent);
	TESS_VAR_LINEAR_TRI(vBitangent, vTESBitangent);
	TESS_VAR_LINEAR_TRI(vHTMask, vTESHTMask);
	// TESS_VAR_LINEAR_TRI(vRenderCondition, vTESRenderCondition);
	
	// tessellate position
	tessLinearTri(gl_Position, gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
	
	#ifdef TEXTURE_HEIGHT
		vec3 displacement = normalize(vNormal) * vec3(texture(texHeight, vTCColor).r * pHeightRemap.x + pHeightRemap.y);
		gl_Position.xyz += displacement;
	#endif
	
	#ifndef PASS_ON_NEXT_STAGE
		// normalize normals. this has to be done after position adjustment since otherwise the coordinate system is wrong
		vNormal = normalize(vNormal * pMatrixVn[0]);
		vTangent = normalize(vTangent * pMatrixVn[0]);
		vBitangent = normalize(vBitangent * pMatrixVn[0]);
		
		#ifdef BILLBOARD
			vReflectDir = gl_Position.xyz;
			vFadeZ = gl_Position.z;
		#else
			vReflectDir = pMatrixV[0] * gl_Position;
			vFadeZ = (pMatrixV[0] * gl_Position).z;
		#endif
		
		#ifdef BILLBOARD
			gl_Position = pMatrixP[0] * gl_Position;
		#else
			gl_Position = pMatrixVP[0] * gl_Position;
		#endif
	#endif
}
