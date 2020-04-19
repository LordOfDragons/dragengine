/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEOGLBILLBOARDLIST_H_
#define _DEOGLBILLBOARDLIST_H_

class deoglRBillboard;



/**
 * @brief Billboard List.
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
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new billboard list. */
	deoglBillboardList();
	/** Cleans up the billboard list. */
	~deoglBillboardList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of billboards. */
	inline int GetCount() const{ return pBillboardCount; }
	/** Retrieves the index of the billboard or -1 if not in the list. */
	int IndexOf( deoglRBillboard *billboard ) const;
	/** Determines if the billboard exists. */
	bool Has( deoglRBillboard *billboard ) const;
	/** Retrieves the billboard at the given index. */
	deoglRBillboard *GetAt( int index ) const;
	/** Adds a billboard. */
	void Add( deoglRBillboard *billboard );
	/** Adds a billboard if missing in the list. Returns true if added. */
	bool AddIfMissing( deoglRBillboard *billboard );
	/** Removes billboard. */
	void Remove( deoglRBillboard *billboard );
	/** Removes billboard if existing in the list. Returns true if removed. */
	bool RemoveIfExisting( deoglRBillboard *billboard );
	/** Removes billboard from the given index. */
	void RemoveFrom( int index );
	/** Removes all billboards. */
	void RemoveAll();
	/*@}*/
	
private:
	void pAddBillboard( deoglRBillboard *billboard );
};

#endif
