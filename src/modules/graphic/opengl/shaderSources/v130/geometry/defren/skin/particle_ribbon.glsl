#include "v130/shared/defren/skin/macros_geometry.glsl"

// layout specifications
layout( lines_adjacency ) in;

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
	layout( triangle_strip, max_vertices=32 ) out;
	
#else
	layout( triangle_strip, max_vertices=4 ) out;
#endif



// Uniform Parameters
///////////////////////

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/skin/ubo_render_parameters.glsl"
#include "v130/shared/defren/skin/ubo_texture_parameters.glsl"
#include "v130/shared/defren/skin/ubo_instance_parameters.glsl"
#include "v130/shared/defren/skin/ubo_dynamic_parameters.glsl"

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
	
	#include "v130/shared/defren/skin/shared_spb_redirect.glsl"
#endif

#include "v130/shared/defren/skin/shared_spb_texture_redirect.glsl"



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
#ifdef DEPTH_ORTHOGONAL
	out float vZCoord;
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

#ifdef NODE_GEOMETRY_OUTPUTS
NODE_GEOMETRY_OUTPUTS
#endif



// Constants
//////////////

const vec3 particleNormal = vec3( 0.0, 0.0, -1.0 );
const vec3 particleTangent = vec3( 1.0, 0.0, 0.0 );
const vec3 particleBitangent = vec3( 0.0, -1.0, 0.0 );

const vec2 tc1 = vec2( 0.0, 0.0 );
const vec2 tc2 = vec2( 1.0, 0.0 );
const vec2 tc3 = vec2( 0.0, 1.0 );
const vec2 tc4 = vec2( 1.0, 1.0 );

const vec3 lup = vec3( 0.0, 1.0, 0.0 );
const float epsilon = 0.00001;
const float pi = 3.14159265;



// Main Function
//////////////////

void emitCorner( in int corner, in vec3 offset, in vec2 tc ){
	vec4 position = gl_in[ corner ].gl_Position;
	position.xyz -= offset;
	
	gl_Position = pMatrixP * position;
	
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
		vReflectDir = position.xyz;
	#endif
	#ifdef FADEOUT_RANGE
		vFadeZ = position.z;
	#endif
	#ifdef CLIP_PLANE
		vClipCoord = position.xyz;
	#endif
	#if ! defined GS_RENDER_CUBE && ! defined GS_RENDER_CASCADED && ! defined GS_RENDER_STEREO
		#ifdef DEPTH_ORTHOGONAL
			#ifdef NO_ZCLIP
				vZCoord = gl_Position.z * 0.5 + 0.5; // we have to do the normalization ourself
				gl_Position.z = 0.0;
			#else
				vZCoord = gl_Position.z;
			#endif
		#endif
	#endif
	#ifdef DEPTH_DISTANCE
		vPosition = position.xyz;
	#endif
	
	vNormal = particleNormal;
	#ifdef WITH_TANGENT
		vTangent = particleTangent;
	#endif
	#ifdef WITH_BITANGENT
		vBitangent = particleBitangent;
	#endif
	
	gl_Layer = 0;
	gl_PrimitiveID = gl_PrimitiveIDIn;
	
	EmitVertex();
}



