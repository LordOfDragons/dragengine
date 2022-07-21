#if defined GS_INSTANCING && ! defined ANDROID
	#extension GL_ARB_gpu_shader5 : require
#endif

precision highp float;
precision highp int;

// layout definitions
#ifdef GS_RENDER_CUBE
	#ifdef GS_INSTANCING
		layout( triangles, invocations=6 ) in;
		layout( triangle_strip, max_vertices=3 ) out;
	#else
		layout( triangles ) in;
		layout( triangle_strip, max_vertices=18 ) out;
	#endif
#endif



// Uniform Parameters
///////////////////////

#include "v130/shared/ubo_defines.glsl"

UBOLAYOUT uniform RenderParameters{
	mat4 pMatrixVP[ 6 ];
	mat4x3 pMatrixV[ 6 ];
	vec4 pTransformZ[ 6 ];
	vec2 pZToDepth;
	vec4 pClipPlane; // normal.xyz, distance
};

#ifdef GS_RENDER_CUBE
	flat in int vSPBIndex[ 3 ];
	#define spbIndex vSPBIndex[0]
	
	#ifdef GS_RENDER_CUBE_CULLING
		flat in int vSPBFlags[ 3 ];
		#define spbFlags vSPBFlags[0]
	#endif
#endif

#ifdef PERSPECTIVE_TO_LINEAR
out float vDepth;
#endif
#ifdef DEPTH_DISTANCE
out vec3 vPosition;
#endif

#include "v130/shared/defren/occmap.glsl"

#ifdef SHARED_SPB
	#define pMatrixModel pSharedSPB[ spbIndex ].pSPBMatrixModel
	#ifdef GS_RENDER_CUBE_CULLING
		#define pLayerVisibility spbFlags
	#endif
#endif

#include "v130/shared/defren/skin/ubo_special_parameters.glsl"
#include "v130/shared/defren/sanitize_position.glsl"



// Main Function
//////////////////

void emitCorner( in int face, in vec4 position ){
	#ifdef PERSPECTIVE_TO_LINEAR
		vDepth = dot( pTransformZ[ face ], position );
	#endif
	#ifdef DEPTH_DISTANCE
		vPosition = pMatrixV[ face ] * position;
	#endif
	
	gl_Position = sanitizePosition( pMatrixVP[ face ] * position );
	
	gl_Layer = face;
	gl_PrimitiveID = gl_PrimitiveIDIn;
	
	EmitVertex();
}

void main( void ){
	int i, face;
	
	#ifdef GS_INSTANCING
	face = gl_InvocationID;
	#else
	for( face=0; face<6; face++ ){
	#endif
		
		#ifdef GS_RENDER_CUBE_CULLING
		// WARNING! there is a nasty bug in the MESA implementation causing 'continue' to
		//          skip the loop increment if used in if-statements resulting in GPU infinite
		//          loop. sometimes continue works but especially here it results in the GPU
		//          dying horribly. the only working solution is to use the code in a way
		//          no 'continue' statement is required to be used
		if( ( pLayerVisibility & ( 1 << face ) ) != 0 ){
		#endif
			
			// emit triangle
			for( i=0; i<3; i++ ){
				emitCorner( face, gl_in[ i ].gl_Position );
			}
			EndPrimitive();
			
		#ifdef GS_RENDER_CUBE_CULLING
		}
		#endif
		
	#ifndef GS_INSTANCING
	}
	#endif
}
