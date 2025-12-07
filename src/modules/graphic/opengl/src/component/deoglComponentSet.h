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

#ifndef _DEOGLCOMPONENTSET_H_
#define _DEOGLCOMPONENTSET_H_

class deoglRComponent;



/**
 * Component set.
 * 
 * Set of pointers to components. The ordering of the components in the list is
 * not defined and can change when components are removed. This allows to
 * remove elements with constant time requirements.
 */
class deoglComponentSet{
private:
	deoglRComponent **pComponents;
	int pComponentCount;
	int pComponentSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create component list. */
	deoglComponentSet();
	
	/** Clean up component list. */
	~deoglComponentSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of components. */
	inline int GetCount() const{ return pComponentCount; }
	
	/** Index of the component or -1 if not in the list. */
	int IndexOfComponent(deoglRComponent *component) const;
	
	/** Component exists. */
	bool Has(deoglRComponent *component) const;
	
	/** Component at the given index. */
	deoglRComponent *GetAt(int index) const;
	
	/** Add component. */
	void Add(deoglRComponent *component);
	
	/** Add component if missing in the list. Returns true if added. */
	bool AddIfMissing(deoglRComponent *component);
	
	/** Remove component. */
	void Remove(deoglRComponent *component);
	
	/** Remove component if existing in the list. Returns true if removed. */
	bool RemoveIfExisting(deoglRComponent *component);
	
	/** Remove component from the given index. */
	void RemoveFrom(int index);
	
	/** Remove all components. */
	void RemoveAll();
	
	/** Mark all components. */
	void MarkAll(bool mark);
	
	/** Remove marked components. */
	void RemoveAllMarked(bool mark);
	/*@}*/
	
private:
	void pAddComponent(deoglRComponent *component);
};

#endif
