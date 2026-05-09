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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "fbxSceneModule.h"
#include "../shared/fbxConnection.h"
#include "../shared/fbxNode.h"
#include "../shared/fbxProperty.h"
#include "../shared/fbxScene.h"
#include "../shared/managed/fbxAnimation.h"
#include "../shared/managed/fbxAnimationCurve.h"
#include "../shared/managed/fbxAnimationEvaluateCurve.h"
#include "../shared/managed/fbxAnimationMove.h"
#include "../shared/managed/fbxAnimationMoveCurves.h"
#include "../shared/managed/fbxMaterial.h"
#include "../shared/managed/fbxModel.h"
#include "../shared/managed/fbxModelCluster.h"
#include "../shared/managed/fbxRig.h"
#include "../shared/managed/fbxRigBone.h"
#include "../shared/managed/fbxTexture.h"
#include "../shared/property/fbxPropertyArrayDouble.h"
#include "../shared/property/fbxPropertyArrayFloat.h"
#include "../shared/property/fbxPropertyString.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationBone.h>
#include <dragengine/resources/animation/deAnimationBuilder.h>
#include <dragengine/resources/animation/deAnimationKeyframe.h>
#include <dragengine/resources/animation/deAnimationManager.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelBone.h>
#include <dragengine/resources/model/deModelBuilder.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelTextureCoordinatesSet.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelWeight.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRigBuilder.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/scene/deScene.h>
#include <dragengine/resources/scene/deSceneManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinBuilder.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/property/deSkinPropertyColor.h>
#include <dragengine/resources/skin/property/deSkinPropertyImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyValue.h>



// Definitions
////////////////

#define LOGSOURCE "FBXScene"



// Builder Classes
///////////////////

class cFbxModelBuilder : public deModelBuilder{
fbxSceneModule &pModule;
const fbxModel &pLoadModel;
const fbxRig *pLoadRig;

public:
cFbxModelBuilder(fbxSceneModule &module, const fbxModel &loadModel, const fbxRig *loadRig) :
pModule(module), pLoadModel(loadModel), pLoadRig(loadRig){}

void BuildModel(deModel *model) override{
pModule.pBuildModel(*model, pLoadModel, pLoadRig);
}
};

class cFbxSkinBuilder : public deSkinBuilder{
fbxSceneModule &pModule;
fbxScene &pFbxScene;
const fbxModel &pLoadModel;

public:
cFbxSkinBuilder(fbxSceneModule &module, fbxScene &fbxscene, const fbxModel &loadModel) :
pModule(module), pFbxScene(fbxscene), pLoadModel(loadModel){}

void BuildSkin(deSkin *skin) override{
pModule.pBuildSkin(*skin, pFbxScene, pLoadModel);
}
};

class cFbxRigBuilder : public deRigBuilder{
fbxSceneModule &pModule;
const fbxRig &pLoadRig;

public:
cFbxRigBuilder(fbxSceneModule &module, const fbxRig &loadRig) :
pModule(module), pLoadRig(loadRig){}

void BuildRig(deRig *rig) override{
pModule.pBuildRig(*rig, pLoadRig);
}
};

class cFbxAnimationBuilder : public deAnimationBuilder{
fbxSceneModule &pModule;
const fbxAnimationMove &pMove;
const fbxRig *pLoadRig;

public:
cFbxAnimationBuilder(fbxSceneModule &module, const fbxAnimationMove &move,
const fbxRig *loadRig) :
pModule(module), pMove(move), pLoadRig(loadRig){}

void BuildAnimation(deAnimation *animation) override{
pModule.pBuildAnimation(*animation, pMove, pLoadRig);
}
};



// Export definition
//////////////////////

#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *FBXSceneCreateModule(deLoadableModule *loadableModule);
#ifdef __cplusplus
}
#endif
#endif

deBaseModule *FBXSceneCreateModule(deLoadableModule *loadableModule){
try{
return new fbxSceneModule(*loadableModule);

}catch(const deException &){
return nullptr;
}
}



// Class fbxSceneModule
/////////////////////////

// Constructor, destructor
////////////////////////////

fbxSceneModule::fbxSceneModule(deLoadableModule &loadableModule) :
deBaseSceneModule(loadableModule){
}

fbxSceneModule::~fbxSceneModule(){
}



// Management
///////////////

