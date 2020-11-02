precision highp float;
precision highp int;

uniform vec4 pOptions; // exposure, 1/(lwhite^2), overbrightLimit, overbrightFactor
uniform vec2 pTCBloomClamp; // clampU, clampV

uniform mediump sampler2D texColor;
uniform mediump sampler2D texToneMapParams;
uniform mediump sampler2D texBloom;

in vec2 vTCBloom;

out vec4 outColor;

const float epsilon = 0.0001;
const ivec2 tcParams = ivec2( 0, 0 );
const vec3 lumiFactors = vec3( 0.2125, 0.7154, 0.0721 );
//const vec3 lumiFactors = vec3( 0.3086, 0.6094, 0.0820 ); // nVidia

float uchimura(float x, float P, float a, float m, float l, float c, float b) {
	// Uchimura 2017, "HDR theory and practice"
	// Math: https://www.desmos.com/calculator/gslcdxvipg
	// Source: https://www.slideshare.net/nikuque/hdr-theory-and-practicce-jp
	float l0 = ((P - m) * l) / a;
	float L0 = m - m / a;
	float L1 = m + (1.0 - m) / a;
	float S0 = m + l0;
	float S1 = m + a * l0;
	float C2 = (a * P) / (P - S1);
	float CP = -C2 / P;
	
	float w0 = 1.0 - smoothstep(0.0, m, x);
	float w2 = step(m + l0, x);
	float w1 = 1.0 - w0 - w2;
	
	float T = m * pow(x / m, c) + b;
	float S = P - (P - S1) * exp(CP * (x - S0));
	float L = m + a * (x - m);
	
	return T * w0 + L * w1 + S * w2;
}

float uchimura(float x) {
	const float P = 1.0;  // max display brightness
	const float a = 1.0;  // contrast
	const float m = 0.22; // linear section start
	const float l = 0.4;  // linear section length
	const float c = 1.33; // black
	const float b = 0.0;  // pedestal
	return uchimura(x, P, a, m, l, c, b);
}

void main( void ){
	ivec2 tc = ivec2( gl_FragCoord.xy );
	
	vec3 params = texelFetch( texToneMapParams, tcParams, 0 ).rgb; // r=avgLum, g=scaleLum, b=lwhite, a=n/a
	vec4 color = texelFetch( texColor, tc, 0 );
	
	// classic reinhard
//	float luminance = dot( color, lumiFactors ) * pOptions.x;
//	luminance = luminance / ( 1.0 + luminance );
	
	// enhanced reinhard
// 	color.rgb += textureLod( texBloom, min( vTCBloom, pTCBloomClamp ), 0.0 ).rgb * vec3( pOptions.x );
// 	
// 	float luminance = dot( color.rgb, lumiFactors );
// 	float adjLum = luminance * params.g;
// 	float finalLum = adjLum * ( 1.0 + adjLum * params.b ) / ( 1.0 + adjLum );
// 	outColor = vec4( color.rgb * vec3( finalLum / ( luminance + epsilon ) ), color.a );
	
	// uchimura, applied per channel
	color *= params.g;
	outColor.r = uchimura( color.r );
	outColor.g = uchimura( color.g );
	outColor.b = uchimura( color.b );
	outColor.a = color.a;
	
	// bloom
	outColor.rgb += textureLod( texBloom, min( vTCBloom, pTCBloomClamp ), 0.0 ).rgb * vec3( pOptions.x );
	
	// NOTE nice comparison of curves: https://www.shadertoy.com/view/WdjSW3
	//      the uchimura curve is the only one which has a linear left side. all others try
	//      to add burning out the blacks which is annoying. the tone map operator should
	//      provide a balance output. if an artist wants burning out blacks he should do
	//      so after the tone mapping is done (still float buffer)
}
