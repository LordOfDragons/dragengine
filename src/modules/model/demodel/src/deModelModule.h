/* 
 * Drag[en]gine Model Module
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

#ifndef _DEMODELMODULE_H_
#define _DEMODELMODULE_H_

#include <dragengine/systems/modules/model/deBaseModelModule.h>
#include <dragengine/threading/deMutex.h>

class deCacheHelper;
class demdlTexCoordSorter;
class demdlWeightSetList;
class deModelLOD;

struct sModelInfos{
	int version;
	int flags;
	int boneCount;
	int textureCount;
	int texCoordSetCount;
	int vertexPositionSetCount;
	int normalCount;
	int tangentCount;
	int weightsCount;
	int vertexCount;
	int triangleCount;
	int quadCount;
	int faceCount;
	demdlWeightSetList *weightSetList;
	demdlTexCoordSorter *texCoordSorter;
	bool isLargeModel;
	const char *filename;
};



/**
 * @brief Drag[en]gine Model Module.
 * Module for loading and saving models in the Drag[en]gine binary
 * model file format.
 */
class deModelModule : public deBaseModelModule{
private:
	deMutex pCacheMutex;
	deCacheHelper *pCacheTCSorter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deModelModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deModelModule();
	/*@}*/
	
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Load model. */
	virtual void LoadModel( decBaseFileReader &reader, deModel &model );
	
	/** \brief Save model. */
	virtual void SaveModel( decBaseFileWriter &writer, const deModel &model );
	/*@}*/
	
private:
	void pLoadVersion0( decBaseFileReader &reader, deModel &model );
	
	void pLoadModel( decBaseFileReader &reader, deModel &model );
	void pLoadBones( decBaseFileReader &reader, deModel &model, sModelInfos &infos );
	void pLoadTextures( decBaseFileReader &reader, deModel &model, sModelInfos &infos );
	void pLoadTexCoordSetsOld( decBaseFileReader &reader, deModel &model, sModelInfos &infos );
	void pLoadTexCoordSets( decBaseFileReader &reader, deModel &model, sModelInfos &infos );
	void pLoadVertPosSets( decBaseFileReader &reader, deModel &model, sModelInfos &infos );
	void pLoadWeights( decBaseFileReader &reader, sModelInfos &infos, deModelLOD &lodMesh );
	void pLoadVertices( decBaseFileReader &reader, sModelInfos &infos, deModelLOD &lodMesh );
	void pLoadTexCoords(decBaseFileReader &reader, sModelInfos& infos, deModelLOD& lodMesh );
	void pLoadVertPosSets(decBaseFileReader &reader, sModelInfos& infos, deModelLOD& lodMesh );
	void pLoadTrianglesOld( decBaseFileReader &reader, sModelInfos &infos, deModelLOD &lodMesh );
	void pLoadQuadsOld( decBaseFileReader &reader, sModelInfos &infos, deModelLOD &lodMesh );
	void pLoadTriangles( decBaseFileReader &reader, sModelInfos &infos, deModelLOD &lodMesh );
	void pLoadQuads( decBaseFileReader &reader, sModelInfos &infos, deModelLOD &lodMesh );
	void pUpdateFaceTexCoordIndices( deModel &model, sModelInfos &infos, deModelLOD &lodMesh );
	
	void pSaveModel( decBaseFileWriter &writer, const deModel &model );
	void pSaveBones( decBaseFileWriter &writer, const deModel &model );
	void pSaveTextures( decBaseFileWriter &writer, const deModel &model );
	void pSaveTexCoordSets( decBaseFileWriter &writer, const deModel &model );
	void pSaveVertPosSets( decBaseFileWriter &writer, const deModel &model );
	void pSaveLods( decBaseFileWriter &writer, const deModel &model );
	void pSaveWeights( decBaseFileWriter &writer, const deModelLOD &lodMesh );
	void pSaveVertices( decBaseFileWriter &writer, const deModelLOD &lodMesh, bool largeModel );
	void pSaveTexCoords(decBaseFileWriter &writer, const deModelLOD& lodMesh, bool largeModel );
	void pSaveVertPosSets(decBaseFileWriter &writer, const deModelLOD& lodMesh, bool largeModel );
	void pSaveTriangles( decBaseFileWriter &writer, const deModelLOD &lodMesh, bool largeModel );
};

#endif
