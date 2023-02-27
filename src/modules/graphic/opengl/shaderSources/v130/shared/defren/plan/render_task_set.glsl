
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
	pRenderTask[ renderStep ].subInstanceCount = uint( 0 );
}