void fbxSceneModule::LoadScene(decBaseFileReader &reader, deScene &scene){
try{
fbxScene fbxscene(reader);
fbxscene.Prepare(*this);

pLoadScene(scene, fbxscene);

}catch(const deException &){
LogErrorFormat("Failed reading scene '%s' at file position %d",
reader.GetFilename(), reader.GetPosition());
throw;
}
}

void fbxSceneModule::SaveScene(decBaseFileWriter &, const deScene &){
DETHROW_INFO(deeInvalidAction, "Saving not supported yet");
}



// Private Functions
//////////////////////

void fbxSceneModule::pLoadScene(deScene &scene, fbxScene &fbxscene){
decString deworldContent;
int objectId = 1;

pLoadObjects(scene, fbxscene, deworldContent, objectId);
pLoadRig(scene, fbxscene);
pLoadAnimations(scene, fbxscene);

pStoreDeworld(scene, deworldContent, objectId);
}

void fbxSceneModule::pLoadObjects(deScene &scene, fbxScene &fbxscene,
decString &deworldContent, int &objectId){
if(!fbxscene.GetNodeObjects()){
return;
}

const auto &children = fbxscene.GetNodeObjects()->GetNodes();
const int count = children.GetCount();

for(int i=0; i<count; i++){
fbxNode &node = *children.GetAt(i);

if(node.GetProperties().GetCount() < 3){
continue;
}

const decString &nodeName = node.GetName();
const decString &nodeType = node.GetProperties().GetAt(2)->CastString().GetValue();

if(nodeName == "Geometry" && nodeType == "Mesh"){
try{
pLoadModel(scene, fbxscene, node, deworldContent, objectId);
}catch(const deException &e){
LogWarnFormat("Failed loading geometry node: %s", e.GetDescription().GetString());
}

}else if(nodeName == "Model" && nodeType == "Light"){
try{
pLoadLight(fbxscene, node, deworldContent, objectId);
}catch(const deException &e){
LogWarnFormat("Failed loading light node: %s", e.GetDescription().GetString());
}
}
}
}

void fbxSceneModule::pLoadModel(deScene &scene, fbxScene &fbxscene, fbxNode &nodeGeometry,
decString &deworldContent, int &objectId){
const fbxModel::Ref loadModel(fbxModel::Ref::New(fbxscene, nodeGeometry));

// get model name from FBX Model node (properties[1] is "Name\0\1Model")
const decString modelName(
loadModel->GetNodeModel().GetProperties().GetAt(1)->CastString().GetValue());
const decString modelKey(decString("model/") + modelName);

// find rig if present
fbxNode * const nodePose = fbxscene.FirstNodeNamedOrNull("Pose");
fbxRig::Ref loadRig;
if(nodePose){
try{
loadRig = fbxRig::Ref::New(fbxscene, nodePose);
loadModel->MatchClusters(*loadRig);
loadModel->BuildWeights();
}catch(const deException &){
loadRig = nullptr;
}
}

// skip if model key already exists (duplicate geometry)
if(scene.HasResourceNamed(modelKey)){
return;
}

// create engine model using builder
cFbxModelBuilder modelBuilder(*this, *loadModel, loadRig);
const deModel::Ref model(GetGameEngine()->GetModelManager()->CreateModel("", modelBuilder));

scene.AddResource(modelKey, model);

// create skin for this model using builder
decString skinKey;
try{
cFbxSkinBuilder skinBuilder(*this, fbxscene, *loadModel);
const deSkin::Ref skin(GetGameEngine()->GetSkinManager()->CreateSkin("", skinBuilder));
if(skin->GetTextureCount() > 0){
skinKey = decString("skin/") + modelName;
if(!scene.HasResourceNamed(skinKey)){
scene.AddResource(skinKey, skin);
}
}
}catch(const deException &e){
LogWarnFormat("Failed creating skin for model '%s': %s",
modelName.GetString(), e.GetDescription().GetString());
}

// add entry to deworld content
const decMatrix &matrix = loadModel->GetMatrix();
const decVector position(matrix.GetPosition());
const decVector rotation(matrix.ToQuaternion().GetEulerAngles() * RAD2DEG);
const decVector scaling(matrix.GetScale());

char idBuf[32];
snprintf(idBuf, sizeof(idBuf), "%x", objectId++);

deworldContent += "\t<object id='";
deworldContent += idBuf;
deworldContent += "'>\n\t\t<classname>SimpleElement</classname>\n";
pAppendVector(deworldContent, "position", position);
pAppendVector(deworldContent, "rotation", rotation);
pAppendVector(deworldContent, "scaling", scaling);
deworldContent += "\t\t<property key='model'>scene://";
deworldContent += modelKey;
deworldContent += "</property>\n";
if(!skinKey.IsEmpty()){
deworldContent += "\t\t<property key='skin'>scene://";
deworldContent += skinKey;
deworldContent += "</property>\n";
}
deworldContent += "\t</object>\n";
}

