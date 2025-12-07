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
	deAnimatorSystem(deEngine *engine);
	
	/** \brief Clean up animator system. */
	virtual ~deAnimatorSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Activate graphic module or NULL if none is active. */
	inline deBaseAnimatorModule *GetActiveModule() const{ return pActiveModule; }
	
	/** \brief Create and assign peer for animator using the active module. */
	void LoadAnimator(deAnimator *animator);
	
	/** \brief Create and assign peer for animator instance using the active module. */
	void LoadAnimatorInstance(deAnimatorInstance *instance);
	
	/** \brief Create and assign peer for animation using the active module. */
	void LoadAnimation(deAnimation *animation);
	
	/** \brief Create and assign peer for component using the active module. */
	void LoadComponent(deComponent *component);
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
