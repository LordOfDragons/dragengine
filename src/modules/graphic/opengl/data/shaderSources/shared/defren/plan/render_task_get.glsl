
// get render task step
void getRenderTaskStep( in uint renderStep, out uint pass, out uint pipeline, out uint tuc,
out uint vao, out uint instance, out uint spbInstance, out uint specialFlags ){
	pass = pRenderTask[ renderStep ].pass;
	pipeline = pRenderTask[ renderStep ].pipeline;
	tuc = pRenderTask[ renderStep ].tuc;
	vao = pRenderTask[ renderStep ].vao;
	instance = pRenderTask[ renderStep ].instance;
	spbInstance = pRenderTask[ renderStep ].spbInstance;
	specialFlags = pRenderTask[ renderStep ].specialFlags;
}