void fbxSceneModule::pBuildModel(deModel &model, const fbxModel &loadModel,
const fbxRig *loadRig){
pBuildModelTextures(model, loadModel);
model.GetTextureCoordinatesSets().Add("default");

if(loadRig){
pBuildModelBones(model, *loadRig);
}

auto modelLod = deModelLOD::Ref::New();
pBuildModelLod(model, *modelLod, loadModel, loadRig);
model.AddLOD(std::move(modelLod));
}

void fbxSceneModule::pBuildModelBones(deModel &model, const fbxRig &rig){
rig.GetBones().Visit([&](const fbxRigBone &bone){
try{
auto b = deModelBone::Ref::New(bone.GetName());
b->SetPosition(bone.GetPosition());
b->SetOrientation(bone.GetOrientation());
if(bone.GetParent()){
b->SetParent(bone.GetParent()->GetIndex());
}
model.AddBone(std::move(b));
}catch(const deException &){
// skip
}
});
}

void fbxSceneModule::pBuildModelTextures(deModel &model, const fbxModel &loadModel){
const int64_t idModel = loadModel.GetModelID();
decTList<fbxConnection*> cons;
loadModel.GetScene().FindConnections(idModel, cons);

cons.Visit([&](fbxConnection *connection){
if(connection->GetTarget() != idModel){
return;
}

const fbxNode &node = *loadModel.GetScene().NodeWithID(connection->OtherID(idModel));
if(node.GetName() == "Material"){
pBuildModelTexture(model, loadModel, node);
}
});
}

void fbxSceneModule::pBuildModelTexture(deModel &model, const fbxModel &loadModel,
const fbxNode &nodeMaterial){
const decString &name = nodeMaterial.GetProperties().GetAt(1)->CastString().GetValue();

auto refTexture = deModelTexture::Ref::New(name, 1024, 1024);
refTexture->SetDecalOffset(model.GetTextureCount());
model.AddTexture(std::move(refTexture));
}

void fbxSceneModule::pBuildModelLod(deModel &model, deModelLOD &lod,
const fbxModel &loadModel, const fbxRig *loadRig){
pBuildModelVertices(model, lod, loadModel, loadRig);
pBuildModelFaces(model, lod, loadModel, loadRig);
}

void fbxSceneModule::pBuildModelVertices(deModel &model, deModelLOD &lod,
const fbxModel &loadModel, const fbxRig *loadRig){
// weight sets
const decTList<int> &wsWeights = loadModel.GetWeightSetWeights();
const decTList<int> &wsFirstWeight = loadModel.GetWeightSetsFirstWeight();
const decTList<int> &wgSetCount = loadModel.GetWeightGroupsSetCount();
const int wgCount = wgSetCount.GetCount();
int wsSetIndex = 0;
int i, j, k;

lod.GetWeights().EnlargeCapacity(wsWeights.GetCount());
lod.GetWeightGroups().EnlargeCapacity(wgCount);

for(i=0; i<wgCount; i++){
const int setCount = wgSetCount.GetAt(i);
const int wgWeightCount = i + 1;

lod.GetWeightGroups().Add(setCount);

for(j=0; j<setCount; j++){
const int firstWeight = wsFirstWeight.GetAt(wsSetIndex++);
for(k=0; k<wgWeightCount; k++){
lod.GetWeights().Add(loadModel.GetWeightAt(wsWeights.GetAt(firstWeight + k)));
}
}
}

// vertices
const int count = loadModel.GetVertexCount();

lod.GetVertices().EnlargeCapacity(count);
lod.SetNormalCount(count);
lod.SetTangentCount(count);

for(i=0; i<count; i++){
lod.GetVertices().Add({loadModel.GetVertexPositionAt(i), loadModel.GetVertexWeightSetAt(i)});
}
}

