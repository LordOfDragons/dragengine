#if defined GS_RENDER_STEREO && defined GS_INSTANCING
	#ifndef OPENGLES
		#extension GL_ARB_gpu_shader5 : require
	#endif
#endif

#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

#ifdef GS_RENDER_STEREO
	#ifdef GS_INSTANCING
		layout( points, invocations=2 ) in;
		layout( triangle_strip, max_vertices=4 ) out;
	#else
		layout( points ) in;
		layout( triangle_strip, max_vertices=8 ) out;
	#endif
#else
	layout( points ) in;
	layout( triangle_strip, max_vertices=4 ) out;
#endif

#include "shared/ubo_defines.glsl"
#include "shared/defren/light/ubo_instance_parameters.glsl"

in vec3 vGSParticleLightColor[ 1 ];
in float vGSParticleLightRange[ 1 ];

#ifdef VS_RENDER_STEREO
	in int vGSLayer[ 1 ];
#endif

out vec3 vLightVolumePos;
out vec3 vParticleLightPosition;
out vec3 vParticleLightColor;
out float vParticleLightRange;

flat out int vLayer;


void emitCorner( in vec3 position, in vec3 range, in int layer ){
	vParticleLightPosition = position;
	vParticleLightColor = vGSParticleLightColor[ 0 ];
	vParticleLightRange = vGSParticleLightRange[ 0 ];
	
	vLightVolumePos = position + range;
	
	gl_Position = pMatrixMVP[ layer ] * vec4( vLightVolumePos, 1 );
	
	vLayer = layer;
	
	gl_Layer = layer;
	gl_PrimitiveID = gl_PrimitiveIDIn;
	
	EmitVertex();
}

void emitParticle( in int layer ){
	vec3 position = pMatrixMV[ layer ] * vec4( gl_in[ 0 ].gl_Position.xyz, 1 );
	vec3 range = vec3( vGSParticleLightRange[ 0 ], -vGSParticleLightRange[ 0 ], 0 );
	
	emitCorner( position, range.yyz, layer ); // -range, -range
	emitCorner( position, range.yxz, layer ); // -range, +range
	emitCorner( position, range.xyz, layer ); // +range, -range
	emitCorner( position, range.xxz, layer ); // +range, +range
	
	EndPrimitive();
}

void main( void ){
	#ifdef GS_RENDER_STEREO
		int eye;
		#ifdef GS_INSTANCING
		eye = gl_InvocationID;
		#else
		for( eye=0; eye<2; eye++ ){
		#endif
			emitParticle( eye );
		#ifndef GS_INSTANCING
		}
		#endif
		
	#elif defined VS_RENDER_STEREO
		emitParticle( vGSLayer[ 0 ] );
	#else
		emitParticle( 0 );
	#endif
}
