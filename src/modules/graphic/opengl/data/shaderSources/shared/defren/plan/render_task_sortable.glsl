
// same as render_task.glsl but with vectorized definitions suitable for sorting
struct sRenderTaskSortable{
   // x: pass
   // x: pipeline
   // y: tuc
   // z: vao
   ivec4 params1;
   
   // x: instance
   // y: spbIndex
   // z: specialFlags
   // w: subInstanceCount
   ivec4 params2;
};

/*
UBOLAYOUT_BIND(1) buffer RenderTask {
	sRenderTaskSortable pRenderTask[];
};
*/
