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

#ifndef _DESYNTHESIZERSOURCEVISITOR_H_
#define _DESYNTHESIZERSOURCEVISITOR_H_

#include "../../../dragengine_export.h"

class deSynthesizerSource;
class deSynthesizerSourceChain;
class deSynthesizerSourceGroup;
class deSynthesizerSourceSound;
class deSynthesizerSourceSynthesizer;
class deSynthesizerSourceWave;


/**
 * \brief Synthesizer source visitor.
 */
class DE_DLL_EXPORT deSynthesizerSourceVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source visitor . */
	deSynthesizerSourceVisitor();
	
	/** \brief Clean up synthesizer source visitor. */
	virtual ~deSynthesizerSourceVisitor();
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
