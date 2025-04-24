#ifdef GS_INSTANCING
	#ifndef OPENGLES
		#extension GL_ARB_gpu_shader5 : require
	#endif
#endif

// layout specifications
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
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/defren/skin/ubo_instance_parameters.glsl"


in vec3 vParticle0[1]; // size, emissivity, rotation
in vec4 vParticle1[1]; // red, green, blue, transparency
flat in int vParticleSheetCount[1];
flat in int vGSSPBIndex[1];
flat in int vGSSPBFlags[1];


out vec2 vTCColor;
out vec2 vTCColorTintMask;
out vec2 vTCNormal;
out vec2 vTCReflectivity;
out vec2 vTCEmissivity;
out vec2 vTCRefractionDistort;
out vec2 vTCAO;
out vec3 vClipCoord;
out vec3 vSkinClipCoord;
out vec3 vPosition;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBitangent;
out vec3 vReflectDir;
out float vFadeZ;
out vec4 vParticleColor; // from curve property
out float vParticleEmissivity; // from curve property
flat out int vSPBIndex;
flat out int vSPBFlags;
flat out int vLayer;


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
		
		vTCColor = tc[i];
		vSPBIndex = vGSSPBIndex[0];
		vSPBFlags = vGSSPBFlags[0];
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
		vParticleColor = vParticle1[0];
		vParticleEmissivity = vParticle0[0].y;
		vNormal = normal;
		vTangent = tangent;
		vBitangent = bitangent;
		
		vLayer = layer;
		gl_Layer = layer;
		gl_PrimitiveID = gl_PrimitiveIDIn;
		
		EmitVertex();
	}
	
	EndPrimitive();
}
*/

void main(void){
	// NOTE: quest requires EmitVertex to be called in main()
	
	vec4 tempRotMat; // cos(z), -sin(z), sin(z), cos(z)
	tempRotMat.xw = cos(vec2(vParticle0[0].z));
	tempRotMat.yz = sin(vec2(vParticle0[0].z)) * vec2(-1.0, 1.0);
	tempRotMat *= vec4(vParticle0[0].x); // tempRotMat * size
	mat2 rotmat = mat2(tempRotMat.x, tempRotMat.y, tempRotMat.z, tempRotMat.w);
	
	vec3 normal = vec3(0.0, 0.0, -1.0);
	vec3 tangent = vec3(rotmat * vec2(1.0, 0.0), 0.0);
	vec3 bitangent = vec3(rotmat * vec2(0.0, -1.0), 0.0);
	
	// emit particles
	int layer;
	
	#ifdef GS_INSTANCING
		layer = gl_InvocationID;
	#else
		#ifdef GS_RENDER_STEREO
			for(layer=0; layer<LAYER_COUNT; layer++){
		#else
			layer = 0;
		#endif
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
		
		vTCColor = tc[i];
		vSPBIndex = vGSSPBIndex[0];
		vSPBFlags = vGSSPBFlags[0];
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
		vParticleColor = vParticle1[0];
		vParticleEmissivity = vParticle0[0].y;
		vNormal = normal;
		vTangent = tangent;
		vBitangent = bitangent;
		
		vLayer = layer;
		gl_Layer = layer;
		gl_PrimitiveID = gl_PrimitiveIDIn;
		
		EmitVertex();
	}
	
	EndPrimitive();
	
	// end emitParticle()
	
	
	#ifndef GS_INSTANCING
		#ifdef GS_RENDER_STEREO
			}
		#endif
	#endif
}
