#ifdef GS_INSTANCING
	#extension GL_ARB_gpu_shader5 : require
#endif

#include "shared/defren/skin/macros_geometry.glsl"

// layout specifications
#ifdef GS_RENDER_STEREO
	#ifdef GS_INSTANCING
		layout( lines_adjacency, invocations=2 ) in;
	#else
		layout( lines_adjacency ) in;
	#endif
#else
	layout( lines_adjacency ) in;
#endif

#define USE_SHEETS 1

#ifdef USE_SHEETS
	// OpenGL requires these minimum limits: MaxVertices=256, MaxComponents=1024.
	// the maximum number of components written per invocation is:
	// - particle parameters: 23 components
	// - opengl internal parameters: 6 components (gl_Position, gl_Layer, gl_PrimitiveID)
	// hence 29 components in total. this results in the maximum supported vertices of:
	//   floor(MaxComponents / 29) = 35
	// sice 4 vertices are required for a full sheet the maximum number of sheets is:
	//   floor(35 / 4) = 8
	// and thus maximum number of vertixes 32 (8*4)
	#ifdef GS_RENDER_STEREO
		#ifdef GS_INSTANCING
			layout( triangle_strip, max_vertices=32 ) out;
		#else
			layout( triangle_strip, max_vertices=64 ) out;
		#endif
	#else
		layout( triangle_strip, max_vertices=32 ) out;
	#endif
	
#else
	#ifdef GS_RENDER_STEREO
		#ifdef GS_INSTANCING
			layout( triangle_strip, max_vertices=4 ) out;
		#else
			layout( triangle_strip, max_vertices=8 ) out;
		#endif
	#else
		layout( triangle_strip, max_vertices=4 ) out;
	#endif
#endif



// Uniform Parameters
///////////////////////

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/defren/skin/ubo_texture_parameters.glsl"
#include "shared/defren/skin/ubo_instance_parameters.glsl"
#include "shared/defren/skin/ubo_dynamic_parameters.glsl"

#ifdef NODE_GEOMETRY_UNIFORMS
NODE_GEOMETRY_UNIFORMS
#endif



// Inputs
///////////

in vec3 vParticle0[ 4 ]; // size, emissivity, rotation
in vec4 vParticle1[ 4 ]; // red, green, blue, transparency

#ifdef SHARED_SPB
	flat in int vGSSPBIndex[ 4 ];
	#define spbIndex vGSSPBIndex[0]
	
	#ifdef GS_RENDER_CUBE
		flat in int vGSSPBFlags[ 4 ];
		#define spbFlags vGSSPBFlags[0]
	#endif
	
	#include "shared/defren/skin/shared_spb_redirect.glsl"
#endif

#include "shared/defren/skin/shared_spb_texture_redirect.glsl"



// Outputs
////////////

out vec2 vTCColor;
#ifdef TEXTURE_COLOR_TINT_MASK
	out vec2 vTCColorTintMask;
#endif
#ifdef TEXTURE_NORMAL
	out vec2 vTCNormal;
#endif
#ifdef TEXTURE_REFLECTIVITY
	out vec2 vTCReflectivity;
#endif
#ifdef WITH_EMISSIVITY
	out vec2 vTCEmissivity;
#endif
#ifdef TEXTURE_REFRACTION_DISTORT
	out vec2 vTCRefractionDistort;
#endif
#ifdef TEXTURE_AO
	out vec2 vTCAO;
#endif

#ifdef CLIP_PLANE
	out vec3 vClipCoord;
#endif
#ifdef SKIN_CLIP_PLANE
	out vec3 vSkinClipCoord;
#endif
#ifdef DEPTH_DISTANCE
	out vec3 vPosition;
#endif
out vec3 vNormal;
#ifdef WITH_TANGENT
	out vec3 vTangent;
#endif
#ifdef WITH_BITANGENT
	out vec3 vBitangent;
#endif

#ifdef WITH_REFLECT_DIR
	out vec3 vReflectDir;
#endif
#ifdef FADEOUT_RANGE
	out float vFadeZ;
