#if defined GS_RENDER_STEREO && defined GS_INSTANCING
	#ifndef OPENGLES
		#extension GL_ARB_gpu_shader5 : require
	#endif
#endif

precision HIGHP float;
precision HIGHP int;

#ifdef GS_RENDER_STEREO
	#ifdef GS_INSTANCING
		layout(points, invocations=2) in;
		layout(triangle_strip, max_vertices=4) out;
	#else
		layout(points) in;
		layout(triangle_strip, max_vertices=8) out;
	#endif
#else
	layout(points) in;
	layout(triangle_strip, max_vertices=4) out;
#endif

#include "shared/ubo_defines.glsl"
#include "shared/defren/light/ubo_instance_parameters.glsl"

in vec3 vGSParticleLightColor[1];
in float vGSParticleLightRange[1];
in int vGSLayer[1];

out vec3 vLightVolumePos;
out vec3 vParticleLightPosition;
out vec3 vParticleLightColor;
out float vParticleLightRange;

flat out int vLayer;


void emitCorner(in vec3 position, in vec3 range, float zfactor, in int layer){
	vParticleLightPosition = position;
	vParticleLightColor = vGSParticleLightColor[0];
	vParticleLightRange = vGSParticleLightRange[0];
	
	vLightVolumePos = position + range;
	
	// in the particle case pMatrixMVP is actually set to pMatrixP
	gl_Position = pMatrixMVP[layer] * vec4(vLightVolumePos, 1);
	
	// rendering like this would place the billboard at the center
	// of the light. this would clip off light in front of the plane
	// if intersecting surfaces. to fix this the depth value has to
	// be shifted towards the camera without altering the projected
	// position of the billboard. the code below does this:
	//
	// gl_Position.xy /= gl_Position.ww;
	// gl_Position.w = 1;
	// vec4 t = pMatrixMVP[layer] * vec4(vLightVolumePos +
	//   mat3(pMatrixMV[layer]) * vec3(0,0,-vGSParticleLightRange[0]), 1);
	// gl_Position.z = t.z / t.w;
	// 
	// the perspective transformation can be avoided like this:
	// 
	// z' = z / w (uncorrected)
	// zc' = zc / wc (corrected)
	// zc' = (zc * w / wc) / w
	// 
	// hence the final code below:
	// 
	// vec4 t = pMatrixMVP[layer] * vec4(vLightVolumePos +
	//   mat3(pMatrixMV[layer]) * vec3(0,0,-vGSParticleLightRange[0]), 1);
	// gl_Position.z = t.z * gl_Position.w / t.w;
	// 
	// the z value can be precalculated since the z position is the same
	// for all corners in the billboard
	gl_Position.z = zfactor * gl_Position.w;
	
	vLayer = layer;
	
	gl_Layer = layer;
	gl_PrimitiveID = gl_PrimitiveIDIn;
	
	//EmitVertex();
}

/*
void emitParticle(in int layer){
	vec3 position = pMatrixMV[layer] * vec4(gl_in[0].gl_Position.xyz, 1);
	vec3 range = vec3(vGSParticleLightRange[0], -vGSParticleLightRange[0], 0);
	
	emitCorner(position, range.yyz, zshift, layer); // -range, -range
	emitCorner(position, range.xyz, zshift, layer); // +range, -range
	emitCorner(position, range.yxz, zshift, layer); // -range, +range
	emitCorner(position, range.xxz, zshift, layer); // +range, +range
	
	EndPrimitive();
}
*/

void main(void){
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
	#else
		layer = vGSLayer[0];
	#endif
	
	
	// emitParticle(layer)
	vec3 position = pMatrixMV[layer] * basePos;
	
	vec4 t = pMatrixMVP[layer] * vec4(position + mat3(pMatrixMV[layer]) * range.zzy, 1);
	float zfactor = t.z / t.w;
	
	emitCorner(position, range.yyz, zfactor, layer); // -range, -range
	EmitVertex();
	
	emitCorner(position, range.xyz, zfactor, layer); // +range, -range
	EmitVertex();
	
	emitCorner(position, range.yxz, zfactor, layer); // -range, +range
	EmitVertex();
	
	emitCorner(position, range.xxz, zfactor, layer); // +range, +range
	EmitVertex();
	
	EndPrimitive();
	// end emitParticle()
	
	
	#ifdef GS_RENDER_STEREO
		#ifndef GS_INSTANCING
			}
		#endif
	#endif
}
