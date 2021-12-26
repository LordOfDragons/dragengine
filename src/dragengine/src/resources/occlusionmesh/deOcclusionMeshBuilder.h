/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEOCCLUSIONMESHBUILDER_H_
#define _DEOCCLUSIONMESHBUILDER_H_

#include "../../dragengine_export.h"

class deOcclusionMesh;


/**
 * \brief Occlusion Mesh Builder.
 */
class DE_DLL_EXPORT deOcclusionMeshBuilder{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new builder. */
	deOcclusionMeshBuilder();
	
	/** \brief Clean up builder. */
	virtual ~deOcclusionMeshBuilder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Build occlusion mesh into the provided object. */
	virtual void BuildOcclusionMesh( deOcclusionMesh *occmesh ) = 0;
	/*@}*/
};

#endif
