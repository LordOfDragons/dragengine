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

// include only once
#ifndef _DEOGLCOLLIDELISTMANAGER_H_
#define _DEOGLCOLLIDELISTMANAGER_H_

// includes

// predefinitions
class deoglCollideList;



// opengl collide list manager
class deoglCollideListManager {
private:
	struct sList{
		deoglCollideList *list;
		bool inUse;
	};
private:
	sList *pLists;
	int pListCount, pListSize;
public:
	// constructor, destructor
	deoglCollideListManager();
	~deoglCollideListManager();
	// collide list management
	inline int GetListCount() const{ return pListCount; }
	deoglCollideList *GetList();
	void ReleaseList( deoglCollideList *list );
private:
	int pFindNextList() const;
};

// end of include only once
#endif
