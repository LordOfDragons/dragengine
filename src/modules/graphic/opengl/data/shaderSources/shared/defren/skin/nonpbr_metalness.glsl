/*
	<!-- shared/defren/skin/nonpbr_metalness.glsl -->
	<define>TEXTURE_NONPBR_ALBEDO</define>
	<define>TEXTURE_NONPBR_METALNESS</define>
*/

// convert non-pbr albedo/metalness to pbr color/reflectivity.
vec3 nonpbrMetalnessToColor(const in vec3 albedo, const in float metalness){
	return mix(albedo, vec3(0.0), metalness);
}

vec3 nonpbrMetalnessToReflectivity(const in vec3 albedo, const in float metalness){
	return mix(vec3(0.04), albedo, metalness);
}