void fbxSceneModule::pBuildModelFaces(deModel &model, deModelLOD &lod,
const fbxModel &loadModel, const fbxRig *loadRig){
const fbxNode &nodeGeometry = loadModel.GetNodeGeometry();

const fbxNode * const nodePVI = nodeGeometry.FirstNodeNamedOrNull("PolygonVertexIndex");
if(!nodePVI || nodePVI->GetProperties().IsEmpty()){
return;
}

const fbxProperty &propPolygonVertexIndex = *nodePVI->GetProperties().First();

const fbxNode * const nodeLayerMaterials = nodeGeometry.FirstNodeNamedOrNull("LayerElementMaterial");
const fbxProperty * const propMaterials = nodeLayerMaterials ?
nodeLayerMaterials->FirstNodeNamed("Materials")->GetProperties().First().Pointer() : nullptr;
bool allSameMaterial = true;
if(nodeLayerMaterials){
const fbxScene::eMappingInformationType matMit =
fbxScene::ConvMappingInformationType(*nodeLayerMaterials);
if(matMit == fbxScene::emitByPolygon){
allSameMaterial = false;
}
}

const fbxNode * const nodeLayerNormals = nodeGeometry.FirstNodeNamedOrNull("LayerElementNormal");
(void)nodeLayerNormals;

const fbxNode * const nodeLayerUVs = nodeGeometry.FirstNodeNamedOrNull("LayerElementUV");
fbxScene::eMappingInformationType uvMit = fbxScene::emitByVertex;
fbxScene::eReferenceInformationType uvRit = fbxScene::eritDirect;
const fbxProperty *propUVIndex = nullptr;
const fbxProperty *propUV = nullptr;

if(nodeLayerUVs){
uvMit = fbxScene::ConvMappingInformationType(*nodeLayerUVs);
uvRit = fbxScene::ConvReferenceInformationType(*nodeLayerUVs);
propUV = nodeLayerUVs->FirstNodeNamed("UV")->GetProperties().GetAt(0);
if(uvRit == fbxScene::eritIndexToDirect){
propUVIndex = nodeLayerUVs->FirstNodeNamed("UVIndex")->GetProperties().GetAt(0);
}
}

const int polyVertIndexCount = propPolygonVertexIndex.GetValueCount();
int i;

// determine triangle count
int faceCount = 0;
for(i=2; i<polyVertIndexCount; i++){
faceCount++;
if(propPolygonVertexIndex.GetValueAtAsInt(i) < 0){
i += 2;
}
}

// texture coordinates
lod.GetTextureCoordinatesSets().Add({});
deModelTextureCoordinatesSet &tcs = lod.GetTextureCoordinatesSets().First();

if(propUV && (propUVIndex || uvMit == fbxScene::emitByVertex)){
const int count = propUV->GetValueCount() / 2;
lod.SetTextureCoordinatesCount(count);
tcs.GetTextureCoordinates().RemoveAll();
tcs.GetTextureCoordinates().EnlargeCapacity(count);
for(i=0; i<count; i++){
tcs.GetTextureCoordinates().Add(fbxScene::ConvUVFbxToDe(propUV->GetValueAtAsVector2(i)));
}
}else{
const int count = faceCount * 3;
lod.SetTextureCoordinatesCount(count);
tcs.GetTextureCoordinates().RemoveAll();
tcs.GetTextureCoordinates().EnlargeCapacity(count);
for(i=0; i<count; i++){
tcs.GetTextureCoordinates().Add({});
}
}

// build faces
int faceIndex0 = propPolygonVertexIndex.GetValueAtAsInt(0);
int faceIndexLast = propPolygonVertexIndex.GetValueAtAsInt(1);
int texCoordIndex = 0;
int polygonIndex = 0;

decVector2 faceUV0, faceUVLast, uv;
int faceUVIndex0 = 0, faceUVIndexLast = 0, uvIndex = 0;
if(propUV && uvMit == fbxScene::emitByPolygonVertex){
faceUV0 = fbxScene::ConvUVFbxToDe(propUV->GetValueAtAsVector2(0));
faceUVLast = fbxScene::ConvUVFbxToDe(propUV->GetValueAtAsVector2(1));
}

lod.GetFaces().EnlargeCapacity(faceCount);

for(i=2; i<polyVertIndexCount; i++){
int texture = allSameMaterial ? 0 : propMaterials->GetValueAtAsInt(polygonIndex);
pEnsureTextureIndex(model, texture);

int index = propPolygonVertexIndex.GetValueAtAsInt(i);

if(propUV){
if(propUVIndex){
uvIndex = propUVIndex->GetValueAtAsInt(i);
}else{
if(uvMit == fbxScene::emitByPolygonVertex){
uv = fbxScene::ConvUVFbxToDe(propUV->GetValueAtAsVector2(i));
}
uvIndex = index;
if(uvIndex < 0){
uvIndex ^= -1;
}
}
}

bool initNextPolygon = false;
if(index < 0){
index ^= -1;
initNextPolygon = i + 2 < polyVertIndexCount;
polygonIndex++;
}

lod.GetFaces().Add({});
deModelFace &face = lod.GetFaces().Last();

face.SetVertex1(index);
face.SetVertex2(faceIndexLast);
face.SetVertex3(faceIndex0);
face.SetNormal1(index);
face.SetNormal2(faceIndexLast);
face.SetNormal3(faceIndex0);
face.SetTangent1(index);
face.SetTangent2(faceIndexLast);
face.SetTangent3(faceIndex0);
face.SetTexture(texture);

if(propUV){
if(uvMit == fbxScene::emitByPolygonVertex && uvRit == fbxScene::eritDirect){
tcs.GetTextureCoordinates().SetAt(texCoordIndex, uv);
face.SetTextureCoordinates1(texCoordIndex++);
tcs.GetTextureCoordinates().SetAt(texCoordIndex, faceUVLast);
face.SetTextureCoordinates2(texCoordIndex++);
tcs.GetTextureCoordinates().SetAt(texCoordIndex, faceUV0);
face.SetTextureCoordinates3(texCoordIndex++);
faceUVLast = uv;
}else{
face.SetTextureCoordinates1(uvIndex);
face.SetTextureCoordinates2(faceUVIndexLast);
face.SetTextureCoordinates3(faceUVIndex0);
}
}else{
face.SetTextureCoordinates1(texCoordIndex++);
face.SetTextureCoordinates2(texCoordIndex++);
face.SetTextureCoordinates3(texCoordIndex++);
}

faceIndexLast = index;
faceUVIndexLast = uvIndex;

if(initNextPolygon){
faceIndex0 = propPolygonVertexIndex.GetValueAtAsInt(i + 1);
faceIndexLast = propPolygonVertexIndex.GetValueAtAsInt(i + 2);

if(propUV){
if(propUVIndex){
faceUVIndex0 = propUVIndex->GetValueAtAsInt(i + 1);
faceUVIndexLast = propUVIndex->GetValueAtAsInt(i + 2);
}else{
if(uvMit == fbxScene::emitByPolygonVertex){
faceUV0 = fbxScene::ConvUVFbxToDe(propUV->GetValueAtAsVector2(i + 1));
faceUVLast = fbxScene::ConvUVFbxToDe(propUV->GetValueAtAsVector2(i + 2));
}
faceUVIndex0 = faceIndex0;
faceUVIndexLast = faceIndexLast;
}
}

i += 2;
}
}
}

