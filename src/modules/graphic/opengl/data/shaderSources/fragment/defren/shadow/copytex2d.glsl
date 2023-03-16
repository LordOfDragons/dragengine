precision highp float;
precision highp int;

uniform HIGHP sampler2D texDepth;

void main( void ){
	gl_FragDepth = texelFetch( texDepth, ivec2( gl_FragCoord.xy ), 0 ).r;
}
