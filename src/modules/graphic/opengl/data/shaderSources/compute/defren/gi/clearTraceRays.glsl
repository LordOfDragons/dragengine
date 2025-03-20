layout(binding=0, rgba16f) uniform writeonly restrict HIGHP image2D texPosition;
layout(binding=1, rgba8_snorm) uniform writeonly restrict HIGHP image2D texNormal;
layout(binding=2, rgba8) uniform writeonly restrict lowp image2D texDiffuse;
layout(binding=3, rgba8) uniform writeonly restrict lowp image2D texReflectivity;
layout(binding=4, rgba16f) uniform writeonly restrict HIGHP image2D texLight;

// desktop can usually do up to 1024 invocation size. on weaker systems like
// smartphones this can down all the way to 256. using the largest value supported
layout(local_size_x=LOCAL_SIZE_X, local_size_y=LOCAL_SIZE_Y) in;

void main(void){
	ivec2 tc = ivec2(gl_GlobalInvocationID);
	
	imageStore(texPosition, tc, vec4(0, 0, 0, 10000));
	imageStore(texNormal, tc, vec4(0, 0, 1, 0));
	imageStore(texDiffuse, tc, vec4(1, 1, 1, 0));
	imageStore(texReflectivity, tc, vec4(0, 0, 0, 1));
	imageStore(texLight, tc, vec4(0, 0, 0, 0));
}
