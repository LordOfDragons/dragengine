precision highp float;
precision highp int;

out vec4 outPacked;

void main( void ){
	float value1 = 0.2;
	float value2 = 0.7;
	
	outPacked = vec4( 0.0 );
	//outPacked.r = packHalf2x16(value1, value2);
}
