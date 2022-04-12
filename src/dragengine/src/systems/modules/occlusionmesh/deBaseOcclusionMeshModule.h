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

#ifndef _DEBASEOCCLUSIONMESHMODULE_H_
#define _DEBASEOCCLUSIONMESHMODULE_H_

#include "../deBaseModule.h"

class decBaseFileReader;
class decBaseFileWriter;
class deOcclusionMesh;


/**
 * \brief Base occlusion mesh module providing support to load occlusion meshes.
 */
class DE_DLL_EXPORT deBaseOcclusionMeshModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseOcclusionMeshModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBaseOcclusionMeshModule();
	/*@}*/
	
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Load occlusion mesh. */
	virtual void LoadOcclusionMesh( decBaseFileReader &reader, deOcclusionMesh &occlusionMesh ) = 0;
	
	/** \brief Save occlusion mesh. */
	virtual void SaveOcclusionMesh( decBaseFileWriter &writer, const deOcclusionMesh &occlusionMesh ) = 0;
	/*@}*/
};

#endif
