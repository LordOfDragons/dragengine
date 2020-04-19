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

#ifndef _DEOALSYNTHESIZERINSTANCE_H_
#define _DEOALSYNTHESIZERINSTANCE_H_

#include <dragengine/resources/synthesizer/deSynthesizerInstance.h>
#include <dragengine/systems/modules/audio/deBaseAudioSynthesizerInstance.h>

class deAudioOpenAL;
class deoalASynthesizerInstance;

class deSynthesizerInstance;



/**
 * \brief Sound level meter resource peer.
 */
class deoalSynthesizerInstance : public deBaseAudioSynthesizerInstance{
private:
	deAudioOpenAL &pOal;
	deSynthesizerInstance &pInstance;
	deoalASynthesizerInstance *pAInstance;
	unsigned int pUpdateTracker;
	bool pDirtyParameters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deoalSynthesizerInstance( deAudioOpenAL &oal, deSynthesizerInstance &instance );
	
	/** \brief Clean up peer. */
	virtual ~deoalSynthesizerInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deAudioOpenAL &GetOal() const{ return pOal; }
	
	/** \brief Instance. */
	inline deSynthesizerInstance &GetInstance() const{ return pInstance; }
	
	/** \brief Audio peer. */
	inline deoalASynthesizerInstance *GetAInstance() const{ return pAInstance; }
	
	/** \brief Update tracker. */
	inline unsigned int GetUpdateTracker() const{ return pUpdateTracker; }
	
	
	
	/**
	 * \brief Synchronize.
	 * \warning Called during synchronization time from main thread.
	 */
	void Synchronize();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Synthesizer changed. */
	virtual void SynthesizerChanged();
	
	/** \brief Play time changed. */
	virtual void PlayTimeChanged();
	/*@}*/
};

#endif
