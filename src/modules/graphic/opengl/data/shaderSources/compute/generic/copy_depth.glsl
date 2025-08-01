precision HIGHP float;
precision HIGHP int;

layout(local_size_x=16, local_size_y=16) in;

// PROBLEM: compute shaders can not read/write depth images... U_U
layout(rgba32f, binding=0) uniform restrict readonly image2DArray inDepth;
layout(rgba32f, binding=1) uniform restrict writeonly image2DArray outDepth;

#ifdef WITH_COLOR
layout(rgba32f, binding=2) uniform restrict readonly image2DArray inColor;
layout(rgba32f, binding=3) uniform restrict writeonly image2DArray outColor;
#endif

void main(){
	ivec3 tc = ivec3(gl_GlobalInvocationID.xyz);
	
	imageStore(outDepth, tc, imageLoad(inDepth, tc));
	
	#ifdef WITH_COLOR
	imageStore(outColor, tc, imageLoad(inColor, tc));
	#endif
}
