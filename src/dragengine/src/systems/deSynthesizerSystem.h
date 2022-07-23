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

#ifndef _DESYNTHESIZERSYSTEM_H_
#define _DESYNTHESIZERSYSTEM_H_

#include "deBaseSystem.h"
#include "../common/math/decMath.h"

class deEngine;
class deLoadableModule;
class deBaseSynthesizerModule;
class deSound;
class deSynthesizer;
class deSynthesizerInstance;


/**
 * \brief Synthesizer system.
 *
 * The synthesizer system is a single type system providing sound support
 * for the engine. An synthesizer module is able to carry out the actions
 * stored in an synthesizer object.
 */
class DE_DLL_EXPORT deSynthesizerSystem : public deBaseSystem{
private:
	deBaseSynthesizerModule *pActiveModule;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer system. */
	deSynthesizerSystem( deEngine *engine );
	
	/** \brief Clean up synthesizer system. */
	virtual ~deSynthesizerSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Activate synthesizer module or NULL if none is active. */
	inline deBaseSynthesizerModule *GetActiveModule() const{ return pActiveModule; }
	
	/** \brief Create and assign peer for sound using the active module. */
	void LoadSound( deSound *sound );
	
	/** \brief Create and assign peer for synthesizer using the active module. */
	void LoadSynthesizer( deSynthesizer *synthesizer );
	
	/** \brief Create and assign peer for synthesizer instance using the active module. */
	void LoadSynthesizerInstance( deSynthesizerInstance *instance );
	/*@}*/
	
	
	
	/** \name Overloadables */
	/*@{*/
	/**
	 * \brief Set active loadable module.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void SetActiveModule( deLoadableModule *module );
	
	/**
	 * \brief Clearcross references and links that could lead to memory leaks.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void ClearPermanents();
	
	/** \brief Carry out here actions right after the system started up. */
	virtual void PostStart();
	
	/** \brief Carry out here actions right before the system shuts down. */
	virtual void PreStop();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
