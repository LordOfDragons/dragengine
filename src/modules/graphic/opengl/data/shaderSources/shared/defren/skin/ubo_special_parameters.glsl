/*
	<!-- shared/defren/skin/ubo_special_parameters.glsl -->
	<uniformBlock name='SpecialParameters' binding='3'/>
	
	<define>SHARED_SPB</define>
*/

#ifdef SHARED_SPB
	// values stored in shared instance parameters
#else

UBOLAYOUT_BIND(3) uniform SpecialParameters{
	// for LayeredRenderingCube enabled shaders bit flag defines which cube faces
	// the object is visible on:
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
	
	// for LayeredRenderingCascaded enabled shaders bit flag defines which cascade
	// the object is visible on:
	// 
	// bit 0: cascade 0
	// bit 1: cascade 1
	// bit 2: cascade 2
	// bit 3: cascade 3
	
	// LayeredRendering == {LayeredRenderingCube, LayeredRenderingCascaded}
	int pLayerVisibility;
};

#endif
