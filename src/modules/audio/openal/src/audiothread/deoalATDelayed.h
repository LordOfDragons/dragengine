/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