void fbxSceneModule::pEnsureTextureIndex(deModel &model, int count){
while(count >= model.GetTextureCount()){
decString baseName("material");
decString name(baseName);
int number = 1;

while(model.HasTextureNamed(name)){
name.Format("%s%02d", baseName.GetString(), number++);
}

auto refTexture = deModelTexture::Ref::New(name, 1024, 1024);
refTexture->SetDecalOffset(model.GetTextureCount());
model.AddTexture(std::move(refTexture));
}
}

void fbxSceneModule::pBuildSkin(deSkin &skin, fbxScene &fbxscene,
const fbxModel &loadModel){
const int64_t idModel = loadModel.GetModelID();
decTList<fbxConnection*> cons;
fbxscene.FindConnections(idModel, cons);

cons.Visit([&](fbxConnection *connection){
if(connection->GetTarget() != idModel){
return;
}

fbxNode &node = *fbxscene.NodeWithID(connection->OtherID(idModel));
if(node.GetName() != "Material"){
return;
}

try{
const fbxMaterial::Ref mat(fbxMaterial::Ref::New(fbxscene, node));
pBuildSkinMaterial(skin, fbxscene, *mat);
}catch(const deException &e){
LogWarnFormat("Failed creating skin material: %s", e.GetDescription().GetString());
}
});
}

