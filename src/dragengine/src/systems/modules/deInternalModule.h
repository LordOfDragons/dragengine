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

#ifndef _DEINTERNALMODULE_H_
#define _DEINTERNALMODULE_H_


#include "deLoadableModule.h"


/**
 * \brief Internal Module.
 *
 * Internal modules are part of the game engine or the program using the game
 * engine. Such modules can be created off the point without any further
 * loading actions. For this create a subclass which overwrites the LoadModule
 * function to create the module in question. Store afterwards the module with
 * SetModule and set the error code with SetErrorCode. This is required as
 * modules are created upon loading and freed upon unloading. The subclass also
 * is responsible to set the module information upon creation time best done
 * inside the constructor but at last before the module is added to the system.
 */
class DE_DLL_EXPORT deInternalModule : public deLoadableModule{
public:
	typedef deTObjectReference<deInternalModule> Ref;
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new internal module object. */
	deInternalModule( deModuleSystem *system );
	
	/** \brief Clean up loadable module. */
	virtual ~deInternalModule();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/**
	 * \brief Create module.
	 * 
	 * Overwrite this function to create the module. Store  afterwards the module
	 * with SetModule and set the error code with SetErrorCode.
	 */
	virtual void CreateModule() = 0;
	
	/**
	 * \brief Load module from the appropriate source.
	 * 
	 * Sets the error code in both the case of success and failure and stored the
	 * module for later retrieval. This function takes care of setting up correctly
	 * the module. The subclass just has to overwrite CreateModule to produce
	 * the module and this function takes care of the rest.
	 */
	virtual void LoadModule();
	
	/**
	 * \brief Unload module.
	 * 
	 * Simply frees the module and sets it to NULL.
	 */
	virtual void UnloadModule();
	/*@}*/
	
	
	
	/** \name Type Testing and Safe Casting */
	/*@{*/
	/** \brief Determines if this is an internal module. */
	virtual bool IsInternalModule() const;
	
	/** \brief Cast to an internal module. */
	virtual deInternalModule *CastToInternalModule();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit module. */
	virtual void Visit( deLoadableModuleVisitor &visitor );
	/*@}*/
};

#endif
