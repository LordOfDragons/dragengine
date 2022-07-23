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

#ifndef _DEANIMATORSYSTEM_H_
#define _DEANIMATORSYSTEM_H_

#include "deBaseSystem.h"
#include "../common/math/decMath.h"

class deComponent;
class deAnimation;
class deEngine;
class deLoadableModule;
class deBaseAnimatorModule;
class deAnimator;
class deAnimatorInstance;


/**
 * \brief Animator System.
 *
 * The animator system is a single type system providing animation support
 * for the engine. An animator module is able to carry out the actions
 * stored in an animator object.
 */
class DE_DLL_EXPORT deAnimatorSystem : public deBaseSystem{
private:
	deBaseAnimatorModule *pActiveModule;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create animator system. */
	deAnimatorSystem( deEngine *engine );
	
	/** \brief Clean up animator system. */
	virtual ~deAnimatorSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Activate graphic module or NULL if none is active. */
	inline deBaseAnimatorModule *GetActiveModule() const{ return pActiveModule; }
	
	/** \brief Create and assign peer for animator using the active module. */
	void LoadAnimator( deAnimator *animator );
	
	/** \brief Create and assign peer for animator instance using the active module. */
	void LoadAnimatorInstance( deAnimatorInstance *instance );
	
	/** \brief Create and assign peer for animation using the active module. */
	void LoadAnimation( deAnimation *animation );
	
	/** \brief Create and assign peer for component using the active module. */
	void LoadComponent( deComponent *component );
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
