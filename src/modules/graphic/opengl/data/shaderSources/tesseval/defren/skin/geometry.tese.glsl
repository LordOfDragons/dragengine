#include "shared/preamble.glsl"

//layout(quads, equal_spacing, cw) in;
layout(triangles, equal_spacing, ccw) in;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/defren/skin/ubo_texture_parameters.glsl"
#include "shared/defren/skin/ubo_instance_parameters.glsl"

#include "shared/interface/skin/samplers.glsl"

#include "shared/interface/skin/tessellation_evaluate.glsl"

#ifdef SHARED_SPB
	#include "shared/defren/skin/shared_spb_redirect.glsl"
#endif
#include "shared/defren/skin/shared_spb_texture_redirect.glsl"
#include "shared/defren/skin/ubo_dynamic_parameters.glsl"

void main(){
	tessellationShaderDefaultOutputs();
	
	// tessellate position
	tessLinearTri(gl_Position, gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
	
	VARCONST int spbIndex = vTESSPBIndex(0);
	
	if(TextureHeight){
		gl_Position.xyz += normalize(vNormal) * vec3(texture(texHeight, vTCColor).r
			* getHeightRemap(spbIndex).x + getHeightRemap(spbIndex).y);
	}
	
	if(!PassOnNextStage){
		// normalize normals. this has to be done after position adjustment since otherwise the coordinate system is wrong
		vNormal = normalize(vNormal * pMatrixVn[0]);
		vTangent = normalize(vTangent * pMatrixVn[0]);
		vBitangent = normalize(vBitangent * pMatrixVn[0]);
		
		if(AnyKindBillboard){
			vReflectDir = vec3(gl_Position);
			vFadeZ = gl_Position.z;
			
			gl_Position = pMatrixP[0] * gl_Position;
			
		}else{
			vReflectDir = pMatrixV[0] * gl_Position;
			vFadeZ = (pMatrixV[0] * gl_Position).z;
			
			gl_Position = pMatrixVP[0] * gl_Position;
		}
	}
}
