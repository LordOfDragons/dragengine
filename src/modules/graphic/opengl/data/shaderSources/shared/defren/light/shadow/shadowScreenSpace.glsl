
// screen space shadows
/////////////////////////

float screenSpaceShadow(ARG_SAMP_HIGHP sampler2DArray samplerDepth, const in vec3 position, const in vec3 lightDir){
	float maxLength = pSSShadowMaxLengthBase + position.z * pSSShadowMaxLengthScalePerMeter;
	float thickness = pSSShadowThicknessBase + position.z * pSSShadowThicknessScalePerMeter;
	int stepCount = int(pSSShadowStepCount);
	
	float stepSize = maxLength / float(stepCount);
	vec3 rayDir = lightDir * stepSize;
	vec3 rayPosition = position;
	
	ivec2 tcFrag = ivec2(gl_FragCoord.xy);
	ivec3 tcTest = ivec3(tcFrag, vLayer);
	
	vec3 screenCoord = vec3(0.0, 0.0, vLayer);
	vec3 testPosition;
	
	ivec2 sizeNoise = textureSize(SHADOW_NOISE_TEXTURE, 0);
	vec4 noiseValue = texelFetch(SHADOW_NOISE_TEXTURE, tcFrag % sizeNoise, 0);
	
	// randomly shift the ray start position. this hides banding of the discrete step size with noise
	rayPosition += rayDir * vec3(noiseValue.x);
	
	for(int i=0; i<stepCount; i++){
		rayPosition += rayDir;
		
		// transform ray position into screen coordinates. if false is returned the ray left the
		// screen and any further testing can be skipped
		if(!positionToScreen(rayPosition, vLayer, screenCoord.xy)){
			break;
		}
		
		// skip if the integer texture coordinates are the same as the original fragment.
		// this happens if the light direction combined with small step size does not move
		// the pixel to sample enough. this avoids fragments testing themselves and considering
		// themselves as blockers which would cause fragments to end up in shadow that should not
		tcTest.xy = ivec2((screenCoord.xy + vec2(1.0)) / vec2(2.0) * pRenderSize);
		if(tcTest.xy == tcFrag){
			continue;
		}
		
		float depth = sampleDepth(samplerDepth, tcTest);
		testPosition = depthToPosition(depth, screenCoord.xy, vLayer);
		
		float diff = rayPosition.z - testPosition.z;
		if(InverseDepth){
			diff = -diff;
		}
		
		// consider in shadow only if the ray position is behind the test position and the
		// ray most probably does not pass behind the blocker. the thickness here is a guess.
		// a better solution would be to stored while rendering the estimated thickness of
		// the geometry. then rays passing behind the blocker could be better detected
		if(diff <= 0.001/*0.0*/ || diff >= thickness){
			continue;
		}
		
		// fade the shadow towards the end of the ray
		float shadow = pow(length(rayPosition - position) / maxLength, 2.0);
		
		// fade the shadow towards the screen border
		vec2 scblend = min((vec2(1.0) - abs(screenCoord.xy)) / vec2(pSSShadowBorderBlendRange), vec2(1.0));
		shadow = mix(1.0, shadow, scblend.x * scblend.y);
		
		return shadow;
	}
	
	return 1.0;
}
