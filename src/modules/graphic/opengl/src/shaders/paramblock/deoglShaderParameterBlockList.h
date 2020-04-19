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

#ifndef _DEOGLSHADERPARAMETERBLOCKLIST_H_
#define _DEOGLSHADERPARAMETERBLOCKLIST_H_

#include <dragengine/common/collection/decObjectList.h>

class deoglShaderParameterBlock;



/**
 * \brief Shader Parameter Block List.
 */
class deoglShaderParameterBlockList{
private:
	decObjectList pList;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	deoglShaderParameterBlockList();
	
	/** \brief Clean up list. */
	~deoglShaderParameterBlockList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of blocks. */
	int GetCount() const;
	
	/** \brief Index of a block or -1 if not in the list. */
	int IndexOf( deoglShaderParameterBlock *block ) const;
	
	/** \brief Block exists. */
	bool Has( deoglShaderParameterBlock *block ) const;
	
	/** \brief Block by index. */
	deoglShaderParameterBlock *GetAt( int index ) const;
	
	/** \brief Add block. */
	void Add( deoglShaderParameterBlock *block );
	
	/** \brief Remove block. */
	void Remove( deoglShaderParameterBlock *block );
	
	/** \brief Remove all blocks. */
	void RemoveAll();
	/*@}*/
};

#endif
