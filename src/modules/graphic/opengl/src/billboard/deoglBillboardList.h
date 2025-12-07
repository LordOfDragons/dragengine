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

#ifndef _DEOGLBILLBOARDLIST_H_
#define _DEOGLBILLBOARDLIST_H_

class deoglRBillboard;



/**
 * Billboard List.
 * List of pointers to billboards. The ordering of the billboards in the list is
 * not defined and can change when billboards are removed. This allows to
 * remove elements with constant time requirements.
 */
class deoglBillboardList{
private:
	deoglRBillboard **pBillboards;
	int pBillboardCount;
	int pBillboardSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new billboard list. */
	deoglBillboardList();
	/** Cleans up the billboard list. */
	~deoglBillboardList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of billboards. */
	inline int GetCount() const{return pBillboardCount;}
	/** Retrieves the index of the billboard or -1 if not in the list. */
	int IndexOf(deoglRBillboard *billboard) const;
	/** Determines if the billboard exists. */
	bool Has(deoglRBillboard *billboard) const;
	/** Retrieves the billboard at the given index. */
	deoglRBillboard *GetAt(int index) const;
	/** Adds a billboard. */
	void Add(deoglRBillboard *billboard);
	/** Adds a billboard if missing in the list. Returns true if added. */
	bool AddIfMissing(deoglRBillboard *billboard);
	/** Removes billboard. */
	void Remove(deoglRBillboard *billboard);
	/** Removes billboard if existing in the list. Returns true if removed. */
	bool RemoveIfExisting(deoglRBillboard *billboard);
	/** Removes billboard from the given index. */
	void RemoveFrom(int index);
	/** Removes all billboards. */
	void RemoveAll();
	/*@}*/
	
private:
	void pAddBillboard(deoglRBillboard *billboard);
};

#endif
