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

#ifndef _DELENGINEMODULELIST_H_
#define _DELENGINEMODULELIST_H_

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/string/decStringList.h>

class delEngineModule;



/**
 * \brief Engine Module List.
 */
class DE_DLL_EXPORT delEngineModuleList{
private:
	decObjectList pModules;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	delEngineModuleList();
	
	/** \brief Clean up list. */
	~delEngineModuleList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of modules. */
	int GetCount() const;
	
	/** \brief Module at index. */
	delEngineModule *GetAt( int index ) const;
	
	/** \brief Highest version named module or nullptr if absent. */
	delEngineModule *GetNamed( const char *name ) const;
	
	/** \brief Named module with version or nullptr if absent. */
	delEngineModule *GetNamed( const char *name, const char *version ) const;
	
	/** \brief Named module with at least version or nullptr if absent. */
	delEngineModule *GetNamedAtLeast( const char *name, const char *version ) const;
	
	/** \brief List of all versions of named module in undefined order. */
	void GetNamed( decObjectList &list, const char *name ) const;
	
	/** \brief List of all module names. */
	void GetNames( decStringList &list ) const;
	
	/** \brief Module is present. */
	bool Has( delEngineModule *module ) const;
	
	/** \brief Named module is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Named module with version is present. */
	bool HasNamed( const char *name, const char *version ) const;
	
	/** \brief Index of module or -1 if absent. */
	int IndexOf( delEngineModule *module ) const;
	
	/** \brief Index of higtest version named module or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Index of named module with version or -1 if absent. */
	int IndexOfNamed( const char *name, const char *version ) const;
	
	/** \brief Add module. */
	void Add( delEngineModule *module );
	
	/** \brief Remove module. */
	void Remove( delEngineModule *module );
	
	/** \brief Remove all modules. */
	void RemoveAll();
	/*@}*/
};

#endif
