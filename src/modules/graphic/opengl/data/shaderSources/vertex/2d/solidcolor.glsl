precision HIGHP float;
precision HIGHP int;

uniform vec4 pPosTransform; // scaleX, scaleY, offsetX, offsetY

in vec2 inPosition;

void main( void ){
	gl_Position = vec4( vec3( inPosition * pPosTransform.xy + pPosTransform.zw, 0.0 ), 1.0 );
}
