// - TBOMaterial: RGBA32UI (stride 1 pixel)
//   stores the instance material parameters in bit-field packed form. each pixel stores one
//   texture material. hence multiple consequitive pixels belong to one instance and the
//   number of textures per instance is variable.
//   
//   stores these material properties (bits required for each component and total bits):
//   - color.tint:              8 8 8     => 24
//   - color.gamma:             8         => 8
//   - roughness.remap.*:       8 8       => 16
//   - roughness.gamma:         8         => 8
//   - reflectivity.multiplier: 8         => 8
//   - emissivity.intensity:    16 16 16  => 48
//   - ignore:                  1         => 1
//   
//   also store the index of the material in the material texture atlas as 14 bits value.
//   this allows to combine it with variation.* bits to fill up 16 bits. 14 bits allows
//   for a maximum of 16383 materials. this equals a 128x128 atlas size or 16 pixel material
//   maps as the worst case
//   
//   gamma is limited to the range from 0.4 to 2.2 mapped to 8 bits. this maps pixel value
//   85 precisely to gamma value 1.
//   
//   pixel has a total of 128 bits. this leads to the following possible layout.
//   - red:
//     - 8: color.tint.r
//     - 8: roughness.remap.lower
//     - 16: emissivity.intensity.r
//   - green:
//     - 8: color.tint.g
//     - 8: roughness.remap.upper
//     - 16: emissivity.intensity.g
//   - blue:
//     - 8: color.tint.b
//     - 8: roughness.gamma
//     - 16: emissivity.intensity.b
//   - alpha:
//     - 8: color.gamma
//     - 8: reflectivity.multiplier
//     - 1: ignore material
//     - 1: (8reserved)
//     - 14: material atlas index
//
// - tboMaterial2: RGBA16F (stride 3 pixel)
//   contains material parameters requiring floating point values to be stored:
//   - pixel 1: texCoordMatrix.row1(rgb) unused(a)
//   - pixel 2: texCoordMatrix.row2(rgb) unused(a)
//   - pixel 3: emissivity(rgb) unused(a)
uniform usamplerBuffer tboGIRayCastMaterial;
uniform samplerBuffer tboGIRayCastMaterial2;

// material atlas textures
uniform sampler2D tboGIRayCastMaterialDiffuse; // diffuse=rgb, tintMask=a
uniform sampler2D tboGIRayCastMaterialReflectivity; // reflectivity=rgb, roughness=a
uniform sampler2D tboGIRayCastMaterialEmissivity; // emissivity=rgb, solidity=a


// Material parameter flag constants
const uint giRayCastMatFlagIgnore = uint( 0x8000 );
const uint giRayCastMatFlagClampTC = uint( 0x4000 );


// Sample material parameters.
// 
// - material: RayResult.material
uvec4 giRayCastMaterialParams( in int material ){
	return texelFetch( tboGIRayCastMaterial, material );
}


// Sample material flags.
// 
// - material: RayResult.material
uint giRayCastMaterialFlags( in int material ){
	return texelFetch( tboGIRayCastMaterial, material ).r;
}


// Retrieve color.tint and color.gamma texture property from material parameters.
// Returns vector vec4(color.tint.rgb, color.gamma).
// 
// - params: giRayCastMaterialParams(RayResult.material)
vec4 giRayCastMaterialColorTintGamma( in uvec4 params ){
	return vec4( params >> 24 )
		* vec4( 1.0 / 255.0, 1.0 / 255.0, 1.0 / 255.0, 1.8 / 255.0 )
		+ vec4( 0.0, 0.0, 0.0, 0.4 );
}


// Retrieve roughness.remap.lower, roughness.remap.upper, roughness.gamma
// and reflectivity.multiplier texture property from material parameters.
// Returns vector vec4(roughness.remap.lower, roughness.remap.upper,
// roughness.gamma, reflectivity.multiplier).
// 
// - params: giRayCastMaterialParams(RayResult.material)
vec4 giRayCastMaterialRoughnessGammaReflMul( in uvec4 params ){
	return vec4( ( params >> 16 ) & uvec4( 0xff ) )
		* vec4( 1.0 / 255.0, 1.0 / 255.0, 1.8 / 255.0, 1.0 / 255.0 )
		+ vec4( 0.0, 0.0, 0.4, 0.0 );
}


