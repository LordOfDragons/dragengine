#include "shared/preamble.glsl"

// layout specifications
#if LAYERED_RENDERING_STEREO
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
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/defren/skin/ubo_instance_parameters.glsl"

#define GEOMETRY_SHADER_INPUT_SIZE 1
#include "shared/interface/skin/geometry.glsl"


const vec2 bc[4] = vec2[4](vec2(-0.5,  0.5), vec2(-0.5, -0.5), vec2( 0.5,  0.5), vec2( 0.5, -0.5));
const vec2 tc[4] = vec2[4](vec2( 0.0,  0.0), vec2( 0.0,  1.0), vec2( 1.0,  0.0), vec2( 1.0,  1.0));


/*
void emitParticle(in int layer, in mat2 rotmat, in vec3 normal, in vec3 tangent, in vec3 bitangent){
	// z and w stays the same for all vertices
	vec3 transformed = pMatrixV[layer] * gl_in[0].gl_Position;
	
	vec4 position = vec4(transformed, 1);
	
	// emit vertices
	int i;
	for(i=0; i<4; i++){
		position.xy = rotmat * bc[i] + transformed.xy;
		
		gl_Position = pMatrixP[layer] * position;
		
		geometryShaderDefaultOutputs(i, layer);
		
		vReflectDir = vec3(position);
		vFadeZ = position.z;
		vClipCoord = vec3(position);
		vSkinClipCoord = vec3(position);
		vPosition = vec3(position);
		vNormal = normal;
		vTangent = tangent;
		vBitangent = bitangent;
		
		EmitVertex();
	}
	
	EndPrimitive();
}
*/

void main(void){
	// NOTE: quest requires EmitVertex to be called in main()
	
	vec4 tempRotMat; // cos(z), -sin(z), sin(z), cos(z)
	tempRotMat.xw = cos(vec2(vGSParticle0[0].z));
	tempRotMat.yz = sin(vec2(vGSParticle0[0].z)) * vec2(-1.0, 1.0);
	tempRotMat *= vec4(vGSParticle0[0].x); // tempRotMat * size
	mat2 rotmat = mat2(tempRotMat.x, tempRotMat.y, tempRotMat.z, tempRotMat.w);
	
	vec3 normal = vec3(0.0, 0.0, -1.0);
	vec3 tangent = vec3(rotmat * vec2(1.0, 0.0), 0.0);
	vec3 bitangent = vec3(rotmat * vec2(0.0, -1.0), 0.0);
	
	// emit particles
	int layer;
	#if LAYERED_RENDERING_STEREO
		#ifdef GS_INSTANCING
			layer = gl_InvocationID;
		#else
			for(layer=0; layer<2; layer++){ // left and right eye
		#endif
		
	#else
		layer = vGSLayer(0);
	#endif
	
	// emitParticle(layer, rotmat, normal, tangent, bitangent)
	
	// z and w stays the same for all vertices
	vec3 transformed = pMatrixV[layer] * gl_in[0].gl_Position;
	
	vec4 position = vec4(transformed, 1);
	
	// emit vertices
	int i;
	for(i=0; i<4; i++){
		position.xy = rotmat * bc[i] + transformed.xy;
		
		gl_Position = pMatrixP[layer] * position;
		
		geometryShaderDefaultOutputs(i, layer);
		
		vTCColor = tc[i];
		vTCColorTintMask = tc[i];
		vTCNormal = tc[i];
		vTCReflectivity = tc[i];
		vTCEmissivity = tc[i];
		vTCRefractionDistort = tc[i];
		vTCAO = tc[i];
		
		vReflectDir = vec3(position);
		vFadeZ = position.z;
		vClipCoord = vec3(position);
		vSkinClipCoord = vec3(position);
		vPosition = vec3(position);
		vNormal = normal;
		vTangent = tangent;
		vBitangent = bitangent;
		
		EmitVertex();
	}
	
	EndPrimitive();
	
	// end emitParticle()
	
	
	#ifndef GS_INSTANCING
		#if LAYERED_RENDERING_STEREO
			}
		#endif
	#endif
}
