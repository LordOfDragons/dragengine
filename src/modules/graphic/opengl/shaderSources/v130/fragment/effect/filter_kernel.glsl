precision mediump float;
precision mediump int;

uniform vec4 pOptions; // rows, cols, stepU, stepV
uniform vec3 pKernel1;
uniform vec3 pKernel2;
uniform vec3 pKernel3;

uniform mediump sampler2D texColor;

in vec2 vTexCoord;

out mediump vec4 outColor;

void main( void ){
	outColor = vec4( texture( texColor, vTexCoord + vec2( -pOptions.z, -pOptions.w ) ).rgb * pKernel1.x, 1.0 );
	outColor.rgb += texture( texColor, vTexCoord + vec2( 0.0, -pOptions.w ) ).rgb * pKernel1.y;
	outColor.rgb += texture( texColor, vTexCoord + vec2( pOptions.z, -pOptions.w ) ).rgb * pKernel1.z;
	
	outColor.rgb += texture( texColor, vTexCoord + vec2( -pOptions.z, 0.0 ) ).rgb * pKernel2.x;
	outColor.rgb += texture( texColor, vTexCoord ).rgb * pKernel2.y;
	outColor.rgb += texture( texColor, vTexCoord + vec2( pOptions.z, 0.0 ) ).rgb * pKernel2.z;
	
	outColor.rgb += texture( texColor, vTexCoord + vec2( -pOptions.z, pOptions.w ) ).rgb * pKernel3.x;
	outColor.rgb += texture( texColor, vTexCoord + vec2( 0.0, pOptions.w ) ).rgb * pKernel3.y;
	outColor.rgb += texture( texColor, vTexCoord + vec2( pOptions.z, pOptions.w ) ).rgb * pKernel3.z;
	
	outColor.rgb = max( outColor.rgb, vec3( 0.0 ) );
}
