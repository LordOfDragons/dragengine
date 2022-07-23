precision highp float;
precision highp int;

uniform vec4 pQuadParams; // scaleX, scaleY, offsetX, offsetY

in vec3 inPosition;

out vec2 vTexCoord;

void main( void ){
	gl_Position = vec4( vec3( inPosition.xy * pQuadParams.xy + pQuadParams.zw, inPosition.z ), 1.0 );
	vTexCoord = inPosition.xy * vec2( 0.5 ) + vec2( 0.5 );
}
