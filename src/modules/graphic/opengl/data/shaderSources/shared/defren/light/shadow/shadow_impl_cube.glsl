
	vec2 tcnoise = gl_FragCoord.xy / vec2(32.0);
	float pdist = length(position.xyz);
	mat2x3 noiseMatrix;
	vec3 no1, no2;
	float shadow;
	
	if(PcfMode == PcfMode9Tap || PcfMode == PcfMode4Tap || PcfMode == PcfModeVarTap){
		float nolen = pdist * params.z;
		
		vec3 aposition = abs(position.xyz);
		bvec4 atest = greaterThan(aposition.xxyy, aposition.yzxz); // (x>y, x>z, y>x, y>z)
		//vec3 r = step(absshapos.xyz, absshapos.yzx) * step(absshapos.xyz, absshapos.zxy);
		//vec2 scale = vec2(dot(absshapos, r) * pShaOpts1.z);
		//o1.xz = (c2 * r.xx + c1) * scale;
		//o2.zy = (c2 * r.yy + c1) * scale;
		if(all(atest.xy)){ // x is largest
			no1 = vec3(0.0, 0.0, nolen);
			no2 = vec3(0.0, nolen, 0.0);
			
		}else if(all(atest.zw)){ // y is largest
			no1 = vec3(nolen, 0.0, 0.0);
			no2 = vec3(0.0, 0.0, nolen);
			
		}else{ // z is largest
			no1 = vec3(nolen, 0.0, 0.0);
			no2 = vec3(0.0, nolen, 0.0);
		}
		
		noiseMatrix = mat2x3(no1, no2);
	}
	
	if(PcfMode == PcfModeVarTap){
		vec2 shascale = params.xy * vec2(clamp(pdist * 5.0, 0.5, 5.0));
		vec4 tc = position;
		vec3 offy;
		int x, y;
		
		no1 *= vec3(shascale.x);
		no2 *= vec3(shascale.y);
		noiseMatrix = mat2x3(no1, no2); //noiseMatrix *= shascale;
		shadow = 0.0;
		
		for(y=-2; y<2; y++){
			offy = position.stp + no2 * vec3(y);
			
			for(x=-2; x<2; x++){
				tc.xyz = offy + no1 * vec3(x);
				shadow += SSM(texsm, tc, tcnoise, noiseMatrix);
			}
		}
		
		shadow /= 25.0;
		
	}else if(PcfMode == PcfMode9Tap){
		shadow = SSM(texsm, position, tcnoise, noiseMatrix);
		
	}else if(PcfMode == PcfMode4Tap){
		shadow = 0.0;
		
	}else{ // PcfMode == PcfMode1Tap
		shadow = SSM(texsm, position, tcnoise, noiseMatrix);
	}
	
	vec4 pcfscale;
	if(PcfMode == PcfMode4Tap || PcfMode == PcfMode9Tap){
		if(PcfMode == PcfMode4Tap){
			pcfscale = pcf4TapTCScale; // (u/2,v/2) (u/2,-v/2) (-u/2,v/2) (-u/2,-v/2)
			
		}else{
			pcfscale = pcf9TapTCScale; // (u,v) (u,-v) (-u,v) (-u,-v)
		}
		
		shadow += SSM(texsm, position, noiseMatrix * pcfscale.xy, tcnoise, ivec2(1,1), noiseMatrix);
		shadow += SSM(texsm, position, noiseMatrix * pcfscale.xw, tcnoise, ivec2(1,-1), noiseMatrix);
		shadow += SSM(texsm, position, noiseMatrix * pcfscale.zy, tcnoise, ivec2(-1,1), noiseMatrix);
		shadow += SSM(texsm, position, noiseMatrix * pcfscale.zw, tcnoise, ivec2(-1,-1), noiseMatrix);
	}
	
	if(PcfMode == PcfMode9Tap){
		// (u, 0) (-u, 0) (0, v) (0, -v)
		shadow += SSM(texsm, position, noiseMatrix * pcf9TapTCScale2.xw, tcnoise, ivec2(1,0), noiseMatrix);
		shadow += SSM(texsm, position, noiseMatrix * -pcf9TapTCScale2.xw, tcnoise, ivec2(-1,0), noiseMatrix);
		shadow += SSM(texsm, position, noiseMatrix * pcf9TapTCScale2.zy, tcnoise, ivec2(0,1), noiseMatrix);
		shadow += SSM(texsm, position, noiseMatrix * -pcf9TapTCScale2.zy, tcnoise, ivec2(0,-1), noiseMatrix);
	}
	
	if(PcfMode == PcfMode4Tap){
		shadow *= pcf4TapWeight;
		
	}else if(PcfMode == PcfMode9Tap){
		shadow *= pcf9TapWeight;
	}
	
	#undef SSM
