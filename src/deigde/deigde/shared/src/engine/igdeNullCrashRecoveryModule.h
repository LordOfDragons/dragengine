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

#ifndef _IGDENULLCRASHRECOVERYMODULE_H_
#define _IGDENULLCRASHRECOVERYMODULE_H_

#include <dragengine/systems/modules/deInternalModule.h>
#include <dragengine/systems/modules/crashrecovery/deBaseCrashRecoveryModule.h>



/**
 * \brief nullptr CrashRecovery Module.
 */
class DE_DLL_EXPORT igdeNullCrashRecoveryModule : public deBaseCrashRecoveryModule{
public:
	class DE_DLL_EXPORT cModule : public deInternalModule{
	public:
		typedef deTObjectReference<cModule> Ref;
		cModule(deModuleSystem *system);
		
	protected:
		virtual ~cModule();
		
	public:
		virtual void CreateModule();
	};
	
	
	
public:
	/** \name Constructor, destructor */
	/*@{*/
	/** \brief Create module. */
	igdeNullCrashRecoveryModule(deLoadableModule &loadableModule);
	
	/** \brief Clean up module. */
	virtual ~igdeNullCrashRecoveryModule();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Initialize module. */
	virtual bool Init();
	
	/** \brief Shut down module and cleans up. */
	virtual void CleanUp();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Crash recovery. */
	virtual bool RecoverFromError();
	/*@}*/
};

#endif
