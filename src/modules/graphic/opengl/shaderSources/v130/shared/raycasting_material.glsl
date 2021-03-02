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
//   - variation:*:             1 1       => 2
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
//     - 1: variation.u
//     - 1: variation.v
//     - 14: material atlas index
uniform usamplerBuffer tboMaterial;


void rayCastSampleMaterial( in RayCastResult result, out vec3 diffuse,
out vec3 reflectivity, out float roughness, out vec3 emissivity ){
	// calculate texture coordinates and material index from hit face
	vec2 texCoord = rayCastFaceTexCoord( result );
	
	// get material parameters
	uvec4 matParams = texelFetch( tboMaterial, result.material );
	
	vec4 colorGamma = vec4( matParams >> 24 )
		* vec4( 1.0 / 255.0, 1.0 / 255.0, 1.0 / 255.0, 1.8 / 255.0 )
		+ vec4( 0.0, 0.0, 0.0, 0.4 );
	vec4 roughnessGammaReflMul = vec4( ( matParams >> 16 ) & uvec4( 0xff ) )
		* vec4( 1.0 / 255.0, 1.0 / 255.0, 1.8 / 255.0, 1.0 / 255.0 )
		+ vec4( 0.0, 0.0, 0.4, 0.0 );
	vec3 emissivityIntensity = vec3( matParams.rgb & uvec3( 0xffff ) ) * vec3( 1.0 / 65535.0 );
	bvec2 variation = notEqual( matParams.aa & uvec2( 0x8000, 0x4000 ), uvec2( 0 ) );
	int materialMapIndex = int( matParams.a & uint( 0x3fff ) );
	
	// sample material map from atlas
	// TODO
	diffuse = vec3( 0.85 );
	reflectivity = vec3( 0.04 );
	roughness = 0.2;
	emissivity = vec3( 0.0 );
	
			diffuse = vec3(texCoord, materialMapIndex);
}
