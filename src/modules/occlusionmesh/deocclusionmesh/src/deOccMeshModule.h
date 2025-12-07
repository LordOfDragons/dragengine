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
	deOccMeshModule(deLoadableModule &loadableModule);
	
	/** \brief Clean up module. */
	virtual ~deOccMeshModule();
	/*@}*/
	
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Load occlusion mesh. */
	virtual void LoadOcclusionMesh(decBaseFileReader &reader, deOcclusionMesh &occlusionMesh);
	
	/** \brief Save occlusion mesh. */
	virtual void SaveOcclusionMesh(decBaseFileWriter &writer, const deOcclusionMesh &occlusionMesh);
	/*@}*/
	
private:
	void pLoadMesh(decBaseFileReader &reader, deOcclusionMesh &mesh);
	void pLoadBones(decBaseFileReader &reader, deOcclusionMesh &mesh, sMeshInfos &infos);
	void pLoadWeights(decBaseFileReader &reader, deOcclusionMesh &mesh, sMeshInfos &infos);
	void pLoadVertices(decBaseFileReader &reader, deOcclusionMesh &mesh, sMeshInfos &infos);
	void pLoadFaces(decBaseFileReader &reader, deOcclusionMesh &mesh, sMeshInfos &infos);
};

#endif