#endif

out vec4 vParticleColor; // from curve property
#ifdef WITH_EMISSIVITY
	out float vParticleEmissivity; // from curve property
#endif

#ifdef SHARED_SPB
	flat out int vSPBIndex;
#endif

#ifdef GS_RENDER_STEREO
	flat out int vLayer;
#else
	const int vLayer = 0;
#endif



// Constants
//////////////

const vec3 particleNormal = vec3( 0, 0, -1 );
const vec3 particleTangent = vec3( 1, 0, 0 );
const vec3 particleBitangent = vec3( 0, -1, 0 );

const vec2 tc1 = vec2( 0, 0 );
const vec2 tc2 = vec2( 1, 0 );
const vec2 tc3 = vec2( 0, 1 );
const vec2 tc4 = vec2( 1, 1 );

const vec3 lup = vec3( 0, 1, 0 );
const float epsilon = 0.00001;
const float pi = 3.14159265;



// Main Function
//////////////////

void emitCorner( in int corner, in vec4 position, in vec3 offset, in vec2 tc, in int layer ){
	position.xyz -= offset;
	
	gl_Position = pMatrixP[ layer ] * position;
	
	#ifdef SHARED_SPB
	vSPBIndex = spbIndex;
	#endif
	
	vParticleColor = vParticle1[ corner ];
	#ifdef WITH_EMISSIVITY
		vParticleEmissivity = vParticle0[ corner ].y;
	#endif
	
	vTCColor = tc;
	#ifdef TEXTURE_COLOR_TINT_MASK
		vTCColorTintMask = tc;
	#endif
	#ifdef TEXTURE_NORMAL
		vTCNormal = tc;
	#endif
	#ifdef TEXTURE_REFLECTIVITY
		vTCReflectivity = tc;
	#endif
	#ifdef WITH_EMISSIVITY
		vTCEmissivity = tc;
	#endif
	#ifdef TEXTURE_REFRACTION_DISTORT
		vTCRefractionDistort = tc;
	#endif
	#ifdef TEXTURE_AO
		vTCAO = tc;
	#endif
	#ifdef WITH_REFLECT_DIR
		vReflectDir = vec3( position );
	#endif
	#ifdef FADEOUT_RANGE
		vFadeZ = position.z;
	#endif
	#ifdef CLIP_PLANE
		vClipCoord = vec3( position );
	#endif
	#ifdef SKIN_CLIP_PLANE
		vSkinClipCoord = vec3( position );
	#endif
	#ifdef DEPTH_DISTANCE
		vPosition = vec3( position );
	#endif
	
	vNormal = particleNormal;
	#ifdef WITH_TANGENT
		vTangent = particleTangent;
	#endif
	#ifdef WITH_BITANGENT
		vBitangent = particleBitangent;
	#endif
	
	#ifdef GS_RENDER_STEREO
		vLayer = layer;
	#endif
	
	gl_Layer = layer;
	gl_PrimitiveID = gl_PrimitiveIDIn;
	
	EmitVertex();
}