void fbxSceneModule::pBuildSkinMaterial(deSkin &skin, fbxScene &fbxscene,
const fbxMaterial &material){
auto texture = deSkinTexture::Ref::New(material.GetName());

const fbxNode &nodeMaterial = material.GetNodeMaterial();
const decColor black(0.0f, 0.0f, 0.0f);

const decColor fbxColor((nodeMaterial.GetPropertyColor("DiffuseColor", black)
* nodeMaterial.GetPropertyFloat("DiffuseFactor", 1.0f)).Normalized());

const decColor fbxSpecular((nodeMaterial.GetPropertyColor("SpecularColor", black)
* nodeMaterial.GetPropertyFloat("SpecularFactor", 1.0f)).Normalized());

const float fbxShininess = decMath::clamp(
nodeMaterial.GetPropertyFloat("Shininess", 0.0f), 0.0f, 64.0f);

const float roughness = powf(1.0f - (fbxShininess / 64.0f), 12.0f);

decColor color;
color.r = fbxColor.r * (1.0f - fbxSpecular.r);
color.g = fbxColor.g * (1.0f - fbxSpecular.g);
color.b = fbxColor.b * (1.0f - fbxSpecular.b);

// find textures connected to material
const int64_t idMaterial = material.GetMaterialID();
decTList<fbxConnection*> texConns;
fbxscene.FindConnections(idMaterial, texConns);

fbxTexture::Ref fbxTexDiffuse;

texConns.Visit([&](const fbxConnection *connection){
if(connection->GetTarget() != idMaterial){
return;
}

fbxNode &node = *fbxscene.NodeWithID(connection->OtherID(idMaterial));
if(node.GetName() != "Texture"){
return;
}

const decString &mpname = connection->GetProperty();
if(mpname == "DiffuseColor" && !fbxTexDiffuse){
fbxTexDiffuse = fbxTexture::Ref::New(fbxscene, node);
}
});

// add texture properties
if(fbxTexDiffuse){
auto propColor = deSkinPropertyImage::Ref::New("color");
propColor->SetPath(fbxTexDiffuse->GetFilename());
texture->AddProperty(std::move(propColor));
}else{
auto propColor = deSkinPropertyColor::Ref::New("color");
propColor->SetColor(color);
texture->AddProperty(std::move(propColor));
}

auto propReflectivity = deSkinPropertyColor::Ref::New("reflectivity");
propReflectivity->SetColor(fbxSpecular);
texture->AddProperty(std::move(propReflectivity));

auto propRoughness = deSkinPropertyValue::Ref::New("roughness");
propRoughness->SetValue(roughness);
texture->AddProperty(std::move(propRoughness));

skin.AddTexture(std::move(texture));
}

void fbxSceneModule::pLoadRig(deScene &scene, fbxScene &fbxscene){
fbxNode * const nodePose = fbxscene.FirstNodeNamedOrNull("Pose");
if(!nodePose){
return;
}

try{
const fbxRig::Ref loadRig(fbxRig::Ref::New(fbxscene, nodePose));

cFbxRigBuilder rigBuilder(*this, *loadRig);
const deRig::Ref rig(GetGameEngine()->GetRigManager()->CreateRig("", rigBuilder));

if(rig->GetBoneCount() > 0){
scene.AddResource("rig/skeleton", rig);
}

}catch(const deException &e){
LogWarnFormat("Failed loading rig: %s", e.GetDescription().GetString());
}
}

void fbxSceneModule::pBuildRig(deRig &rig, const fbxRig &loadRig){
loadRig.GetBones().Visit([&](const fbxRigBone &b){
try{
auto rigBone = deRigBone::Ref::New(b.GetName());
rigBone->SetPosition(b.GetPosition());
rigBone->SetRotation(b.GetOrientation().GetEulerAngles());
if(b.GetParent()){
rigBone->SetParent(b.GetParent()->GetIndex());
}
rig.AddBone(std::move(rigBone));
}catch(const deException &e){
LogWarnFormat("Failed loading rig bone: %s", e.GetDescription().GetString());
}
});
}

