// usage:
// #include "shared/interface/skin/variation.glsl"

// functions required to be define last because they are based on stuff defined above
// NOTE: according to opengl website the layer is clamped like this:
//         layer = clamp(floor(texCoord.p​ + 0.5), 0, d​ - 1).
//       thus the texCoord.p has to be reduced by 0.5 to map the entire range, example:
//         layerCount = 3
//         texCoord.p = 0.0 .. 3.0
//         texCoord.p - 0.5 = -0.5 .. 2.5
//         clamped => -0.5 .. 0.5 => 0; 0.5 .. 1.5 => 1; 1.5 .. 2.5 => 2
//       this way all value have the same dominance

vec3 tcTexVar(in vec2 tc, in int layerCount){
	vec3 p = fract(floor(tc.sts * getVariationEnableScale(vSPBIndex).xyx + pVariationSeed.xyx) * vec3(0.1031));
	p += dot(p, p.yzx + vec3(19.19));
	return vec3(tc, fract((p.x + p.y) * p.z) * float(layerCount) - 0.5);
}

#if 0
vec3 tcTexVar(in vec2 tc, in int layerCount){
	return vec3(tc, fract(sin(dot(floor(tc) / vec2(1024)
		* getVariationEnableScale(vSPBIndex) + pVariationSeed, vec2(12.9898, 78.233)))
			* 43758.5453123) * float(layerCount) - 0.5);
}
#endif

vec4 textureVariation(in sampler2D tex2d, in sampler2DArray texarr, const in vec2 tc){
	// functions are defined right before main due to Shared-SPB support
	if(WithVariations){
		return texture(texarr, tcTexVar(tc, textureSize(texarr, 0).z));
		
	}else{
		return texture(tex2d, tc);
	}
}

vec4 textureVariationLod(in sampler2D tex2d, in sampler2DArray texarr, const in vec2 tc, const in float lod){
	// functions are defined right before main due to Shared-SPB support
	if(WithVariations){
		return textureLod(texarr, tcTexVar(tc, textureSize(texarr, 0).z), lod);
		
	}else{
		return textureLod(tex2d, tc, lod);
	}
}


#ifdef NVIDIA_SAMPLER_COUNT_WORKAROUND
	#ifdef WITH_VARIATION
		#define TEXTURE(s,tc) texture(s##Array, tcTexVar(tc, textureSize(s##Array, 0).z))
		#define TEXTURE_LOD(s,tc,l) textureLod(s##Array, tcTexVar(tc, textureSize(s##Array, 0).z), l)
	#else
		#define TEXTURE(s,tc) texture(s, tc)
		#define TEXTURE_LOD(s,tc,l) textureLod(s, tc, l)
	#endif
	
#else
	#define TEXTURE(s,tc) textureVariation(s, s##Array, tc)
	#define TEXTURE_LOD(s,tc,l) textureVariationLod(s, s##Array, tc, l)
#endif
