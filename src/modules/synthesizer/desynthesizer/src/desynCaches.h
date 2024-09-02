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

#ifndef _DESYNCACHES_H_
#define _DESYNCACHES_H_

#include <dragengine/threading/deMutex.h>

class deCacheHelper;
class deDESynthesizer;



/**
 * \brief Caches.
 */
class desynCaches{
private:
	deDESynthesizer &pDSyn;
	deMutex pMutex;
	
	deCacheHelper *pSound;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create caches. */
	desynCaches( deDESynthesizer &dsyn );
	
	/** \brief Cleans up caches. */
	~desynCaches();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Lock caches. */
	void Lock();
	
	/** \brief Unlock caches. */
	void Unlock();
	
	/** \brief Sound cache. */
	inline deCacheHelper &GetSound() const{ return *pSound; }
	
	
	
private:
	void pCleanUp();
};

#endif
