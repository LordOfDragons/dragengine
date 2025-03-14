#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

in vec2 vTexCoord;
flat in float vRimThickness;
flat in vec3 vColor;
flat in vec3 vColorRim;

out vec3 outColor;

void main( void ){
	float radius = length( vTexCoord );
	if( radius > 1.0 ){
		discard; // outside sphere
	}
	
	outColor = radius > vRimThickness ? vColorRim : vColor;
}