void fbxSceneModule::pLoadAnimations(deScene &scene, fbxScene &fbxscene){
fbxNode * const nodePose = fbxscene.FirstNodeNamedOrNull("Pose");

try{
fbxRig::Ref loadRig;
if(nodePose){
loadRig = fbxRig::Ref::New(fbxscene, nodePose);
}

const fbxAnimation::Ref loadAnimation(fbxAnimation::Ref::New(fbxscene));
if(loadRig){
loadAnimation->MatchRig(*loadRig);
}

loadAnimation->GetMoves().Visit([&](const fbxAnimationMove &move){
try{
cFbxAnimationBuilder animBuilder(*this, move, loadRig);
const deAnimation::Ref animation(
GetGameEngine()->GetAnimationManager()->CreateAnimation("", animBuilder));

const decString animKey(decString("animation/") + move.GetName());
if(!scene.HasResourceNamed(animKey)){
scene.AddResource(animKey, animation);
}

}catch(const deException &e){
LogWarnFormat("Failed loading animation move '%s': %s",
move.GetName().GetString(), e.GetDescription().GetString());
}
});

}catch(const deException &e){
LogWarnFormat("Failed loading animations: %s", e.GetDescription().GetString());
}
}

void fbxSceneModule::pBuildAnimation(deAnimation &animation, const fbxAnimationMove &move,
const fbxRig *loadRig){
if(loadRig){
loadRig->GetBones().Visit([&](const fbxRigBone &rb){
auto bone = deAnimationBone::Ref::New();
bone->SetName(rb.GetName());
animation.AddBone(std::move(bone));
});
}

auto animMove = deAnimationMove::Ref::New();
animMove->SetName(move.GetName());

const int boneCount = animation.GetBoneCount();
for(int i=0; i<boneCount; i++){
animMove->AddKeyframeList(deAnimationMove::KeyframeListRef::New());
}

int playtime = 0;

move.GetCurveNodes().Visit([&](const fbxAnimationMoveCurves &c){
if(!c.GetRigBone() || c.GetTargetProperty() == fbxAnimationMoveCurves::etpUnsupported){
return;
}

const fbxAnimationCurve * const curves[3] = {
c.GetCurveX(), c.GetCurveY(), c.GetCurveZ()};
if(!curves[0] && !curves[1] && !curves[2]){
return;
}

int lastTime = 0;
for(int j=0; j<3; j++){
if(!curves[j]){
continue;
}
const int timeCount = curves[j]->GetPropertyTime().GetValueCount();
if(timeCount > 0){
lastTime = decMath::max(lastTime, move.TimeToFrame(
fbxAnimation::ConvTime(
curves[j]->GetPropertyTime().GetValueAtAsLong(timeCount - 1))));
}
}

deAnimationKeyframe::List &kflist =
animMove->GetKeyframeList(c.GetRigBone()->GetIndex());
for(int j=0; j<=lastTime; j++){
kflist.Add({});
}

playtime = decMath::max(playtime, lastTime);

const decMatrix &animMatrix = c.GetRigBone()->GetAnimMatrix();
const fbxScene::eRotationOrder rotationOrder = c.GetRigBone()->GetRotationOrder();

fbxAnimationEvaluateCurve evaluators[3] = {
{move, curves[0], c.GetDefaultValue().x},
{move, curves[1], c.GetDefaultValue().y},
{move, curves[2], c.GetDefaultValue().z}};

switch(c.GetTargetProperty()){
case fbxAnimationMoveCurves::etpPosition:
for(int k=0; k<=lastTime; k++){
deAnimationKeyframe &kf = kflist.GetAt(k);
kf.SetTime(move.FrameToTime(k));
kf.SetPosition(animMatrix.Transform(evaluators[0].NextValue(),
evaluators[1].NextValue(), evaluators[2].NextValue()));
}
break;

case fbxAnimationMoveCurves::etpRotation:
for(int k=0; k<=lastTime; k++){
deAnimationKeyframe &kf = kflist.GetAt(k);
kf.SetTime(move.FrameToTime(k));
kf.SetRotation(
fbxScene::CreateRotationMatrix(
decVector(evaluators[0].NextValue(),
evaluators[1].NextValue(),
evaluators[2].NextValue()) * DEG2RAD,
rotationOrder)
.QuickMultiply(animMatrix)
.Normalized()
.GetEulerAngles());
}
break;

case fbxAnimationMoveCurves::etpScale:
for(int k=0; k<=lastTime; k++){
deAnimationKeyframe &kf = kflist.GetAt(k);
kf.SetTime(move.FrameToTime(k));
kf.SetScale(
decMatrix::CreateScale(evaluators[0].NextValue(),
evaluators[1].NextValue(), evaluators[2].NextValue())
.QuickMultiply(animMatrix)
.GetScale());
}
break;

default:
break;
}
});

animMove->SetPlaytime(move.FrameToTime(playtime));
animation.AddMove(std::move(animMove));
}

