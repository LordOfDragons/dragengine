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

#ifndef _DESYNTHESIZERSOURCESOUND_H_
#define _DESYNTHESIZERSOURCESOUND_H_

#include "deSynthesizerSource.h"
#include "../../sound/deSound.h"


/**
 * \brief Synthesizer sound source.
 */
class DE_DLL_EXPORT deSynthesizerSourceSound : public deSynthesizerSource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSynthesizerSourceSound> Ref;
	
	
private:
	deSound::Ref pSound;
	
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
