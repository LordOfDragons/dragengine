/* 
 * Drag[en]gine Game Engine
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

#ifndef _DESYNTHESIZERSOURCECHAIN_H_
#define _DESYNTHESIZERSOURCECHAIN_H_

#include "deSynthesizerSource.h"

#include "../../../common/collection/decObjectList.h"

class deSound;


/**
 * \brief Synthesizer chain source.
 * 
 * Plays back on the chain sound files like an announcer. The sound file
 * to play is picked from a list of sound files using the select target.
 * The play target is used to play, wait, pause or stop playback.
 * 
 * NULL sounds are allowed to be added providing a way to disable
 * a sound without modifying the select curve to deal with it.
 * 
 * This source is best used for announcer type sound playback or other
 * kind of playback requiring chaining sound files together.
 * 
 */
class DE_DLL_EXPORT deSynthesizerSourceChain : public deSynthesizerSource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSynthesizerSourceChain> Ref;
	
	
	
private:
	decObjectList pSounds;
	
	float pMinSpeed;
	float pMaxSpeed;
	
	deSynthesizerControllerTarget pTargetSpeed;
	deSynthesizerControllerTarget pTargetSelect;
	deSynthesizerControllerTarget pTargetPlay;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source. */
	deSynthesizerSourceChain();
	
protected:
	/**
	 * \brief Clean up synthesizer source.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSynthesizerSourceChain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Number of sound files. */
	int GetSoundCount() const;
	
	/** \brief Sound at index. */
	deSound *GetSoundAt( int index ) const;
	
	/** \brief Add sound. */
	void AddSound( deSound *sound );
	
	/** \brief Remove sound from index. */
	void RemoveSound( int index );
	
	/** \brief Remove all sounds. */
	void RemoveAllSounds();
	
	
	
	/**
	 * \brief Minimum play speed.
	 * 
	 * A value of 1 plays at orignal speed. A value of 2 would play double as fast. Negative
	 * values play backwards. A value of -1 would play backwards at original speed.
	 */
	inline float GetMinSpeed() const{ return pMinSpeed; }
	
	/**
	 * \brief Set minimum play speed.
	 * 
	 * A value of 1 plays at orignal speed. A value of 2 would play double as fast. Negative
	 * values play backwards. A value of -1 would play backwards at original speed.
	 */
	void SetMinSpeed( float speed );
	
	/**
	 * \brief Maximum play speed.
	 * 
	 * A value of 1 plays at orignal speed. A value of 2 would play double as fast. Negative
	 * values play backwards. A value of -1 would play backwards at original speed.
	 */
	inline float GetMaxSpeed() const{ return pMaxSpeed; }
	
	/**
	 * \brief Set maximum play speed.
	 * 
	 * A value of 1 plays at orignal speed. A value of 2 would play double as fast. Negative
	 * values play backwards. A value of -1 would play backwards at original speed.
	 */
	void SetMaxSpeed( float speed );
	
	
	
	/** \brief Play speed target. */
	inline deSynthesizerControllerTarget &GetTargetSpeed(){ return pTargetSpeed; }
	inline const deSynthesizerControllerTarget &GetTargetSpeed() const{ return pTargetSpeed; }
	
	/** \brief Select sound target. */
	inline deSynthesizerControllerTarget &GetTargetSelect(){ return pTargetSelect; }
	inline const deSynthesizerControllerTarget &GetTargetSelect() const{ return pTargetSelect; }
	
	/**
	 * \brief Play target.
	 * 
	 * Plays, waits, pauses or stops the source. This target is best used with links using
	 * constant interpolation curves. A value of 1 plays back the source. Once the current
	 * sound is finished the next is played according to the select target. A value of 2/3
	 * waits playing. This is the same as play except once the sound finished no next sound
	 * is played until target switches to play. This can be used to add pauses between
	 * sounds. A value of 1/3 pauses the playback freezing playback position. A value of 0
	 * stops playback and rewinds playback position to the beginning.
	 */
	inline deSynthesizerControllerTarget &GetTargetPlay(){ return pTargetPlay; }
	inline const deSynthesizerControllerTarget &GetTargetPlay() const{ return pTargetPlay; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit source. */
	virtual void Visit( deSynthesizerSourceVisitor &visitor );
	/*@}*/
};

#endif
