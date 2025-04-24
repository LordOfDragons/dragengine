#ifdef GS_INSTANCING
	#ifndef OPENGLES
		#extension GL_ARB_gpu_shader5 : require
	#endif
#endif

// layout specifications
#ifdef GS_RENDER_STEREO
	#ifdef GS_INSTANCING
		layout(lines_adjacency, invocations=2) in;
	#else
		layout(lines_adjacency) in;
	#endif
#else
	layout(lines_adjacency) in;
#endif

#define USE_SHEETS

#ifdef USE_SHEETS
	// OpenGL requires these minimum limits: MaxVertices=256, MaxComponents=1024.
	// the maximum number of components written per invocation is:
	// - particle parameters: 23 components
	// - opengl internal parameters: 6 components (gl_Position, gl_Layer, gl_PrimitiveID)
	// hence 29 components in total. this results in the maximum supported vertices of:
	//   floor(MaxComponents / 29) = 35
	// since 4 vertices are required for a full sheet the maximum number of sheets is:
	//   floor(35 / 4) = 8
	// and thus maximum number of vertices 32 (8*4)
	//
	// this calculation is now more complicated. right now the maximum count of possible
	// particle parameters is 43. combined with the opengl internal parameters this yields
	// 49 components. this gives a maximum of floor(MaxComponents / 49) = 20 vertices.
	// with 4 vertices per sheet this yields floor(20 / 4) = 5. hence the maximum count
	// of sheets is 5 and the maximum count of vertices 20
	//
	// this problem can be solved by moving the calculation of vertices in the beam into
	// a compute shader filling a VBO. this way all kinds of sheet counts can be done
	// without even needing a geometry shader to begin with.
	#ifdef GS_RENDER_STEREO
		#ifdef GS_INSTANCING
			layout(triangle_strip, max_vertices=20) out;
		#else
			layout(triangle_strip, max_vertices=40) out;
		#endif
	#else
		layout(triangle_strip, max_vertices=20) out;
	#endif
	
#else
	#ifdef GS_RENDER_STEREO
		#ifdef GS_INSTANCING
			layout(triangle_strip, max_vertices=4) out;
		#else
			layout(triangle_strip, max_vertices=8) out;
		#endif
	#else
		layout(triangle_strip, max_vertices=4) out;
	#endif
#endif


#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"


in vec3 vParticle0[4]; // size, emissivity, rotation
in vec4 vParticle1[4]; // red, green, blue, transparency
flat in int vParticleSheetCount[4];
flat in int vGSSPBIndex[4];
flat in int vGSSPBFlags[4];

#include "shared/defren/skin/shared_spb_texture_redirect.glsl"


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


const vec3 particleNormal = vec3(0, 0, -1);
const vec3 particleTangent = vec3(1, 0, 0);
const vec3 particleBitangent = vec3(0, -1, 0);

const vec2 tc1 = vec2(0, 0);
const vec2 tc2 = vec2(1, 0);
const vec2 tc3 = vec2(0, 1);
const vec2 tc4 = vec2(1, 1);

const vec3 lup = vec3(0, 1, 0);
const float epsilon = 0.00001;
const float pi = 3.14159265;


void emitCorner(in int corner, in vec4 position, in vec3 offset, in vec2 tc, in int layer){
	position.xyz -= offset;
	
	gl_Position = pMatrixP[layer] * position;
	
	vSPBIndex = vGSSPBIndex[0];
	vSPBFlags = vGSSPBFlags[0];
	vParticleColor = vParticle1[corner];
	vParticleEmissivity = vParticle0[corner].y;
	
	vTCColor = tc;
	vTCColorTintMask = tc;
	vTCNormal = tc;
	vTCReflectivity = tc;
	vTCEmissivity = tc;
	vTCRefractionDistort = tc;
	vTCAO = tc;
	vReflectDir = vec3(position);
	vFadeZ = position.z;
	vClipCoord = vec3(position);
	vSkinClipCoord = vec3(position);
	vPosition = vec3(position);
	vNormal = particleNormal;
	vTangent = particleTangent;
	vBitangent = particleBitangent;
	
	vLayer = layer;
	gl_Layer = layer;
	gl_PrimitiveID = gl_PrimitiveIDIn;
	
	//EmitVertex();
}

