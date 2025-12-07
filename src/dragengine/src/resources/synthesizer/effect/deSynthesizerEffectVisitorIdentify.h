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

#ifndef _DESYNTHESIZEREFFECTVISITORIDENTIFY_H_
#define _DESYNTHESIZEREFFECTVISITORIDENTIFY_H_

#include "deSynthesizerEffectVisitor.h"


/**
 * \brief Identify synthesizer effect visitor.
 */
class DE_DLL_EXPORT deSynthesizerEffectVisitorIdentify : public deSynthesizerEffectVisitor{
public:
	enum eEffectTypes{
		eetUnknown,
		eetStretch
	};
	
	
	
private:
	deSynthesizerEffect *pEffect;
	eEffectTypes pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create identify synthesizer effect visitor. */
	deSynthesizerEffectVisitorIdentify();
	
	/** \brief Clean up identify synthesizer effect visitor. */
	~deSynthesizerEffectVisitorIdentify() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Effect type. */
	inline eEffectTypes GetType() const{ return pType; }
	
	/** \brief Effect is unknown. */
	inline bool IsUnknown() const{ return pType == eetUnknown; }
	
	/** \brief Effect is a stretch effect. */
	inline bool IsStretch() const{ return pType == eetStretch; }
	
	/**
	 * \brief Cast to stretch effect.
	 * \throws deeInvalidParam Type of effect is not deSynthesizerEffectVisitorIdentify::eetStretch.
	 */
	deSynthesizerEffectStretch &CastToStretch() const;
	
	/** \brief Reset visitor. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effect. */
	void VisitEffect(deSynthesizerEffect &effect) override;
	
	/** \brief Visit stretch effect. */
	void VisitStretch(deSynthesizerEffectStretch &effect) override;
	/*@}*/
};

#endif
