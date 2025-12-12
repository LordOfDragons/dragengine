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

#ifndef _IGDEPROPERTYLIST_H_
#define _IGDEPROPERTYLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDProperty;



/**
 * \brief Property list.
 */
class DE_DLL_EXPORT igdeGDPropertyList{
private:
	decObjectOrderedSet pProperties;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create property list. */
	igdeGDPropertyList();
	
	/** \brief Create copy of property list. */
	igdeGDPropertyList(const igdeGDPropertyList &list);
	
	/** \brief Clean up property list. */
	~igdeGDPropertyList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of propertyes. */
	int GetCount() const;
	
	/** \brief Property at position. */
	igdeGDProperty *GetAt(int index) const;
	
	/** \brief Named property or nullptr if absent. */
	igdeGDProperty *GetNamed(const char *name) const;
	
	/** \brief Index of named property or -1 if absent. */
	int IndexOf(igdeGDProperty *proeprty) const;
	
	/** \brief Index of named property or -1 if absent. */
	int IndexOfNamed(const char *name) const;
	
	/** \brief Property is present. */
	bool Has(igdeGDProperty *proeprty) const;
	
	/** \brief Named property is present. */
	bool HasNamed(const char *name) const;
	
	/** \brief Add property. */
	void Add(igdeGDProperty *proeprty);
	
	/** \brief Remove property. */
	void Remove(igdeGDProperty *proeprty);
	
	/** \brief Remove all propertyes. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom(const igdeGDPropertyList &list);
	
	/** \brief Set from another property list. */
	igdeGDPropertyList &operator=(const igdeGDPropertyList &list);
	/*@}*/
};

#endif
