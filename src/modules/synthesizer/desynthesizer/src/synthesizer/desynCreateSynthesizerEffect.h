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

#ifndef _DESYNCREATESYNTHESIZEREFFECT_H_
#define _DESYNCREATESYNTHESIZEREFFECT_H_

#include <dragengine/resources/synthesizer/effect/deSynthesizerEffectVisitor.h>

class deDESynthesizer;
class desynSynthesizer;
class desynSynthesizerEffect;
class desynSynthesizerSource;



/**
 * \brief Create synthesizer effect visitor.
 */
class desynCreateSynthesizerEffect : public deSynthesizerEffectVisitor{
private:
	const desynSynthesizer &pSynthesizer;
	desynSynthesizerSource &pSource;
	const int pFirstLink;
	
	desynSynthesizerEffect *pEffect;
	desynSynthesizerEffect *pPreviousEffect;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	desynCreateSynthesizerEffect( const desynSynthesizer &synthesizer,
		int firstLink, desynSynthesizerSource &source );
	
	/** \brief Clean up visitor. */
	virtual ~desynCreateSynthesizerEffect();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Created effect or \em NULL if not present. */
	inline desynSynthesizerEffect *GetEffect() const{ return pEffect; }
	
	/** \brief Reset. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effect. */
	virtual void VisitEffect( deSynthesizerEffect &effect );
	
	/** \brief Visit stretch effect. */
	virtual void VisitStretch( deSynthesizerEffectStretch &effect );
	/*@}*/
};

#endif
