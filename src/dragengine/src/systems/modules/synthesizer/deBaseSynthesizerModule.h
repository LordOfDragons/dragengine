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

#ifndef _DEBASESYNTHESIZERMODULE_H_
#define _DEBASESYNTHESIZERMODULE_H_

#include "../deBaseModule.h"

class deBaseSynthesizerSound;
class deBaseSynthesizerSynthesizer;
class deBaseSynthesizerSynthesizerInstance;

class deSound;
class deSynthesizer;
class deSynthesizerInstance;


/**
 * \brief Base synthesizer module.
 */
class DE_DLL_EXPORT deBaseSynthesizerModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseSynthesizerModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBaseSynthesizerModule();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/**
	 * \brief Init module and prepare for synthesizer processing.
	 * \returns true on success or false on failure.
	 */
	virtual bool Init() = 0;
	
	/**
	 * \brief Clean up module.
	 * 
	 * All resources have to be freed and running threads stopped or killed if needed.
	 */
	virtual void CleanUp() = 0;
	/*@}*/
	
	
	
	/** \name Synthesizer Management */
	/*@{*/
	/** \brief Create peer for sound. */
	virtual deBaseSynthesizerSound *CreateSound( deSound *sound ) = 0;
	
	/**
	 * \brief Create peer for synthesizer or NULL if no explicit peer is required.
	 * 
	 * Default implementation return NULL.
	 */
	virtual deBaseSynthesizerSynthesizer *CreateSynthesizer( deSynthesizer *synthesizer ) = 0;
	
	/**
	 * \brief Create peer for synthesizer instance or NULL if no explicit peer is required.
	 * 
	 * Default implementation return NULL.
	 */
	virtual deBaseSynthesizerSynthesizerInstance *CreateSynthesizerInstance(
		deSynthesizerInstance *instance ) = 0;
	/*@}*/
};

#endif
