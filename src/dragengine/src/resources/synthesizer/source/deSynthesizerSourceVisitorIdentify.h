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

#ifndef _DESYNTHESIZERSOURCEVISITORIDENTIFY_H_
#define _DESYNTHESIZERSOURCEVISITORIDENTIFY_H_

#include "deSynthesizerSourceVisitor.h"


/**
 * \brief Identify synthesizer source visitor.
 */
class DE_DLL_EXPORT deSynthesizerSourceVisitorIdentify : public deSynthesizerSourceVisitor{
public:
	enum eSourceTypes{
		estUnknown,
		estSound,
		estWave,
		estChain,
		estSynthesizer,
		estGroup
	};
	
	
	
private:
	deSynthesizerSource *pSource;
	eSourceTypes pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create identify synthesizer source visitor. */
	deSynthesizerSourceVisitorIdentify();
	
	/** \brief Clean up identify synthesizer source visitor. */
	virtual ~deSynthesizerSourceVisitorIdentify();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Source type. */
	inline eSourceTypes GetType() const{return pType;}
	
	/** \brief Source is unknown. */
	inline bool IsUnknown() const{return pType == estUnknown;}
	
	/** \brief Source is a sound source. */
	inline bool IsSound() const{return pType == estSound;}
	
	/** \brief Source is a wave generator source. */
	inline bool IsWave() const{return pType == estWave;}
	
	/** \brief Source is a chain source. */
	inline bool IsChain() const{return pType == estChain;}
	
	/** \brief Source is a synthesizer source. */
	inline bool IsSynthesizer() const{return pType == estSynthesizer;}
	
	/** \brief Source is a group source. */
	inline bool IsGroup() const{return pType == estGroup;}
	
	/**
	 * \brief Cast to sound source.
	 * \throws deeInvalidParam Type of source is not deSynthesizerSourceVisitorIdentify::estSound.
	 */
	deSynthesizerSourceSound &CastToSound() const;
	
	/**
	 * \brief Cast to wave generator source.
	 * \throws deeInvalidParam Type of source is not deSynthesizerSourceVisitorIdentify::estWave.
	 */
	deSynthesizerSourceWave &CastToWave() const;
	
	/**
	 * \brief Cast to chain source.
	 * \throws deeInvalidParam Type of source is not deSynthesizerSourceVisitorIdentify::estChain.
	 */
	deSynthesizerSourceChain &CastToChain() const;
	
	/**
	 * \brief Cast to synthesizer source.
	 * \throws deeInvalidParam Type of source is not deSynthesizerSourceVisitorIdentify::estSynthesizer.
	 */
	deSynthesizerSourceSynthesizer &CastToSynthesizer() const;
	
	/**
	 * \brief Cast to group source.
	 * \throws deeInvalidParam Type of source is not deSynthesizerSourceVisitorIdentify::estGroup.
	 */
	deSynthesizerSourceGroup &CastToGroup() const;
	
	/** \brief Reset visitor. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit source. */
	virtual void VisitSource(deSynthesizerSource &source);
	
	/** \brief Visit sound source. */
	virtual void VisitSound(deSynthesizerSourceSound &source);
	
	/** \brief Visit wave generator source. */
	virtual void VisitWave(deSynthesizerSourceWave &source);
	
	/** \brief Visit chain source. */
	virtual void VisitChain(deSynthesizerSourceChain &source);
	
	/** \brief Visit synthesizer source. */
	virtual void VisitSynthesizer(deSynthesizerSourceSynthesizer &source);
	
	/** \brief Visit group source. */
	virtual void VisitGroup(deSynthesizerSourceGroup &source);
	/*@}*/
};

#endif
