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
	virtual ~deSynthesizerEffectVisitorIdentify();
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
	virtual void VisitEffect( deSynthesizerEffect &effect );
	
	/** \brief Visit stretch effect. */
	virtual void VisitStretch( deSynthesizerEffectStretch &effect );
	/*@}*/
};

#endif
