// the index-ubo contains uint indices into the shared-spb ubo. due to the std140 alignment
// properties of arrays one uint in array form would consume 4-units of space hence wasting
// three vector components. to overcome this limitation the index-ubo has one array parameter
// with ivec4 format. each component is one index like this:
//   ivec4[0] = [index0, index1, index2, index3]
//   ivec4[1] = [index4, index5, index6, index7]
//   ...
// 
// this requires a bit of bit operation to obtain the correct index.
// 
// UBOLAYOUT uniform ivec4 pSPBInstanceIndex[ ?? ];
// 
// const int pSPBIndex = pSPBInstanceIndex[ gl_InstanceID / 4 ][ gl_InstanceID % 4 ];
// 

#ifdef SPB_SSBO_INSTANCE_ARRAY
	#include "shared/defren/plan/render_task.glsl"
	
	UBOLAYOUT_BIND(1) readonly buffer InstanceIndexSSBO{
		sRenderTask pSPBInstanceIndex[];
	};
	
#else
	UBOLAYOUT uniform InstanceIndex{
		#ifdef SPB_INSTANCE_ARRAY_SIZE
		ivec4 pSPBInstanceIndex[ SPB_INSTANCE_ARRAY_SIZE ];
		#else
		ivec4 pSPBInstanceIndex[];
		#endif
	};
#endif

uniform int pSPBInstanceIndexBase;
