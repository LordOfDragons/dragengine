/*
	<!-- shared/defren/skin/nonpbr_metalness.glsl -->
	<define>TEXTURE_NONPBR_ALBEDO</define>
	<define>TEXTURE_NONPBR_METALNESS</define>
*/

// convert non-pbr albedo/metalness to pbr color/reflectivity.
#if defined TEXTURE_NONPBR_ALBEDO || defined TEXTURE_NONPBR_METALNESS
vec3 nonpbrMetalnessToColor( in vec3 albedo, in float metalness ){
	return mix( albedo, vec3( 0 ), metalness );
}

vec3 nonpbrMetalnessToReflectivity( in vec3 albedo, in float metalness ){
	return mix( vec3( 0.04 ), albedo, metalness );
}
#endif
