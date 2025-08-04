#include "shared/preamble.glsl"
#include "shared/defren/skin/macros_geometry.glsl"


// Samplers
/////////////

#ifdef WITH_VARIATIONS
	// functions are defined right before main due to Shared-SPB support
	#define SAMPLER_2D sampler2DArray
	#define TEXTURE(s,tc) textureLod(s, vec3(tc, 0), 0.0)
	//#define TEXTURE(s,tc) textureLod(s, tcTexVar(tc, textureSize(s, 0).z, 0))
#else
	#define SAMPLER_2D sampler2D
	#define TEXTURE(s,tc) textureLod(s, tc, 0.0)
#endif

#ifdef TEXTURE_COLOR
	uniform lowp SAMPLER_2D texColor;
#endif
#ifdef TEXTURE_TRANSPARENCY
	uniform lowp SAMPLER_2D texTransparency;
#endif
#ifdef TEXTURE_COLOR_TINT_MASK
	uniform lowp SAMPLER_2D texColorTintMask;
#endif
#ifdef TEXTURE_SOLIDITY
	uniform lowp SAMPLER_2D texSolidity;
#endif
#ifdef TEXTURE_REFLECTIVITY
	uniform lowp SAMPLER_2D texReflectivity;
#endif
#ifdef TEXTURE_ROUGHNESS
	uniform lowp SAMPLER_2D texRoughness;
#endif
#ifdef TEXTURE_EMISSIVITY
	uniform mediump SAMPLER_2D texEmissivity;
#endif
#ifdef TEXTURE_ENVROOM
	uniform lowp samplerCube texEnvRoom;
#endif
#ifdef TEXTURE_ENVROOM_MASK
	uniform lowp SAMPLER_2D texEnvRoomMask;
#endif
#ifdef TEXTURE_ENVROOM_EMISSIVITY
	uniform mediump samplerCube texEnvRoomEmissivity;
#endif
#ifdef TEXTURE_NONPBR_ALBEDO
	uniform lowp SAMPLER_2D texNonPbrAlbedo;
#endif
#ifdef TEXTURE_NONPBR_METALNESS
	uniform lowp SAMPLER_2D texNonPbrMetalness;
#endif



// Inputs
///////////

VARYING_BIND(0) in vec2 vTexCoord;


// Outputs
////////////

layout(location=0) out vec4 outDiffuse; // diffuse=rgb, tintMask=a
layout(location=1) out vec4 outReflectivity; // reflectivity=rgb, roughness=a
layout(location=2) out vec4 outEmissivity; // emissivity=rgb, solidity=a



#if defined TEXTURE_NONPBR_ALBEDO || defined TEXTURE_NONPBR_METALNESS
	#include "shared/defren/skin/nonpbr_metalness.glsl"
#endif



// Main Function
//////////////////

void main( void ){
	#if defined TEXTURE_NONPBR_ALBEDO || defined TEXTURE_NONPBR_METALNESS
		vec3 nonpbrAlbedo = vec3( 0 );
		#ifdef TEXTURE_NONPBR_ALBEDO
			nonpbrAlbedo = TEXTURE( texNonPbrAlbedo, vTexCoord ).rgb;
		#endif
		
		float nonpbrMetalness = 0;
		#ifdef TEXTURE_NONPBR_METALNESS
			nonpbrMetalness = TEXTURE( texNonPbrMetalness, vTexCoord ).r;
		#endif
	#endif
	
	// texture property "color"
	#if defined TEXTURE_NONPBR_ALBEDO || defined TEXTURE_NONPBR_METALNESS
		outDiffuse.rgb = nonpbrMetalnessToColor( nonpbrAlbedo, nonpbrMetalness );
	#elif defined TEXTURE_COLOR
		#ifdef TEXTURE_TRANSPARENCY
			outDiffuse.rgb = TEXTURE( texColor, vTexCoord ).rgb;
			//outDiffuse.a = TEXTURE( texTransparency, vTexCoord ).r;
		#else
			//outDiffuse = TEXTURE( texColor, vTexCoord );
			outDiffuse.rgb = TEXTURE( texColor, vTexCoord ).rgb;
		#endif
	#else
		#ifdef TEXTURE_TRANSPARENCY
			outDiffuse.rgb = vec3( 0.0 );
			//outDiffuse.a = TEXTURE( texTransparency, vTexCoord ).r;
		#else
			outDiffuse = vec4( 0.0, 0.0, 0.0, 1.0 );
		#endif
	#endif
	
	// texture property "color.tint.mask"
	#ifdef TEXTURE_COLOR_TINT_MASK
		outDiffuse.a = TEXTURE( texColorTintMask, vTexCoord ).r;
	#else
		outDiffuse.a = 1.0;
	#endif
	
	// texture property "reflectivity"
	#if defined TEXTURE_NONPBR_ALBEDO || defined TEXTURE_NONPBR_METALNESS
		outReflectivity.rgb = nonpbrMetalnessToReflectivity( nonpbrAlbedo, nonpbrMetalness );
	#elif defined TEXTURE_REFLECTIVITY
		outReflectivity.rgb = TEXTURE( texReflectivity, vTexCoord ).rgb;
	#else
		outReflectivity.rgb = vec3( 0.0 );
	#endif
	
	// texture property "roughness"
	#ifdef TEXTURE_ROUGHNESS
		outReflectivity.a = TEXTURE( texRoughness, vTexCoord ).r;
	#else
		outReflectivity.a = 1.0;
	#endif
	
	// texture property "emissivity"
	#ifdef TEXTURE_EMISSIVITY
		outEmissivity.rgb = TEXTURE( texEmissivity, vTexCoord ).rgb;
	#else
		outEmissivity.rgb = vec3( 0.0 );
	#endif
	
	// texture property "solidity"
	#ifdef TEXTURE_SOLIDITY
		outEmissivity.a = TEXTURE( texSolidity, vTexCoord ).r;
	#else
		outEmissivity.a = 1.0;
	#endif
	
	// environment room replaces the diffuse component.
	// since we have no direction here we use the back face
	#if defined TEXTURE_ENVROOM || defined TEXTURE_ENVROOM_EMISSIVITY
		vec3 envRoomDir = vec3( vTexCoord * vec2( 2 ) - vec2( 1 ), 1.0 );
		#ifdef TEXTURE_ENVROOM
			vec3 envRoomColor = textureLod( texEnvRoom, envRoomDir, 0.0 ).rgb;
		#else
			vec3 envRoomColor = vec3( 0.0 );
		#endif
		
		#ifdef TEXTURE_ENVROOM_MASK
			float envRoomMask = TEXTURE( texEnvRoomMask, vTexCoord ).r;
			outDiffuse.rgb = mix( outDiffuse.rgb, envRoomColor, vec3( envRoomMask ) );
		#else
			outDiffuse.rgb = envRoomColor;
		#endif
	#endif
	
	#ifdef TEXTURE_ENVROOM_EMISSIVITY
		#ifdef TEXTURE_ENVROOM_MASK
			outEmissivity.rgb += textureLod( texEnvRoomEmissivity, envRoomDir, 0.0 ).rgb * vec3( envRoomMask );
		#else
			outEmissivity.rgb += textureLod( texEnvRoomEmissivity, envRoomDir, 0.0 ).rgb;
		#endif
	#endif
}