// Transform face texture coordinates by material texture coordinate transformation.
// 
// - material: RayResult.material
// - texCoord: giRayCastFaceTexCoord(RayResult.face, RayResult.barycentric)
vec2 giRayCastTCTransform( in int material, in vec2 texCoord ){
	return vec2( vec3( texCoord, 1.0 ) * mat2x3(
		texelFetch( tboGIRayCastMaterial2, material * 3 ).xyz,
		texelFetch( tboGIRayCastMaterial2, material * 3 + 1 ).xyz ) );
}


// Calculate material texture coordinates from face texture coordinates.
// 
// - params: giRayCastMaterialParams(RayResult.material)
// - texCoord: giRayCastTCTransform(RayResult.material,
//                giRayCastFaceTexCoord(RayResult.face, RayResult.barycentric))
ivec2 giRayCastMaterialTC( in uvec4 params, in vec2 texCoord ){
	int mapIndex = int( params.r & uint( 0x3fff ) );
	
	ivec2 matTC = ivec2( mapIndex % pGIMaterialMapsPerRow, mapIndex / pGIMaterialMapsPerRow );
	matTC *= ivec2( pGIMaterialMapSize ); // base coord of material map
	
	ivec2 realMatTC = ivec2( texCoord * vec2( pGIMaterialMapSize ) );
	matTC += ( ( params.r & giRayCastMatFlagClampTC ) != 0 )
		? clamp( realMatTC, ivec2( 0 ), ivec2( pGIMaterialMapSize - 1 ) )
		: realMatTC % ivec2( pGIMaterialMapSize );
	
	return matTC;
}


// Sample material color.
// 
// - params: giRayCastMaterialParams(RayResult.material)
// - texCoord: giRayCastMaterialTC(...)
vec3 giRayCastSampleColor( in uvec4 params, in ivec2 texCoord ){
	vec4 temp = texelFetch( tboGIRayCastMaterialDiffuse, texCoord, 0 );
	vec4 colorTintGamma = giRayCastMaterialColorTintGamma( params );
	vec3 color = pow( temp.rgb, vec3( colorTintGamma.a ) );
	return mix( color, color * colorTintGamma.rgb, temp.a );
}


// Sample material reflectivity and roughness.
// 
// - params: giRayCastMaterialParams(RayResult.material)
// - texCoord: giRayCastMaterialTC(...)
void giRayCastSampleReflectivityRoughness( in uvec4 params, in ivec2 texCoord,
out vec3 reflectivity, out float roughness ){
	vec4 temp = texelFetch( tboGIRayCastMaterialReflectivity, texCoord, 0 );
	vec4 roughnessGammaReflMul = giRayCastMaterialRoughnessGammaReflMul( params );
	
	reflectivity = temp.rgb * vec3( roughnessGammaReflMul.a );
	
	roughness = pow( temp.a, roughnessGammaReflMul.z );
	roughness = roughness * roughnessGammaReflMul.x + roughnessGammaReflMul.y;
	roughness = clamp( roughness, 0.0, 1.0 );
}


// Sample material emissivity.
// 
// - material: RayResult.material
// - texCoord: giRayCastMaterialTC(...)
vec3 giRayCastMaterialEmissivity( in int material, in ivec2 texCoord ){
	return texelFetch( tboGIRayCastMaterialEmissivity, texCoord, 0 ).rgb
		* texelFetch( tboGIRayCastMaterial2, material * 3 + 2 ).rgb;
}


// Sample all material properties.
// 
// - material: RayResult.material
// - params: giRayCastMaterialParams(RayResult.material)
// - texCoord: giRayCastMaterialTC(...)
void giRayCastMaterialAll( in int material, in uvec4 params, in ivec2 texCoord,
out vec3 color, out vec3 reflectivity, out float roughness, out vec3 emissivity ){
	color = giRayCastSampleColor( params, texCoord );
	giRayCastSampleReflectivityRoughness( params, texCoord, reflectivity, roughness );
	emissivity = giRayCastMaterialEmissivity( material, texCoord );
}

// Sample all material properties.
// 
// - material: RayResult.material
// - texCoord: giRayCastFaceTexCoord(RayResult.face, RayResult.barycentric)
void giRayCastMaterialAll( in int material, in vec2 texCoord, out vec3 color,
out vec3 reflectivity, out float roughness, out vec3 emissivity ){
	uvec4 params = giRayCastMaterialParams( material );
	ivec2 matTC = giRayCastMaterialTC( params, giRayCastTCTransform( material, texCoord ) );
	
	color = giRayCastSampleColor( params, matTC );
	giRayCastSampleReflectivityRoughness( params, matTC, reflectivity, roughness );
	emissivity = giRayCastMaterialEmissivity( material, matTC );
}
