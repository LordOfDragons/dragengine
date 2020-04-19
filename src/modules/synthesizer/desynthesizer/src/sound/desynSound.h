/* 
 * Drag[en]gine Synthesizer Module
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
