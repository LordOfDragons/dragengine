#include "shared/preamble.glsl"

// layout definitions
#ifdef LAYERED_RENDERING_STEREO
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

#include "shared/interface/skin/geometry.glsl"


// dual view rendering
#ifdef LAYERED_RENDERING_STEREO

void emitCorner(in int layer, in int corner, in vec4 position, in vec4 preTransformedPosition){
	gl_Position = preTransformedPosition;
	
	geometryShaderDefaultOutputs(corner, layer);
	
	vNormal = normalize(vGSNormal(corner) * pMatrixVn[layer]);
	vTangent = normalize(vGSTangent(corner) * pMatrixVn[layer]);
	vBitangent = normalize(vGSBitangent[corner] * pMatrixVn[layer]);
	vReflectDir = pMatrixV[layer] * position;
	vHTMask = vGSHTMask(corner);
	
	if(AnyKindBillboard){
		vFadeZ = position.z;
	}else{
		vFadeZ = (pMatrixV[layer] * position).z;
	}
	
	//EmitVertex();
}

void emitCorner(in int layer, in int corner, in vec4 position){
	vec4 preTransformedPosition;
	
	if(AnyKindBillboard){
		if(LayeredRendering == LayeredRenderingStereo){
			// during vertex shader the left view position has been used.
			// if this is the right view correct the transform
			if(layer == 1){
				preTransformedPosition = pMatrixP[layer] * vec4(pCameraStereoTransform * position, 1);
				
			}else{
				preTransformedPosition = pMatrixP[layer] * position;
			}
			
		}else{
			preTransformedPosition = pMatrixP[layer] * position;
		}
		
	}else{
		preTransformedPosition = pMatrixVP[layer] * position;
	}
	
	emitCorner(layer, corner, position, preTransformedPosition);
}

#endif



// Main Function
//////////////////

#ifdef LAYERED_RENDERING_STEREO

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
		
		geometryShaderDefaultOutputs(i, 0);
		
		vSPBIndex = vGSSPBIndex(0);
		
		EmitVertex();
	}
	
	EndPrimitive();
}

#endif
