#include "shared/preamble.glsl"

// layout definitions
#if LAYERED_RENDERING_CUBE
	#ifdef GS_INSTANCING
		layout(triangles, invocations=6) in;
		layout(triangle_strip, max_vertices=3) out;
	#else
		layout(triangles) in;
		layout(triangle_strip, max_vertices=18) out;
	#endif
	
#elif LAYERED_RENDERING_CASCADED
	#ifdef GS_INSTANCING
		layout(triangles, invocations=4) in;
		layout(triangle_strip, max_vertices=3) out;
	#else
		layout(triangles) in;
		layout(triangle_strip, max_vertices=12) out;
	#endif
	
#elif LAYERED_RENDERING_STEREO
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
#include "shared/defren/occmap.glsl"

// inputs
VARYING_BIND(0) in float vGSDepth[3];
VARYING_BIND(1) in vec3 vGSPosition[3];
VARYING_BIND(2) in vec3 vGSClipCoord[3];
VARYING_BIND(3) flat in int vGSSPBIndex[3];
VARYING_BIND(4) flat in int vGSSPBFlags[3];
VARYING_BIND(5) flat in int vGSLayer[3];

#ifdef SHARED_SPB
	#include "shared/defren/skin/shared_spb_redirect.glsl"
#endif


// outputs
VARYING_BIND(0) out float vDepth;
VARYING_BIND(1) out vec3 vPosition;
VARYING_BIND(2) out vec3 vClipCoord;
VARYING_BIND(3) flat out int vSPBIndex;
VARYING_BIND(4) flat out int vSPBFlags;
VARYING_BIND(5) flat out int vLayer;


#include "shared/defren/sanitize_position.glsl"


#include "shared/defren/skin/depth_offset.glsl"

void emitCorner(in int layer, in vec4 position, in vec4 preTransformedPosition){
	gl_Position = preTransformedPosition;
	
	vSPBIndex = vGSSPBIndex[0];
	vDepth = dot(getTransformZ(layer), position);
	vPosition = getMatrixV(layer) * position;
	vClipCoord = getMatrixV(layer) * position;
	
	if(DepthOffset){
		#if LAYERED_RENDERING_CUBE
			VARCONST int depthLayer = 0;
		#else
			VARCONST int depthLayer = layer;
		#endif
		
		if(DepthDistance){
			applyDepthOffset(depthLayer, vPosition.z);
			
		}else{
			applyDepthOffset(depthLayer);
		}
	}
	
	vLayer = layer;
	gl_Layer = layer;
	gl_PrimitiveID = gl_PrimitiveIDIn;
	
	//EmitVertex();
}

void emitCorner(in int layer, in vec4 position){
	emitCorner(layer, position, getMatrixVP(layer) * position);
}


// render cube
#if LAYERED_RENDERING_CUBE

void main(void){
	// NOTE: quest requires EmitVertex to be called in main()
	
	int i, face;
	
	#ifdef GS_INSTANCING
	face = gl_InvocationID;
	#else
	for(face=0; face<6; face++){
	#endif
		
		if(GSRenderCubeCulling){
			// WARNING! there is a nasty bug in the MESA implementation causing 'continue' to
			//          skip the loop increment if used in if-statements resulting in GPU infinite
			//          loop. sometimes continue works but especially here it results in the GPU
			//          dying horribly. the only working solution is to use the code in a way
			//          no 'continue' statement is required to be used
			//
			// should not be a problem anymore with newer MESA drivers
			if((vGSSPBFlags[0] & (1 << face)) == 0){
				#ifdef GS_INSTANCING
				return;
				#else
				continue;
				#endif
			}
		}
		
		for(i=0; i<3; i++){
			emitCorner(face, gl_in[i].gl_Position);
			EmitVertex();
		}
		EndPrimitive();
		
	#ifndef GS_INSTANCING
	}
	#endif
}

// render stereo
#elif LAYERED_RENDERING_STEREO

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

// render cascaded
#elif LAYERED_RENDERING_CASCADED

// TODO

#endif
