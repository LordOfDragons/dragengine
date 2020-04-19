//#extension GL_ARB_tessellation_shader : enable

// request high precision if the graphic card supports this
#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

// some helper definitions to make the code easier to read
#if defined TEXTURE_SOLIDITY || defined OUTPUT_COLOR || defined TEXTURE_HEIGHT
	#define REQUIRES_TEX_COLOR 1
#endif

#ifdef TEXTURE_HEIGHT
	#define REQUIRES_NORMAL 1
#endif



// Definitions
////////////////

layout( triangles, equal_spacing, ccw ) in;



// Uniform Parameters
///////////////////////

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/skin/ubo_render_parameters.glsl"
#include "v130/shared/defren/skin/ubo_texture_parameters.glsl"
#include "v130/shared/defren/skin/ubo_instance_parameters.glsl"
#include "v130/shared/defren/skin/ubo_dynamic_parameters.glsl"



// Samplers
/////////////

#ifdef TEXTURE_HEIGHT
	uniform mediump sampler2D texHeight;
#endif



// Inputs
///////////

#ifdef REQUIRES_TEX_COLOR
	in vec2 vTESTCColor[];
#endif
#ifdef CLIP_PLANE
	in vec3 vTESClipCoord[];
#endif
#ifdef DEPTH_DISTANCE
	in vec3 vTESPosition[];
#endif
#ifdef HEIGHT_MAP
	in float vTESHTMask[];
#endif
#ifdef PROP_FIELD
	in float vTESRenderCondition[];
#endif
#ifdef REQUIRES_NORMAL
	in vec3 vTESNormal[];
	#ifdef TEXTURE_NORMAL
		in vec3 vTESTangent[];
		in vec3 vTESBitangent[];
	#endif
#endif



// Outputs
////////////

#ifdef GS_RENDER_CUBE
	#ifdef REQUIRES_TEX_COLOR
		out vec2 vGSTCColor;
		#define vTCColor vGSTCColor
	#endif
	#ifdef CLIP_PLANE
		out vec3 vGSClipCoord;
		#define vClipCoord vGSClipCoord
	#endif
	#ifdef DEPTH_ORTHOGONAL
		out float vGSZCoord;
		#define vZCoord vGSZCoord
	#endif
	#ifdef DEPTH_DISTANCE
		out vec3 vGSPosition;
		#define vPosition vGSPosition
	#endif
	#ifdef HEIGHT_MAP
		out float vGSHTMask;
		#define vHTMask vGSHTMask
	#endif
	#ifdef PROP_FIELD
		out float vGSRenderCondition;
	#endif
	#ifdef REQUIRES_NORMAL
		out vec3 vGSNormal;
		#define vNormal vGSNormal
		#ifdef TEXTURE_NORMAL
			out vec3 vGSTangent;
			#define vTangent vGSTangent
			out vec3 vGSBitangent;
			#define vBitangent vGSBitangent
		#endif
	#endif
	
#else
	#ifdef REQUIRES_TEX_COLOR
		out vec2 vTCColor;
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
	#ifdef HEIGHT_MAP
		out float vHTMask;
	#endif
	#ifdef REQUIRES_NORMAL
		out vec3 vNormal;
		#ifdef TEXTURE_NORMAL
			out vec3 vTangent;
			out vec3 vBitangent;
		#endif
	#endif
#endif



// Main Function
//////////////////

#include "v130/shared/tessellation.glsl"

void main(){
	//#ifdef TESSELLATION_LINEAR
	// tessellate varying parameters
	#ifdef REQUIRES_TEX_COLOR
		TESS_VAR_LINEAR_TRI( vTCColor, vTESTCColor );
	#endif
	#ifdef CLIP_PLANE
		TESS_VAR_LINEAR_TRI( vClipCoord, vTESClipCoord );
	#endif
	#ifdef DEPTH_DISTANCE
		TESS_VAR_LINEAR_TRI( vPosition, vTESPosition );
	#endif
	#ifdef HEIGHT_MAP
		TESS_VAR_LINEAR_TRI( vHTMask, vTESHTMask );
	#endif
	#ifdef REQUIRES_NORMAL
		TESS_VAR_LINEAR_TRI( vNormal, vTESNormal );
		#ifdef TEXTURE_NORMAL
			TESS_VAR_LINEAR_TRI( vTangent, vTESTangent );
			TESS_VAR_LINEAR_TRI( vBitangent, vTESBitangent );
		#endif
	#endif
	
	// tessellate position
	tessLinearTri( gl_Position, gl_in[ 0 ].gl_Position, gl_in[ 1 ].gl_Position, gl_in[ 2 ].gl_Position );
	
	#ifdef REQUIRES_NORMAL
		vec3 displacement = normalize( vNormal ) * vec3( texture( texHeight, vTCColor ).r * pHeightRemap.x + pHeightRemap.y );
		gl_Position.xyz += displacement;
	#endif
	
	#ifdef BILLBOARD
		gl_Position = pMatrixP * gl_Position;
	#else
		gl_Position = pMatrixVP * gl_Position;
	#endif
	
	#ifdef DEPTH_ORTHOGONAL
		#ifdef NO_ZCLIP
			vZCoord = gl_Position.z * 0.5 + 0.5; // we have to do the normalization ourself
			gl_Position.z = 0.0;
		#else
			vZCoord = gl_Position.z;
		#endif
	#endif
	
	#ifdef REQUIRES_NORMAL
		#ifdef DEPTH_DISTANCE
			#ifndef BILLBOARD
				vPosition = pMatrixV * vec4( vPosition + displacement, 1.0 );
			#endif
		#endif
		#ifdef CLIP_PLANE
			#ifndef BILLBOARD
				vClipCoord = pMatrixV * vec4( vClipCoord + displacement, 1.0 );
			#endif
		#endif
	#endif
	
	// normalize normals again. required since for displacement the normals have to be in the proper coordinate system
	#ifdef REQUIRES_NORMAL
		vNormal = normalize( vNormal * pMatrixVn );
		#ifdef TEXTURE_NORMAL
			vTangent = normalize( vTangent * pMatrixVn );
			vBitangent = normalize( vBitangent * pMatrixVn );
		#endif
	#endif
}
