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

#ifndef _DEALGPMODULELIST_H_
#define _DEALGPMODULELIST_H_

#include "../../common/collection/decObjectList.h"

class dealGPModule;



/**
 * \brief Game Profile Module List.
 */
class dealGPModuleList{
private:
	decObjectList pModules;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game profile module list. */
	dealGPModuleList();
	
	/** \brief Clean up game profile module list. */
	~dealGPModuleList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of modules. */
	int GetModuleCount() const;
	
	/** \brief Module at index. */
	dealGPModule *GetModuleAt(int index) const;
	
	/** \brief Module with the name or \em NULL if not found. */
	dealGPModule *GetModuleNamed(const char *name) const;
	
	/** \brief Module exists. */
	bool HasModule(dealGPModule *module) const;
	
	/** \brief Module with name exists. */
	bool HasModuleNamed(const char *name) const;
	
	/** \brief Index of module or -1 if not found. */
	int IndexOfModule(dealGPModule *module) const;
	
	/** \brief Index of module with name or -1 if not found. */
	int IndexOfModuleNamed(const char *name) const;
	
	/** \brief Add module. */
	void AddModule(dealGPModule *module);
	
	/** \brief Remove module. */
	void RemoveModule(dealGPModule *module);
	
	/** \brief Remove all modules. */
	void RemoveAllModules();
	/*@}*/
};

#endif
