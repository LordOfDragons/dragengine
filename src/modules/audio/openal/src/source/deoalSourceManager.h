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

#ifndef _DEOALSOURCEMANAGER_H_
#define _DEOALSOURCEMANAGER_H_

#include "../deoalBasics.h"
#include <dragengine/common/collection/decPointerList.h>

class decUnicodeString;
class deoalSource;


/**
 * \brief OpenAL source manager.
 * 
 * Keeps track of openal sources objects. Sources are limited resources
 * on the hardware and have to be reused as much as possible to avoid
 * running out of openal memory. A source object is either unbound if not
 * used by an engine resource or bound if in use. The important number is
 * used to determine what source to reassign in case not enough sources
 * are available by the hardware and is located in the range from 0 to 1
 * where 1 is the most important. The owner of an openal source object
 * has to check before each access to the source if it is still the owner.
 * This avoids the need for callback mechanisms. Source objects are only
 * held by the deoalSourceManager. Users only store a pointer. The owner
 * is identified using a memory pointer location. If the owner releases
 * the source it has to set the owner pointer to NULL.
 * 
 */
class deoalSourceManager{
private:
	deoalAudioThread &pAudioThread;
	
	decPointerList pSources;
	int pCountBound;
	int pCountUnbound;
	
	int pEstimatedMaxBound;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create openal sources manager. */
	deoalSourceManager( deoalAudioThread &audioThread );
	
	/** \brief Clean up openal sources manager. */
	~deoalSourceManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief OpenAL module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	
	
	/** \brief Number of managed sources. */
	int GetSourceCount() const;
	
	/** \brief Number of bound sources. */
	inline int GetCountBound() const{ return pCountBound; }
	
	/** \brief Number of unbound sources. */
	inline int GetCountUnbound() const{ return pCountUnbound; }
	
	/** \brief Managed source by index for debug purpose. */
	deoalSource *GetSourceAt( int index ) const;
	
	/**
	 * \brief Bind source.
	 * \details Returns the first unbound resource. If not existing a new managed
	 *          source is created and bound. If no more sources can be created
	 *          due to limitations of the underlaying hardware the source with the
	 *          lowest importance is rebound.
	 */
	deoalSource *BindSource( void *owner );
	
	/** \brief Unbind source. */
	void UnbindSource( deoalSource *source );
	
	
	
	/** \brief Dnformation for developer mode display. */
	void DebugOutput( decUnicodeString &output );
	
	/** \brief Print a quick status line to the console. */
	void PrintStatusLine();
	/*@}*/
	
private:
	deoalSource *pNextUnboundSource() const;
	deoalSource *pBestRebindableSource() const;
	deoalSource *pCreateNewSource();
};

#endif
