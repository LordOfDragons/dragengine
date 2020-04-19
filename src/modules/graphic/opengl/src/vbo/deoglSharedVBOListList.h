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

#ifndef _DEOGLSHAREDVBOLISTLIST_H_
#define _DEOGLSHAREDVBOLISTLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class deoglVBOLayout;
class deoglRenderThread;
class deoglSharedVBOList;



/**
 * @brief OpenGL Shared VBO List List.
 * List of shared VBO lists. Convenience class to store various types of shared VBO lists.
 * The list is gready so lists are created if not existing and stay around for as long as
 * the graphic module runs.
 */
class deoglSharedVBOListList{
public:
	deoglRenderThread &pRenderThread;
	int pMaxSize;
	decPointerList pLists;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shared vbo list list. */
	deoglSharedVBOListList( deoglRenderThread &renderThread, int maxSize );
	/** Cleans up the shared vbo list list. */
	~deoglSharedVBOListList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	/** Retrieves the maximum vbo size. */
	inline int GetMaxSize() const{ return pMaxSize; }
	
	/** Retrieves the number of lists. */
	int GetCount() const;
	/** Retrieves the list at the given location. */
	deoglSharedVBOList *GetAt( int index ) const;
	/** Determines if a matching list exists. */
	bool HasWith( const deoglVBOLayout &layout, GLenum drawType ) const;
	/** Retrieves the matching list creating it if not existing already. */
	deoglSharedVBOList *GetWith( const deoglVBOLayout &layout, GLenum drawType );
	
	/** \brief Prepare all lists. */
	void PrepareAllLists();
	/*@}*/
};

#endif
