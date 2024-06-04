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

#ifndef _DEALENGINEMODULELIST_H_
#define _DEALENGINEMODULELIST_H_

#include "../../common/collection/decObjectList.h"
#include "../../common/string/decStringList.h"

class dealEngineModule;



/**
 * \brief Engine Module List.
 */
class dealEngineModuleList{
private:
	decObjectList pModules;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new module list. */
	dealEngineModuleList();
	/** \brief Cleans up the module list. */
	~dealEngineModuleList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieve the number of modules. */
	int GetModuleCount() const;
	/** \brief Retrieve the module at the given position. */
	dealEngineModule *GetModuleAt( int index ) const;
	/** \brief Retrieve the highest version module with the given name or NULL if not found. */
	dealEngineModule *GetModuleNamed( const char *name ) const;
	/** \brief Retrieve the module with the given name and version or NULL if not found. */
	dealEngineModule *GetModuleNamed( const char *name, const char *version ) const;
	/** \brief Retrieve list of all versions of a named module in arbitrary order. */
	void GetModulesNamed( decObjectList &list, const char *name ) const;
	/** \brief Retrieve list of all module names. */
	void GetModulesNames( decStringList &list ) const;
	/** \brief Determines if a module exists. */
	bool HasModule( dealEngineModule *module ) const;
	/** \brief Determines if a module with the given name exists. */
	bool HasModuleNamed( const char *name ) const;
	/** \brief Determines if a module with the given name and version exists. */
	bool HasModuleNamed( const char *name, const char *version ) const;
	/** \brief Retrieve the index of a module or -1 if not found. */
	int IndexOfModule( dealEngineModule *module ) const;
	/** \brief Retrieve the index of the higtest version module with the given name or -1 if not found. */
	int IndexOfModuleNamed( const char *name ) const;
	/** \brief Retrieve the index of the module with the given name and version or -1 if not found. */
	int IndexOfModuleNamed( const char *name, const char *version ) const;
	/** \brief Adds a module. */
	void AddModule( dealEngineModule *module );
	/** \brief Removes a module. */
	void RemoveModule( dealEngineModule *module );
	/** \brief Removes all modules. */
	void RemoveAllModules();
	/*@}*/
};

#endif
