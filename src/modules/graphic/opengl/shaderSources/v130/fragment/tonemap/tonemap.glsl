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

void main( void ){
	ivec2 tc = ivec2( gl_FragCoord.xy );
	
	vec3 params = texelFetch( texToneMapParams, tcParams, 0 ).rgb; // r=avgLum, g=scaleLum, b=lwhite, a=n/a
	vec4 color = texelFetch( texColor, tc, 0 );
	
	// calculate luminance
//	float luminance = dot( color, lumiFactors );
//	float finalLuminance = luminance * pOptions.x;
//	finalLuminance = finalLuminance / ( 1.0 + finalLuminance );
	
	// classic reinhard
//	float luminance = dot( color, lumiFactors ) * pOptions.x;
//	luminance = luminance / ( 1.0 + luminance );
	
	// enhanced reinhard
	//float scaleLum = pOptions.x;
	//float lwhite = pOptions.y;
/*	vec4 params = texture2D( texToneMapParams, vec2( 0.5, 0.5 ) ); // r=luminance, g=scaleLum, b=lwhite, a=n/a
	float scaleLum = params.g;
	float lwhite = params.b;
	
	float luminance = dot( color, lumiFactors ) * scaleLum;
	//float lwhite = 1.0 / ( 2.0 * 2.0 );
	float finalLuminance = luminance * ( 1.0 + luminance * lwhite ) / ( 1.0 + luminance );
	
	// adaptive logaritmic mapping
//	float luminance = dot( color, lumiFactors ) * pOptions.x;
//	float lwmax = 2.0;
//	float t = log( 10.0 ) / log( lwmax + 1 ); //100.0 * 0.01; // 1
//	float b = 0.85;
//	luminance = t * log( luminance + 1 ) / log( 2.0 + 8.0 * pow( luminance / lwmax, log( b ) / log( 0.5 ) ) );
	
	// calculate final color including overbrighting
//	gl_FragColor.rgb = color * finalLuminance / ( luminance + 0.00001 );
	
//	gl_FragColor.rgb = normalize( color ) * finalLuminance;
//	gl_FragColor.rgb = color * ( finalLuminance / luminance ) + vec3( max( ( luminance - pOptions.z ) * pOptions.w, 0.0 ) );
//	gl_FragColor.rgb = sqrt( color * ( finalLuminance / luminance ) );
//	gl_FragColor.rgb = sqrt( color * ( pOptions.x * finalLuminance ) );
//	gl_FragColor.rgb = color * ( pOptions.x * finalLuminance );
	gl_FragColor.rgb = color * scaleLum * sqrt( finalLuminance );
	//	smoothstep( pOptions.z, 1.0, luminance ) );
	*/
	
	
	
	color.rgb += textureLod( texBloom, min( vTCBloom, pTCBloomClamp ), 0.0 ).rgb * vec3( pOptions.x );
	
	float luminance = dot( color.rgb, lumiFactors );
	float adjLum = luminance * params.g;
	float finalLum = adjLum * ( 1.0 + adjLum * params.b ) / ( 1.0 + adjLum );
	//float finalLum = adjLum / ( 1.0 + adjLum );
	outColor = vec4( color.rgb * vec3( finalLum / ( luminance + epsilon ) ), color.a );
	//outColor = vec4( ( color.rgb / vec3( luminance + epsilon ) ) * finalLum, color.a );
	
//	outColor.rgb += texture( texBloom, vTCBloom ).rgb * vec3( pOptions.x );// * ( finalLum - pOptions.x;
	
	//outColor.rgb += texture( texBloom, vTexCoord ).rgb;
	
	//vec4 params = texture2D( texToneMapParams, vec2( 0.0, 0.0 ) ); // r=avgLum, g=scaleLum, b=lwhite, a=n/a
	//float scaleLum = params.g;
	/*float threshold = 0.8; // m$(5)
	float offset = 5.0; // m$(10)
	
	color = max( color * scaleLum - threshold, 0.0 );
	gl_FragColor.rgb = color / ( color + offset );*/
	
	//float threshold = 0.8;
	//gl_FragColor.rgb = max( color.rgb * scaleLum - threshold, 0.0 ) / threshold;
}
