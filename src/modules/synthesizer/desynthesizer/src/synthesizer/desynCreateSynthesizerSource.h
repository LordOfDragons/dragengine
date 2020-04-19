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
	desynCreateSynthesizerSource( desynSynthesizer &synthesizer, int firstLink );
	
	/** \brief Clean up visitor. */
	virtual ~desynCreateSynthesizerSource();
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
