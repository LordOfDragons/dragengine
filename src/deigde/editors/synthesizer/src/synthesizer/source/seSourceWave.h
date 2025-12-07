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

#ifndef _SESOURCEWAVE_H_
#define _SESOURCEWAVE_H_

#include "seSource.h"

#include <dragengine/resources/synthesizer/source/deSynthesizerSourceWave.h>



/**
 * \brief Synthesizer source wave generator.
 */
class seSourceWave : public seSource{
private:
	deSynthesizerSourceWave::eWaveType pWaveType;
	
	float pMinFrequency;
	float pMaxFrequency;
	
	seControllerTarget pTargetFrequency;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create source. */
	seSourceWave();
	
	/** \brief Create copy of source. */
	seSourceWave(const seSourceWave &copy);
	
	/** \brief Clean up source. */
	virtual ~seSourceWave();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Wave type. */
	inline deSynthesizerSourceWave::eWaveType GetWaveType() const{ return pWaveType; }
	
	/** \brief Set wave type. */
	void SetWaveType(deSynthesizerSourceWave::eWaveType type);
	
	
	
	/** \brief Minimum play frequency in Hz. */
	inline float GetMinFrequency() const{ return pMinFrequency; }
	
	/** \brief Set minimum frequency in Hz. */
	void SetMinFrequency(float frequency);
	
	/** \brief Maximum frequency in Hz. */
	inline float GetMaxFrequency() const{ return pMaxFrequency; }
	
	/** \brief Set maximum frequency in Hz. */
	void SetMaxFrequency(float frequency);
	
	
	
	/** \brief Play frequency target. */
	inline seControllerTarget &GetTargetFrequency(){ return pTargetFrequency; }
	inline const seControllerTarget &GetTargetFrequency() const{ return pTargetFrequency; }
	
	
	
	/** \brief Create engine synthesizer source. */
	virtual deSynthesizerSource *CreateEngineSource();
	
	/** \brief Update targets. */
	virtual void UpdateTargets();
	
	/** \brief Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage(seLink *link) const;
	
	/** \brief Removes a link from all targets using it. */
	virtual void RemoveLinkFromTargets(seLink *link);
	
	/** \brief Removes all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** \brief Create a copy of this source. */
	virtual seSource *CreateCopy() const;
	
	/** \brief List all links of all source targets. */
	virtual void ListLinks(seLinkList& list);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another synthesizer difference source to this synthesizer difference source. */
	virtual seSourceWave &operator=(const seSourceWave &copy);
	/*@}*/
};

#endif
