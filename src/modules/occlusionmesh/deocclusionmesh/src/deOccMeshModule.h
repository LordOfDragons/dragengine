/* 
 * Drag[en]gine Occlusion Mesh Module
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

#ifndef _DEOCCMESHMODULE_H_
#define _DEOCCMESHMODULE_H_

#include <dragengine/systems/modules/occlusionmesh/deBaseOcclusionMeshModule.h>

class deoccmWeightSetList;

struct sMeshInfos{
	int version;
	int flags;
	int boneCount;
	int weightsCount;
	int vertexCount;
	int cornerCount;
	int faceCount;
	int doubleSidedFaceCount;
	deoccmWeightSetList *weightSetList;
};



/**
 * @brief Drag[en]gine Occlusion Mesh Module.
 * Module for loading and saving occlusion meshes in the Drag[en]gine occlusion mesh file format.
 */
class deOccMeshModule : public deBaseOcclusionMeshModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deOccMeshModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deOccMeshModule();
	/*@}*/
	
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Load occlusion mesh. */
	virtual void LoadOcclusionMesh( decBaseFileReader &reader, deOcclusionMesh &occlusionMesh );
	
	/** \brief Save occlusion mesh. */
	virtual void SaveOcclusionMesh( decBaseFileWriter &writer, const deOcclusionMesh &occlusionMesh );
	
	/**
	 * \brief File is compressed.
	 * \version 1.12
	 * \note Call has to be thread safe.
	 */
	virtual bool IsCompressed( decBaseFileReader &reader );
	/*@}*/
	
private:
	void pLoadMesh( decBaseFileReader &reader, deOcclusionMesh &mesh );
	void pLoadBones( decBaseFileReader &reader, deOcclusionMesh &mesh, sMeshInfos &infos );
	void pLoadWeights( decBaseFileReader &reader, deOcclusionMesh &mesh, sMeshInfos &infos );
	void pLoadVertices( decBaseFileReader &reader, deOcclusionMesh &mesh, sMeshInfos &infos );
	void pLoadFaces( decBaseFileReader &reader, deOcclusionMesh &mesh, sMeshInfos &infos );
};

#endif
