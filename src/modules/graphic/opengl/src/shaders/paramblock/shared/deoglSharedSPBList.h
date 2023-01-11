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

#ifndef _DEOGLSHAREDSPBLIST_H_
#define _DEOGLSHAREDSPBLIST_H_

#include <dragengine/common/collection/decObjectList.h>

#include "../deoglShaderParameterBlock.h"

class deoglRenderThread;
class deoglSharedSPB;
class deoglSharedSPBElement;



/**
 * \brief OpenGL shared shader parameter block List.
 */
class deoglSharedSPBList{
private:
	deoglRenderThread &pRenderThread;
	const deoglShaderParameterBlock::Ref pLayout;
	int pSize;
	decObjectList pSPBs;
	
	
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shared shader parameter block list. */
	deoglSharedSPBList( deoglRenderThread &renderThread, deoglShaderParameterBlock *layout );
	
public:
	/** \brief Clean up shared shader parameter block list. */
	virtual ~deoglSharedSPBList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief Layout reference. */
	inline const deoglShaderParameterBlock::Ref &GetLayout() const{ return pLayout; }
	
	/** \brief Maximum number of elements per block. */
	int GetSize() const;
	
	/** \brief Number of blocks. */
	int GetCount() const;
	
	/** \brief Block at index. */
	deoglSharedSPB *GetAt( int index ) const;
	
	/**
	 * \brief Add element.
	 * 
	 * Caller obtains reference to the element. Release reference if not used anymore.
	 * Element is removed from the shared parameter block once all references are released.
	 */
	deoglSharedSPBElement *AddElement();
	/*@}*/
	
	
	
protected:
	virtual deoglShaderParameterBlock::Ref pCreateBlock() const = 0;
};

#endif
