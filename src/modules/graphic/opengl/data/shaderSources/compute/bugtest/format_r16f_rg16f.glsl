layout(binding=0, r16f) uniform writeonly restrict mediump image2DArray texTest1;
layout(binding=1, rg16f) uniform writeonly restrict mediump image2DArray texTest2;

layout(local_size_x=1) in;

void main(void){
	imageStore(texText1, ivec3(gl_GlobalInvocationID), vec4(0.25));
	imageStore(texText2, ivec3(gl_GlobalInvocationID), vec4(0.75));
}
