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

#ifndef _DEOALDELAYEDOPERATIONS_H_
#define _DEOALDELAYEDOPERATIONS_H_

#include <dragengine/threading/deMutex.h>

class deoalAudioThread;
class deoalDelayedDeletion;



/**
 * \brief Delayed operations.
 */
class deoalATDelayed{
private:
	deoalAudioThread &pAudioThread;
	
	deMutex pMutexFree;
	bool pHasFreeOperations;
	deoalDelayedDeletion *pRootDeletion;
	deoalDelayedDeletion *pTailDeletion;
	int pDeletionCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create delayed operations object. */
	deoalATDelayed( deoalAudioThread &audioThread );
	
	/** \brief Clean up delayed operations object. */
	~deoalATDelayed();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Audio thread. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	
	
	/** \brief Has free operations (not thread-safe). */
	inline bool GetHasFreeOperations() const{ return pHasFreeOperations; }
	
	/**
	 * \brief Process free operations (thread-safe).
	 * 
	 * \param deleteAll Set to \em true to delete all pending tasks. If set to \em false only
	 *                  a maximum number of objects is deleted to avoid hickups if a large
	 *                  amount of delete task piled up.
	 */
	void ProcessFreeOperations( bool deleteAll );
	
	
	
	/** \brief Add deletion (thread-safe). */
	void AddDeletion( deoalDelayedDeletion *deletion );
	/*@}*/
};

#endif