/*
void emitRibbon(in int layer){
	// calculate the ribbon properties
	vec3 ribbonAxis1, ribbonAxis2;
	
	#ifdef USE_SHEETS
		mat3 matRot1, matRot2;
		//int sheetCount = 3;
		// passing through sheet count to avoid using SSBO in geometry shader (pParticleSheetCount)
		#define sheetCount vParticleSheetCount[0]
		int s;
		
		float rotAngle = pi / float(sheetCount);
		vec3 sSc = vec3(sin(rotAngle), -sin(rotAngle), cos(rotAngle));
		vec3 c1 = vec3(1.0 - sSc.z);
	#endif
	
	// calculate positions
	vec4 p[4];
	int i;
	
	for(i=0; i<4; i++){
		p[i] = vec4(pMatrixV[layer] * gl_in[i].gl_Position, 1);
	}
	
	
	
	// calculate first ribbon axis and rotation matrix
	vec3 up = normalize(vec3(-p[1]));
	
	ribbonAxis1 = cross(up, vec3(p[2]) - vec3(p[0]));
	float len = length(ribbonAxis1);
	if(len < epsilon){
		ribbonAxis1 = vec3(1, 0, 0);
		
	}else{
		ribbonAxis1 /= len;
	}
	if(ribbonAxis1.y < 0.0){
		ribbonAxis1 = -ribbonAxis1;
	}
	
	#ifdef USE_SHEETS
		//vec4 view2 = vec4(cross(ribbonAxis1, up), 1);
		//vec4 view2 = vec4(normalize(cross(vec3(p[2]) - vec3(p[0]), up)), 1);
		vec4 view2 = vec4(normalize(vec3(p[2]) - vec3(p[0])), 1);
		vec3 v1 = view2.xxx * view2.xyz * c1 + view2.wzy * sSc.zxy;
		vec3 v2 = view2.xyy * view2.yyz * c1 + view2.zwx * sSc.yzx;
		vec3 v3 = view2.xyz * view2.zzz * c1 + view2.yxw * sSc.xyz;
		matRot1 = mat3(v1, v2, v3);
	#endif
	
	ribbonAxis1 *= vec3(vParticle0[1].x * 0.5);
	
	
	
	// calculate second ribbon axis and rotation matrix
	up = normalize(vec3(-p[2]));
	
	ribbonAxis2 = cross(up, vec3(p[3]) - vec3(p[1]));
	len = length(ribbonAxis2);
	if(len < epsilon){
		ribbonAxis2 = vec3(1, 0, 0);
		
	}else{
		ribbonAxis2 /= len;
	}
	if(ribbonAxis1.y < 0.0){
		ribbonAxis1 = -ribbonAxis1;
	}
	
	if(dot(vec2(ribbonAxis1), vec2(ribbonAxis2)) < 0.0){
		ribbonAxis1 = -ribbonAxis1;
	}
	
	#ifdef USE_SHEETS
		//view2 = vec4(cross(ribbonAxis2, up), 1);
		//view2 = vec4(normalize(cross(vec3(p[3]) - vec3(p[1]), up)), 1);
		view2 = vec4(normalize(vec3(p[3]) - vec3(p[1])), 1);
		v1 = view2.xxx * view2.xyz * c1 + view2.wzy * sSc.zxy;
		v2 = view2.xyy * view2.yyz * c1 + view2.zwx * sSc.yzx;
		v3 = view2.xyz * view2.zzz * c1 + view2.yxw * sSc.xyz;
		matRot2 = mat3(v1, v2, v3);
	#endif
	
	ribbonAxis2 *= vec3(vParticle0[2].x * 0.5);
	
	
	
	// generate billboard(s)
	#ifdef USE_SHEETS
	for(i=0; i<sheetCount; i++){
	#endif
		emitCorner(1, p[1], -ribbonAxis1, tc1, layer);
		emitCorner(2, p[2], -ribbonAxis2, tc2, layer);
		emitCorner(1, p[1], ribbonAxis1, tc3, layer);
		emitCorner(2, p[2], ribbonAxis2, tc4, layer);
		EndPrimitive();
		
	#ifdef USE_SHEETS
		ribbonAxis1 = matRot1 * ribbonAxis1;
		ribbonAxis2 = matRot2 * ribbonAxis2;
	}
	#endif
}
*/


