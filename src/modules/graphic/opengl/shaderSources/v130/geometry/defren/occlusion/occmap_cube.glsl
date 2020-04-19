#if defined GS_RENDER_CUBE_INSTANCING && ! defined ANDROID
	#extension GL_ARB_gpu_shader5 : require
#endif

precision highp float;
precision highp int;

// layout definitions
#ifdef GS_RENDER_CUBE
	#ifdef GS_RENDER_CUBE_INSTANCING
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
		#define pCubeFaceVisible spbFlags
	#endif
#endif

#include "v130/shared/defren/skin/ubo_special_parameters.glsl"



// Main Function
//////////////////

void main( void ){
	int i;
	
	gl_PrimitiveID = gl_PrimitiveIDIn;
	
	#ifdef GS_RENDER_CUBE_INSTANCING
	gl_Layer = gl_InvocationID;
	#else
	for( int face=0; face<6; face++ ){
		gl_Layer = face;
	#endif
		
		#ifdef GS_RENDER_CUBE_CULLING
			// WARNING! there is a nasty bug in the MESA implementation causing 'continue' to
			//          skip the loop increment if used in if-statements resulting in GPU infinite
			//          loop. sometimes continue works but especially here it results in the GPU
			//          dying horribly. the only working solution is to use the code in a way
			//          no 'continue' statement is required to be used
			
			#ifdef GS_RENDER_CUBE_INSTANCING
			if( ( pCubeFaceVisible & ( 1 << gl_Layer ) ) == 0 ){
				return;
			}
			#else
			if( ( pCubeFaceVisible & ( 1 << gl_Layer ) ) != 0 ){
			#endif
		#endif
		
		// emit triangle
		for( i=0; i<3; i++ ){
			gl_Position = pMatrixVP[ gl_Layer ] * gl_in[ i ].gl_Position;
			#ifdef PERSPECTIVE_TO_LINEAR
				vDepth = dot( pTransformZ[ gl_Layer ], gl_in[ i ].gl_Position )
					* pZToDepth.x + pZToDepth.y;
			#endif
			#ifdef DEPTH_DISTANCE
				vPosition = pMatrixV[ gl_Layer ] * gl_in[ i ].gl_Position;
			#endif
			
			EmitVertex();
		}
		
		EndPrimitive();
		
		
		#if defined GS_RENDER_CUBE_CULLING && ! defined GS_RENDER_CUBE_INSTANCING
		}
		#endif
		
	#ifndef GS_RENDER_CUBE_INSTANCING
	}
	#endif
}
