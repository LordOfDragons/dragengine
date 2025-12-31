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

#ifndef _SYNESOURCEWAVE_H_
#define _SYNESOURCEWAVE_H_

#include "syneSource.h"

#include <dragengine/resources/synthesizer/source/deSynthesizerSourceWave.h>



/**
 * \brief Synthesizer source wave generator.
 */
class syneSourceWave : public syneSource{
public:
	typedef deTObjectReference<syneSourceWave> Ref;
	
private:
	deSynthesizerSourceWave::eWaveType pWaveType;
	
	float pMinFrequency;
	float pMaxFrequency;
	
	syneControllerTarget::Ref pTargetFrequency;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create source. */
	syneSourceWave();
	
	/** \brief Create copy of source. */
	syneSourceWave(const syneSourceWave &copy);
	
	/** \brief Clean up source. */
protected:
	~syneSourceWave() override;
public:
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
	inline const syneControllerTarget::Ref &GetTargetFrequency() const{ return pTargetFrequency; }
	
	
	
	/** \brief Create engine synthesizer source. */
	virtual deSynthesizerSource::Ref CreateEngineSource();
	
	/** \brief Update targets. */
	void UpdateTargets() override;
	
	/** \brief Retrieve the number of targets using a given link. */
	int CountLinkUsage(syneLink *link) const override;
	
	/** \brief Removes a link from all targets using it. */
	void RemoveLinkFromTargets(syneLink *link) override;
	
	/** \brief Removes all links from all targets. */
	void RemoveLinksFromAllTargets() override;
	
	/** \brief Create a copy of this source. */
	syneSource::Ref CreateCopy() const override;
	
	/** \brief List all links of all source targets. */
	void ListLinks(syneLink::List& list) override;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another synthesizer difference source to this synthesizer difference source. */
	virtual syneSourceWave &operator=(const syneSourceWave &copy);
	/*@}*/
};

#endif
