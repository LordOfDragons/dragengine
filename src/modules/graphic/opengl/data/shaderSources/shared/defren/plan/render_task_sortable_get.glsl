
// get render task sortable step
void getRenderTaskStep( in uint renderStep, out ivec4 params1, out ivec4 params2 ){
	params1 = pRenderTask[ renderStep ].params1;
	params2 = pRenderTask[ renderStep ].params2;
}
