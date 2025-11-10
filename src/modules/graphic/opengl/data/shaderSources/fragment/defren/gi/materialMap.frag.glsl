#include "shared/preamble.glsl"
#include "shared/defren/skin/macros_geometry.glsl"


// Samplers
/////////////

#include "shared/interface/skin/samplers.glsl"


// Inputs
///////////

#include "shared/interface/2d/fragment.glsl"


// Outputs
////////////

layout(location=0) out vec4 outDiffuse; // diffuse=rgb, tintMask=a
layout(location=1) out vec4 outReflectivity; // reflectivity=rgb, roughness=a
layout(location=2) out vec4 outEmissivity; // emissivity=rgb, solidity=a


// TextureNonPbrAlbedo || TextureNonPbrMetalness
#include "shared/defren/skin/nonpbr_metalness.glsl"


// Main Function
//////////////////

vec4 textureVariation(in ARG_SAMP_MEDP sampler2D tex2d, in ARG_SAMP_MEDP sampler2DArray texarr, const in vec2 tc){
	// functions are defined right before main due to Shared-SPB support
	if(WithVariations){
		return textureLod(texarr, vec3(tc, 0.0), 0.0);
		//return textureLod(texarr, tcTexVar(tc, textureSize(texarr, 0).z, 0))
		
	}else{
		return textureLod(tex2d, tc, 0.0);
	}
}

#ifdef NVIDIA_SAMPLER_COUNT_WORKAROUND
	#ifdef WITH_VARIATION
		#define TEXTURE(s,sarr,tc) textureLod(sarr, vec3(tc, 0.0), 0.0)
	#else
		#define TEXTURE(s,sarr,tc) textureLod(s, tc, 0.0)
	#endif
	
#else
	#define TEXTURE(s,sarr,tc) textureVariation(s, sarr, tc)
#endif


void main( void ){
	vec3 nonpbrAlbedo;
	float nonpbrMetalness;
	
	if(TextureNonPbrAlbedo || TextureNonPbrMetalness){
		nonpbrAlbedo = vec3(0.0);
		if(TextureNonPbrAlbedo){
			nonpbrAlbedo = vec3(TEXTURE(texNonPbrAlbedo, texNonPbrAlbedoArray, vTexCoord));
		}
		
		nonpbrMetalness = 0.0;
		if(TextureNonPbrMetalness){
			nonpbrMetalness = TEXTURE(texNonPbrMetalness, texNonPbrMetalnessArray, vTexCoord).r;
		}
	}
	
	// texture property "color"
	if(TextureNonPbrAlbedo || TextureNonPbrMetalness){
		outDiffuse.rgb = nonpbrMetalnessToColor(nonpbrAlbedo, nonpbrMetalness);
		
	}else if(TextureColor){
		if(TextureTransparency){
			outDiffuse.rgb = vec3(TEXTURE(texColor, texColorArray, vTexCoord));
			//outDiffuse.a = TEXTURE(texTransparency, texTransparencyArray, vTexCoord).r;
			
		}else{
			//outDiffuse = TEXTURE(texColor, texColorArray, vTexCoord);
			outDiffuse.rgb = vec3(TEXTURE(texColor, texColorArray, vTexCoord));
		}
		
	}else{
		if(TextureTransparency){
			outDiffuse.rgb = vec3(0.0);
			//outDiffuse.a = TEXTURE(texTransparency, texTransparencyArray, vTexCoord).r;
			
		}else{
			outDiffuse = vec4(0.0, 0.0, 0.0, 1.0);
		}
	}
	
	// texture property "color.tint.mask"
	if(TextureColorTintMask){
		outDiffuse.a = TEXTURE(texColorTintMask, texColorTintMaskArray, vTexCoord).r;
		
	}else{
		outDiffuse.a = 1.0;
	}
	
	// texture property "reflectivity"
	if(TextureNonPbrAlbedo || TextureNonPbrMetalness){
		outReflectivity.rgb = nonpbrMetalnessToReflectivity(nonpbrAlbedo, nonpbrMetalness);
		
	}else if(TextureReflectivity){
		outReflectivity.rgb = vec3(TEXTURE(texReflectivity, texReflectivityArray, vTexCoord));
		
	}else{
		outReflectivity.rgb = vec3(0.0);
	}
	
	// texture property "roughness"
	if(TextureRoughness){
		outReflectivity.a = TEXTURE(texRoughness, texRoughnessArray, vTexCoord).r;
		
	}else{
		outReflectivity.a = 1.0;
	}
	
	// texture property "emissivity"
	if(TextureEmissivity){
		outEmissivity.rgb = vec3(TEXTURE(texEmissivity, texEmissivityArray, vTexCoord));
		
	}else{
		outEmissivity.rgb = vec3(0.0);
	}
	
	// texture property "solidity"
	if(TextureSolidity){
		outEmissivity.a = TEXTURE(texSolidity, texSolidityArray, vTexCoord).r;
		
	}else{
		outEmissivity.a = 1.0;
	}
	
	// environment room replaces the diffuse component.
	// since we have no direction here we use the back face
	float envRoomMask = 1.0;
	vec3 envRoomDir;
	
	if(TextureEnvRoom || TextureEnvRoomEmissivity){
		envRoomDir = vec3(vTexCoord * vec2(2.0) - vec2(1.0), 1.0);
		
		vec3 envRoomColor;
		if(TextureEnvRoom){
			envRoomColor = textureLod(texEnvRoom, envRoomDir, 0.0).rgb;
			
		}else{
			envRoomColor = vec3(0.0);
		}
		
		if(TextureEnvRoomMask){
			envRoomMask = TEXTURE(texEnvRoomMask, texEnvRoomMaskArray, vTexCoord).r;
			outDiffuse.rgb = mix(outDiffuse.rgb, envRoomColor, vec3(envRoomMask));
			
		}else{
			outDiffuse.rgb = envRoomColor;
		}
	}
	
	if(TextureEnvRoomEmissivity){
		if(TextureEnvRoom){
			outEmissivity.rgb += textureLod(texEnvRoomEmissivity, envRoomDir, 0.0).rgb * vec3(envRoomMask);
			
		}else{
			outEmissivity.rgb += textureLod(texEnvRoomEmissivity, envRoomDir, 0.0).rgb;
		}
	}
}

#undef TEXTURE
