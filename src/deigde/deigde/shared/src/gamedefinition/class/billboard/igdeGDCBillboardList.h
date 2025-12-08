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

#ifndef _IGDEGDCBILLBOARDLIST_H_
#define _IGDEGDCBILLBOARDLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCBillboard;



/**
 * \brief Game Definition Class Billboard List.
 */
class DE_DLL_EXPORT igdeGDCBillboardList{
private:
	decObjectOrderedSet pBillboards;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCBillboardList();
	
	/** \brief Create copy of a game definition class billboard list. */
	igdeGDCBillboardList(const igdeGDCBillboardList &list);
	
	/** \brief Clean up list. */
	~igdeGDCBillboardList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of billboards. */
	int GetCount() const;
	
	/** \brief Billboard by index. */
	igdeGDCBillboard *GetAt(int index) const;
	
	/** \brief Index of an billboard or -1 if not found. */
	int IndexOf(igdeGDCBillboard::Ref billboard) const;
	
	/** \brief Determines if an billboard exists. */
	bool Has(igdeGDCBillboard::Ref billboard) const;
	
	/** \brief Adds an billboard. */
	void Add(igdeGDCBillboard::Ref billboard);
	
	/** \brief Inserts a new billboard. */
	void InsertAt(igdeGDCBillboard::Ref billboard, int index);
	
	/** \brief Moves an billboard to a new position. */
	void MoveTo(igdeGDCBillboard::Ref billboard, int index);
	
	/** \brief Removes an billboard. */
	void Remove(igdeGDCBillboard::Ref billboard);
	
	/** \brief Removes all billboards. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom(const igdeGDCBillboardList &list);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list from another list. */
	igdeGDCBillboardList &operator=(const igdeGDCBillboardList &list);
	/*@}*/
};

#endif