void main(void){
	// NOTE: quest requires EmitVertex to be called in main()
	int layer;
	
	#ifdef GS_INSTANCING
		layer = gl_InvocationID;
	#else
		#ifdef GS_RENDER_STEREO
			for(layer=0; layer<2; layer++){
		#else
			layer = 0;
		#endif
	#endif
	
	
	// emitRibbon(layer)
	
	// calculate the ribbon properties
	vec3 ribbonAxis1, ribbonAxis2;
	
	#ifdef USE_SHEETS
		mat3 matRot1, matRot2;
		//int sheetCount = 3;
		// passing through sheet count to avoid using SSBO in geometry shader (pParticleSheetCount)
		#define sheetCount vParticleSheetCount[0]
		int s;
		
		float rotAngle = pi / float(sheetCount);
		vec3 sSc = vec3(sin(rotAngle), -sin(rotAngle), cos(rotAngle));
		vec3 c1 = vec3(1.0 - sSc.z);
	#endif
	
	// calculate positions
	vec4 p[4];
	int i;
	
	for(i=0; i<4; i++){
		p[i] = vec4(pMatrixV[layer] * gl_in[i].gl_Position, 1);
	}
	
	
	
	// calculate first ribbon axis and rotation matrix
	vec3 up = normalize(vec3(-p[1]));
	
	ribbonAxis1 = cross(up, vec3(p[2]) - vec3(p[0]));
	float len = length(ribbonAxis1);
	if(len < epsilon){
		ribbonAxis1 = vec3(1, 0, 0);
		
	}else{
		ribbonAxis1 /= len;
	}
	if(ribbonAxis1.y < 0.0){
		ribbonAxis1 = -ribbonAxis1;
	}
	
	#ifdef USE_SHEETS
		//vec4 view2 = vec4(cross(ribbonAxis1, up), 1);
		//vec4 view2 = vec4(normalize(cross(vec3(p[2]) - vec3(p[0]), up)), 1);
		vec4 view2 = vec4(normalize(vec3(p[2]) - vec3(p[0])), 1);
		vec3 v1 = view2.xxx * view2.xyz * c1 + view2.wzy * sSc.zxy;
		vec3 v2 = view2.xyy * view2.yyz * c1 + view2.zwx * sSc.yzx;
		vec3 v3 = view2.xyz * view2.zzz * c1 + view2.yxw * sSc.xyz;
		matRot1 = mat3(v1, v2, v3);
	#endif
	
	ribbonAxis1 *= vec3(vParticle0[1].x * 0.5);
	
	
	
	// calculate second ribbon axis and rotation matrix
	up = normalize(vec3(-p[2]));
	
	ribbonAxis2 = cross(up, vec3(p[3]) - vec3(p[1]));
	len = length(ribbonAxis2);
	if(len < epsilon){
		ribbonAxis2 = vec3(1, 0, 0);
		
	}else{
		ribbonAxis2 /= len;
	}
	if(ribbonAxis1.y < 0.0){
		ribbonAxis1 = -ribbonAxis1;
	}
	
	if(dot(vec2(ribbonAxis1), vec2(ribbonAxis2)) < 0.0){
		ribbonAxis1 = -ribbonAxis1;
	}
	
	#ifdef USE_SHEETS
		//view2 = vec4(cross(ribbonAxis2, up), 1);
		//view2 = vec4(normalize(cross(vec3(p[3]) - vec3(p[1]), up)), 1);
		view2 = vec4(normalize(vec3(p[3]) - vec3(p[1])), 1);
		v1 = view2.xxx * view2.xyz * c1 + view2.wzy * sSc.zxy;
		v2 = view2.xyy * view2.yyz * c1 + view2.zwx * sSc.yzx;
		v3 = view2.xyz * view2.zzz * c1 + view2.yxw * sSc.xyz;
		matRot2 = mat3(v1, v2, v3);
	#endif
	
	ribbonAxis2 *= vec3(vParticle0[2].x * 0.5);
	
	
	
	// generate billboard(s)
	#ifdef USE_SHEETS
	for(i=0; i<sheetCount; i++){
	#endif
		emitCorner(1, p[1], -ribbonAxis1, tc1, layer);
		EmitVertex();
		
		emitCorner(2, p[2], -ribbonAxis2, tc2, layer);
		EmitVertex();
		
		emitCorner(1, p[1], ribbonAxis1, tc3, layer);
		EmitVertex();
		
		emitCorner(2, p[2], ribbonAxis2, tc4, layer);
		EmitVertex();
		
		EndPrimitive();
		
	#ifdef USE_SHEETS
		ribbonAxis1 = matRot1 * ribbonAxis1;
		ribbonAxis2 = matRot2 * ribbonAxis2;
	}
	#endif
	
	// end emitRibbon()
	
	
	#ifndef GS_INSTANCING
		#ifdef GS_RENDER_STEREO
			}
		#endif
	#endif
}