void fbxSceneModule::pLoadLight(fbxScene &fbxscene, fbxNode &modelNode,
decString &deworldContent, int &objectId){
// get light transform
const decMatrix matrix(modelNode.GetTransformation().QuickMultiply(
fbxscene.GetTransformation()));
const decVector position(matrix.GetPosition());
const decVector rotation(matrix.ToQuaternion().GetEulerAngles() * RAD2DEG);

// light properties
float intensity = 1.0f;
float range = 10.0f;

modelNode.GetPropertyFloatIf("Intensity", intensity);
intensity *= 0.01f; // FBX intensity is in percent

// color
decColor color(1.0f, 1.0f, 1.0f);
modelNode.GetPropertyColorIf("Color", color);

// write deworld entry
char idBuf[32];
snprintf(idBuf, sizeof(idBuf), "%x", objectId++);

deworldContent += "\t<object id='";
deworldContent += idBuf;
deworldContent += "'>\n\t\t<classname>BaseLight</classname>\n";
pAppendVector(deworldContent, "position", position);
pAppendVector(deworldContent, "rotation", rotation);

char valueBuf[64];
snprintf(valueBuf, sizeof(valueBuf), "%g", intensity);
deworldContent += "\t\t<property key='light.intensity'>";
deworldContent += valueBuf;
deworldContent += "</property>\n";

snprintf(valueBuf, sizeof(valueBuf), "%g", range * 0.5f);
deworldContent += "\t\t<property key='light.halfIntensityDistance'>";
deworldContent += valueBuf;
deworldContent += "</property>\n";

snprintf(valueBuf, sizeof(valueBuf), "%g", range);
deworldContent += "\t\t<property key='light.range'>";
deworldContent += valueBuf;
deworldContent += "</property>\n";

deworldContent += "\t</object>\n";
}

void fbxSceneModule::pStoreDeworld(deScene &scene, const decString &deworldContent,
int objectCount){
char nextIdBuf[64];
snprintf(nextIdBuf, sizeof(nextIdBuf), "\t<nextObjectID>%x</nextObjectID>\n", objectCount);

decString xml("<?xml version='1.0' encoding='UTF-8'?>\n<world>\n"
"\t<size x='500' y='500' z='500'/>\n"
"\t<gravity x='0' y='-9.81' z='0'/>\n");
xml += nextIdBuf;
xml += deworldContent;
xml += "</world>\n";

const decMemoryFile::Ref memFile(decMemoryFile::Ref::New("scene.deworld"));
const decMemoryFileWriter::Ref writer(decMemoryFileWriter::Ref::New(memFile, false));
writer->Write(xml.GetString(), xml.GetLength());

scene.AddFile("scene.deworld", memFile);
}

void fbxSceneModule::pAppendXmlFloat(decString &xml, float value) const{
char buf[64];
snprintf(buf, sizeof(buf), "%g", value);
xml += buf;
}

void fbxSceneModule::pAppendVector(decString &xml, const char *tag, const decVector &v) const{
xml += "\t\t<";
xml += tag;
xml += " x='";
pAppendXmlFloat(xml, v.x);
xml += "' y='";
pAppendXmlFloat(xml, v.y);
xml += "' z='";
pAppendXmlFloat(xml, v.z);
xml += "'/>\n";
}



#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class fbxSceneModuleInternal : public deInternalModule{
public:
using Ref = deTObjectReference<fbxSceneModuleInternal>;

fbxSceneModuleInternal(deModuleSystem *system) : deInternalModule(system){
SetName("FBXScene");
SetDescription("Handles scenes in the binary FBX format.");
SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
SetVersion(MODULE_VERSION);
SetType(deModuleSystem::emtScene);
SetDirectoryName("fbxscene");
GetPatternList().Add(".fbx");
SetDefaultExtension(".fbx");
SetPriority(1);
SetNoSaving(true);
SetDefaultLoggingName();
}

void CreateModule() override{
SetModule(FBXSceneCreateModule(this));
if(!GetModule()){
SetErrorCode(eecCreateModuleFailed);
}
}
};

deTObjectReference<deInternalModule> fbxSceneRegisterInternalModule(deModuleSystem *system){
return fbxSceneModuleInternal::Ref::New(system);
}
#endif
