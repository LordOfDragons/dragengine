/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
	deModelModule(deLoadableModule &loadableModule);
	
	/** \brief Clean up module. */
	virtual ~deModelModule();
	/*@}*/
	
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Load model. */
	virtual void LoadModel(decBaseFileReader &reader, deModel &model);
	
	/** \brief Save model. */
	virtual void SaveModel(decBaseFileWriter &writer, const deModel &model);
	/*@}*/
	
private:
	void pLoadVersion0(decBaseFileReader &reader, deModel &model);
	
	void pLoadModel(decBaseFileReader &reader, deModel &model);
	void pLoadBones(decBaseFileReader &reader, deModel &model, sModelInfos &infos);
	void pLoadTextures(decBaseFileReader &reader, deModel &model, sModelInfos &infos);
	void pLoadTexCoordSetsOld(decBaseFileReader &reader, deModel &model, sModelInfos &infos);
	void pLoadTexCoordSets(decBaseFileReader &reader, deModel &model, sModelInfos &infos);
	void pLoadVertPosSets(decBaseFileReader &reader, deModel &model, sModelInfos &infos);
	void pLoadWeights(decBaseFileReader &reader, sModelInfos &infos, deModelLOD &lodMesh);
	void pLoadVertices(decBaseFileReader &reader, sModelInfos &infos, deModelLOD &lodMesh);
	void pLoadTexCoords(decBaseFileReader &reader, sModelInfos& infos, deModelLOD& lodMesh);
	void pLoadVertPosSets(decBaseFileReader &reader, sModelInfos& infos, deModelLOD& lodMesh);
	void pLoadTrianglesOld(decBaseFileReader &reader, sModelInfos &infos, deModelLOD &lodMesh);
	void pLoadQuadsOld(decBaseFileReader &reader, sModelInfos &infos, deModelLOD &lodMesh);
	void pLoadTriangles(decBaseFileReader &reader, sModelInfos &infos, deModelLOD &lodMesh);
	void pLoadQuads(decBaseFileReader &reader, sModelInfos &infos, deModelLOD &lodMesh);
	void pUpdateFaceTexCoordIndices(deModel &model, sModelInfos &infos, deModelLOD &lodMesh);
	
	void pSaveModel(decBaseFileWriter &writer, const deModel &model);
	void pSaveBones(decBaseFileWriter &writer, const deModel &model);
	void pSaveTextures(decBaseFileWriter &writer, const deModel &model);
	void pSaveTexCoordSets(decBaseFileWriter &writer, const deModel &model);
	void pSaveVertPosSets(decBaseFileWriter &writer, const deModel &model);
	void pSaveLods(decBaseFileWriter &writer, const deModel &model);
	void pSaveWeights(decBaseFileWriter &writer, const deModelLOD &lodMesh);
	void pSaveVertices(decBaseFileWriter &writer, const deModelLOD &lodMesh, bool largeModel);
	void pSaveTexCoords(decBaseFileWriter &writer, const deModelLOD& lodMesh, bool largeModel);
	void pSaveVertPosSets(decBaseFileWriter &writer, const deModelLOD& lodMesh, bool largeModel);
	void pSaveTriangles(decBaseFileWriter &writer, const deModelLOD &lodMesh, bool largeModel);
};

#endif