void emitRibbon( in int layer ){
	// calculate the ribbon properties
	vec3 ribbonAxis1, ribbonAxis2;
	
	#ifdef USE_SHEETS
		mat3 matRot1, matRot2;
		//int sheetCount = 3;
		#define sheetCount pParticleSheetCount
		int s;
		
		float rotAngle = pi / float( sheetCount );
		vec3 sSc = vec3( sin( rotAngle ), -sin( rotAngle ), cos( rotAngle ) );
		vec3 c1 = vec3( 1.0 - sSc.z );
	#endif
	
	// calculate positions
	vec4 p[ 4 ];
	int i;
	
	for( i=0; i<4; i++ ){
		p[ i ] = vec4( pMatrixV[ layer ] * gl_in[ i ].gl_Position, 1 );
	}
	
	
	
	// calculate first ribbon axis and rotation matrix
	vec3 up = normalize( vec3( -p[ 1 ] ) );
	
	ribbonAxis1 = cross( up, vec3( p[ 2 ] ) - vec3( p[ 0 ] ) );
	float len = length( ribbonAxis1 );
	if( len < epsilon ){
		ribbonAxis1 = vec3( 1, 0, 0 );
		
	}else{
		ribbonAxis1 /= len;
	}
	if( ribbonAxis1.y < 0 ){
		ribbonAxis1 = -ribbonAxis1;
	}
	
	#ifdef USE_SHEETS
		//vec4 view2 = vec4( cross( ribbonAxis1, up ), 1 );
		//vec4 view2 = vec4( normalize( cross( vec3( p[ 2 ] ) - vec3( p[ 0 ] ), up ) ), 1 );
		vec4 view2 = vec4( normalize( vec3( p[ 2 ] ) - vec3( p[ 0 ] ) ), 1 );
		vec3 v1 = view2.xxx * view2.xyz * c1 + view2.wzy * sSc.zxy;
		vec3 v2 = view2.xyy * view2.yyz * c1 + view2.zwx * sSc.yzx;
		vec3 v3 = view2.xyz * view2.zzz * c1 + view2.yxw * sSc.xyz;
		matRot1 = mat3( v1, v2, v3 );
	#endif
	
	ribbonAxis1 *= vec3( vParticle0[ 1 ].x * 0.5 );
	
	
	
	// calculate second ribbon axis and rotation matrix
	up = normalize( vec3( -p[ 2 ] ) );
	
	ribbonAxis2 = cross( up, vec3( p[ 3 ] ) - vec3( p[ 1 ] ) );
	len = length( ribbonAxis2 );
	if( len < epsilon ){
		ribbonAxis2 = vec3( 1, 0, 0 );
		
	}else{
		ribbonAxis2 /= len;
	}
	if( ribbonAxis1.y < 0 ){
		ribbonAxis1 = -ribbonAxis1;
	}
	
	if( dot( vec2( ribbonAxis1 ), vec2( ribbonAxis2 ) ) < 0 ){
		ribbonAxis1 = -ribbonAxis1;
	}
	
	#ifdef USE_SHEETS
		//view2 = vec4( cross( ribbonAxis2, up ), 1 );
		//view2 = vec4( normalize( cross( vec3( p[ 3 ] ) - vec3( p[ 1 ] ), up ) ), 1 );
		view2 = vec4( normalize( vec3( p[ 3 ] ) - vec3( p[ 1 ] ) ), 1 );
		v1 = view2.xxx * view2.xyz * c1 + view2.wzy * sSc.zxy;
		v2 = view2.xyy * view2.yyz * c1 + view2.zwx * sSc.yzx;
		v3 = view2.xyz * view2.zzz * c1 + view2.yxw * sSc.xyz;
		matRot2 = mat3( v1, v2, v3 );
	#endif
	
	ribbonAxis2 *= vec3( vParticle0[ 2 ].x * 0.5 );
	
	
	
	// generate billboard(s)
	#ifdef USE_SHEETS
	for( i=0; i<sheetCount; i++ ){
	#endif
		emitCorner( 1, p[ 1 ], -ribbonAxis1, tc1, layer );
		emitCorner( 2, p[ 2 ], -ribbonAxis2, tc2, layer );
		emitCorner( 1, p[ 1 ], ribbonAxis1, tc3, layer );
		emitCorner( 2, p[ 2 ], ribbonAxis2, tc4, layer );
		
		EndPrimitive();
		
	#ifdef USE_SHEETS
		ribbonAxis1 = matRot1 * ribbonAxis1;
		ribbonAxis2 = matRot2 * ribbonAxis2;
	}
	#endif
}



void main( void ){
	// emit ribbons
	int layer;
	
	#ifdef GS_INSTANCING
	layer = gl_InvocationID;
	#else
	#ifdef GS_RENDER_STEREO
		#define LAYER_COUNT 2
	#else
		#define LAYER_COUNT 1
	#endif
	for( layer=0; layer<LAYER_COUNT; layer++ ){
	#endif
		
		emitRibbon( layer );
		
	#ifndef GS_INSTANCING
	}
	#endif
}
