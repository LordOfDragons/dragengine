#ifdef GS_INSTANCING
	#ifndef OPENGLES
		#extension GL_ARB_gpu_shader5 : require
	#endif
#endif


// layout definitions
#ifdef GS_RENDER_STEREO
	#ifdef GS_INSTANCING
		layout(triangles, invocations=2) in;
		layout(triangle_strip, max_vertices=3) out;
	#else
		layout(triangles) in;
		layout(triangle_strip, max_vertices=6) out;
	#endif
#else
	layout(triangles) in;
	layout(triangle_strip, max_vertices=3) out;
#endif

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"


in vec2 vGSTCColor[3];
in vec2 vGSTCColorTintMask[3];
in vec2 vGSTCNormal[3];
in vec2 vGSTCReflectivity[3];
in vec2 vGSTCEmissivity[3];
in vec2 vGSTCRefractionDistort[3];
in vec2 vGSTCAO[3];
in vec3 vGSNormal[3];
in vec3 vGSTangent[3];
in vec3 vGSBitangent[3];
in float vGSHTMask[3];
// in float vGSRenderCondition[3];
flat in int vGSSPBIndex[3];
flat in int vGSSPBFlags[3];

#define spbIndex vGSSPBIndex[0]
#define spbFlags vGSSPBFlags[0]


out vec2 vTCColor;
out vec2 vTCColorTintMask;
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
out float vFadeZ;
flat out int vSPBIndex;
flat out int vLayer;


// dual view rendering
#ifdef GS_RENDER_STEREO

void emitCorner(in int layer, in int corner, in vec4 position, in vec4 preTransformedPosition){
	gl_Position = preTransformedPosition;
	
	vSPBIndex = spbIndex;
	vTCColor = vGSTCColor[corner];
	vTCColorTintMask = vGSTCColorTintMask[corner];
	vTCNormal = vGSTCNormal[corner];
	vTCReflectivity = vGSTCReflectivity[corner];
	vTCEmissivity = vGSTCEmissivity[corner];
	vTCRefractionDistort = vGSTCRefractionDistort[corner];
	vTCAO = vGSTCAO[corner];
	vNormal = normalize(vGSNormal[corner] * pMatrixVn[layer]);
	vTangent = normalize(vGSTangent[corner] * pMatrixVn[layer]);
	vBitangent = normalize(vGSBitangent[corner] * pMatrixVn[layer]);
	vReflectDir = pMatrixV[layer] * position;
	vHTMask = vGSHTMask[corner];
	
	#ifdef BILLBOARD
		vFadeZ = position.z;
	#else
		vFadeZ = (pMatrixV[layer] * position).z;
	#endif
	
	vLayer = layer;
	gl_Layer = layer;
	gl_PrimitiveID = gl_PrimitiveIDIn;
	
	//EmitVertex();
}

void emitCorner(in int layer, in int corner, in vec4 position){
	vec4 preTransformedPosition;
	
	#ifdef BILLBOARD
		#ifdef GS_RENDER_STEREO
			// during vertex shader the left view position has been used.
			// if this is the right view correct the transform
			if(layer == 1){
				preTransformedPosition = pMatrixP[layer] * vec4(pCameraStereoTransform * position, 1);
				
			}else{
				preTransformedPosition = pMatrixP[layer] * position;
			}
		#else
			preTransformedPosition = pMatrixP[layer] * position;
		#endif
	#else
		preTransformedPosition = pMatrixVP[layer] * position;
	#endif
	
	emitCorner(layer, corner, position, preTransformedPosition);
}

#endif



// Main Function
//////////////////

#ifdef GS_RENDER_STEREO

void main(void){
	// NOTE: quest requires EmitVertex to be called in main()
	
// 	if(vGSRenderCondition[0] >= 5.0){
// 		return;
// 	}
	
	int eye;
	
	#ifdef GS_INSTANCING
	eye = gl_InvocationID;
	#else
	for(eye=0; eye<2; eye++){
	#endif
		
		int i;
		for(i=0; i<3; i++){
			emitCorner(eye, i, gl_in[i].gl_Position);
			EmitVertex();
		}
		EndPrimitive();
		
	#ifndef GS_INSTANCING
	}
	#endif
}

#else

void main(void){
// 	if(vGSRenderCondition[0] >= 5.0){
// 		return;
// 	}
	
	int i;
	for(i=0; i<3; i++){
		gl_Position = gl_in[i].gl_Position;
		
		vTCColor = vGSTCColor[i];
		vSPBIndex = spbIndex;
		vTCColorTintMask = vGSTCColorTintMask[i];
		vTCNormal = vGSTCNormal[i];
		vTCReflectivity = vGSTCReflectivity[i];
		vTCEmissivity = vGSTCEmissivity[i];
		vTCRefractionDistort = vGSTCRefractionDistort[i];
		vTCAO = vGSTCAO[i];
		vNormal = vGSNormal[i];
		vTangent = vGSTangent[i];
		vBitangent = vGSBitangent[i];
		vReflectDir = vGSReflectDir[i];
		vHTMask = vGSHTMask[i];
		vFadeZ = vGSFadeZ[i];
		
		vLayer = 0;
		gl_Layer = 0;
		gl_PrimitiveID = gl_PrimitiveIDIn;
		
		EmitVertex();
	}
	
	EndPrimitive();
}

#endif // GS_RENDER_STEREO
