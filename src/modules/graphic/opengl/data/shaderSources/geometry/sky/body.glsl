#ifdef GS_INSTANCING
	#ifndef OPENGLES
		#extension GL_ARB_gpu_shader5 : require
	#endif
#endif

#if defined GS_RENDER_STEREO
	#ifdef GS_INSTANCING
		layout( triangles, invocations=2 ) in;
		layout( triangle_strip, max_vertices=3 ) out;
	#else
		layout( triangles ) in;
		layout( triangle_strip, max_vertices=6 ) out;
	#endif
#endif

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

in vec2 vGSTexCoord[ 3 ];

out vec2 vTexCoord;

flat out int vLayer;

void main( void ){
	int eye;
	#ifdef GS_INSTANCING
	eye = gl_InvocationID;
	#else
	for( eye=0; eye<2; eye++ ){
	#endif
		
		int corner;
		for( corner=0; corner<3; corner++ ){
			gl_Position = pMatrixSkyBody[ eye ] * gl_in[ corner ].gl_Position;
			
			// make sure Z is exactly -1 after transformation for depth test to work correctly
			gl_Position.z = -gl_Position.w;
			
			vTexCoord = vGSTexCoord[ corner ];
			vLayer = eye;
			
			gl_Layer = eye;
			gl_PrimitiveID = gl_PrimitiveIDIn;
			
			EmitVertex();
		}
		
		EndPrimitive();
		
	#ifndef GS_INSTANCING
	}
	#endif
}
