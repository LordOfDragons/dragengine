layout(binding=0, rgba16f) uniform writeonly restrict mediump image2D texPosition;
layout(binding=1, rgba8_snorm) uniform writeonly restrict mediump image2D texNormal;
layout(binding=2, rgba8) uniform writeonly restrict lowp image2D texDiffuse;
layout(binding=3, rgba8) uniform writeonly restrict lowp image2D texReflectivity;
layout(binding=4, rgba16f) uniform writeonly restrict mediump image2D texLight;

layout( local_size_x=128, local_size_y=8 ) in;

void main( void ){
	ivec2 tc = ivec2( gl_GlobalInvocationID );
	
	imageStore( texPosition, tc, vec4( 0, 0, 0, 10000 ) );
	imageStore( texNormal, tc, vec4( 0, 0, 1, 0 ) );
	imageStore( texDiffuse, tc, vec4( 1, 1, 1, 0 ) );
	imageStore( texReflectivity, tc, vec4( 0, 0, 0, 1 ) );
	imageStore( texLight, tc, vec4( 0, 0, 0, 0 ) );
}
