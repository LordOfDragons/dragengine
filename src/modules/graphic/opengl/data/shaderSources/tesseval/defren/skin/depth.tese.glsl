//#extension GL_ARB_tessellation_shader : enable

#include "shared/preamble.glsl"

layout(triangles, equal_spacing, ccw) in;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/defren/skin/ubo_texture_parameters.glsl"
#include "shared/defren/skin/ubo_instance_parameters.glsl"

#include "shared/interface/skin/samplers.glsl"

#include "shared/interface/skin/tessellation_evaluate.glsl"

#ifdef SHARED_SPB
	#define VAR_SPB_INDEX vTESSPBIndex(0)
	#include "shared/defren/skin/shared_spb_redirect.glsl"
#endif
#include "shared/defren/skin/shared_spb_texture_redirect.glsl"
#include "shared/defren/skin/ubo_dynamic_parameters.glsl"

#include "shared/interface/skin/variation.glsl"


void main(){
	tessellationShaderDefaultOutputs();
	
	// tessellate position
	tessLinearTri(gl_Position, gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
	
	if(TextureHeight){
		gl_Position.xyz += normalize(vNormal) * vec3(TEXTURE(texHeight, texHeightArray, vTCColor).r
			* getHeightRemap(VAR_SPB_INDEX).x + getHeightRemap(VAR_SPB_INDEX).y);
	}
	
	// normalize normals again. required since for displacement
	// the normals have to be in the proper coordinate system
	vNormal = normalize(vNormal * pMatrixVn[0]);
	vTangent = normalize(vTangent * pMatrixVn[0]);
	vBitangent = normalize(vBitangent * pMatrixVn[0]);
	
	if(PassOnNextStage){
		vDoubleSided = pDoubleSided ? 1 : 0;
		
	}else{
		if(AnyKindBillboard){
			vPosition = vec3(gl_Position);
			vReflectDir = vec3(gl_Position);
			vClipCoord = vec3(gl_Position);
			vFadeZ = gl_Position.z;
			
		}else{
			vPosition = pMatrixV[0] * gl_Position;
			vReflectDir = pMatrixV[0] * gl_Position;
			vClipCoord = pMatrixV[0] * gl_Position;
			vFadeZ = (pMatrixV[0] * gl_Position).z;
		}
		vSkinClipCoord = vec3(gl_Position);
		
		if(AnyKindBillboard){
			gl_Position = pMatrixP[0] * gl_Position;
		}else{
			gl_Position = pMatrixVP[0] * gl_Position;
		}
	}
}
