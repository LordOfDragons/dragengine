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

#ifndef _SESOURCECHAIN_H_
#define _SESOURCECHAIN_H_

#include "seSource.h"

#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/collection/decObjectList.h>

class deEngine;
class deSound;



/**
 * \brief Synthesizer source chain.
 */
class seSourceChain : public seSource{
private:
	deEngine *pEngine;
	
	decStringList pPathSounds;
	decObjectList pSounds;
	
	float pMinSpeed;
	float pMaxSpeed;
	
	seControllerTarget pTargetSpeed;
	seControllerTarget pTargetSelect;
	seControllerTarget pTargetPlay;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source. */
	seSourceChain(deEngine *engine);
	
	/** \brief Create copy of synthesizer source. */
	seSourceChain(const seSourceChain &copy);
	
	/** \brief Clean up synthesizer source. */
	~seSourceChain() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Path to sound files. */
	inline const decStringList &GetPathSounds() const{ return pPathSounds; }
	
	/** \brief Set path to sound file. */
	void SetPathSound(int index, const char *path);
	
	/** \brief Add path to sound file. */
	void AddPathSound(const char *path);
	
	/** \brief Insert path to sound file. */
	void InsertPathSound(const char *path, int index);
	
	/** \brief Move path to sound file. */
	void MovePathSound(int from, int to);
	
	/** \brief Remove path to sound file. */
	void RemovePathSound(int index);
	
	/** \brief Remove all path to sound files. */
	void RemoveAllPathSounds();
	
	/** \brief Update sounds. */
	void UpdateSounds();
	
	/** \brief Sound at index. */
	deSound *GetSoundAt(int index) const;
	
	
	
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
	
	
	
	/** \brief Speed target. */
	inline seControllerTarget &GetTargetSpeed(){ return pTargetSpeed; }
	inline const seControllerTarget &GetTargetSpeed() const{ return pTargetSpeed; }
	
	/** \brief Select sound target. */
	inline seControllerTarget &GetTargetSelect(){ return pTargetSelect; }
	inline const seControllerTarget &GetTargetSelect() const{ return pTargetSelect; }
	
	/**
	 * \brief Play target.
	 * \details Plays, waits, pauses or stops the source. This target is best used with links using
	 *          constant interpolation curves. A value of 1 plays back the source. Once the current
	 *          sound is finished the next is played according to the select target. A value of 2/3
	 *          waits playing. This is the same as play except once the sound finished no next sound
	 *          is played until target switches to play. This can be used to add pauses between
	 *          sounds. A value of 1/3 pauses the playback freezing playback position. A value of 0
	 *          stops playback and rewinds playback position to the beginning.
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
	virtual seSourceChain &operator=(const seSourceChain &copy);
	/*@}*/
};

#endif
