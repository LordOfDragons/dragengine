precision highp float;
precision highp int;


uniform mediump sampler2D texPosition;
uniform mediump sampler2D texNormal;
uniform mediump sampler2D texMaterial;
uniform mediump sampler2D texTexCoord;


flat in ivec2 vRayOffset;


out vec4 outPosition;
out vec3 outNormal;
out float outMaterial;
out vec2 outTexCoord;


void main( void ){
	ivec2 tc = ivec2( gl_FragCoord.xy ) + vRayOffset;
	
	outPosition = texelFetch( texPosition, tc, 0 );
	outNormal = texelFetch( texNormal, tc, 0 ).rgb;
	outMaterial = texelFetch( texMaterial, tc, 0 ).r;
	outTexCoord = texelFetch( texTexCoord, tc, 0 ).rg;
}
