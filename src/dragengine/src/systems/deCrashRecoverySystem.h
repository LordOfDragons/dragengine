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

#ifndef _DECRASHRECOVERYSYSTEM_H_
#define _DECRASHRECOVERYSYSTEM_H_

#include "deBaseSystem.h"

class deLoadableModule;
class deBaseCrashRecoveryModule;


/**
 * \brief Crash Recovery System.
 *
 * Provides modules for handling errors in one of the single type engine systems
 * during run time. Such modules take over the run time during an error and try
 * to reach a working state again so the engine can continue running.
 */
class DE_DLL_EXPORT deCrashRecoverySystem : public deBaseSystem{
private:
	deBaseCrashRecoveryModule *pActiveModule;
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new crash recovery system linked to the given engine. */
	deCrashRecoverySystem(deEngine *engine);
	
	/** \brief Clean up physics system. */
	~deCrashRecoverySystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	inline deBaseCrashRecoveryModule *GetActiveModule() const{return pActiveModule;}
	
	/** \brief Yield control to the active crash recovery module. */
	bool RecoverFromError();
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
};

#endif
