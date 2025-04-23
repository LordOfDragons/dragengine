precision HIGHP float;
precision HIGHP int;

layout(location=0) out vec4 outDiffuse; // diffuse.r, diffuse.g, diffuse.b, transparency
/*
layout(location=1) out vec4 outNormal; // normal.x, normal.y, normal.z, blend
layout(location=2) out vec4 outReflectivity; // reflectivity.r, reflectivity.g, reflectivity.b, blend
layout(location=3) out vec4 outRoughness; // roughness, n/a, n/a, blend
layout(location=4) out vec4 outAOSolidity; // ao, ssao, solidity, blend
layout(location=5) out vec4 outSubSurface; // subsurface.rgb, blend
layout(location=6) out vec4 outColor; // color.r, color.g, color.b, n/a
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
