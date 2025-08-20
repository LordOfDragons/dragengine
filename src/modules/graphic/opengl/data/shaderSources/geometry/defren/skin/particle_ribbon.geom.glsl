#include "shared/preamble.glsl"

// layout specifications
#if LAYERED_RENDERING_STEREO
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
	// - particle parameters: 49 components
	// - opengl internal parameters: 6 components (gl_Position, gl_Layer, gl_PrimitiveID)
	// hence 55 components in total. this results in the maximum supported vertices of:
	//   floor(MaxComponents / 55) = 18
	// since 4 vertices are required for a full sheet the maximum number of sheets is:
	//   floor(18 / 4) = 4
	// and thus maximum number of vertices 16 (4*4)
	//
	// hence the maximum count of sheets is 4 and the maximum count of vertices 16
	//
	// this problem can be solved by moving the calculation of vertices in the beam into
	// a compute shader filling a VBO. this way all kinds of sheet counts can be done
	// without even needing a geometry shader to begin with.
	#if LAYERED_RENDERING_STEREO
		#ifdef GS_INSTANCING
			layout(triangle_strip, max_vertices=16) out;
		#else
			layout(triangle_strip, max_vertices=32) out;
		#endif
		
	#else
		layout(triangle_strip, max_vertices=16) out;
	#endif
	
#else
	#if LAYERED_RENDERING_STEREO
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

#define GEOMETRY_SHADER_INPUT_SIZE 4
#include "shared/interface/skin/geometry.glsl"

#include "shared/defren/skin/shared_spb_texture_redirect.glsl"


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
	
	geometryShaderDefaultOutputs(corner, layer);
	
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
	
	//EmitVertex();
}

/*
void emitRibbon(in int layer){
	// calculate the ribbon properties
	vec3 ribbonAxis1, ribbonAxis2;
	
	#ifdef USE_SHEETS
		mat3 matRot1, matRot2;
		//int sheetCount = 3;
		// passing through sheet count to avoid using SSBO in geometry shader (getParticleSheetCount())
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
	
	ribbonAxis1 *= vec3(vGSParticle0[1].x * 0.5);
	
	
	
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
	
	ribbonAxis2 *= vec3(vGSParticle0[2].x * 0.5);
	
	
	
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
	#if LAYERED_RENDERING_STEREO
		#ifdef GS_INSTANCING
			layer = gl_InvocationID;
		#else
			for(layer=0; layer<2; layer++){ // left and right eye
		#endif
		
	#else
		layer = vGSLayer(0);
	#endif
	
	
	// emitRibbon(layer)
	
	// calculate the ribbon properties
	vec3 ribbonAxis1, ribbonAxis2;
	
	#ifdef USE_SHEETS
		mat3 matRot1, matRot2;
		//int sheetCount = 3;
		// passing through sheet count to avoid using SSBO in geometry shader
		int s;
		
		VARCONST int sheetCount = vGSParticleSheetCount[0];
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
	
	ribbonAxis1 *= vec3(vGSParticle0[1].x * 0.5);
	
	
	
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
	
	ribbonAxis2 *= vec3(vGSParticle0[2].x * 0.5);
	
	
	
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
		#if LAYERED_RENDERING_STEREO
			}
		#endif
	#endif
}
