precision highp float;
precision highp int;

uniform mediump sampler2D texPosition;
uniform mediump sampler2D texNormal;
uniform lowp sampler2D texDiffuse;
uniform lowp sampler2D texReflectivity;
uniform mediump sampler2D texLight;

flat in ivec2 vRayOffset;

out float outPosition;
out vec3 outNormal;
out vec3 outDiffuse;
out vec4 outReflectivity;
out vec3 outLight;

void main( void ){
	ivec2 tc = ivec2( gl_FragCoord.xy ) + vRayOffset;
	
	outPosition = texelFetch( texPosition, tc, 0 ).a;
	outNormal = texelFetch( texNormal, tc, 0 ).rgb;
	outDiffuse = texelFetch( texDiffuse, tc, 0 ).rgb;
	outReflectivity = texelFetch( texReflectivity, tc, 0 );
	outLight = texelFetch( texLight, tc, 0 ).rgb;
}
