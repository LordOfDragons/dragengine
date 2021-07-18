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

#ifndef _DESYNTHESIZERSOURCESOUND_H_
#define _DESYNTHESIZERSOURCESOUND_H_

#include "deSynthesizerSource.h"
#include "../../sound/deSoundReference.h"


/**
 * \brief Synthesizer sound source.
 */
class deSynthesizerSourceSound : public deSynthesizerSource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSynthesizerSourceSound> Ref;
	
	
	
private:
	deSoundReference pSound;
	
	float pMinSpeed;
	float pMaxSpeed;
	bool pLooping;
	
	deSynthesizerControllerTarget pTargetSpeed;
	deSynthesizerControllerTarget pTargetPlay;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source. */
	deSynthesizerSourceSound();
	
protected:
	/**
	 * \brief Clean up synthesizer source.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSynthesizerSourceSound();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Sound or NULL if silent. */
	inline deSound *GetSound() const{ return pSound; }
	
	/** \brief Set sound or NULL if silent. */
	void SetSound( deSound *sound );
	
	
	
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
	
	/** \brief Sound is played back looping. */
	inline bool GetLooping() const{ return pLooping; }
	
	/** \brief Set if sound is played back looping. */
	void SetLooping( bool looping );
	
	
	
	/** \brief Play speed target. */
	inline deSynthesizerControllerTarget &GetTargetSpeed(){ return pTargetSpeed; }
	inline const deSynthesizerControllerTarget &GetTargetSpeed() const{ return pTargetSpeed; }
	
	/**
	 * \brief Play target.
	 * 
	 * Plays, pauses or stops the source. This target is best used with links using constant
	 * interpolation curves. A value of 1 plays back the source. A value of 0.5 pauses the
	 * playback freezing playback position. A value of 0 stops playback and rewinds playback
	 * position to the beginning.
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
