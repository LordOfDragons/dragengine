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

#ifndef _SEEFFECTSTRETCH_H_
#define _SEEFFECTSTRETCH_H_

#include "seEffect.h"

#include <dragengine/resources/synthesizer/effect/deSynthesizerEffectStretch.h>



/**
 * \brief Synthesizer effect stretch time and pitch.
 */
class seEffectStretch : public seEffect{
private:
	float pMinTime;
	float pMaxTime;
	float pMinPitch;
	float pMaxPitch;
	
	seControllerTarget pTargetTime;
	seControllerTarget pTargetPitch;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create effect. */
	seEffectStretch();
	
	/** \brief Create copy of effect. */
	seEffectStretch(const seEffectStretch &copy);
	
	/** \brief Clean up effect. */
	~seEffectStretch() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Minimum time stretch factor. */
	inline float GetMinTime() const{ return pMinTime; }
	
	/** \brief Set minimum time stretch factor. */
	void SetMinTime(float stretch);
	
	/** \brief Maximum time stretch factor. */
	inline float GetMaxTime() const{ return pMaxTime; }
	
	/** \brief Set maximum time stretch factor. */
	void SetMaxTime(float stretch);
	
	/** \brief Minimum pitch shift factor. */
	inline float GetMinPitch() const{ return pMinPitch; }
	
	/** \brief Set minimum pitch shift factor. */
	void SetMinPitch(float shift);
	
	/** \brief Maximum pitch shift factor. */
	inline float GetMaxPitch() const{ return pMaxPitch; }
	
	/** \brief Set maximum pitch shift factor. */
	void SetMaxPitch(float shift);
	
	
	
	/** \brief Time stretch target. */
	inline seControllerTarget &GetTargetTime(){ return pTargetTime; }
	inline const seControllerTarget &GetTargetTime() const{ return pTargetTime; }
	
	/** \brief Pitch stretch target. */
	inline seControllerTarget &GetTargetPitch(){ return pTargetPitch; }
	inline const seControllerTarget &GetTargetPitch() const{ return pTargetPitch; }
	
	
	
	/** \brief Create engine synthesizer effect. */
	virtual deSynthesizerEffect *CreateEngineEffect();
	
	/** \brief Update targets. */
	void UpdateTargets() override;
	
	/** \brief Retrieve the number of targets using a given link. */
	int CountLinkUsage(seLink *link) const override;
	
	/** \brief Removes a link from all targets using it. */
	void RemoveLinkFromTargets(seLink *link) override;
	
	/** \brief Removes all links from all targets. */
	void RemoveLinksFromAllTargets() override;
	
	/** \brief Create a copy of this effect. */
	seEffect *CreateCopy() const override;
	
	/** \brief List all links of all effect targets. */
	void ListLinks(seLinkList& list) override;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another synthesizer difference effect to this synthesizer difference effect. */
	virtual seEffectStretch &operator=(const seEffectStretch &copy);
	/*@}*/
};

#endif
