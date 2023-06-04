#ifdef GS_INSTANCING
	#extension GL_ARB_gpu_shader5 : require
#endif

#include "shared/defren/skin/macros_geometry.glsl"

// layout specifications
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


// Uniform Parameters
///////////////////////

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/defren/skin/ubo_instance_parameters.glsl"



// Inputs
///////////

in vec3 vParticle0[ 1 ]; // size, emissivity, rotation
in vec4 vParticle1[ 1 ]; // red, green, blue, transparency

#ifdef SHARED_SPB
	flat in int vGSSPBIndex[ 1 ];
	#define spbIndex vGSSPBIndex[0]
	
	#ifdef GS_RENDER_CUBE
		flat in int vGSSPBFlags[ 1 ];
		#define spbFlags vGSSPBFlags[0]
	#endif
	
	#include "shared/defren/skin/shared_spb_redirect.glsl"
#endif



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

const vec2 bc[4] = vec2[4]( vec2( -0.5,  0.5 ), vec2( -0.5, -0.5 ), vec2(  0.5,  0.5 ), vec2(  0.5, -0.5 ) );
const vec2 tc[4] = vec2[4]( vec2(  0.0,  0.0 ), vec2(  0.0,  1.0 ), vec2(  1.0,  0.0 ), vec2(  1.0,  1.0 ) );



// Main Function
//////////////////

void emitParticle( in int layer, in mat2 rotmat, in vec3 normal, in vec3 tangent, in vec3 bitangent ){
	// z and w stays the same for all vertices
	vec3 transformed = pMatrixV[ layer ] * gl_in[ 0 ].gl_Position;
	
	vec4 position = vec4( transformed, 1 );
	
	// emit vertices
	int i;
	for( i=0; i<4; i++ ){
		position.xy = rotmat * bc[i] + transformed.xy;
		
		gl_Position = pMatrixP[ layer ] * position;
		
		vTCColor = tc[i];
		#ifdef SHARED_SPB
		vSPBIndex = spbIndex;
		#endif
		#ifdef TEXTURE_COLOR_TINT_MASK
			vTCColorTintMask = tc[i];
		#endif
		#ifdef TEXTURE_NORMAL
			vTCNormal = tc[i];
		#endif
		#ifdef TEXTURE_REFLECTIVITY
			vTCReflectivity = tc[i];
		#endif
		#ifdef WITH_EMISSIVITY
			vTCEmissivity = tc[i];
		#endif
		#ifdef TEXTURE_REFRACTION_DISTORT
			vTCRefractionDistort = tc[i];
		#endif
		#ifdef TEXTURE_AO
			vTCAO = tc[i];
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
		#ifdef DEPTH_DISTANCE
			vPosition = position.xyz;
		#endif
		
		vParticleColor = vParticle1[ 0 ];
		#ifdef WITH_EMISSIVITY
			vParticleEmissivity = vParticle0[ 0 ].y;
		#endif
		
		vNormal = normal;
		#ifdef WITH_TANGENT
			vTangent = tangent;
		#endif
		#ifdef WITH_BITANGENT
			vBitangent = bitangent;
		#endif
		
		#ifdef GS_RENDER_STEREO
			vLayer = layer;
		#endif
		
		gl_Layer = layer;
		gl_PrimitiveID = gl_PrimitiveIDIn;
		
		EmitVertex();
	}
	
	EndPrimitive();
}

void main( void ){
	vec4 tempRotMat; // cos(z), -sin(z), sin(z), cos(z)
	tempRotMat.xw = cos( vec2( vParticle0[ 0 ].z ) );
	tempRotMat.yz = sin( vec2( vParticle0[ 0 ].z ) ) * vec2( -1.0, 1.0 );
	tempRotMat *= vec4( vParticle0[ 0 ].x ); // tempRotMat * size
	mat2 rotmat = mat2( tempRotMat.x, tempRotMat.y, tempRotMat.z, tempRotMat.w );
	
	vec3 normal = vec3( 0, 0, -1 );
	
	#ifdef WITH_TANGENT
		vec3 tangent = vec3( rotmat * vec2( 1, 0 ), 0 );
	#else
		vec3 tangent = vec3( 0 );
	#endif
	
	#ifdef WITH_BITANGENT
		vec3 bitangent = vec3( rotmat * vec2( 0, -1 ), 0 );
	#else
		vec3 bitangent = vec3( 0 );
	#endif
	
	// emit particles
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
		
		emitParticle( layer, rotmat, normal, tangent, bitangent );
		
	#ifndef GS_INSTANCING
	}
	#endif
}
