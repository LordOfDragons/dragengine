precision highp float;
precision highp int;

uniform mat4x4 pMatrixMVP; // matrix model-view-projection

in vec3 inPosition;

void main( void ){
	gl_Position = pMatrixMVP * vec4( inPosition, 1 );
}
