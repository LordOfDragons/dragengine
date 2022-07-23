/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	inline eSourceTypes GetType() const{ return pType; }
	
	/** \brief Source is unknown. */
	inline bool IsUnknown() const{ return pType == estUnknown; }
	
	/** \brief Source is a sound source. */
	inline bool IsSound() const{ return pType == estSound; }
	
	/** \brief Source is a wave generator source. */
	inline bool IsWave() const{ return pType == estWave; }
	
	/** \brief Source is a chain source. */
	inline bool IsChain() const{ return pType == estChain; }
	
	/** \brief Source is a synthesizer source. */
	inline bool IsSynthesizer() const{ return pType == estSynthesizer; }
	
	/** \brief Source is a group source. */
	inline bool IsGroup() const{ return pType == estGroup; }
	
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
	virtual void VisitSource( deSynthesizerSource &source );
	
	/** \brief Visit sound source. */
	virtual void VisitSound( deSynthesizerSourceSound &source );
	
	/** \brief Visit wave generator source. */
	virtual void VisitWave( deSynthesizerSourceWave &source );
	
	/** \brief Visit chain source. */
	virtual void VisitChain( deSynthesizerSourceChain &source );
	
	/** \brief Visit synthesizer source. */
	virtual void VisitSynthesizer( deSynthesizerSourceSynthesizer &source );
	
	/** \brief Visit group source. */
	virtual void VisitGroup( deSynthesizerSourceGroup &source );
	/*@}*/
};

#endif
