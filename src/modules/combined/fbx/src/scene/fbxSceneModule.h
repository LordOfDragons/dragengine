/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _FBXSCENEMODULE_H_
#define _FBXSCENEMODULE_H_

#include "../shared/fbxScene.h"
#include <dragengine/systems/modules/scene/deBaseSceneModule.h>

class fbxScene;
class fbxModel;
class fbxRig;
class fbxMaterial;
class fbxNode;
class fbxAnimationMove;
class deModel;
class deModelLOD;
class deSkin;
class deRig;
class deAnimation;

class cFbxModelBuilder;
class cFbxSkinBuilder;
class cFbxRigBuilder;
class cFbxAnimationBuilder;


/**
 * \brief FBX Scene Module.
 *
 * Loads FBX files as scene resources. Extracts all geometry objects as model resources,
 * materials as skin resources, skeletons as rig resources and animations as animation
 * resources. Also creates a deworld memory file containing object placement information.
 */
class fbxSceneModule : public deBaseSceneModule{
public:
/** \name Constructors and Destructors */
/*@{*/
/** \brief Create module. */
fbxSceneModule(deLoadableModule &loadableModule);

/** \brief Clean up module. */
~fbxSceneModule() override;
/*@}*/



/** \name Management */
/*@{*/
/** \brief Load scene from file. */
void LoadScene(decBaseFileReader &reader, deScene &scene) override;

/** \brief Save scene to file. */
void SaveScene(decBaseFileWriter &writer, const deScene &scene) override;
/*@}*/



private:
friend class cFbxModelBuilder;
friend class cFbxSkinBuilder;
friend class cFbxRigBuilder;
friend class cFbxAnimationBuilder;

void pLoadScene(deScene &scene, fbxScene &fbxscene);

void pLoadObjects(deScene &scene, fbxScene &fbxscene,
decString &deworldContent, int &objectId);

void pLoadModel(deScene &scene, fbxScene &fbxscene, fbxNode &nodeGeometry,
decString &deworldContent, int &objectId);

void pBuildModel(deModel &model, const fbxModel &loadModel, const fbxRig *loadRig);

void pBuildModelBones(deModel &model, const fbxRig &rig);

void pBuildModelTextures(deModel &model, const fbxModel &loadModel);

void pBuildModelTexture(deModel &model, const fbxModel &loadModel,
const fbxNode &nodeMaterial);

void pBuildModelLod(deModel &model, deModelLOD &lod, const fbxModel &loadModel,
const fbxRig *loadRig);

void pBuildModelVertices(deModel &model, deModelLOD &lod, const fbxModel &loadModel,
const fbxRig *loadRig);

void pBuildModelFaces(deModel &model, deModelLOD &lod, const fbxModel &loadModel,
const fbxRig *loadRig);

void pEnsureTextureIndex(deModel &model, int count);

void pBuildSkin(deSkin &skin, fbxScene &fbxscene, const fbxModel &loadModel);

void pBuildSkinMaterial(deSkin &skin, fbxScene &fbxscene, const fbxMaterial &material);

void pLoadRig(deScene &scene, fbxScene &fbxscene);

void pBuildRig(deRig &rig, const fbxRig &loadRig);

void pLoadAnimations(deScene &scene, fbxScene &fbxscene);

void pBuildAnimation(deAnimation &animation, const fbxAnimationMove &move,
const fbxRig *loadRig);

void pLoadLight(fbxScene &fbxscene, fbxNode &modelNode,
decString &deworldContent, int &objectId);

void pStoreDeworld(deScene &scene, const decString &deworldContent, int objectCount);

void pAppendXmlFloat(decString &xml, float value) const;

void pAppendVector(decString &xml, const char *tag, const decVector &v) const;
};

#endif
