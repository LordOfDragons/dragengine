#if defined GS_RENDER_STEREO && defined GS_INSTANCING
	#ifndef OPENGLES
		#extension GL_ARB_gpu_shader5 : require
	#endif
#endif

precision HIGHP float;
precision HIGHP int;

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
	
	//EmitVertex();
}

/*
void emitParticle(in int layer){
	vec3 position = pMatrixMV[ layer ] * vec4( gl_in[ 0 ].gl_Position.xyz, 1 );
	vec3 range = vec3( vGSParticleLightRange[ 0 ], -vGSParticleLightRange[ 0 ], 0 );
	
	emitCorner(position, range.yyz, layer); // -range, -range
	emitCorner(position, range.yxz, layer); // -range, +range
	emitCorner(position, range.xyz, layer); // +range, -range
	emitCorner(position, range.xxz, layer); // +range, +range
	
	EndPrimitive();
}
*/

void main( void ){
	// NOTE: quest requires EmitVertex to be called in main()
	vec4 basePos = vec4(gl_in[0].gl_Position.xyz, 1);
	vec3 range = vec3(vGSParticleLightRange[0], -vGSParticleLightRange[0], 0);
	
	
	int layer;
	#ifdef GS_RENDER_STEREO
		#ifdef GS_INSTANCING
			layer = gl_InvocationID;
		#else
			for(layer=0; layer<2; layer++){ // left and right eye
		#endif
	#elif defined VS_RENDER_STEREO
		layer = vGSLayer[0];
	#else
		layer = 0;
	#endif
	
	
	// emitParticle(layer)
	vec3 position = pMatrixMV[layer] * basePos;
	
	emitCorner(position, range.yyz, layer); // -range, -range
	EmitVertex();
	
	emitCorner(position, range.yxz, layer); // -range, +range
	EmitVertex();
	
	emitCorner(position, range.xyz, layer); // +range, -range
	EmitVertex();
	
	emitCorner(position, range.xxz, layer); // +range, +range
	EmitVertex();
	
	EndPrimitive();
	// end emitParticle()
	
	
	#ifdef GS_RENDER_STEREO
		#ifndef GS_INSTANCING
			}
		#endif
	#endif
}
