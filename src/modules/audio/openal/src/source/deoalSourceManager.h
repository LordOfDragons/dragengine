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
