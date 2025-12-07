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

#ifndef _GDEOCCOMPONENTLIST_H_
#define _GDEOCCOMPONENTLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCComponent;



/**
 * \brief Object class component list.
 */
class gdeOCComponentList{
private:
	decObjectOrderedSet pComponents;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class component list. */
	gdeOCComponentList();
	
	/** \brief Create copy of object class component list. */
	gdeOCComponentList(const gdeOCComponentList &list);
	
	/** \brief Clean up object class component list. */
	~gdeOCComponentList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class components. */
	int GetCount() const;
	
	/** \brief Object class at position. */
	gdeOCComponent *GetAt(int index) const;
	
	/** \brief Index of named object class component or -1 if absent. */
	int IndexOf(gdeOCComponent *component) const;
	
	/** \brief Object class is present. */
	bool Has(gdeOCComponent *component) const;
	
	/** \brief Add object class component. */
	void Add(gdeOCComponent *component);
	
	/** \brief Remove object class component. */
	void Remove(gdeOCComponent *component);
	
	/** \brief Remove all object class components. */
	void RemoveAll();
	
	/** \brief Set from another object class component list. */
	gdeOCComponentList &operator=(const gdeOCComponentList &list);
	/*@}*/
};

#endif
