#include "shared/preamble.glsl"

// layout definitions
#ifdef GS_RENDER_CUBE
	#ifdef GS_INSTANCING
		layout(triangles, invocations=6) in;
		layout(triangle_strip, max_vertices=3) out;
	#else
		layout(triangles) in;
		layout(triangle_strip, max_vertices=18) out;
	#endif
	
#elif defined GS_RENDER_CASCADED
	#ifdef GS_INSTANCING
		layout(triangles, invocations=4) in;
		layout(triangle_strip, max_vertices=3) out;
	#else
		layout(triangles) in;
		layout(triangle_strip, max_vertices=12) out;
	#endif
	
#elif defined GS_RENDER_STEREO
	#ifdef GS_INSTANCING
		layout(triangles, invocations=2) in;
		layout(triangle_strip, max_vertices=3) out;
	#else
		layout(triangles) in;
		layout(triangle_strip, max_vertices=6) out;
	#endif
#endif


#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

in vec2 vGSTCColor[3];
in float vGSHTMask[3];
in vec3 vGSNormal[3];
in vec3 vGSTangent[3];
in vec3 vGSBitangent[3];
flat in int vGSDoubleSided[3];
flat in int vGSSPBIndex[3];
flat in int vGSSPBFlags[3];

#define spbIndex vGSSPBIndex[0]
#define spbFlags vGSSPBFlags[0]

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
flat out int vLayer;
flat out int vSPBIndex;


// Layered rendering
//////////////////////

#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED || defined GS_RENDER_STEREO

#include "shared/defren/skin/depth_offset.glsl"

