/*
	<!-- shared/defren/skin/dither.glsl -->
	<define>TEXTURE_SOLIDITY</define>
	<define>TEXTURE_TRANSPARENCY</define>
*/


const int DitherSolidityMethodBayer = 0;
const int DitherSolidityMethodGradientNoise = 1;
const int DitherSolidityMethodBlueNoise = 2;

const int ditherSolidityMethod = DitherSolidityMethodBlueNoise;

const int ditherBayerMatrix[16] = int[](0, 8, 2, 10, 12, 4, 14, 6, 3, 11, 1, 9, 15, 7, 13, 5);


// binary dither using Bayer matrix
float ditherBinaryBayer(in float value, const in int spbIndex){
	ivec2 fc = ivec2(gl_FragCoord.xy) & 3;
	float threshold = float(ditherBayerMatrix[fc.x + fc.y * 4]) / 16.0;
	return value > threshold ? 1.0 : 0.0;
}

// binary dither using interleaved gradient noise
float ditherBinaryGradientNoise(in float value, const in int spbIndex){
	ivec2 fc = ivec2(gl_FragCoord.xy);
	float threshold = fract(52.9829189 * fract(0.06711056 * float(fc.x) + 0.00583715 * float(fc.y)));
	return value > threshold ? 1.0 : 0.0;
}

// binary dither using blue noise
float ditherBinaryBlueNoise(in float value, const in int spbIndex, const in int seed){
	ivec2 tc = ivec2(gl_FragCoord.xy);
	ivec2 size = textureSize(texNoise, 0); // required since texelFetch is undefined outside boundaries
	float threshold = texelFetch(texNoise, tc % size, 0)[seed % 4];
	return value > threshold ? 1.0 : 0.0;
}


// dither solidity
float ditherSolidity(in float solidity, const in int spbIndex, const in int seed){
	if(ditherSolidityMethod == DitherSolidityMethodBayer){
		return ditherBinaryBayer(solidity, spbIndex);
		
	}else if(ditherSolidityMethod == DitherSolidityMethodGradientNoise){
		return ditherBinaryGradientNoise(solidity, spbIndex);
		
	}else if(ditherSolidityMethod == DitherSolidityMethodBlueNoise){
		return ditherBinaryBlueNoise(solidity, spbIndex, seed);
	}
	return solidity;
}

float ditherSolidity(in float solidity, const in int spbIndex){
	return ditherSolidity(solidity, spbIndex, 0);
}
