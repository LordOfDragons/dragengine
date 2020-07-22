/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALATLEAKTRACKER_H_
#define _DEOALATLEAKTRACKER_H_

#include <dragengine/common/collection/decPointerSet.h>
#include <dragengine/threading/deMutex.h>

class deoalAudioThread;


// enable leak tracking.
#define ENABLE_LEAK_TRACKING


#ifdef ENABLE_LEAK_TRACKING
	#define IF_LEAK_CHECK( cmd ) cmd
	
	#define LEAK_CHECK_CREATE(audioThread,type) \
		(audioThread).GetLeakTracker().Add ## type( this )
	
	#define LEAK_CHECK_FREE(audioThread,type) \
		(audioThread).GetLeakTracker().Remove ## type( this )
	
	#define LEAK_CHECK_REPORT_LEAKS(audioThread,type) \
		(audioThread).GetLeakTracker().ReportLeaks( #type, \
			(audioThread).GetLeakTracker().Get ## type() )
	
#else
	#define IF_LEAK_CHECK( cmd )
	
	#define LEAK_CHECK_CREATE(audioThread,type)
	#define LEAK_CHECK_FREE(audioThread,type)
	
	#define LEAK_CHECK_REPORT_LEAKS(audioThread,type)
#endif


/**
 * \brief Audio thread leak tracker.
 */
class deoalATLeakTracker{
private:
	deoalAudioThread &pAudioThread;
	deMutex pMutex;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render thread texture related object. */
	deoalATLeakTracker( deoalAudioThread &audioThread );
	
	/** \brief Clean up render thread texture related object. */
	virtual ~deoalATLeakTracker();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Report leaks. */
	void ReportLeaks( const char *name, const decPointerSet &tracker );
	/*@}*/
	
	
	
#ifdef ENABLE_LEAK_TRACKING
#define DECLARE_TRACKER(type) \
private: decPointerSet p ## type; \
public: inline decPointerSet &Get ## type(){ return p ## type; } \
public: inline void Add ## type( void *object ){ AddTracked( p ## type, object ); } \
public: inline void Remove ## type( void *object ){ RemoveTracked( p ## type, object ); }
	DECLARE_TRACKER( Component )
	DECLARE_TRACKER( Microphone )
	DECLARE_TRACKER( Model )
	DECLARE_TRACKER( Skin )
	DECLARE_TRACKER( Sound )
	DECLARE_TRACKER( SoundLevelMeter )
	DECLARE_TRACKER( Speaker )
	DECLARE_TRACKER( SynthesizerInstance )
	DECLARE_TRACKER( VideoPlayer )
	DECLARE_TRACKER( World )
	
	void AddTracked( decPointerSet &list, void *object );
	void RemoveTracked( decPointerSet &list, void *object );
#endif
};

#endif
