
// set render task step
void setRenderTaskStepGeometry( in uint renderStep, in uint pass, in uint geometry,
in uint pipelineList, in uint pipelineType, in uint pipelineModifier, in uint specialFlags ){
	setRenderTaskStep( renderStep, pass,
		skinTexturePipeline(
			pElementGeometries[ geometry ].skinTexture,
			pipelineList,
			pipelineType, pipelineModifier ),
		elementGeometryTUC( geometry, pipelineType ),
		pElementGeometries[ geometry ].vao,
		pElementGeometries[ geometry ].instance,
		pElementGeometries[ geometry ].spbInstance,
		specialFlags );
}
