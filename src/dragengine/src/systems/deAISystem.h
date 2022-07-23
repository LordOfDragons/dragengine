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

#ifndef _DEAISYSTEM_H_
#define _DEAISYSTEM_H_

#include "deBaseSystem.h"
#include "../common/math/decMath.h"

class deHeightTerrain;
class deBaseAIModule;
class deBaseAINavigationBlocker;
class deBaseAINavigationSpace;
class deBaseAINavigator;
class deBaseAIWorld;
class deEngine;
class deLoadableModule;
class deNavigationBlocker;
class deNavigationSpace;
class deNavigator;
class deWorld;


/**
 * \brief AI System.
 * The ai system is a single type system providing ai support
 * for the engine. An ai module is able to carry out the actions
 * stored in an ai object.
 */
class DE_DLL_EXPORT deAISystem : public deBaseSystem{
private:
	deBaseAIModule *pActiveModule;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new ai system  linked to the given engine. */
	deAISystem( deEngine *engine );
	
	/** \brief Clean up ai system. */
	virtual ~deAISystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Activated graphic module. */
	inline deBaseAIModule *GetActiveModule() const{ return pActiveModule; }
	
	/** \brief Create peer object for the given world using the active module and assigns it. */
	void LoadWorld( deWorld *world );
	
	/** \brief Create peer object for the given navigation space using the active module and assigns it. */
	void LoadNavigationSpace( deNavigationSpace *navspace );
	
	/** \brief Create peer object for the given navigation blocker using the active module and assigns it. */
	void LoadNavigationBlocker( deNavigationBlocker *blocker );
	
	/** \brief Create peer object for the given navigator using the active module and assigns it. */
	void LoadNavigator( deNavigator *navigator );
	
	/**
	 * \brief Create peer for height terrain using the active module and assigns it.
	 * 
	 * The AI Module is not required to create a peer.
	 * 
	 * \throws deeInvalidParam \em heightTerrain has an AI peer.
	 * \throws deeInvalidParam AI module failed to create the peer.
	 */
	void LoadHeightTerrain( deHeightTerrain &heightTerrain );
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
