#ifdef SHARED_SPB
	// values stored in shared instance parameters
#else

UBOLAYOUT uniform SpecialParameters{
	// used by GS_RENDER_CUBE enabled shaders.
	// 
	// bit flag defining which cube faces the object is visible on:
	// 
	// bit 0: X+
	// bit 1: X-
	// bit 2: Y-
	// bit 3: Y+
	// bit 4: Z+
	// bit 5: Z-
	// 
	// bits are counted from the LSB on. Y+ and Y- is switched due to the way OpenGL
	// handles cube maps.
	// 
	// another way to define this variable would be as bool[6]. due to the way GLSL std140
	// aligns data members this requires 32 bytes to store while the other solution needs 4
	int pCubeFaceVisible;
};

#endif
