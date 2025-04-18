#ifdef GS_INSTANCING
	#ifndef OPENGLES
		#extension GL_ARB_gpu_shader5 : require
	#endif
#endif

#include "shared/defren/skin/macros_geometry.glsl"

// layout definitions
#ifdef GS_RENDER_STEREO
	#ifdef GS_INSTANCING
		layout( triangles, invocations=2 ) in;
		layout( triangle_strip, max_vertices=3 ) out;
	#else
		layout( triangles ) in;
		layout( triangle_strip, max_vertices=6 ) out;
	#endif
#else
	layout( triangles ) in;
	layout( triangle_strip, max_vertices=3 ) out;
#endif

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"



// Inputs
///////////

in vec2 vGSTCColor[ 3 ];
#ifdef TEXTURE_COLOR_TINT_MASK
	in vec2 vGSTCColorTintMask[ 3 ];
#endif
#ifdef TEXTURE_NORMAL
	in vec2 vGSTCNormal[ 3 ];
#endif
#ifdef WITH_REFLECTIVITY
	in vec2 vGSTCReflectivity[ 3 ];
#endif
#ifdef WITH_EMISSIVITY
	in vec2 vGSTCEmissivity[ 3 ];
#endif
#ifdef TEXTURE_REFRACTION_DISTORT
	in vec2 vGSTCRefractionDistort[ 3 ];
#endif
#ifdef TEXTURE_AO
	in vec2 vGSTCAO[ 3 ];
#endif

in vec3 vGSNormal[ 3 ];
#ifdef WITH_TANGENT
	in vec3 vGSTangent[ 3 ];
#endif
#ifdef WITH_BITANGENT
	in vec3 vGSBitangent[ 3 ];
#endif
#ifdef HEIGHT_MAP
	in float vGSHTMask[ 3 ];
#endif

// in float vGSRenderCondition[ 3 ];

#ifdef SHARED_SPB
	flat in int vGSSPBIndex[ 3 ];
	#define spbIndex vGSSPBIndex[0]
	
	#ifdef GS_RENDER_CUBE
		flat in int vGSSPBFlags[ 3 ];
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
#ifdef WITH_REFLECTIVITY
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
#ifdef HEIGHT_MAP
	out float vHTMask;
#endif
#ifdef FADEOUT_RANGE
	out float vFadeZ;
#endif

#ifdef SHARED_SPB
	flat out int vSPBIndex;
#endif

#ifdef GS_RENDER_STEREO
	flat out int vLayer;
#else
	const int vLayer = 0;
#endif



// dual view rendering
#ifdef GS_RENDER_STEREO

void emitCorner( in int layer, in int corner, in vec4 position, in vec4 preTransformedPosition ){
	gl_Position = preTransformedPosition;
	
	#ifdef SHARED_SPB
	vSPBIndex = spbIndex;
	#endif
	
	vTCColor = vGSTCColor[ corner ];
	#ifdef TEXTURE_COLOR_TINT_MASK
		vTCColorTintMask = vGSTCColorTintMask[ corner ];
	#endif
	#ifdef TEXTURE_NORMAL
		vTCNormal = vGSTCNormal[ corner ];
	#endif
	#ifdef WITH_REFLECTIVITY
		vTCReflectivity = vGSTCReflectivity[ corner ];
	#endif
	#ifdef WITH_EMISSIVITY
		vTCEmissivity = vGSTCEmissivity[ corner ];
	#endif
	#ifdef TEXTURE_REFRACTION_DISTORT
		vTCRefractionDistort = vGSTCRefractionDistort[ corner ];
	#endif
	#ifdef TEXTURE_AO
		vTCAO = vGSTCAO[ corner ];
	#endif
	
	vNormal = normalize( vGSNormal[ corner ] * pMatrixVn[ layer ] );
	#ifdef WITH_TANGENT
		vTangent = normalize( vGSTangent[ corner ] * pMatrixVn[ layer ] );
	#endif
	#ifdef WITH_BITANGENT
		vBitangent = normalize( vGSBitangent[ corner ] * pMatrixVn[ layer ] );
	#endif
	
	#ifdef WITH_REFLECT_DIR
		vReflectDir = pMatrixV[ layer ] * position;
	#endif
	#ifdef HEIGHT_MAP
		vHTMask = vGSHTMask[ corner ];
	#endif
	#ifdef FADEOUT_RANGE
		#ifdef BILLBOARD
			vFadeZ = position.z;
		#else
			vFadeZ = ( pMatrixV[ layer ] * position ).z;
		#endif
	#endif
	
	vLayer = layer;
	
	gl_Layer = layer;
	gl_PrimitiveID = gl_PrimitiveIDIn;
	
	//EmitVertex();
}

