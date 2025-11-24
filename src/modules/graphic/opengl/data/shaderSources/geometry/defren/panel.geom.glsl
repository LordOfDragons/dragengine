#include "shared/preamble.glsl"

#ifdef LAYERED_RENDERING_STEREO
	#ifdef GS_INSTANCING
		layout( triangles, invocations=2 ) in;
		layout( triangle_strip, max_vertices=3 ) out;
	#else
		layout( triangles ) in;
		layout( triangle_strip, max_vertices=6 ) out;
	#endif
	
#else
	layout( triangles ) in;
	layout( triangle_strip, max_vertices=3 ) out;
#endif


UNIFORM_BIND(1) uniform mat4 pMatrixVP;
UNIFORM_BIND(2) uniform mat4 pMatrixVP2;

#include "shared/interface/2d/geometry.glsl"


#ifdef LAYERED_RENDERING_STEREO

void main( void ){
	int eye;
	
	#ifdef GS_INSTANCING
	eye = gl_InvocationID;
	#else
	for( eye=0; eye<2; eye++ ){
	#endif
		
		int i;
		for( i=0; i<3; i++ ){
			gl_Position = ( eye == 0 ? pMatrixVP : pMatrixVP2 ) * gl_in[ i ].gl_Position;
			geometryShaderDefaultOutputs(i, eye);
			EmitVertex();
		}
		EndPrimitive();
		
	#ifndef GS_INSTANCING
	}
	#endif
}

#else

void main( void ){
	int i;
	for( i=0; i<3; i++ ){
		gl_Position = gl_in[ i ].gl_Position;
		geometryShaderDefaultOutputs(i);
		EmitVertex();
	}
	
	EndPrimitive();
}

#endif
