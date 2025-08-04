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


// uniforms
#include "shared/ubo_defines.glsl"

#ifdef WITH_SHADOWMAP
	#include "shared/defren/ubo_render_parameters.glsl"
	const vec4 pTransformZ[6] = vec4[6](vec4(0.0), vec4(0.0), vec4(0.0), vec4(0.0), vec4(0.0), vec4(0.0));
	
#else
	UBOLAYOUT_BIND(0) uniform RenderParameters{
		mat4 pMatrixVP[6];
		mat4x3 pMatrixV[6];
		vec4 pTransformZ[6];
		vec2 pZToDepth;
		vec4 pClipPlane[2]; // normal.xyz, distance
	};
	const vec4 pDepthOffset[4] = vec4[4](vec4(0.0), vec4(0.0), vec4(0.0), vec4(0.0));
#endif

// inputs
VARYING_BIND(0) flat in int vGSSPBIndex[3];
VARYING_BIND(1) flat in int vGSSPBFlags[3];

#ifdef SHARED_SPB
	#include "shared/defren/skin/shared_spb_redirect.glsl"
#endif


// outputs
VARYING_BIND(0) out float vDepth;
VARYING_BIND(1) out vec3 vPosition;
VARYING_BIND(2) out vec3 vClipCoord;
VARYING_BIND(3) flat out int vLayer;
VARYING_BIND(4) flat out int vSPBIndex;


#include "shared/defren/sanitize_position.glsl"


#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED || defined GS_RENDER_STEREO

#include "shared/defren/skin/depth_offset.glsl"

void emitCorner(in int layer, in vec4 position, in vec4 preTransformedPosition){
	gl_Position = preTransformedPosition;
	
	vSPBIndex = vGSSPBIndex[0];
	vDepth = dot(pTransformZ[layer], position);
	vPosition = pMatrixV[layer] * position;
	vClipCoord = pMatrixV[layer] * position;
	
	#if defined DEPTH_OFFSET
		#ifdef GS_RENDER_CUBE
			#ifdef DEPTH_DISTANCE
				applyDepthOffset(0, vPosition.z);
			#else
				applyDepthOffset(0);
			#endif
		#else
			#ifdef DEPTH_DISTANCE
				applyDepthOffset(layer, vPosition.z);
			#else
				applyDepthOffset(layer);
			#endif
		#endif
	#endif
	
	vLayer = layer;
	gl_Layer = layer;
	gl_PrimitiveID = gl_PrimitiveIDIn;
	
	//EmitVertex();
}

void emitCorner(in int layer, in vec4 position){
	emitCorner(layer, position, pMatrixVP[layer] * position);
}

#endif


// render cube
#ifdcef GS_RENDER_CUBE

void main(void){
	// NOTE: quest requires EmitVertex to be called in main()
	
	int i, face;
	
	#ifdef GS_INSTANCING
	face = gl_InvocationID;
	#else
	for(face=0; face<6; face++){
	#endif
		
		#ifdef GS_RENDER_CUBE_CULLING
		// WARNING! there is a nasty bug in the MESA implementation causing 'continue' to
		//          skip the loop increment if used in if-statements resulting in GPU infinite
		//          loop. sometimes continue works but especially here it results in the GPU
		//          dying horribly. the only working solution is to use the code in a way
		//          no 'continue' statement is required to be used
		if((vGSSPBFlags[0] & (1 << face)) != 0){
		#endif
			
			for(i=0; i<3; i++){
				emitCorner(face, gl_in[i].gl_Position);
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

#endif

// render stereo
#ifdef GS_RENDER_STEREO

void main(void){
	// NOTE: quest requires EmitVertex to be called in main()
	
	int eye;
	
	#ifdef GS_INSTANCING
	eye = gl_InvocationID;
	#else
	for(eye=0; eye<2; eye++){
	#endif
		
		int i;
		for(i=0; i<3; i++){
			emitCorner(eye, gl_in[i].gl_Position);
			EmitVertex();
		}
		EndPrimitive();
		
	#ifndef GS_INSTANCING
	}
	#endif
}

#endif // GS_RENDER_STEREO
