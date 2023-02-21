
// render task step.
struct sRenderTask{
	// pipeline index
	// NOTE fits into 16-bit
	uint pipeline;
	
	// texture unit configuration
	// NOTE fits into 16-bit
	uint tuc;
	
	// shared VAO index
	// NOTE maybe fits into 16-bit
	uint vao;
	
	// render task shared instance index
	uint instance;
	
	// shared parameter block instance index starting at index 1. 0 means no spb
	// instance is used. if present a sub instance is added to the instance for
	// rendering using either (spbIndex) or (spbIndex, specialFlags) depending
	// if special flags are used or not
	uint spbInstance;
	
	// special flags. will be written to intance data for special uses if required
	// NOTE fits into 8-bit
	uint specialFlags;
	
	// count of sub instances. used by final render task only
	uint subInstanceCount;
};

/*
UBOLAYOUT_BIND(4) buffer RenderTask {
	sRenderTask pRenderTask[];
};
*/


/*
NOTE building render task

requirements:
- new SSBO of type uvec4 storing the intermediate data required for building the render task.
  the SSBO has to be large enough to contain "pipelinCount" + "textureCount" + "vaoCount"
  + "instanceCount". the counts are the global pool counts and thus reusable across multiple
  shader runs. this avoids the need for a read-back. all pool entries of the same type are
  located in a continuous block. each "entry" consumes 3 uint components.
  the meaning of the components are:
  - first: index to first entry of this type
  - count: count of all entries using this type
  - counter: counter for calculating "first" of child entries
  
- new UBO config parameters:
  - firstPipeline: index of first pipeline in the intermediate ssbo
  - pipelineCount: count of pipelines in the intermediate ssbo
  - firstTexture: index of first texture in the intermediate ssbo
  - textureCount: count of textures in the intermediate ssbo
  - firstVao: index of first vao in the intermediate ssbo
  - countVao: count of vaos in the intermediate ssbo
  - firstInstance: index of first instance in the intermediate ssbo 
  - countInstance: count of instances in the intermediate ssbo
  
1) clear intermediate SSBO to 0

2) run shader building list of all render task steps. this uses "spbInstance" and "specialFlags"
   but not "subInstanceCount" which is left at 0. for each step also apply these atomic
   operations on the intermediate SSBO:
   - atomicAdd(ssbo.pipelines[pipeline].count, 1​)
   - atomicAdd(ssbo.textures[texture].count, 1)
   - atomicAdd(ssbo.vaos[vao].count, 1)
   - atomicAdd(ssbo.instances[instance].count, 1)
   
   => problem: this counts instances up for each sub instance but in the final render task
               subinstances are grouped into instances hence only one instance should be
               counted not all subinstances
   
   => one solution would be to add another shader run counting sub instances per instance.
      this way atomicAdd() with negative count (actually count - 1) could be used on parent
      pipeline, texture and vao. then the counts would be correct for one instance per group
      of sub instances
      => not a good solution to use negative counts. not using this method
   
   => another solution would be to update the counts only on ssbo.instances[instance].count .
      then a run can be made across all instances and for each instance with count > 0 the
      parent counts can be updated by 1 instead of count. this way no extra run with negate
      value can be used and the amount of atomic operations can be reduced too. actually this
      can be carried further by updating only the parent vao counts. then run vaos and update
      the parent texture counts (with count not 1) and so forth. this would be the least count
      of atomic operations required to update all counters properly
   
   => another solution would be to update first the instance count by 1. if the old count
      as returned by atomicAdd is 0 also increment pipeline, texture and vao count. this
      way the counts are only incremented by one instance (the first one) and only one
      shader run is required. might be slower than multiple runs as above but is the most
      simple solution to implement and validate

3) run shader across all pipelines in the intermediate buffer allocating continuous blocks to
   pipelines with count > 0 (dispatch from firstPipeline to firstPipeline + pipelinCount).
   for this a shared atomic counter can be used since the result is not required afterwards
   - ssbo.pipelines[invocation].first = atomicAdd(sharedVarCounter, ssbo.pipelines[invocation].count)
   
   => problem: since child elements can be used across multiple parent elements this would
               require pipelineCount * textureCount * vaoCount * instanceCount entries to
               cover the worst case. the required memory would explode.
               => this solution is thus not working
   
4) run shader across all textures in the intermediate buffer allocating continuous blocks to
   textures with count > 0 (dispatch from firstTexture to firstTexture + textureCount).
   blocks are continuous inside the parent pipelines. for this the counter variable of the
   pipeline entries are used
   - ssbo.textures[invocation].first = atomicAdd(ssbo.pipelines[parent].counter, ssbo.textures[invocation].count)
   
5) run shader across all vaos in the intermediate buffer allocating continuous blocks to vaos
   with count > 0 (dispatch from firstVao to firstVao + vaoCount). blocks are continuous inside
   the parent texture. for this the counter variable of the texture entries are used
   - ssbo.vaos[invocation].first = atomicAdd(ssbo.textures[parent].counter, ssbo.vaos[invocation].count)
   
6) run shader across all instances in the intermediate buffer allocating continuous blocks to
   instances with count > 0 (dispatch from firstInstance to firstInstance + instanceCount).
   blocks are continuous inside the parent vao. for this the counter variable of the vao
   entries are used.
   - ssbo.instances[invocation].first = atomicAdd(ssbo.vaos[parent].counter, ssbo.instances[invocation].count)
   
7) run shader building final render task using the grouping stored in the intermediate ssbo.
   this one is tricky. only the first render step using the instance must write the step
   to the render task. all sub instances instead should be written to the index ssbo and
   referenced in the render step
   
   - stepIndex = ssbo.pipelines[pipeline].first + ssbo.textures[texture].first
                 + ssbo.vaos[vao].first + ssbo.instances[instance].first
                 //+ atomicAdd(ssbo.instances[instance].counter, 1)
   - write step to stepIndex
   
   => one solution would be to run a shader across all instances reseting the counter
      variable to 0. then while running the shader the atomicAdd has to be used to
      get the sub instance index to write to. if the index is 0 write the render step

*/
