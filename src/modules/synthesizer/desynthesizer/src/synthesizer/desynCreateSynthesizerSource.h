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

#ifndef _DESYNCREATESYNTHESIZERSOURCE_H_
#define _DESYNCREATESYNTHESIZERSOURCE_H_

#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitor.h>

class deDESynthesizer;
class desynSynthesizer;
class desynSynthesizerSource;



/**
 * \brief Create synthesizer source visitor.
 */
class desynCreateSynthesizerSource : public deSynthesizerSourceVisitor{
private:
	desynSynthesizer &pSynthesizer;
	const int pFirstLink;
	
	desynSynthesizerSource *pSource;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	desynCreateSynthesizerSource(desynSynthesizer &synthesizer, int firstLink);
	
	/** \brief Clean up visitor. */
	~desynCreateSynthesizerSource() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Created source or \em NULL if not present. */
	inline desynSynthesizerSource *GetSource() const{ return pSource; }
	
	/** \brief Reset. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit source. */
	void VisitSource(deSynthesizerSource &source) override;
	
	/** \brief Visit sound source. */
	void VisitSound(deSynthesizerSourceSound &source) override;
	
	/** \brief Visit wave generator source. */
	void VisitWave(deSynthesizerSourceWave &source) override;
	
	/** \brief Visit chain source. */
	void VisitChain(deSynthesizerSourceChain &source) override;
	
	/** \brief Visit synthesizer source. */
	void VisitSynthesizer(deSynthesizerSourceSynthesizer &source) override;
	
	/** \brief Visit group source. */
	void VisitGroup(deSynthesizerSourceGroup &source) override;
	/*@}*/
};

#endif
