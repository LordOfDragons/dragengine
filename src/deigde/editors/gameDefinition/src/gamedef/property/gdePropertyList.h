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

#ifndef _GDEPROPERTYLIST_H_
#define _GDEPROPERTYLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeProperty;



/**
 * \brief Property list.
 */
class gdePropertyList{
private:
	decObjectOrderedSet pProperties;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create property list. */
	gdePropertyList();
	
	/** \brief Create copy of property list. */
	gdePropertyList(const gdePropertyList &list);
	
	/** \brief Clean up property list. */
	~gdePropertyList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of propertyes. */
	int GetCount() const;
	
	/** \brief Property at position. */
	gdeProperty *GetAt(int index) const;
	
	/** \brief Named property or \em NULL if absent. */
	gdeProperty *GetNamed(const char *name) const;
	
	/** \brief Index of named property or -1 if absent. */
	int IndexOf(gdeProperty *proeprty) const;
	
	/** \brief Index of named property or -1 if absent. */
	int IndexOfNamed(const char *name) const;
	
	/** \brief Property is present. */
	bool Has(gdeProperty *proeprty) const;
	
	/** \brief Named property is present. */
	bool HasNamed(const char *name) const;
	
	/** \brief Add property. */
	void Add(gdeProperty *proeprty);
	
	/** \brief Remove property. */
	void Remove(gdeProperty *proeprty);
	
	/** \brief Remove all propertyes. */
	void RemoveAll();
	
	/** \brief Set from another property list. */
	gdePropertyList &operator=(const gdePropertyList &list);
	/*@}*/
};

#endif
