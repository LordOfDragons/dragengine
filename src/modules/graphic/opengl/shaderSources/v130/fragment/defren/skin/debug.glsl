precision highp float;
precision highp int;

out vec4 outDiffuse; // diffuse.r, diffuse.g, diffuse.b, transparency
/*
out vec4 outNormal; // normal.x, normal.y, normal.z, blend
out vec4 outReflectivity; // reflectivity.r, reflectivity.g, reflectivity.b, blend
out vec4 outRoughness; // roughness, n/a, n/a, blend
out vec4 outAOSolidity; // ao, ssao, solidity, blend
out vec4 outSubSurface; // subsurface.rgb, blend
out vec4 outColor; // color.r, color.g, color.b, n/a
*/

void main( void ){
	outDiffuse = vec4( 1.0, 1.0, 1.0, 1.0 );
	/*
	outNormal = vec4( 0.0, 0.0, 1.0, 1.0 );
	outReflectivity = vec4( 0.0, 1.0, 1.0, 1.0 );
	outRoughness = vec4( 1.0, 1.0, 1.0, 1.0 );
	outAOSolidity = vec4( 1.0, 1.0, 1.0, 1.0 );
	outSubSurface = vec4( 0.0, 0.0, 0.0, 0.0 );
	outColor = vec4( 0.0, 0.0, 0.0, 1.0 );
	*/
}
