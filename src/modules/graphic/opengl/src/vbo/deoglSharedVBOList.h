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

#ifndef _DEOGLSHAREDVBOLIST_H_
#define _DEOGLSHAREDVBOLIST_H_

#include <dragengine/common/collection/decObjectList.h>

#include "../deoglBasics.h"
#include "deoglVBOLayout.h"

class deoglRenderThread;
class deoglSharedVBO;
class deoglSharedVBOBlock;



/**
 * A shared VBO list contains a list of VBOs all having the same layout. Each VBO contains
 * one or more blocks of data belonging to different resources. Each VBO has one VAO assigned
 * to reduce the VAO switching. Shared VBOs can be used in a static or dynamic way but all
 * data has to fit into one VBO.
 */
class deoglSharedVBOList{
public:
	deoglRenderThread &pRenderThread;
	deoglVBOLayout pLayout;
	GLenum pDrawType;
	int pMaxSize;
	int pMaxPointCount;
	int pMaxIndexSize;
	int pMaxIndexCount;
	decObjectList pVBOs;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared vbo list. */
	deoglSharedVBOList( deoglRenderThread &renderThread, const deoglVBOLayout &layout,
		GLenum drawType, int maxSize, int maxIndexSize );
	
	/** Clean up shared vbo list. */
	~deoglSharedVBOList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Layout. */
	inline const deoglVBOLayout &GetLayout() const{ return pLayout; }
	
	/** Draw type. */
	inline GLenum GetDrawType() const{ return pDrawType; }
	
	/** Maximum VBO size. */
	inline int GetMaxSize() const{ return pMaxSize; }
	
	/** Maximum count of points. */
	inline int GetMaxPointCount() const{ return pMaxPointCount; }
	
	/** Maximum IBO size. */
	inline int GetMaxIndexSize() const{ return pMaxIndexSize; }
	
	/** Maximum count of indices. */
	inline int GetMaxIndexCount() const{ return pMaxIndexCount; }
	
	/** VBO list matches the layout and draw type. */
	bool Matches( const deoglVBOLayout &layout, GLenum drawType ) const;
	
	/** Count of VBOs. */
	int GetCount() const;
	
	/** VBO at index. */
	deoglSharedVBO *GetAt( int index ) const;
	
	/** Add block of data to a matching VBO returning the resulting block. */
	deoglSharedVBOBlock *AddData( int size, int indexCount = 0 );
	
	/** Prepare all VBOs not yet prepared. */
	void PrepareVBOs();
	/*@}*/
};

#endif