void main( void ){
	// calculate the ribbon properties
	vec3 ribbonAxis1, ribbonAxis2;
	vec4 position;
	
	#ifdef USE_SHEETS
		mat3 matRot1, matRot2;
		//int sheetCount = 3;
		#define sheetCount pParticleSheetCount
		int s;
		
		float rotAngle = pi / float( sheetCount );
		vec3 sSc = vec3( sin( rotAngle ), -sin( rotAngle ), cos( rotAngle ) );
		vec3 c1 = vec3( 1.0 - sSc.z );
	#endif
	
	
	
	// calculate first ribbon axis and rotation matrix
	vec3 up = normalize( vec3( -gl_in[ 1 ].gl_Position ) );
	
	ribbonAxis1 = cross( up, vec3( gl_in[ 2 ].gl_Position ) - vec3( gl_in[ 0 ].gl_Position ) );
	float len = length( ribbonAxis1 );
	if( len < epsilon ){
		ribbonAxis1 = vec3( 1.0, 0.0, 0.0 );
		
	}else{
		ribbonAxis1 /= len;
	}
	if( ribbonAxis1.y < 0.0 ){
		ribbonAxis1 = -ribbonAxis1;
	}
	
	#ifdef USE_SHEETS
		//vec4 view2 = vec4( cross( ribbonAxis1, up ), 1.0 );
		//vec4 view2 = vec4( normalize( cross( vec3( gl_in[ 2 ].gl_Position ) - vec3( gl_in[ 0 ].gl_Position ), up ) ), 1.0 );
		vec4 view2 = vec4( normalize( vec3( gl_in[ 2 ].gl_Position ) - vec3( gl_in[ 0 ].gl_Position ) ), 1.0 );
		vec3 v1 = view2.xxx * view2.xyz * c1 + view2.wzy * sSc.zxy;
		vec3 v2 = view2.xyy * view2.yyz * c1 + view2.zwx * sSc.yzx;
		vec3 v3 = view2.xyz * view2.zzz * c1 + view2.yxw * sSc.xyz;
		matRot1 = mat3( v1, v2, v3 );
	#endif
	
	ribbonAxis1 *= vec3( vParticle0[ 1 ].x * 0.5 );
	
	
	
	// calculate second ribbon axis and rotation matrix
	up = normalize( vec3( -gl_in[ 2 ].gl_Position ) );
	
	ribbonAxis2 = cross( up, vec3( gl_in[ 3 ].gl_Position ) - vec3( gl_in[ 1 ].gl_Position ) );
	len = length( ribbonAxis2 );
	if( len < epsilon ){
		ribbonAxis2 = vec3( 1.0, 0.0, 0.0 );
		
	}else{
		ribbonAxis2 /= len;
	}
	if( ribbonAxis1.y < 0.0 ){
		ribbonAxis1 = -ribbonAxis1;
	}
	
	if( dot( vec2( ribbonAxis1 ), vec2( ribbonAxis2 ) ) < 0.0 ){
		ribbonAxis1 = -ribbonAxis1;
	}
	
	#ifdef USE_SHEETS
		//view2 = vec4( cross( ribbonAxis2, up ), 1.0 );
		//view2 = vec4( normalize( cross( vec3( gl_in[ 3 ].gl_Position ) - vec3( gl_in[ 1 ].gl_Position ), up ) ), 1.0 );
		view2 = vec4( normalize( vec3( gl_in[ 3 ].gl_Position ) - vec3( gl_in[ 1 ].gl_Position ) ), 1.0 );
		v1 = view2.xxx * view2.xyz * c1 + view2.wzy * sSc.zxy;
		v2 = view2.xyy * view2.yyz * c1 + view2.zwx * sSc.yzx;
		v3 = view2.xyz * view2.zzz * c1 + view2.yxw * sSc.xyz;
		matRot2 = mat3( v1, v2, v3 );
	#endif
	
	ribbonAxis2 *= vec3( vParticle0[ 2 ].x * 0.5 );
	
	
	
	// generate billboard(s)
	#ifdef USE_SHEETS
	for( s=0; s<sheetCount; s++ ){
	#endif
		emitCorner( 1, -ribbonAxis1, tc1 );
		emitCorner( 2, -ribbonAxis2, tc2 );
		emitCorner( 1, ribbonAxis1, tc3 );
		emitCorner( 2, ribbonAxis2, tc4 );
		
		EndPrimitive();
		
	#ifdef USE_SHEETS
		ribbonAxis1 = matRot1 * ribbonAxis1;
		ribbonAxis2 = matRot2 * ribbonAxis2;
	}
	#endif
}
