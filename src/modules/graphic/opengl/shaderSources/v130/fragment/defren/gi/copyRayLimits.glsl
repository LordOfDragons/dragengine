precision highp float;
precision highp int;

uniform mediump sampler2D texDistance;

flat in ivec2 vRayOffset;

out float outDistanceLimit;

void main( void ){
	ivec2 tc = ivec2( gl_FragCoord.xy ) + vRayOffset;
	
	outDistanceLimit = texelFetch( texDistance, tc, 0 ).r;
}
