/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#ifndef _SESOURCESOUND_H_
#define _SESOURCESOUND_H_

#include "seSource.h"

#include <dragengine/common/string/decString.h>

class deEngine;
class deSound;



/**
 * \brief Synthesizer source sound.
 */
class seSourceSound : public seSource{
private:
	deEngine *pEngine;
	
	decString pPathSound;
	deSound *pSound;
	
	float pMinSpeed;
	float pMaxSpeed;
	bool pLooping;
	
	seControllerTarget pTargetSpeed;
	seControllerTarget pTargetPlay;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source. */
	seSourceSound( deEngine *engine );
	
	/** \brief Create copy of synthesizer source. */
	seSourceSound( const seSourceSound &copy );
	
	/** \brief Clean up synthesizer source. */
	virtual ~seSourceSound();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Path to sound file. */
	inline const decString &GetPathSound() const{ return pPathSound; }
	
	/** \brief Set path to sound file. */
	void SetPathSound( const char *path );
	
	/** \brief Update sound. */
	void UpdateSound();
	
	/** \brief Sound or \em null if silent. */
	inline deSound *GetSound() const{ return pSound; }
	
	
	
	/**
	 * \brief Minimum play speed.
	 * \details A value of 1 plays at orignal speed. A value of 2 would play double as fast. Negative
	 *          values play backwards. A value of -1 would play back at original speed.
	 */
	inline float GetMinSpeed() const{ return pMinSpeed; }
	
	/**
	 * \brief Set minimum play speed.
	 * \details A value of 1 plays at orignal speed. A value of 2 would play double as fast. Negative
	 *          values play backwards. A value of -1 would play back at original speed.
	 */
	void SetMinSpeed( float speed );
	
	/**
	 * \brief Maximum play speed.
	 * \details A value of 1 plays at orignal speed. A value of 2 would play double as fast. Negative
	 *          values play backwards. A value of -1 would play back at original speed.
	 */
	inline float GetMaxSpeed() const{ return pMaxSpeed; }
	
	/**
	 * \brief Set maximum play speed.
	 * \details A value of 1 plays at orignal speed. A value of 2 would play double as fast. Negative
	 *          values play backwards. A value of -1 would play back at original speed.
	 */
	void SetMaxSpeed( float speed );
	
	/** \brief Sound is played back looping. */
	inline bool GetLooping() const{ return pLooping; }
	
	/** \brief Set if sound is played back looping. */
	void SetLooping( bool looping );
	
	
	
	/** \brief Speed target. */
	inline seControllerTarget &GetTargetSpeed(){ return pTargetSpeed; }
	inline const seControllerTarget &GetTargetSpeed() const{ return pTargetSpeed; }
	
	/**
	 * \brief Play target.
	 * \details Plays, pauses or stops the source. This target is best used with links using constant
	 *          interpolation curves. A value of 1 plays back the source. A value of 0.5 pauses the
	 *          playback freezing playback position. A value of 0 stops playback and rewinds playback
	 *          position to the beginning.
	 */
	inline seControllerTarget &GetTargetPlay(){ return pTargetPlay; }
	inline const seControllerTarget &GetTargetPlay() const{ return pTargetPlay; }
	
	/** \brief Create an engine synthesizer source. */
	virtual deSynthesizerSource *CreateEngineSource();
	
	/** \brief Update targets. */
	virtual void UpdateTargets();
	
	/** \brief Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage( seLink *link ) const;
	
	/** \brief Remove a link from all targets using it. */
	virtual void RemoveLinkFromTargets( seLink *link );
	
	/** \brief Remove all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** \brief Create a copy of this source. */
	virtual seSource *CreateCopy() const;
	
	/** \brief List all links of all source targets. */
	virtual void ListLinks( seLinkList& list );
	
	/** \brief Parent synthesizer changed. */
	virtual void SynthesizerChanged();
	
	/** \brief Synthesizer directory changed. */
	virtual void SynthesizerDirectoryChanged();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another animation source to this source. */
	virtual seSourceSound &operator=( const seSourceSound &copy );
	/*@}*/
};

#endif
