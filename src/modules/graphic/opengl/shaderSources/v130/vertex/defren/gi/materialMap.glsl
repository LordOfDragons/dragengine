precision highp float;
precision highp int;

uniform vec4 pQuadParams; // scaleX, scaleY, offsetX, offsetY

in vec2 inPosition;

out vec2 vTexCoord;

void main( void ){
	gl_Position = vec4( inPosition * pQuadParams.xy + pQuadParams.zw, 0, 1 );
	vTexCoord = inPosition * vec2( 0.5 ) + vec2( 0.5 );
}
