
float thicknessShadow2D(in ARG_SAMP_HIGHP sampler2D texsm, in vec3 position){
	float thickness = SSSHATEX(texsm, position.st);
	thickness = pShadowDepthTransform.z / (pShadowDepthTransform.w - thickness);
	thickness = (pShadowDepthTransform.z / (pShadowDepthTransform.w - position.p)) - thickness;
	
	//return max(thickness, 0.0);
	return thickness;
}
