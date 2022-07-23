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

#ifndef _DEBASESYNTHESIZERSYNTHESIZER_H_
#define _DEBASESYNTHESIZERSYNTHESIZER_H_

#include "../../../dragengine_export.h"


/**
 * \brief Synthesizer module synthesizer peer.
 */
class DE_DLL_EXPORT deBaseSynthesizerSynthesizer{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseSynthesizerSynthesizer();
	
	/** \brief Clean up peer. */
	virtual ~deBaseSynthesizerSynthesizer();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Parameters changed. */
	virtual void ParametersChanged();
	
	/** \brief Controllers changed. */
	virtual void ControllersChanged();
	
	/** \brief Links changed. */
	virtual void LinksChanged();
	
	/** \brief Sources changed. */
	virtual void SourcesChanged();
	/*@}*/
};

#endif
