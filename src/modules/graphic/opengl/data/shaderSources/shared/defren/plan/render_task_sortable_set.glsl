
// set render task sotrable step
void setRenderTaskStep( in uint renderStep, in ivec4 params1, in ivec4 params2 ){
	pRenderTask[ renderStep ].params1 = params1;
	pRenderTask[ renderStep ].params2 = params2;
}
