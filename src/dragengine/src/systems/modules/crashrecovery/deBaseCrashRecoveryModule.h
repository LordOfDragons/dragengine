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

#ifndef _DEBASECRASHRECOVERYMODULE_H_
#define _DEBASECRASHRECOVERYMODULE_H_

#include "../deBaseModule.h"



/**
 * \brief Base Crash Recovery Module.
 *
 * Crash Recovery Modules are responsible for handling errors during the execution
 * of the game engine. If anyone of the single type modules fails the crash recovery
 * module is fired up to allow the user to locate the error and recover from it.
 * Errors in the crash recovery module itself lead to an immediate termination of
 * the engine hence this is the module to pay special attention to in terms of
 * error safeness.
 */
class DE_DLL_EXPORT deBaseCrashRecoveryModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new module. */
	deBaseCrashRecoveryModule(deLoadableModule &loadableModule);
	
	/** \brief Clean up module. */
	~deBaseCrashRecoveryModule() override;
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Initialize crash recovery module. */
	virtual bool Init() = 0;
	
	/** \brief Shut down crash recovery module and cleans up. */
	virtual void CleanUp() = 0;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Take over the run time in the case of an error.
	 * 
	 * The modules has to either provide its own event queues (in the case of ToolKits)
	 * or to operate the event queues of the deOS object itself. Once the engine has
	 * reached a state where execution can continue as before this function has to exit
	 * with a true value. If false is returned a non recoverable error is assumed and
	 * the engine is shut down gracefully.
	 */
	virtual bool RecoverFromError() = 0;
	/*@}*/
};

#endif
