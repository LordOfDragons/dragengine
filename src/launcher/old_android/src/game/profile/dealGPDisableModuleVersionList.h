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

#ifndef _DEALGPDISABLEMODULEVERSIONLIST_H_
#define _DEALGPDISABLEMODULEVERSIONLIST_H_

#include "../../common/collection/decObjectList.h"

class dealGPDisableModuleVersion;



/**
 * \brief Game profile disable module version list.
 */
class dealGPDisableModuleVersionList{
private:
	decObjectList pList;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create disabled module version list. */
	dealGPDisableModuleVersionList();
	
	/** \brief Create copy of disabled module version list. */
	dealGPDisableModuleVersionList(const dealGPDisableModuleVersionList &copy);
	
	/** \brief Clean up disabled module version list. */
	~dealGPDisableModuleVersionList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of entries. */
	int GetCount() const;
	
	/** \brief Entry at the given position. */
	dealGPDisableModuleVersion *GetAt(int index) const;
	
	/** \brief Matching entry or \em NULL if not found. */
	dealGPDisableModuleVersion *GetWith(const char *name, const char *version) const;
	
	/** \brief Entry exists. */
	bool Has(dealGPDisableModuleVersion *entry) const;
	
	/** \brief Matching entry exists. */
	bool HasWith(const char *name, const char *version) const;
	
	/** \brief Index of an entry or -1 if not found. */
	int IndexOf(dealGPDisableModuleVersion *entry) const;
	
	/** \brief Index of a matching entry or -1 if not found. */
	int IndexOfWith(const char *name, const char *version) const;
	
	/** \brief Add an entry. */
	void Add(dealGPDisableModuleVersion *entry);
	
	/** \brief Remove an entry. */
	void Remove(dealGPDisableModuleVersion *entry);
	
	/** \brief Remove all entries. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set disable module version list to a copy of another list. */
	dealGPDisableModuleVersionList &operator=(const dealGPDisableModuleVersionList &other);
	/*@}*/
};

#endif