void emitCorner( in int layer, in int corner, in vec4 position ){
	vec4 preTransformedPosition;
	
	#ifdef BILLBOARD
		#ifdef GS_RENDER_STEREO
			// during vertex shader the left view position has been used.
			// if this is the right view correct the transform
			if( layer == 1 ){
				preTransformedPosition = pMatrixP[ layer ] * vec4( pCameraStereoTransform * position, 1 );
				
			}else{
				preTransformedPosition = pMatrixP[ layer ] * position;
			}
		#else
			preTransformedPosition = pMatrixP[ layer ] * position;
		#endif
	#else
		preTransformedPosition = pMatrixVP[ layer ] * position;
	#endif
	
	emitCorner( layer, corner, position, preTransformedPosition );
}

#endif



// Main Function
//////////////////

#ifdef GS_RENDER_STEREO

void main( void ){
	// NOTE: quest requires EmitVertex to be called in main()
	
// 	if( vGSRenderCondition[ 0 ] >= 5.0 ){
// 		return;
// 	}
	
	int eye;
	
	#ifdef GS_INSTANCING
	eye = gl_InvocationID;
	#else
	for( eye=0; eye<2; eye++ ){
	#endif
		
		int i;
		for( i=0; i<3; i++ ){
			emitCorner(eye, i, gl_in[i].gl_Position);
			EmitVertex();
		}
		EndPrimitive();
		
	#ifndef GS_INSTANCING
	}
	#endif
}

#else

void main( void ){
// 	if( vGSRenderCondition[ 0 ] >= 5.0 ){
// 		return;
// 	}
	
	int i;
	for( i=0; i<3; i++ ){
		gl_Position = gl_in[ i ].gl_Position;
		
		vTCColor = vGSTCColor[ i ];
		#ifdef SHARED_SPB
		vSPBIndex = spbIndex;
		#endif
		#ifdef TEXTURE_COLOR_TINT_MASK
			vTCColorTintMask = vGSTCColorTintMask[ i ];
		#endif
		#ifdef TEXTURE_NORMAL
			vTCNormal = vGSTCNormal[ i ];
		#endif
		#ifdef WITH_REFLECTIVITY
			vTCReflectivity = vGSTCReflectivity[ i ];
		#endif
		#ifdef WITH_EMISSIVITY
			vTCEmissivity = vGSTCEmissivity[ i ];
		#endif
		#ifdef TEXTURE_REFRACTION_DISTORT
			vTCRefractionDistort = vGSTCRefractionDistort[ i ];
		#endif
		#ifdef TEXTURE_AO
			vTCAO = vGSTCAO[ i ];
		#endif
		
		vNormal = vGSNormal[ i ];
		#ifdef WITH_TANGENT
			vTangent = vGSTangent[ i ];
		#endif
		#ifdef WITH_BITANGENT
			vBitangent = vGSBitangent[ i ];
		#endif
		#ifdef WITH_REFLECT_DIR
			vReflectDir = vGSReflectDir[ i ];
		#endif
		#ifdef HEIGHT_MAP
			vHTMask = vGSHTMask[ i ];
		#endif
		#ifdef FADEOUT_RANGE
			vFadeZ = vGSFadeZ[ i ];
		#endif
		
		gl_Layer = 0;
		gl_PrimitiveID = gl_PrimitiveIDIn;
		
		EmitVertex();
	}
	
	EndPrimitive();
}

#endif // GS_RENDER_STEREO
