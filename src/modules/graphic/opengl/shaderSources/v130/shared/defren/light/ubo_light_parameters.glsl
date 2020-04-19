UBOLAYOUT uniform LightParameters{
	#ifndef PARTICLE_LIGHT
		vec3 pLightColor;
		
		float pLightRange;
			// used by:     point, spot
			// not used by: sky
		
		vec3 pLightColorAmbient;
			// used by:     sky
			// not used by: point, spot
		
		float pLightAmbientRatio;
			// used by:     point, spot
			// not used by: sky
	#endif
	
	float pLightAttenuationCoefficient;
	float pLightDampingCoefficient;
	float pLightDampingThreshold;
	
	float pLightImageGamma;
	
	#if defined SPOT_LIGHT || defined PROJECTOR_LIGHT
		float pLightSpotFactor;
		float pLightSpotBase;
		float pLightSpotExponent;
	#endif
};
