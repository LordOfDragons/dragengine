/* 
 * Drag[en]gine Synthesizer Module
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
