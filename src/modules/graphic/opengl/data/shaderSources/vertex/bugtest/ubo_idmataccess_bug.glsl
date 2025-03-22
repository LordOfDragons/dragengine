precision HIGHP float;
precision HIGHP int;

in vec2 inPosition;

void main(){
	gl_Position = vec4( vec3( inPosition * vec2( 2.0 ) - vec2( 1.0 ), 0.0 ), 1.0 );
}
