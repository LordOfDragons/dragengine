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

#ifndef _DELGPDISABLEMODULEVERSIONLIST_H_
#define _DELGPDISABLEMODULEVERSIONLIST_H_

#include <dragengine/common/collection/decObjectList.h>

class delGPDisableModuleVersion;


/**
 * \brief Game profile disable module version list.
 */
class DE_DLL_EXPORT delGPDisableModuleVersionList{
private:
	decObjectList pList;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	delGPDisableModuleVersionList();
	
	/** \brief Create copy of list. */
	delGPDisableModuleVersionList(const delGPDisableModuleVersionList &copy);
	
	/** \brief Clean up list. */
	~delGPDisableModuleVersionList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of entries. */
	int GetCount() const;
	
	/** \brief Entry at index. */
	delGPDisableModuleVersion *GetAt(int index) const;
	
	/** \brief Matching entry or nullptr if absent. */
	delGPDisableModuleVersion *GetWith(const char *name, const char *version) const;
	
	/** \brief Entry is present. */
	bool Has(delGPDisableModuleVersion *entry) const;
	
	/** \brief Matching entry is present. */
	bool HasWith(const char *name, const char *version) const;
	
	/** \brief Index of entry or -1 if absent. */
	int IndexOf(delGPDisableModuleVersion *entry) const;
	
	/** \brief Index of matching entry or -1 if absent. */
	int IndexOfWith(const char *name, const char *version) const;
	
	/** \brief Add entry. */
	void Add(delGPDisableModuleVersion *entry);
	
	/** \brief Remove entry. */
	void Remove(delGPDisableModuleVersion *entry);
	
	/** \brief Remove all entries. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy list. */
	delGPDisableModuleVersionList &operator=(const delGPDisableModuleVersionList &other);
	/*@}*/
};

#endif
