/*
	<!-- shared/defren/light/ubo_light_parameters.glsl -->
	<uniformBlock name='LightParameters' binding='2'/>
*/

/*
affects:
S: spot
PR: projector light
P: point
PE: particle
SL: sky layer
GI: gi
~X: all except X
*/

UBOLAYOUT_BIND(2) uniform LightParameters{
	vec3 pLightColor; // ~PE
	float pLightRange; // P, S, PR
	vec3 pLightColorAmbient; // SL
	float pLightAmbientRatio; // P, S, PR
	vec3 pLightColorAmbientGI; // GI
	
	float pLightAttenuationCoefficient;
	float pLightDampingCoefficient;
	float pLightDampingThreshold;
	
	float pLightImageGamma;
	
	float pLightSpotFactor; // S, PR
	float pLightSpotBase; // S, PR
	float pLightSpotExponent; // S, PR
};
