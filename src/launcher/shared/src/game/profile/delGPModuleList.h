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

#ifndef _DELGPMODULELIST_H_
#define _DELGPMODULELIST_H_

#include <dragengine/common/collection/decObjectList.h>

class delGPModule;


/**
 * \brief Game Profile Module List.
 */
class DE_DLL_EXPORT delGPModuleList{
private:
	decObjectList pModules;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	delGPModuleList();
	
	/** \brief Clean up list. */
	~delGPModuleList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of modules. */
	int GetCount() const;
	
	/** \brief Module at index. */
	delGPModule *GetAt(int index) const;
	
	/** \brief Named module or nullptr if absent. */
	delGPModule *GetNamed(const char *name) const;
	
	/** \brief Module is present. */
	bool Has(delGPModule *module) const;
	
	/** \brief Named module is present. */
	bool HasNamed(const char *name) const;
	
	/** \brief Index of module or -1 if absent. */
	int IndexOf(delGPModule *module) const;
	
	/** \brief Index of named module or -1 if absent. */
	int IndexOfNamed(const char *name) const;
	
	/** \brief Add module. */
	void Add(delGPModule *module);
	
	/** \brief Remove module. */
	void Remove(delGPModule *module);
	
	/** \brief Remove all modules. */
	void RemoveAll();
	
	/** \brief Update module list with another. */
	void Update(const delGPModuleList &list);
	/*@}*/
};

#endif
