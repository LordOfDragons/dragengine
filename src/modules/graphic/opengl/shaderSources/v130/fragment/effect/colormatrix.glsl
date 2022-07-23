precision mediump float;
precision mediump int;

uniform mat4 pColorMatrix;
uniform vec4 pColorOffset;

uniform mediump sampler2DArray texColor;

in vec2 vTexCoord;

#ifdef GS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out mediump vec4 outColor;

void main( void ){
	outColor = pColorMatrix * texture( texColor, vec3( vTexCoord, vLayer ) ) + pColorOffset;
	/*
	gl_FragColor.r = dot( pMatrixRow1, color );
	gl_FragColor.g = dot( pMatrixRow2, color );
	gl_FragColor.b = dot( pMatrixRow3, color );
	gl_FragColor.a = dot( pMatrixRow4, color );
	gl_FragColor += pTranslation;
	*/
}
