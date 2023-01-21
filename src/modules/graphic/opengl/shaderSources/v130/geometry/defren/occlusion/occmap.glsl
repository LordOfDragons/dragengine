#ifdef GS_INSTANCING
	#extension GL_ARB_gpu_shader5 : require
#endif

// layout definitions
#ifdef GS_RENDER_CUBE
	#ifdef GS_INSTANCING
		layout( triangles, invocations=6 ) in;
		layout( triangle_strip, max_vertices=3 ) out;
	#else
		layout( triangles ) in;
		layout( triangle_strip, max_vertices=18 ) out;
	#endif
	
#elif defined GS_RENDER_CASCADED
	#ifdef GS_INSTANCING
		layout( triangles, invocations=4 ) in;
		layout( triangle_strip, max_vertices=3 ) out;
	#else
		layout( triangles ) in;
		layout( triangle_strip, max_vertices=12 ) out;
	#endif
	
#elif defined GS_RENDER_STEREO
	#ifdef GS_INSTANCING
		layout( triangles, invocations=2 ) in;
		layout( triangle_strip, max_vertices=3 ) out;
	#else
		layout( triangles ) in;
		layout( triangle_strip, max_vertices=6 ) out;
	#endif
#endif



// uniforms
#include "v130/shared/ubo_defines.glsl"

#ifdef WITH_SHADOWMAP
	#include "v130/shared/defren/ubo_render_parameters.glsl"
#else
	UBOLAYOUT uniform RenderParameters{
		mat4 pMatrixVP[ 6 ];
		mat4x3 pMatrixV[ 6 ];
		vec4 pTransformZ[ 6 ];
		vec2 pZToDepth;
		vec4 pClipPlane[ 2 ]; // normal.xyz, distance
	};
#endif


// inputs
#ifdef SHARED_SPB
	flat in int vGSSPBIndex[ 3 ];
	#define spbIndex vGSSPBIndex[0]
	
	#if defined GS_RENDER_CUBE && defined GS_RENDER_CUBE_CULLING
		flat in int vGSSPBFlags[ 3 ];
		#define spbFlags vGSSPBFlags[0]
		#ifdef GS_RENDER_CUBE_CULLING
			#define pLayerVisibility spbFlags
		#endif
	#endif
	
	#include "v130/shared/defren/skin/shared_spb_redirect.glsl"
#endif


// outputs
#ifdef PERSPECTIVE_TO_LINEAR
	out float vDepth;
#endif
#ifdef DEPTH_DISTANCE
	out vec3 vPosition;
#endif
#ifdef USE_CLIP_PLANE
	out vec3 vClipCoord;
#endif

flat out int vLayer;

#ifdef SHARED_SPB
	flat out int vSPBIndex;
#endif


#include "v130/shared/defren/sanitize_position.glsl"


#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED || defined GS_RENDER_STEREO

#if defined DEPTH_OFFSET
	#include "v130/shared/defren/skin/depth_offset.glsl"
#endif

void emitCorner( in int layer, in vec4 position, in vec4 preTransformedPosition ){
	gl_Position = preTransformedPosition;
	
	#ifdef SHARED_SPB
	vSPBIndex = spbIndex;
	#endif
	
	#ifdef PERSPECTIVE_TO_LINEAR
		vDepth = dot( pTransformZ[ layer ], position );
	#endif
	#ifdef DEPTH_DISTANCE
		vPosition = pMatrixV[ layer ] * position;
	#endif
	#ifdef USE_CLIP_PLANE
		vClipCoord = pMatrixV[ layer ] * position;
	#endif
	
	#if defined DEPTH_OFFSET
		#ifdef GS_RENDER_CUBE
			applyDepthOffset( 0 );
		#else
			applyDepthOffset( layer );
		#endif
	#endif
	
	vLayer = layer;
	
	gl_Layer = layer;
	gl_PrimitiveID = gl_PrimitiveIDIn;
	
	EmitVertex();
}

void emitCorner( in int layer, in vec4 position ){
	emitCorner( layer, position, pMatrixVP[ layer ] * position );
}

#endif


// render cube
#ifdcef GS_RENDER_CUBE

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

#endif

// render stereo
#ifdef GS_RENDER_STEREO

void main( void ){
	int eye;
	
	#ifdef GS_INSTANCING
	eye = gl_InvocationID;
	#else
	for( eye=0; eye<2; eye++ ){
	#endif
		
		int i;
		for( i=0; i<3; i++ ){
			emitCorner( eye, gl_in[ i ].gl_Position );
		}
		EndPrimitive();
		
	#ifndef GS_INSTANCING
	}
	#endif
}

#endif // GS_RENDER_STEREO
