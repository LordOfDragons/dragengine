/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
	deAISystem(deEngine *engine);
	
	/** \brief Clean up ai system. */
	virtual ~deAISystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Activated graphic module. */
	inline deBaseAIModule *GetActiveModule() const{return pActiveModule;}
	
	/** \brief Create peer object for the given world using the active module and assigns it. */
	void LoadWorld(deWorld *world);
	
	/** \brief Create peer object for the given navigation space using the active module and assigns it. */
	void LoadNavigationSpace(deNavigationSpace *navspace);
	
	/** \brief Create peer object for the given navigation blocker using the active module and assigns it. */
	void LoadNavigationBlocker(deNavigationBlocker *blocker);
	
	/** \brief Create peer object for the given navigator using the active module and assigns it. */
	void LoadNavigator(deNavigator *navigator);
	
	/**
	 * \brief Create peer for height terrain using the active module and assigns it.
	 * 
	 * The AI Module is not required to create a peer.
	 * 
	 * \throws deeInvalidParam \em heightTerrain has an AI peer.
	 * \throws deeInvalidParam AI module failed to create the peer.
	 */
	void LoadHeightTerrain(deHeightTerrain &heightTerrain);
	/*@}*/
	
	
	
	/** \name Overloadables */
	/*@{*/
	/**
	 * \brief Set active loadable module.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void SetActiveModule(deLoadableModule *module);
	
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
