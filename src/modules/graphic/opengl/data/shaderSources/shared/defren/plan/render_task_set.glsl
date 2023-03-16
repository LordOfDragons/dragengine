
// set render task step
void setRenderTaskStep( in uint renderStep, in uint pass, in uint pipeline, in uint tuc,
in uint vao, in uint instance, in uint spbInstance, in uint specialFlags ){
	pRenderTask[ renderStep ].pass = pass;
	pRenderTask[ renderStep ].pipeline = pipeline;
	pRenderTask[ renderStep ].tuc = tuc;
	pRenderTask[ renderStep ].vao = vao;
	pRenderTask[ renderStep ].instance = instance;
	pRenderTask[ renderStep ].spbInstance = spbInstance;
	pRenderTask[ renderStep ].specialFlags = specialFlags;
	
	// this is a bit special. in the CPU version a sub instance is added by all geometry
	// not requiring instancing. for all these sub instance count has to be set to 1.
	// those requiring instancing though require sub instance count to be set to 0.
	// prop field clusters for example are such geometry.
	// 
	// this distinction is required to later on properly group sub instances. this is
	// only allowed for geometries not requiring explicit instancing
	pRenderTask[ renderStep ].subInstanceCount = spbInstance == uint( 0 ) ? uint( 0 ) : uint( 1 );
}
