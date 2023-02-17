/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEOGLRRTLEAKTRACKER_H_
#define _DEOGLRRTLEAKTRACKER_H_

#include <dragengine/common/collection/decPointerSet.h>
#include <dragengine/threading/deMutex.h>

class deoglRenderThread;



// enable leak tracking.
// #define ENABLE_LEAK_TRACKING



#ifdef ENABLE_LEAK_TRACKING
	#define IF_LEAK_CHECK( cmd ) cmd
	
	#define LEAK_CHECK_CREATE(renderThread,type) \
		(renderThread).GetLeakTracker().Add ## type( this )
	
	#define LEAK_CHECK_FREE(renderThread,type) \
		(renderThread).GetLeakTracker().Remove ## type( this )
	
	#define LEAK_CHECK_REPORT_LEAKS(renderThread,type) \
		(renderThread).GetLeakTracker().ReportLeaks( #type, \
			(renderThread).GetLeakTracker().Get ## type() )
	
#else
	#define IF_LEAK_CHECK( cmd )
	
	#define LEAK_CHECK_CREATE(renderThread,type)
	#define LEAK_CHECK_FREE(renderThread,type)
	
	#define LEAK_CHECK_REPORT_LEAKS(renderThread,type)
#endif



/**
 * Render thread leak tracker.
 */
class deoglRTLeakTracker{
private:
	deoglRenderThread &pRenderThread;
	deMutex pMutex;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render thread texture related object. */
	deoglRTLeakTracker( deoglRenderThread &renderThread );
	
	/** Clean up render thread texture related object. */
	virtual ~deoglRTLeakTracker();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Report leaks. */
	void ReportLeaks( const char *name, const decPointerSet &tracker );
	/*@}*/
	
	
	
#ifdef ENABLE_LEAK_TRACKING
#define DECLARE_TRACKER(type) \
private: decPointerSet p ## type; \
public: inline decPointerSet &Get ## type(){ return p ## type; } \
public: inline void Add ## type( void *object ){ AddTracked( p ## type, object ); } \
public: inline void Remove ## type( void *object ){ RemoveTracked( p ## type, object ); }
	DECLARE_TRACKER( Billboard )
	DECLARE_TRACKER( Camera )
	DECLARE_TRACKER( CanvasCanvasView )
	DECLARE_TRACKER( CanvasImage )
	DECLARE_TRACKER( CanvasPaint )
	DECLARE_TRACKER( CanvasRenderWorld )
	DECLARE_TRACKER( CanvasText )
	DECLARE_TRACKER( CanvasVideoPlayer )
	DECLARE_TRACKER( CanvasView )
	DECLARE_TRACKER( CaptureCanvas )
	DECLARE_TRACKER( Component )
	DECLARE_TRACKER( ComponentLOD )
	DECLARE_TRACKER( ComponentTexture )
	DECLARE_TRACKER( DebugDrawer )
	DECLARE_TRACKER( Decal )
	DECLARE_TRACKER( DSRenderableCamera )
	DECLARE_TRACKER( DSRenderableCanvas )
	DECLARE_TRACKER( DSRenderableColor )
	DECLARE_TRACKER( DSRenderableImage )
	DECLARE_TRACKER( DSRenderableValue )
	DECLARE_TRACKER( DSRenderableVideoFrame )
	DECLARE_TRACKER( DynamicSkin )
	DECLARE_TRACKER( EffectColorMatrix )
	DECLARE_TRACKER( EffectDistortImage )
	DECLARE_TRACKER( EffectFilterKernel )
	DECLARE_TRACKER( EffectOverlayImage )
	DECLARE_TRACKER( EnvironmentMapProbe )
	DECLARE_TRACKER( Font )
	DECLARE_TRACKER( HeightTerrain )
	DECLARE_TRACKER( HTSector )
	DECLARE_TRACKER( Image )
	DECLARE_TRACKER( Light )
	DECLARE_TRACKER( Lumimeter )
	DECLARE_TRACKER( Model )
	DECLARE_TRACKER( OcclusionMesh )
	DECLARE_TRACKER( ParticleEmitter )
	DECLARE_TRACKER( ParticleEmitterType )
	DECLARE_TRACKER( ParticleEmitterInstance )
	DECLARE_TRACKER( ParticleEmitterInstanceType )
	DECLARE_TRACKER( PropField )
	DECLARE_TRACKER( PropFieldType )
	DECLARE_TRACKER( RenderWindow )
	DECLARE_TRACKER( Skin )
	DECLARE_TRACKER( Sky )
	DECLARE_TRACKER( SkyLayer )
	DECLARE_TRACKER( SkyInstance )
	DECLARE_TRACKER( World )
	
	void AddTracked( decPointerSet &list, void *object );
	void RemoveTracked( decPointerSet &list, void *object );
#endif
};

#endif
