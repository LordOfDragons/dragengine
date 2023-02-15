
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
};

/*
UBOLAYOUT_BIND(4) buffer RenderTask {
	sRenderTask pRenderTask[];
};
*/
