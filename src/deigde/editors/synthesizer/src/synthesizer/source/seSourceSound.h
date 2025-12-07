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

#ifndef _SESOURCESOUND_H_
#define _SESOURCESOUND_H_

#include "seSource.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/src/resources/sound/deSound.h>

class deEngine;



/**
 * \brief Synthesizer source sound.
 */
class seSourceSound : public seSource{
private:
	deEngine *pEngine;
	
	decString pPathSound;
	deSound::Ref pSound;
	
	float pMinSpeed;
	float pMaxSpeed;
	bool pLooping;
	
	seControllerTarget pTargetSpeed;
	seControllerTarget pTargetPlay;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source. */
	seSourceSound(deEngine *engine);
	
	/** \brief Create copy of synthesizer source. */
	seSourceSound(const seSourceSound &copy);
	
	/** \brief Clean up synthesizer source. */
	~seSourceSound() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Path to sound file. */
	inline const decString &GetPathSound() const{ return pPathSound; }
	
	/** \brief Set path to sound file. */
	void SetPathSound(const char *path);
	
	/** \brief Update sound. */
	void UpdateSound();
	
	/** \brief Sound or \em null if silent. */
	inline const deSound::Ref &GetSound() const{ return pSound; }
	
	
	
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
	void SetMinSpeed(float speed);
	
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
	void SetMaxSpeed(float speed);
	
	/** \brief Sound is played back looping. */
	inline bool GetLooping() const{ return pLooping; }
	
	/** \brief Set if sound is played back looping. */
	void SetLooping(bool looping);
	
	
	
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
	void UpdateTargets() override;
	
	/** \brief Retrieve the number of targets using a given link. */
	int CountLinkUsage(seLink *link) const override;
	
	/** \brief Remove a link from all targets using it. */
	void RemoveLinkFromTargets(seLink *link) override;
	
	/** \brief Remove all links from all targets. */
	void RemoveLinksFromAllTargets() override;
	
	/** \brief Create a copy of this source. */
	seSource *CreateCopy() const override;
	
	/** \brief List all links of all source targets. */
	void ListLinks(seLinkList& list) override;
	
	/** \brief Parent synthesizer changed. */
	void SynthesizerChanged() override;
	
	/** \brief Synthesizer directory changed. */
	void SynthesizerDirectoryChanged() override;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another animation source to this source. */
	virtual seSourceSound &operator=(const seSourceSound &copy);
	/*@}*/
};

#endif