void emitCorner(in int layer, in int corner, in vec4 position, in vec4 preTransformedPosition){
	gl_Position = preTransformedPosition;
	
	vSPBIndex = spbIndex;
	vTCColor = vGSTCColor[corner];
	vNormal = normalize(vGSNormal[corner] * pMatrixVn[layer]);
	vTangent = normalize(vGSTangent[corner] * pMatrixVn[layer]);
	vBitangent = normalize(vGSBitangent[corner] * pMatrixVn[layer]);
	vSkinClipCoord = vec3(position);
	vHTMask = vGSHTMask[corner];
	
	#ifdef BILLBOARD
		vReflectDir = vec3(position);
		vPosition = vec3(position);
		vClipCoord = vec3(position);
		vFadeZ = position.z;
	#else
		vReflectDir = pMatrixV[layer] * position;
		vPosition = pMatrixV[layer] * position;
		vClipCoord = pMatrixV[layer] * position;
		vFadeZ = (pMatrixV[layer] * position).z;
	#endif
	
	#if defined DEPTH_OFFSET
		// pDoubleSided is passed on from the vertex shader to avoid requiring SSBO
		// (ubo_instance_parameters) in geometry shaders for compatibility reasons
		bool doubleSided = vGSDoubleSided[0] == 1;
		
		#ifdef GS_RENDER_CUBE
			#ifdef DEPTH_DISTANCE
				applyDepthOffset(0, vNormal, doubleSided, vPosition.z);
			#else
				applyDepthOffset(0, vNormal, doubleSided);
			#endif
		#else
			#ifdef DEPTH_DISTANCE
				applyDepthOffset(layer, vNormal, doubleSided, vPosition.z);
			#else
				applyDepthOffset(layer, vNormal, doubleSided);
			#endif
		#endif
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



// Cube Map Rendering
///////////////////////

#ifdef GS_RENDER_CUBE
// cube map handling. each triangle is send to one of the cube map faces using
// appropriate matrix transformation

void main(void){
	// NOTE: quest requires EmitVertex to be called in main()
	
	int face;
	
	#ifdef GS_INSTANCING
	face = gl_InvocationID;
	#else
	for(face=0; face<6; face++){
	#endif
		
		// TODO per-face matrix. has to be applied to pMatrixVP, pMatrixV and pMatrixVn.
		//      how can this be made optimal?
		// 
		// pMatrixP
		//    this needs no modification
		// 
		// pMatrixV
		//    this transform vertices into camera spaces. the most simple soution is to
		//    use the first face matrix as reference matrix (positive x-axis). for each
		//    face can be defined a 3x3 rotation matrix converting from the positive
		//    x-axis matrix into the required face matrix. this matrices can be defined
		//    as a constant matrix array and should be fast. the result is this:
		//    
		//       vPosition = cRotFace[gl_InvocationID] 
		//          * (pMatrixV * vec4(gl_in[i].gl_Position, 1.0));
		//    
		//    works since pMatrixV is max4x3 returning a vec3 and cRotFaceis a mat3
		//    
		// pMatrixVn
		//    works similar to pMatrixV but needs transposed cRotFace to work like this:
		//    
		//       vNormal = normalize((vGSNormal[i] * pMatrixVn)
		//          * cRotFaceN[gl_InvocationID]);
		// 
		// pMatrixVP
		//    can not be done directly since the rotation matrix has to go in between the
		//    view and projection part. has to be done like this instead:
		//    
		//       gl_Position = pMatrixP
		//          * vec4(cRotFace[gl_InvocationID]
		//             * (pMatrixV * vec4(gl_in[i].gl_Position), 1.0), 1.0);
		
		#ifdef GS_RENDER_CUBE_CULLING
		// WARNING! there is a nasty bug in the MESA implementation causing 'continue' to
		//          skip the loop increment if used in if-statements resulting in GPU infinite
		//          loop. sometimes continue works but especially here it results in the GPU
		//          dying horribly. the only working solution is to use the code in a way
		//          no 'continue' statement is required to be used
		if((spbFlags & (1 << face)) != 0){
		#endif
			
			// emit triangle
			int i;
			for(i=0; i<3; i++){
				emitCorner(face, i, gl_in[i].gl_Position);
				EmitVertex();
			}
			EndPrimitive();
			
		#ifdef GS_RENDER_CUBE_CULLING
		}
		#endif
		
	#ifndef GS_INSTANCING
	}
	#endif
}

#endif // GS_RENDER_CUBE



// Cascaded Rendering
///////////////////////

#ifdef GS_RENDER_CASCADED

void main(void){
	// NOTE: quest requires EmitVertex to be called in main()
	
	int cascade;
	
	#ifdef GS_INSTANCING
	cascade = gl_InvocationID;
	#else
	for(cascade=0; cascade<4; cascade++){
	#endif
		
		// determine if the triangle has to be emitted in this cascade. for this transform
		// the triangle as if it is emitted. the triangle is now in cascade space which is
		// orthogonal. if the triangle extends intersects the (-0.5,-0.5) - (0.5,0.5) box
		// then the triangle affects the cascade.
		// 
		// NOTE: since cascade space is orthogonal pMatrixP is identify and can be omitted
		// 
		// WARNING! there is a nasty bug in the MESA implementation causing 'continue' to
		//          skip the loop increment if used in if-statements resulting in GPU infinite
		//          loop. sometimes continue works but especially here it results in the GPU
		//          dying horribly. the only working solution is to use the code in a way
		//          no 'continue' statement is required to be used
		
		/*
		vec3 position[3];
		int i;
		
		for(i=0; i<3; i++){
			#ifdef BILLBOARD
			position[i] = gl_in[i].gl_Position;
			#else
			position[i] = pMatrixV[cascade] * gl_in[i].gl_Position;
			#endif
		}
		
		// it would be possible to box check also the Z coordinate. with orthogonal
		// depth though elements outside the Z range have been already culled.
		// this speeds up the test by just checking the X and Y coordinates
		// 
		// the test below is like this in full terms (for all components):
		//   maxExtend > -1 and minExtend < 1
		// 
		// since two coordinates are checked this can be collapsed into a single
		// vec4 value. with the following formula transformation one lessThan call
		// is enough to check all conditions:
		//   -(maxExtend) < -(-1) and minExtend < 1   =>
		//   -maxExtend < 1 and minExtend < 1
		//
		vec4 boxCheck = vec4(
			 min(min(position[0].xy, position[1].xy), position[2].xy),
			-max(max(position[0].xy, position[1].xy), position[2].xy));
		
		if(all(lessThan(boxCheck, vec4(1.0)))){
			// emit triangle
			for(i=0; i<3; i++){
				emitCorner(cascade, i, gl_in[i].gl_Position, vec4(position[i], 1.0));
				EmitVertex();
			}
			EndPrimitive();
		}
		*/
		
		if((spbFlags & (1 << cascade)) != 0){
			// emit triangle
			int i;
			for(i=0; i<3; i++){
				#ifdef BILLBOARD
				emitCorner(cascade, i, gl_in[i].gl_Position, gl_in[i].gl_Position);
				#else
				emitCorner(cascade, i, gl_in[i].gl_Position, vec4(pMatrixV[cascade] * gl_in[i].gl_Position, 1));
				#endif
				EmitVertex();
			}
			EndPrimitive();
		}
		
		
	#ifndef GS_INSTANCING
	}
	#endif
}

#endif // GS_RENDER_CASCADED



// Dual Viewport Rendering
////////////////////////////

#ifdef GS_RENDER_STEREO

void main(void){
	// NOTE: quest requires EmitVertex to be called in main()
	
	int eye;
	
	#ifdef GS_INSTANCING
	eye = gl_InvocationID;
	#else
	for(eye=0; eye<2; eye++){
	#endif
		
		// emit triangle
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

#endif // GS_RENDER_STEREO
