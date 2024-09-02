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

#ifndef _DESYNSOUND_H_
#define _DESYNSOUND_H_

#include "../desynBasics.h"

#include <dragengine/systems/modules/synthesizer/deBaseSynthesizerSound.h>

class deSound;
class deDESynthesizer;



/**
 * \brief Sound resource peer.
 */
class desynSound : public deBaseSynthesizerSound{
private:
	deDESynthesizer &pModule;
	deSound &pSound;
	
	bool pValid;
	bool pIsUsed;
	bool pIsCached;
	
	char *pStreamData;
	int pStreamDataSize;
	bool pStreaming;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sound peer. */
	desynSound( deDESynthesizer &module, deSound &sound );
	
	/** \brief Clean up sound peer. */
	virtual ~desynSound();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deDESynthesizer &GetModule() const{ return pModule; }
	
	/** \brief Sound resource. */
	inline deSound &GetSound() const{ return pSound; }
	
	
	
	/** \brief Stream data or \em NULL if not loaded. */
	inline char *GetStreamData() const{ return pStreamData; }
	
	/** \brief Stream data size in bytes or 0 if not loaded. */
	inline int GetStreamDataSize() const{ return pStreamDataSize; }
	
	/** \brief Sound is too large requiring streaming. */
	inline bool GetStreaming() const{ return pStreaming; }
	
	/** \brief Load entire sound data into buffer if not done already. */
	void LoadEntireSound();
	
	/** \brief Sound is used. */
	inline bool IsUsed() const{ return pIsUsed; }
	
	/**
	 * \brief Prepare sound.
	 * 
	 * Has no effect on streaming sounds or sounds marked used.
	 * 
	 * Marks sound used. Loads samples if not present and write cache.
	 */
	void Prepare();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pLoadFromCache();
	void pWriteToCache();
	
	void pDetermineStreaming();
	void pDetermineValid();
};

#endif
