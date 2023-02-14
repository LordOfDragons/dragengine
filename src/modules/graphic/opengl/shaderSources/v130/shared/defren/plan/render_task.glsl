
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
	
	// shared parameter block instance index starting at index 1. 0 means no spb instance is used.
	uint spbInstance;
	
	// special flags. will be written to intance data for special uses if required
	// NOTE fits into 8-bit
	uint specialFlags;
};


// define this somewhere. variable name pRenderTask has to stay intact
/*
UBOLAYOUT_BIND(2) readonly buffer RenderTask {
	sRenderTask pRenderTask[];
};
*/
