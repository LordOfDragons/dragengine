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

#ifndef _FBXMODELMODULE_H_
#define _FBXMODELMODULE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/model/deBaseModelModule.h>
#include <dragengine/threading/deMutex.h>

class fbxScene;
class fbxNode;
class fbxProperty;
class fbxRig;
class fbxRigBone;
class fbxModel;

class deCacheHelper;
class deModelLOD;
class decPointerList;


/**
 * \brief FBX Model Module.
 */
class fbxModelModule : public deBaseModelModule{
private:
	deMutex pCacheMutex;
	deCacheHelper *pCacheTCSorter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	fbxModelModule(deLoadableModule &loadableModule);
	
	/** \brief Clean up module. */
	virtual ~fbxModelModule();
	/*@}*/
	
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Load model. */
	virtual void LoadModel(decBaseFileReader &reader, deModel &model);
	
	/** \brief Save model. */
	virtual void SaveModel(decBaseFileWriter &writer, const deModel &model);
	/*@}*/
	
	
	
private:
	void pLoadModel(deModel &model, fbxScene &scene);
	
	void pLoadModelBones(deModel &model, const fbxRig &rig);
	
	void pLoadModelBone(deModel &model, const fbxRigBone &rigBone);
	
	void pLoadModelTextures(deModel &model, const fbxModel &loadModel);
	
	void pLoadModelTexture(deModel &model, const fbxModel &loadModel, const fbxNode &nodeMaterial);
	
	void pLoadModelLod(deModel &model, deModelLOD &lod,
		const fbxModel &loadModel, const fbxRig *loadRig);
	
	void pLoadModelVertices(deModel &model, deModelLOD &lod,
		const fbxModel &loadModel, const fbxRig *loadRig);
	
	void pLoadModelFaces(deModel &model, deModelLOD &lod,
		const fbxModel &loadModel, const fbxRig *loadRig);
	
	void pEnsureTextureIndex(deModel &model, int count);
};

#endif
