precision highp float;
precision highp int;

uniform vec4 pPosToBloom; // scaleX, scaleY, offsetX, offsetY

in vec2 inPosition;

out vec2 vTCBloom;

void main( void ){
	gl_Position = vec4( vec3( inPosition, 0.0 ), 1.0 );
	vTCBloom = inPosition.xy * pPosToBloom.xy + pPosToBloom.zw;
}
