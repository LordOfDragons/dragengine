
	vec2 tcnoise = gl_FragCoord.xy / vec2(32.0);
	float shadow;
	
	if(PcfMode == PcfModeVarTap){
		vec2 shascale = params.xy * vec2(clamp(position.p * 5.0, 0.5, 5.0));
		
		// problem is that position.q is perspective in the shadow map. for the var-tap though
		// the distance would be required. just multiplying by position.q doesn't work since
		// then it's relative to the scaling. besides q is not transported in the position
		// unless required for special use
		ivec2 vtcount = ivec2(-2, 2);
		float vtweight = 25.0; // 5 pixel * 5 pixel
		ES2DTC tc = position;
		int x, y;
		
		shadow = 0.0;
		
		for(y=vtcount.x; y<vtcount.y; y++){
			tc.t = shascale.y * float(y) + position.t;
			
			for(x=vtcount.x; x<vtcount.y; x++){
				tc.s = shascale.x * float(x) + position.s;
				shadow += SSM(texsm, tc, tcnoise, params.zz);
			}
		}
		
		shadow /= vtweight;
		
	}else if(PcfMode == PcfMode9Tap){
		shadow = SSM(texsm, position, tcnoise, params.zz);
		
	}else if(PcfMode == PcfMode4Tap){
		shadow = 0.0;
		
	}else{ // PcfMode == PcfMode1Tap
		shadow = SSM(texsm, position, tcnoise, params.zz);
	}
	
	vec4 tcoffset;
	if(PcfMode == PcfMode4Tap || PcfMode == PcfMode9Tap){
		if(PcfMode == PcfMode4Tap){
			tcoffset = params.xyxy * pcf4TapTCScale; // (u/2,v/2) (u/2,-v/2) (-u/2,v/2) (-u/2,-v/2)
			
		}else{
			tcoffset = params.xyxy * pcf9TapTCScale; // (u,v) (u,-v) (-u,v) (-u,-v)
		}
		
		shadow += SSM2(texsm, position, tcoffset.xy, tcnoise, ivec2(1,1), params.zz);
		shadow += SSM2(texsm, position, tcoffset.xw, tcnoise, ivec2(1,-1), params.zz);
		shadow += SSM2(texsm, position, tcoffset.zy, tcnoise, ivec2(-1,1), params.zz);
		shadow += SSM2(texsm, position, tcoffset.zw, tcnoise, ivec2(-1,-1), params.zz);
	}
	
	if(PcfMode == PcfMode9Tap){
		tcoffset = params.xyxy * pcf9TapTCScale2; // (u, 0) (-u, 0) (0, v) (0, -v)
		
		shadow += SSM2(texsm, position,  tcoffset.xw, tcnoise, ivec2(1,0), params.zz);
		shadow += SSM2(texsm, position, -tcoffset.xw, tcnoise, ivec2(-1,0), params.zz);
		shadow += SSM2(texsm, position,  tcoffset.zy, tcnoise, ivec2(0,1), params.zz);
		shadow += SSM2(texsm, position, -tcoffset.zy, tcnoise, ivec2(0,-1), params.zz);
	}
	
	if(PcfMode == PcfMode4Tap){
		shadow *= pcf4TapWeight;
		
	}else if(PcfMode == PcfMode9Tap){
		shadow *= pcf9TapWeight;
	}
	
	#undef SSM
	#undef SSM2
	#undef ES2D
	#undef ES2DTC
