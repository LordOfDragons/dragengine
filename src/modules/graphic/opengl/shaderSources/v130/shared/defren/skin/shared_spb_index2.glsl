// call this from inside main to initialize global values. always include this since
// it does declare spbIndex and spbFlags either as variables or 0-defines

// BUG NOTE
// on the tested systems a few exhibit a very annoying and strange bug in the GLSL compiler.
// If the following code is used:
// 
//   spbIndex = pSPBInstanceIndex[ _spbIndexCoord.x ][ _spbIndexCoord.y ];
// 
// garbage is stored in spbIndex instead of the correct value. MESA 18.x seems to be
// affected by this bug but only on some drivers. It was impossible to create a reliable
// check in deoglCapabilities to detect this driver bug. For this reason a permanent
// workaround is used which does not fail on the affected systems but is potentially
// a bit slower as it could be. The hope is though on modern GLSL compilers to sucessfully
// optimize away this workaround.
// 
// Basically the bug seems to affect the use of array notation to access the component
// of the ivec4. By storing the ivec4 first into a temporary variable (first array access)
// and then accessing the component out of this temporary variable the problem is solved.

#ifdef SHARED_SPB
	int spbIndex;
	#ifdef GS_RENDER_CUBE
	int spbFlags;
	#endif
	
	{ // scoping to drop temporary variables freeing registers
	int _spbIndexIndex = pSPBInstanceIndexBase + gl_InstanceID;
	
	#ifdef GS_RENDER_CUBE
		ivec2 _spbIndexCoord = ivec2( _spbIndexIndex / 2, ( _spbIndexIndex % 2 ) * 2 );
		//int spbIndex = pSPBInstanceIndex[ _spbIndexCoord.x ][ _spbIndexCoord.y ];
		//int spbFlags = pSPBInstanceIndex[ _spbIndexCoord.x ][ _spbIndexCoord.y + 1 ];
		ivec4 _spbTempIndex = pSPBInstanceIndex[ _spbIndexCoord.x ];
		spbIndex = _spbTempIndex[ _spbIndexCoord.y ];
		spbFlags = _spbTempIndex[ _spbIndexCoord.y + 1 ];
		
	#else
		//int spbIndex = pSPBInstanceIndex[ _spbIndexIndex / 4 ][ _spbIndexIndex % 4 ];
		ivec4 _spbTempIndex = pSPBInstanceIndex[ _spbIndexIndex / 4 ];
		spbIndex = _spbTempIndex[ _spbIndexIndex % 4 ];
	#endif
	} // end of scoping
	
#else
	#define spbIndex 0
	#ifdef GS_RENDER_CUBE
		#define spbFlags 0
	#endif
#endif
