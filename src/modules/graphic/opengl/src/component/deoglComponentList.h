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

#ifndef _DEOGLCOMPONENTLIST_H_
#define _DEOGLCOMPONENTLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class deoglRComponent;


/**
 * Component pointer list.
 */
class deoglComponentList{
private:
	decPointerList pComponents;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create list. */
	deoglComponentList();
	
	/** Clean up list. */
	~deoglComponentList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Count of components. */
	int GetCount() const;
	
	/** Index of the component or -1 if absent. */
	int IndexOfComponent(deoglRComponent *component) const;
	
	/** Component at index. */
	deoglRComponent *GetAt(int index) const;
	
	/** Add component. */
	void Add(deoglRComponent *component);
	
	/** Remove component from index. */
	void RemoveFrom(int index);
	
	/** Remove all components. */
	void RemoveAll();
	/*@}*/
};

#endif
