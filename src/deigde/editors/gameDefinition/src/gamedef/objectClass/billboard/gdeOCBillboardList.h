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

#ifndef _GDEOCBILLBOARDLIST_H_
#define _GDEOCBILLBOARDLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCBillboard;



/**
 * \brief Object class billboard list.
 */
class gdeOCBillboardList{
private:
	decObjectOrderedSet pBillboards;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class billboard list. */
	gdeOCBillboardList();
	
	/** \brief Create copy of object class billboard list. */
	gdeOCBillboardList( const gdeOCBillboardList &list );
	
	/** \brief Clean up object class billboard list. */
	~gdeOCBillboardList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class billboards. */
	int GetCount() const;
	
	/** \brief Object class billboard at position. */
	gdeOCBillboard *GetAt( int index ) const;
	
	/** \brief Index of named object class billboard or -1 if absent. */
	int IndexOf( gdeOCBillboard *billboard ) const;
	
	/** \brief Object class billboard is present. */
	bool Has( gdeOCBillboard *billboard ) const;
	
	/** \brief Add object class billboard. */
	void Add( gdeOCBillboard *billboard );
	
	/** \brief Remove object class billboard. */
	void Remove( gdeOCBillboard *billboard );
	
	/** \brief Remove all object class billboards. */
	void RemoveAll();
	
	/** \brief Set from another object class billboard list. */
	gdeOCBillboardList &operator=( const gdeOCBillboardList &list );
	/*@}*/
};

#endif
