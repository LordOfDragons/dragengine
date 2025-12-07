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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deRigModule.h"
#include "dermNameList.h"
#include "dermWriteShape.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRigConstraint.h>



// Export definition
//////////////////////

#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *DERigCreateModule(deLoadableModule *loadableModule);
#ifdef  __cplusplus
}
#endif
#endif


// Entry function
///////////////////

deBaseModule *DERigCreateModule(deLoadableModule *loadableModule){
	deBaseModule *module = NULL;
	
	try{
		module = new deRigModule(*loadableModule);
		
	}catch(const deException &){
		return NULL;
	}
	
	return module;
}



// Class deRigModule
//////////////////////

// Constructor, destructor
////////////////////////////

deRigModule::deRigModule(deLoadableModule &loadableModule) :
deBaseRigModule(loadableModule){
}

deRigModule::~deRigModule(){
}



// Loading and saving
///////////////////////

void deRigModule::LoadRig(decBaseFileReader &file, deRig &rig){
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser(GetGameEngine()->GetLogger()).ParseXml(&file, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(! root || strcmp(root->GetName(), "rig") != 0){
		DETHROW(deeInvalidParam);
	}
	
	pParseRig(root, rig);
}

void deRigModule::SaveRig(decBaseFileWriter &file, const deRig &rig){
	decXmlWriter writer(&file);
	writer.WriteXMLDeclaration();
	pWriteRig(writer, rig);
}



// Private functions
//////////////////////

decXmlElementTag *deRigModule::pGetTagAt(decXmlElementTag *tag, int index){
	decXmlElement *element = tag->GetElementAt(index);
	
	if(element->CanCastToElementTag()){
		return element->CastToElementTag();
		
	}else{
		return NULL;
	}
}

decXmlElementTag *deRigModule::pGetTagAt(const decXmlElementTag &tag, int index){
	decXmlElement * const element = tag.GetElementAt(index);
	
	if(element->CanCastToElementTag()){
		return element->CastToElementTag();
		
	}else{
		return NULL;
	}
}

decXmlAttValue *deRigModule::pFindAttribute(decXmlElementTag *tag, const char *name){
	decXmlAttValue *value;
	decXmlElement *element;
	int i;
	
	for(i=0; i<tag->GetElementCount(); i++){
		element = tag->GetElementAt(i);
		if(element->CanCastToAttValue()){
			value = element->CastToAttValue();
			if(strcmp(value->GetName(), name) == 0){
				return value;
			}
		}
	}
	
	return NULL;
}

const char *deRigModule::pGetAttributeString(decXmlElementTag *tag, const char *name){
	decXmlAttValue *value = pFindAttribute(tag, name);
	
	if(value){
		return value->GetValue();
		
	}else{
		LogErrorFormat("Missing Attribute '%s' in tag '%s'\n", name, tag->GetName().GetString());
		DETHROW(deeInvalidParam);
	}
}

int deRigModule::pGetAttributeInt(decXmlElementTag *tag, const char *name){
	decXmlAttValue *value = pFindAttribute(tag, name);
	
	if(value){
		return (int)strtol(value->GetValue(), NULL, 10);
		
	}else{
		LogErrorFormat("Missing Attribute '%s' in tag '%s'\n", name, tag->GetName().GetString());
		DETHROW(deeInvalidParam);
	}
}

float deRigModule::pGetAttributeFloat(decXmlElementTag *tag, const char *name){
	decXmlAttValue *value = pFindAttribute(tag, name);
	
	if(value){
		return strtof(value->GetValue(), NULL);
		
	}else{
		LogErrorFormat("Missing Attribute '%s' in tag '%s'\n", name, tag->GetName().GetString());
		DETHROW(deeInvalidParam);
	}
}



void deRigModule::pParseRig(decXmlElementTag *root, deRig &rig){
	const char *rootBone = NULL;
	decXmlCharacterData *cdata;
	dermNameList boneNameList;
	decXmlElementTag *tag;
	int c, constraintCount;
	int b, boneCount;
	decVector vector;
	int i;
	
	decShapeList shapes;
	decStringList shapeProperties;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = pGetTagAt(root, i);
		if(tag){
			if(strcmp(tag->GetName(), "bone") == 0){
				pParseBone(tag, rig, boneNameList);
				
			}else if(strcmp(tag->GetName(), "sphere") == 0){
				pParseSphere(tag, shapes, shapeProperties);
				
			}else if(strcmp(tag->GetName(), "cylinder") == 0){
				pParseCylinder(tag, shapes, shapeProperties);
				
			}else if(strcmp(tag->GetName(), "capsule") == 0){
				pParseCapsule(tag, shapes, shapeProperties);
				
			}else if(strcmp(tag->GetName(), "box") == 0){
				pParseBox(tag, shapes, shapeProperties);
				
			}else if(strcmp(tag->GetName(), "hull") == 0){
				pParseHull(tag, shapes, shapeProperties);
				
			}else if(strcmp(tag->GetName(), "rootBone") == 0){
				cdata = tag->GetFirstData();
				if(cdata){
					rootBone = cdata->GetData();
				}
				
			}else if(strcmp(tag->GetName(), "modelCollision") == 0){
				cdata = tag->GetFirstData();
				
				if(cdata){
					if(strcmp(cdata->GetData(), "true") == 0){
						rig.SetModelCollision(true);
						
					}else if(strcmp(cdata->GetData(), "false") == 0){
						rig.SetModelCollision(false);
						
					}else{
						LogErrorFormat("Unknown value '%s' in tag '%s'\n",
							cdata->GetData().GetString(), tag->GetName().GetString());
					}
					
				}else{
					LogErrorFormat("Unknown value '' in tag '%s'\n",
						tag->GetName().GetString());
				}
				
			}else if(strcmp(tag->GetName(), "centralMassPoint") == 0){
				vector.Set(0.0f, 0.0f, 0.0f);
				pParseVector(tag, vector);
				rig.SetCentralMassPoint(vector);
			}
		}
	}
	
	rig.SetShapes(shapes);
	rig.SetShapeProperties(shapeProperties);
	
	boneCount = boneNameList.GetNameCount();
	if(boneCount > 0){
		for(b=0; b<boneCount; b++){
			boneNameList.SetNameNumberAt(b, rig.IndexOfBoneNamed(boneNameList.GetNameAt(b)));
		}
		
		boneCount = rig.GetBoneCount();
		for(b=0; b<boneCount; b++){
			deRigBone &bone = rig.GetBoneAt(b);
			constraintCount = bone.GetConstraintCount();
			
			for(c=0; c<constraintCount; c++){
				deRigConstraint &constraint = bone.GetConstraintAt(c);
				
				if(constraint.GetParentBone() != -1){
					constraint.SetParentBone(boneNameList.GetNameNumberAt(constraint.GetParentBone()));
				}
			}
		}
	}
	
	if(rootBone){
		rig.SetRootBone(rig.IndexOfBoneNamed(rootBone));
		
	}else{
		for(b=0; b<boneCount; b++){
			if(rig.GetBoneAt(b).GetParent() == -1){
				rig.SetRootBone(b);
				break;
			}
		}
	}
}

void deRigModule::pParseBone(decXmlElementTag *root, deRig &rig, dermNameList &boneNameList){
	decVector ikLimitsLower(TWO_PI, TWO_PI, TWO_PI);
	decVector ikLimitsUpper(0.0f, 0.0f, 0.0f);
	decVector ikResistance(0.0f, 0.0f, 0.0f);
	bool ikLocked[3] = {false, false, false};
	decXmlCharacterData *cdata;
	const char *name = NULL;
	deRigBone *bone = NULL;
	decXmlElementTag *tag;
	decVector vector;
	int i;
	
	decShapeList shapes;
	decStringList shapeProperties;
	
//	if( pFindAttribute( root, "name" ) ){
		name = pGetAttributeString(root, "name");
		
/*	}else{ // DEPRECATED
		for(i=0; i<root->GetElementCount(); i++){
			tag = pGetTagAt(root, i);
			if(tag){
				if(strcmp(tag->GetName(), "name") == 0){
					name = tag->GetFirstData()->GetData();
				}
			}
		}
	}*/
	
	if(! name){
		LogWarn("Bone missing name, ignoring.\n");
		return;
	}
	if(rig.HasBoneNamed(name)){
		LogWarnFormat("Duplicate bone '%s', ignoring.\n", name);
		return;
	}
	
	try{
		bone = new deRigBone(name);
		if(! bone) DETHROW(deeOutOfMemory);
		
		for(i=0; i<root->GetElementCount(); i++){
			tag = pGetTagAt(root, i);
			if(tag){
				/*if( strcmp( tag->GetName(), "name" ) == 0 ){ // DEPRECATED
					// done already
					
				}else */if(strcmp(tag->GetName(), "parent") == 0){
					cdata = tag->GetFirstData();
					if(cdata){
						bone->SetParent(rig.IndexOfBoneNamed(cdata->GetData()));
						
						if(bone->GetParent() == -1){
							LogErrorFormat("Parent '%s' not found for bone '%s'!\n",
								cdata->GetData().GetString(), name);
						}
						
					}else{
						bone->SetParent(-1);
					}
					
				}else if(strcmp(tag->GetName(), "position") == 0){
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					bone->SetPosition(vector);
					
				}else if(strcmp(tag->GetName(), "rotation") == 0){
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					bone->SetRotation(vector * DEG2RAD);
					
				}else if(strcmp(tag->GetName(), "mass") == 0){
					cdata = tag->GetFirstData();
					if(cdata){
						bone->SetMass(strtof(cdata->GetData(), NULL));
						
					}else{
						bone->SetMass(0.0f);
					}
					
				}else if(strcmp(tag->GetName(), "centralMassPoint") == 0){
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					bone->SetCentralMassPoint(vector);
					
				}else if(strcmp(tag->GetName(), "dynamic") == 0){
					cdata = tag->GetFirstData();
					
					if(cdata){
						if(strcmp(cdata->GetData(), "true") == 0){
							bone->SetDynamic(true);
							
						}else if(strcmp(cdata->GetData(), "false") == 0){
							bone->SetDynamic(false);
							
						}else{
							LogErrorFormat("Unknown value '%s' in tag '%s'\n",
								cdata->GetData().GetString(), tag->GetName().GetString());
						}
						
					}else{
						LogErrorFormat("Unknown value '' in tag '%s'\n",
							tag->GetName().GetString());
					}
					
				}else if(strcmp(tag->GetName(), "ikX") == 0){
					pParseBoneIK(tag, ikLimitsLower.x, ikLimitsUpper.x, ikResistance.x, ikLocked[0]);
					
				}else if(strcmp(tag->GetName(), "ikY") == 0){
					pParseBoneIK(tag, ikLimitsLower.y, ikLimitsUpper.y, ikResistance.y, ikLocked[1]);
					
				}else if(strcmp(tag->GetName(), "ikZ") == 0){
					pParseBoneIK(tag, ikLimitsLower.z, ikLimitsUpper.z, ikResistance.z, ikLocked[2]);
					
				}else if(strcmp(tag->GetName(), "sphere") == 0){
					pParseSphere(tag, shapes, shapeProperties);
					
				}else if(strcmp(tag->GetName(), "cylinder") == 0){
					pParseCylinder(tag, shapes, shapeProperties);
					
				}else if(strcmp(tag->GetName(), "capsule") == 0){
					pParseCapsule(tag, shapes, shapeProperties);
					
				}else if(strcmp(tag->GetName(), "box") == 0){
					pParseBox(tag, shapes, shapeProperties);
					
				}else if(strcmp(tag->GetName(), "constraint") == 0){
					pParseConstraint(tag, rig, bone, boneNameList);
				}
			}
		}
		
		bone->SetIKLimits(ikLimitsLower, ikLimitsUpper);
		bone->SetIKResistance(ikResistance);
		bone->SetIKLockedX(ikLocked[0]);
		bone->SetIKLockedY(ikLocked[1]);
		bone->SetIKLockedZ(ikLocked[2]);
		
		rig.AddBone(bone);
		
	}catch(const deException &){
		if(bone) delete bone;
		throw;
	}
	
	bone->SetShapes(shapes);
	bone->SetShapeProperties(shapeProperties);
}

void deRigModule::pParseBoneIK(decXmlElementTag *root, float &lower, float &upper, float &resistance, bool &locked){
	decXmlCharacterData *cdata;
	decXmlElementTag *tag;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = pGetTagAt(root, i);
		
		if(tag){
			if(strcmp(tag->GetName(), "lower") == 0){
				cdata = tag->GetFirstData();
				
				if(cdata){
					lower = strtof(cdata->GetData(), NULL) * DEG2RAD;
					
				}else{
					lower = 0.0f;
				}
				
			}else if(strcmp(tag->GetName(), "upper") == 0){
				cdata = tag->GetFirstData();
				
				if(cdata){
					upper = strtof(cdata->GetData(), NULL) * DEG2RAD;
					
				}else{
					upper = 0.0f;
				}
				
			}else if(strcmp(tag->GetName(), "resistance") == 0){
				cdata = tag->GetFirstData();
				
				if(cdata){
					resistance = strtof(cdata->GetData(), NULL);
					
				}else{
					resistance = 0.0f;
				}
				
			}else if(strcmp(tag->GetName(), "locked") == 0){
				cdata = tag->GetFirstData();
				
				if(cdata){
					locked = ((int)strtol(cdata->GetData(), NULL, 10) != 0);
					
				}else{
					locked = false;
				}
			}
		}
	}
}

void deRigModule::pParseSphere(decXmlElementTag *root, decShapeList &shapes, decStringList &shapeProperties){
	decShapeSphere *sphere = NULL;
	decXmlCharacterData *cdata;
	decXmlElementTag *tag;
	decString property;
	decVector vector;
	int i;
	
	try{
		sphere = new decShapeSphere(1.0f);
		
		for(i=0; i<root->GetElementCount(); i++){
			tag = pGetTagAt(root, i);
			if(tag){
				if(strcmp(tag->GetName(), "center") == 0){
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					sphere->SetPosition(vector);
					
				}else if(strcmp(tag->GetName(), "position") == 0){
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					sphere->SetPosition(vector);
					
				}else if(strcmp(tag->GetName(), "radius") == 0){
					cdata = tag->GetFirstData();
					
					if(cdata){
						sphere->SetRadius(strtof(cdata->GetData(), NULL));
						
					}else{
						sphere->SetRadius(0.0f);
					}
					
				}else if(strcmp(tag->GetName(), "property") == 0){
					cdata = tag->GetFirstData();
					
					if(cdata){
						property = cdata->GetData();
						
					}else{
						property.Empty();
					}
				}
			}
		}
		
		shapeProperties.Add(property);
		shapes.Add(sphere);
		
	}catch(const deException &){
		if(sphere) delete sphere;
		throw;
	}
}

void deRigModule::pParseCylinder(decXmlElementTag *root, decShapeList &shapes, decStringList &shapeProperties){
	decShapeCylinder *cylinder = NULL;
	decXmlCharacterData *cdata;
	decXmlElementTag *tag;
	decString property;
	decVector vector;
	int i;
	
	try{
		cylinder = new decShapeCylinder(1.0f, 1.0f);
		if(! cylinder) DETHROW(deeOutOfMemory);
		
		for(i=0; i<root->GetElementCount(); i++){
			tag = pGetTagAt(root, i);
			if(tag){
				/*if( strcmp( tag->GetName(), "center" ) == 0 ){ // DEPRECATED
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					cylinder->SetPosition(vector);
					
				}else */if(strcmp(tag->GetName(), "position") == 0){
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					cylinder->SetPosition(vector);
					
				/*}else if( strcmp( tag->GetName(), "direction" ) == 0 ){ // DEPRECATED
					
				}else if(strcmp(tag->GetName(), "orientation") == 0){ // DEPRECATED
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					cylinder->SetOrientation(decMatrix::CreateRotation(vector * DEG2RAD).ToQuaternion());*/
					
				}else if(strcmp(tag->GetName(), "rotation") == 0){
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					cylinder->SetOrientation(decMatrix::CreateRotation(vector * DEG2RAD).ToQuaternion());
					
				}else if(strcmp(tag->GetName(), "radius") == 0){
					cdata = tag->GetFirstData();
					
					if(cdata){
						cylinder->SetRadius(strtof(cdata->GetData(), NULL));
						
					}else{
						cylinder->SetRadius(0.0f);
					}
					
				}else if(strcmp(tag->GetName(), "topRadius") == 0){
					cdata = tag->GetFirstData();
					
					if(cdata){
						cylinder->SetTopRadius(strtof(cdata->GetData(), NULL));
						
					}else{
						cylinder->SetTopRadius(0.0f);
					}
					
				}else if(strcmp(tag->GetName(), "bottomRadius") == 0){
					cdata = tag->GetFirstData();
					
					if(cdata){
						cylinder->SetBottomRadius(strtof(cdata->GetData(), NULL));
						
					}else{
						cylinder->SetBottomRadius(0.0f);
					}
					
				}else if(strcmp(tag->GetName(), "halfHeight") == 0){
					cdata = tag->GetFirstData();
					
					if(cdata){
						cylinder->SetHalfHeight(strtof(cdata->GetData(), NULL));
						
					}else{
						cylinder->SetHalfHeight(0.0f);
					}
					
				/*}else if( strcmp( tag->GetName(), "angle" ) == 0 ){ // DEPRECATED
					//cone->SetAngle( strtof( tag->GetFirstData()->GetData(), NULL ) );
					// useless... if we have a radius already an angle is not any more infos*/
					
				}else if(strcmp(tag->GetName(), "property") == 0){
					cdata = tag->GetFirstData();
					
					if(cdata){
						property = cdata->GetData();
						
					}else{
						property.Empty();
					}
				}
			}
		}
		
		shapeProperties.Add(property);
		shapes.Add(cylinder);
		
	}catch(const deException &){
		if(cylinder) delete cylinder;
		throw;
	}
}

void deRigModule::pParseCapsule(decXmlElementTag *root, decShapeList &shapes, decStringList &shapeProperties){
	decShapeCapsule *capsule = NULL;
	decXmlCharacterData *cdata;
	decXmlElementTag *tag;
	decString property;
	decVector vector;
	int i;
	
	try{
		capsule = new decShapeCapsule(1.0f, 1.0f);
		if(! capsule) DETHROW(deeOutOfMemory);
		
		for(i=0; i<root->GetElementCount(); i++){
			tag = pGetTagAt(root, i);
			if(tag){
				/*if( strcmp( tag->GetName(), "center" ) == 0 ){ // DEPRECATED
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					capsule->SetPosition(vector);
					
				}else */if(strcmp(tag->GetName(), "position") == 0){
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					capsule->SetPosition(vector);
					
				/*}else if( strcmp( tag->GetName(), "orientation" ) == 0 ){ // DEPRECATED
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					capsule->SetOrientation(decMatrix::CreateRotation(vector * DEG2RAD).ToQuaternion());*/
					
				}else if(strcmp(tag->GetName(), "rotation") == 0){
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					capsule->SetOrientation(decMatrix::CreateRotation(vector * DEG2RAD).ToQuaternion());
					
				}else if(strcmp(tag->GetName(), "radius") == 0){
					cdata = tag->GetFirstData();
					
					if(cdata){
						capsule->SetRadius(strtof(cdata->GetData(), NULL));
						
					}else{
						capsule->SetRadius(0.0f);
					}
					
				}else if(strcmp(tag->GetName(), "topRadius") == 0){
					cdata = tag->GetFirstData();
					
					if(cdata){
						capsule->SetTopRadius(strtof(cdata->GetData(), NULL));
						
					}else{
						capsule->SetTopRadius(0.0f);
					}
					
				}else if(strcmp(tag->GetName(), "bottomRadius") == 0){
					cdata = tag->GetFirstData();
					
					if(cdata){
						capsule->SetBottomRadius(strtof(cdata->GetData(), NULL));
						
					}else{
						capsule->SetBottomRadius(0.0f);
					}
					
				}else if(strcmp(tag->GetName(), "halfHeight") == 0){
					cdata = tag->GetFirstData();
					
					if(cdata){
						capsule->SetHalfHeight(strtof(cdata->GetData(), NULL));
						
					}else{
						capsule->SetHalfHeight(0.0f);
					}
					
				}else if(strcmp(tag->GetName(), "property") == 0){
					cdata = tag->GetFirstData();
					
					if(cdata){
						property = cdata->GetData();
						
					}else{
						property.Empty();
					}
				}
			}
		}
		
		shapeProperties.Add(property);
		shapes.Add(capsule);
		
	}catch(const deException &){
		if(capsule) delete capsule;
		throw;
	}
}

void deRigModule::pParseBox(decXmlElementTag *root, decShapeList &shapes, decStringList &shapeProperties){
	decShapeBox *box = NULL;
	decXmlElementTag *tag;
	decString property;
	decVector vector;
	int i;
	
	try{
		box = new decShapeBox(decVector(1.0f, 1.0f, 1.0f));
		if(! box) DETHROW(deeOutOfMemory);
		
		for(i=0; i<root->GetElementCount(); i++){
			tag = pGetTagAt(root, i);
			if(tag){
				/*if( strcmp( tag->GetName(), "center" ) == 0 ){ // DEPRECATED
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					box->SetPosition(vector);
					
				}else */if(strcmp(tag->GetName(), "position") == 0){
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					box->SetPosition(vector);
					
				/*}else if( strcmp( tag->GetName(), "halfSize" ) == 0 ){ // DEPRECATED
					box->SetHalfExtends(
						decVector(pGetAttributeFloat(tag, "x"),
						pGetAttributeFloat(tag, "y"),
						pGetAttributeFloat(tag, "z")));*/
					
				}else if(strcmp(tag->GetName(), "halfExtends") == 0){
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					box->SetHalfExtends(vector);
					
				/*}else if( strcmp( tag->GetName(), "orientation" ) == 0 ){ // DEPRECATED
					if(pFindAttribute(tag, "x")){ // DEPRECATED
						matrix.SetRotation(pGetAttributeFloat(tag, "x") * DEG2RAD,
							pGetAttributeFloat(tag, "y") * DEG2RAD,
							pGetAttributeFloat(tag, "z") * DEG2RAD);
						box->SetOrientation(matrix.ToQuaternion());
						
					}else{
						vector.Set(0.0f, 0.0f, 0.0f);
						pParseVector(tag, vector);
						box->SetOrientation(decMatrix::CreateRotation(vector * DEG2RAD).ToQuaternion());
					}*/
					
				}else if(strcmp(tag->GetName(), "rotation") == 0){
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					box->SetOrientation(decMatrix::CreateRotation(vector * DEG2RAD).ToQuaternion());
					
				}else if(strcmp(tag->GetName(), "property") == 0){
					const decXmlCharacterData * const cdata = tag->GetFirstData();
					
					if(cdata){
						property = cdata->GetData();
						
					}else{
						property.Empty();
					}
				}
			}
		}
		
		shapeProperties.Add(property);
		shapes.Add(box);
		
	}catch(const deException &){
		if(box) delete box;
		throw;
	}
}

void deRigModule::pParseHull(decXmlElementTag *root, decShapeList &shapes, decStringList &shapeProperties){
	decShapeHull *hull = NULL;
	decString property;
	decVector vector;
	int i;
	
	try{
		hull = new decShapeHull;
		
		int pointCount = 0;
		for(i=0; i<root->GetElementCount(); i++){
			decXmlElementTag * const tag = pGetTagAt(root, i);
			if(tag && tag->GetName() == "point"){
				pointCount++;
			}
		}
		hull->SetPointCount(pointCount);
		
		int pointIndex = 0;
		for(i=0; i<root->GetElementCount(); i++){
			decXmlElementTag * const tag = pGetTagAt(root, i);
			if(! tag){
				continue;
			}
			
			if(tag->GetName() == "position"){
				vector.Set(0.0f, 0.0f, 0.0f);
				pParseVector(tag, vector);
				hull->SetPosition(vector);
				
			}else if(tag->GetName() == "halfExtends"){
				vector.Set(0.0f, 0.0f, 0.0f);
				pParseVector(tag, vector);
				hull->SetOrientation(decMatrix::CreateRotation(vector * DEG2RAD).ToQuaternion());
				
			}else if(tag->GetName() == "point"){
				vector.Set(0.0f, 0.0f, 0.0f);
				pParseVector(tag, vector);
				hull->SetPointAt(pointIndex++, vector);
				
			}else if(tag->GetName() == "property"){
				const decXmlCharacterData * const cdata = tag->GetFirstData();
				
				if(cdata){
					property = cdata->GetData();
					
				}else{
					property.Empty();
				}
			}
		}
		
		shapeProperties.Add(property);
		shapes.Add(hull);
		
	}catch(const deException &){
		if(hull){
			delete hull;
		}
		throw;
	}
}

void deRigModule::pParseConstraint(decXmlElementTag *root, deRig &rig, deRigBone *bone, dermNameList &boneNameList){
	deRigConstraint *constraint = NULL;
	decXmlCharacterData *cdata;
	decXmlElementTag *tag;
	decVector vector;
	int i;
	
	try{
		constraint = new deRigConstraint;
		if(! constraint) DETHROW(deeOutOfMemory);
		
		for(i=0; i<root->GetElementCount(); i++){
			tag = pGetTagAt(root, i);
			if(tag){
				if(strcmp(tag->GetName(), "position") == 0){
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					constraint->SetReferencePosition(vector);
					
				}else if(strcmp(tag->GetName(), "rotation") == 0){
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					constraint->SetReferenceOrientation(decMatrix::CreateRotation(vector * DEG2RAD).ToQuaternion());
					
				}else if(strcmp(tag->GetName(), "position2") == 0){ // DEPRECATED
					
				}else if(strcmp(tag->GetName(), "offset") == 0){
					vector.Set(0.0f, 0.0f, 0.0f);
					pParseVector(tag, vector);
					constraint->SetBoneOffset(vector);
					
				}else if(strcmp(tag->GetName(), "rotation2") == 0){ // DEPRECATED
					
				}else if(strcmp(tag->GetName(), "bone") == 0){
					cdata = tag->GetFirstData();
					
					if(cdata){
						constraint->SetParentBone(boneNameList.AddName(cdata->GetData()));
						
					}else{
						constraint->SetParentBone(-1);
					}
					
				}else if(strcmp(tag->GetName(), "rope") == 0){
					cdata = tag->GetFirstData();
					
					if(cdata){
						if(strcmp(cdata->GetData(), "true") == 0){
							constraint->SetIsRope(true);
							
						}else if(strcmp(cdata->GetData(), "false") == 0){
							constraint->SetIsRope(false);
							
						}else{
							LogErrorFormat("Unknown value '%s' in tag '%s'\n",
								cdata->GetData().GetString(), tag->GetName().GetString());
						}
						
					}else{
						LogErrorFormat("Unknown value '' in tag '%s'\n",
							tag->GetName().GetString());
					}
					
				}else if(strcmp(tag->GetName(), "breakingThreshold") == 0){
					cdata = tag->GetFirstData();
					if(cdata){
						constraint->SetBreakingThreshold(strtof(cdata->GetData(), NULL));
					}
					
				}else if(strcmp(tag->GetName(), "limits") == 0){
					pParseConstraintLimits(tag, constraint);
					
				}else if(strcmp(tag->GetName(), "springStiffness") == 0){
					pParseConstraintSpringStiffness(tag, constraint);
					
				}else if(strcmp(tag->GetName(), "damping") == 0){
					pParseConstraintDamping(tag, constraint);
					
				}else if(strcmp(tag->GetName(), "linearX") == 0){
					pParseConstraintDof(*tag, constraint->GetDofLinearX(), true);
					
				}else if(strcmp(tag->GetName(), "linearY") == 0){
					pParseConstraintDof(*tag, constraint->GetDofLinearY(), true);
					
				}else if(strcmp(tag->GetName(), "linearZ") == 0){
					pParseConstraintDof(*tag, constraint->GetDofLinearZ(), true);
					
				}else if(strcmp(tag->GetName(), "angularX") == 0){
					pParseConstraintDof(*tag, constraint->GetDofAngularX(), false);
					
				}else if(strcmp(tag->GetName(), "angularY") == 0){
					pParseConstraintDof(*tag, constraint->GetDofAngularY(), false);
					
				}else if(strcmp(tag->GetName(), "angularZ") == 0){
					pParseConstraintDof(*tag, constraint->GetDofAngularZ(), false);
				}
			}
		}
		
		bone->AddConstraint(constraint);
		
	}catch(const deException &){
		if(constraint) delete constraint;
		throw;
	}
}

void deRigModule::pParseConstraintDof(const decXmlElementTag &root,
deColliderConstraintDof &dof, bool linearConstraint){
	const int count = root.GetElementCount();
	float value;
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = pGetTagAt(root, i);
		
		if(tag){
			if(strcmp(tag->GetName(), "limitLower") == 0){
				const decXmlCharacterData * const cdata = tag->GetFirstData();
				
				if(cdata){
					value = strtof(cdata->GetData(), NULL);
					
					if(! linearConstraint){
						value *= DEG2RAD;
					}
					
					dof.SetLowerLimit(value);
				}
				
			}else if(strcmp(tag->GetName(), "limitUpper") == 0){
				const decXmlCharacterData * const cdata = tag->GetFirstData();
				
				if(cdata){
					value = strtof(cdata->GetData(), NULL);
					
					if(! linearConstraint){
						value *= DEG2RAD;
					}
					
					dof.SetUpperLimit(value);
				}
				
			}else if(strcmp(tag->GetName(), "staticFriction") == 0){
				const decXmlCharacterData * const cdata = tag->GetFirstData();
				if(cdata){
					dof.SetStaticFriction(strtof(cdata->GetData(), NULL));
				}
				
			}else if(strcmp(tag->GetName(), "kinematicFriction") == 0){
				const decXmlCharacterData * const cdata = tag->GetFirstData();
				if(cdata){
					dof.SetKinematicFriction(strtof(cdata->GetData(), NULL));
				}
				
			}else if(strcmp(tag->GetName(), "springStiffness") == 0){
				const decXmlCharacterData * const cdata = tag->GetFirstData();
				if(cdata){
					dof.SetSpringStiffness(strtof(cdata->GetData(), NULL));
				}
			}
		}
	}
}

void deRigModule::pParseConstraintLimits(decXmlElementTag *root, deRigConstraint *constraint){
	decXmlElementTag *tag;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = pGetTagAt(root, i);
		if(tag){
			if(strcmp(tag->GetName(), "linear") == 0){
				pParseConstraintLimitsLinear(tag, constraint);
				
			}else if(strcmp(tag->GetName(), "angular") == 0){
				pParseConstraintLimitsAngular(tag, constraint);
			}
		}
	}
}

void deRigModule::pParseConstraintLimitsLinear(decXmlElementTag *root, deRigConstraint *constraint){
	decVector lower, upper;
	decXmlElementTag *tag;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = pGetTagAt(root, i);
		if(tag){
			if(strcmp(tag->GetName(), "lower") == 0){
				pParseVector(tag, lower);
				
			}else if(strcmp(tag->GetName(), "upper") == 0){
				pParseVector(tag, upper);
			}
		}
	}
	
	constraint->GetDofLinearX().SetLowerLimit(lower.x);
	constraint->GetDofLinearY().SetLowerLimit(lower.y);
	constraint->GetDofLinearZ().SetLowerLimit(lower.z);
	
	constraint->GetDofLinearX().SetUpperLimit(upper.x);
	constraint->GetDofLinearY().SetUpperLimit(upper.y);
	constraint->GetDofLinearZ().SetUpperLimit(upper.z);
}

void deRigModule::pParseConstraintLimitsAngular(decXmlElementTag *root, deRigConstraint *constraint){
	decVector lower, upper;
	decXmlElementTag *tag;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = pGetTagAt(root, i);
		if(tag){
			if(strcmp(tag->GetName(), "lower") == 0){
				pParseVector(tag, lower);
				
			}else if(strcmp(tag->GetName(), "upper") == 0){
				pParseVector(tag, upper);
			}
		}
	}
	
	constraint->GetDofAngularX().SetLowerLimit(lower.x * DEG2RAD);
	constraint->GetDofAngularY().SetLowerLimit(lower.y * DEG2RAD);
	constraint->GetDofAngularZ().SetLowerLimit(lower.z * DEG2RAD);
	
	constraint->GetDofAngularX().SetUpperLimit(upper.x * DEG2RAD);
	constraint->GetDofAngularY().SetUpperLimit(upper.y * DEG2RAD);
	constraint->GetDofAngularZ().SetUpperLimit(upper.z * DEG2RAD);
}

void deRigModule::pParseConstraintSpringStiffness(decXmlElementTag *root, deRigConstraint *constraint){
	decXmlElementTag *tag;
	decVector vector;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = pGetTagAt(root, i);
		if(tag){
			if(strcmp(tag->GetName(), "linear") == 0){
				pParseVector(tag, vector);
				constraint->GetDofLinearX().SetSpringStiffness(vector.x);
				constraint->GetDofLinearY().SetSpringStiffness(vector.y);
				constraint->GetDofLinearZ().SetSpringStiffness(vector.z);
				
			}else if(strcmp(tag->GetName(), "angular") == 0){
				pParseVector(tag, vector);
				constraint->GetDofAngularX().SetSpringStiffness(vector.x);
				constraint->GetDofAngularY().SetSpringStiffness(vector.y);
				constraint->GetDofAngularZ().SetSpringStiffness(vector.z);
			}
		}
	}
}

void deRigModule::pParseConstraintDamping(decXmlElementTag *root, deRigConstraint *constraint){
	decXmlCharacterData *cdata;
	decXmlElementTag *tag;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = pGetTagAt(root, i);
		if(tag){
			if(strcmp(tag->GetName(), "linear") == 0){
				cdata = tag->GetFirstData();
				
				if(cdata){
					constraint->SetLinearDamping(strtof(cdata->GetData(), NULL));
				}
				
			}else if(strcmp(tag->GetName(), "angular") == 0){
				cdata = tag->GetFirstData();
				
				if(cdata){
					constraint->SetAngularDamping(strtof(cdata->GetData(), NULL));
				}
				
			}else if(strcmp(tag->GetName(), "spring") == 0){
				cdata = tag->GetFirstData();
				
				if(cdata){
					constraint->SetSpringDamping(strtof(cdata->GetData(), NULL));
				}
			}
		}
	}
}

void deRigModule::pParseVector(decXmlElementTag *root, decVector &vector){
//	decXmlElementTag *tag;
//	int i;
	
	if(pFindAttribute(root, "x")){
		vector.x = pGetAttributeFloat(root, "x");
	}
	
	if(pFindAttribute(root, "y")){
		vector.y = pGetAttributeFloat(root, "y");
	}
	
	if(pFindAttribute(root, "z")){
		vector.z = pGetAttributeFloat(root, "z");
	}
	
	// DEPRECATED
	/*for( i=0; i<root->GetElementCount(); i++ ){
		tag = pGetTagAt(root, i);
		if(tag){
			if(strcmp(tag->GetName(), "x") == 0){
				vector.x = strtof(tag->GetFirstData()->GetData(), NULL);
				
			}else if(strcmp(tag->GetName(), "y") == 0){
				vector.y = strtof(tag->GetFirstData()->GetData(), NULL);
				
			}else if(strcmp(tag->GetName(), "z") == 0){
				vector.z = strtof(tag->GetFirstData()->GetData(), NULL);
			}
		}
	}*/
}



void deRigModule::pWriteRig(decXmlWriter &writer, const deRig &rig){
	writer.WriteOpeningTag("rig", false, true);
	
	dermWriteShape writeShape(writer);
	int i;
	
	if(rig.GetRootBone() != -1){
		writer.WriteDataTagString("rootBone", rig.GetBoneAt(rig.GetRootBone()).GetName());
	}
	
	if(rig.GetModelCollision()){
		writer.WriteDataTagBool("modelCollision", true);
	}
	
	const decVector &cmp = rig.GetCentralMassPoint();
	if(! cmp.IsZero()){
		writer.WriteOpeningTagStart("centralMassPoint");
		writer.WriteAttributeFloat("x", cmp.x);
		writer.WriteAttributeFloat("y", cmp.y);
		writer.WriteAttributeFloat("z", cmp.z);
		writer.WriteOpeningTagEnd(true);
	}
	
	const int boneCount = rig.GetBoneCount();
	for(i=0; i<boneCount; i++){
		pWriteBone(writer, rig, rig.GetBoneAt(i));
	}
	
	const decStringList &shapeProperties = rig.GetShapeProperties();
	const decShapeList &shapes = rig.GetShapes();
	const int shapeCount = shapes.GetCount();
	for(i=0; i<shapeCount; i++){
		writeShape.SetProperty(shapeProperties.GetAt(i));
		shapes.GetAt(i)->Visit(writeShape);
	}
	
	writer.WriteClosingTag("rig", true);
}

void deRigModule::pWriteBone(decXmlWriter &writer, const deRig &rig, const deRigBone &bone){
	dermWriteShape writeShape(writer);
	int parent = bone.GetParent();
	int i;
	
	writer.WriteOpeningTagStart("bone");
	writer.WriteAttributeString("name", bone.GetName());
	writer.WriteOpeningTagEnd();
	
	if(parent != -1){
		writer.WriteDataTagString("parent", rig.GetBoneAt(parent).GetName());
	}
	
	const decVector &position = bone.GetPosition();
	if(! position.IsZero()){
		writer.WriteOpeningTagStart("position");
		writer.WriteAttributeFloat("x", position.x);
		writer.WriteAttributeFloat("y", position.y);
		writer.WriteAttributeFloat("z", position.z);
		writer.WriteOpeningTagEnd(true);
	}
	
	const decVector rotation(bone.GetRotation() * RAD2DEG);
	if(! rotation.IsZero()){
		writer.WriteOpeningTagStart("rotation");
		writer.WriteAttributeFloat("x", rotation.x);
		writer.WriteAttributeFloat("y", rotation.y);
		writer.WriteAttributeFloat("z", rotation.z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(fabsf(bone.GetMass() - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("mass", bone.GetMass());
	}
	
	const decVector &cmp = bone.GetCentralMassPoint();
	if(! cmp.IsZero()){
		writer.WriteOpeningTagStart("centralMassPoint");
		writer.WriteAttributeFloat("x", cmp.x);
		writer.WriteAttributeFloat("y", cmp.y);
		writer.WriteAttributeFloat("z", cmp.z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(bone.GetDynamic()){
		writer.WriteDataTagBool("dynamic", bone.GetDynamic());
	}
	
	const decVector ikLimitsLower(bone.GetIKLimitsLower() * RAD2DEG);
	const decVector ikLimitsUpper(bone.GetIKLimitsUpper() * RAD2DEG);
	const decVector &ikResistance = bone.GetIKResistance();
	
	if(! ikLimitsUpper.IsZero() || ! ikResistance.IsZero()
	|| ! ikLimitsLower.IsEqualTo(decVector(360.0f, 360.0f, 360.0f))
	|| bone.GetIKLockedX() || bone.GetIKLockedY() || bone.GetIKLockedZ()){
		if(fabsf(ikLimitsLower.x - 360.0f) > FLOAT_SAFE_EPSILON
		|| fabsf(ikLimitsUpper.x) > FLOAT_SAFE_EPSILON
		|| fabsf(ikResistance.x) > FLOAT_SAFE_EPSILON
		|| bone.GetIKLockedX()){
			writer.WriteOpeningTag("ikX");
			if(fabsf(ikLimitsLower.x - 360.0f) > FLOAT_SAFE_EPSILON
			|| fabsf(ikLimitsUpper.x) > FLOAT_SAFE_EPSILON){
				writer.WriteDataTagFloat("lower", ikLimitsLower.x);
				writer.WriteDataTagFloat("upper", ikLimitsUpper.x);
			}
			if(fabsf(ikResistance.x) > FLOAT_SAFE_EPSILON){
				writer.WriteDataTagFloat("resistance", ikResistance.x);
			}
			if(bone.GetIKLockedX()){
				writer.WriteDataTagInt("locked", bone.GetIKLockedX() ? 1 : 0);
			}
			writer.WriteClosingTag("ikX");
		}
		
		if(fabsf(ikLimitsLower.y - 360.0f) > FLOAT_SAFE_EPSILON
		|| fabsf(ikLimitsUpper.y) > FLOAT_SAFE_EPSILON
		|| fabsf(ikResistance.y) > FLOAT_SAFE_EPSILON
		|| bone.GetIKLockedY()){
			writer.WriteOpeningTag("ikY");
			if(fabsf(ikLimitsLower.y - 360.0f) > FLOAT_SAFE_EPSILON
			|| fabsf(ikLimitsUpper.y) > FLOAT_SAFE_EPSILON){
				writer.WriteDataTagFloat("lower", ikLimitsLower.y);
				writer.WriteDataTagFloat("upper", ikLimitsUpper.y);
			}
			if(fabsf(ikResistance.y) > FLOAT_SAFE_EPSILON){
				writer.WriteDataTagFloat("resistance", ikResistance.y);
			}
			if(bone.GetIKLockedY()){
				writer.WriteDataTagInt("locked", bone.GetIKLockedY() ? 1 : 0);
			}
			writer.WriteClosingTag("ikY");
		}
		
		if(fabsf(ikLimitsLower.z - 360.0f) > FLOAT_SAFE_EPSILON
		|| fabsf(ikLimitsUpper.z) > FLOAT_SAFE_EPSILON
		|| fabsf(ikResistance.z) > FLOAT_SAFE_EPSILON
		|| bone.GetIKLockedZ()){
			writer.WriteOpeningTag("ikZ");
			if(fabsf(ikLimitsLower.z - 360.0f) > FLOAT_SAFE_EPSILON
			|| fabsf(ikLimitsUpper.z) > FLOAT_SAFE_EPSILON){
				writer.WriteDataTagFloat("lower", ikLimitsLower.z);
				writer.WriteDataTagFloat("upper", ikLimitsUpper.z);
			}
			if(fabsf(ikResistance.z) > FLOAT_SAFE_EPSILON){
				writer.WriteDataTagFloat("resistance", ikResistance.z);
			}
			if(bone.GetIKLockedZ()){
				writer.WriteDataTagInt("locked", bone.GetIKLockedZ() ? 1 : 0);
			}
			writer.WriteClosingTag("ikZ");
		}
	}
	
	const decStringList &shapeProperties = bone.GetShapeProperties();
	const decShapeList &shapes = bone.GetShapes();
	const int shapeCount = shapes.GetCount();
	for(i=0; i<shapeCount; i++){
		writeShape.SetProperty(shapeProperties.GetAt(i));
		shapes.GetAt(i)->Visit(writeShape);
	}
	
	const int constraintCount = bone.GetConstraintCount();
	for(i=0; i<constraintCount; i++){
		pWriteConstraint(writer, rig, bone.GetConstraintAt(i));
	}
	
	writer.WriteClosingTag("bone");
}

void deRigModule::pWriteConstraint(decXmlWriter &writer, const deRig &rig, const deRigConstraint &constraint){
	int parentBone = constraint.GetParentBone();
	
	writer.WriteOpeningTag("constraint");
	
	if(parentBone != -1){
		writer.WriteDataTagString("bone", rig.GetBoneAt(parentBone).GetName());
	}
	
	const decVector &referencePosition = constraint.GetReferencePosition();
	if(! referencePosition.IsZero()){
		writer.WriteOpeningTagStart("position");
		writer.WriteAttributeFloat("x", referencePosition.x);
		writer.WriteAttributeFloat("y", referencePosition.y);
		writer.WriteAttributeFloat("z", referencePosition.z);
		writer.WriteOpeningTagEnd(true);
	}
	
	const decVector referenceRotation(decMatrix::CreateFromQuaternion(
		constraint.GetReferenceOrientation()).GetEulerAngles() / DEG2RAD);
	if(! referenceRotation.IsZero()){
		writer.WriteOpeningTagStart("rotation");
		writer.WriteAttributeFloat("x", referenceRotation.x);
		writer.WriteAttributeFloat("y", referenceRotation.y);
		writer.WriteAttributeFloat("z", referenceRotation.z);
		writer.WriteOpeningTagEnd(true);
	}
	
	const decVector &boneOffset = constraint.GetBoneOffset();
	if(! boneOffset.IsZero()){
		writer.WriteOpeningTagStart("offset");
		writer.WriteAttributeFloat("x", boneOffset.x);
		writer.WriteAttributeFloat("y", boneOffset.y);
		writer.WriteAttributeFloat("z", boneOffset.z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(constraint.GetIsRope()){
		writer.WriteDataTagBool("rope", true);
	}
	
	if(fabsf(constraint.GetBreakingThreshold()) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("breakingThreshold", constraint.GetBreakingThreshold());
	}
	
	// damping
	const float dampingLinear = constraint.GetLinearDamping();
	const float dampingAngular = constraint.GetAngularDamping();
	const float dampingSpring = constraint.GetSpringDamping();
	const bool writeDampingLinear = fabsf(dampingLinear - 1.0f) > FLOAT_SAFE_EPSILON;
	const bool writeDampingAngular = fabsf(dampingAngular - 1.0f) > FLOAT_SAFE_EPSILON;
	const bool writeDampingSpring = fabsf(dampingSpring - 1.0f) > FLOAT_SAFE_EPSILON;
	
	if(writeDampingLinear || writeDampingAngular || writeDampingSpring){
		writer.WriteOpeningTag("damping");
		
		if(writeDampingLinear){
			writer.WriteDataTagFloat("linear", dampingLinear);
		}
		if(writeDampingAngular){
			writer.WriteDataTagFloat("angular", dampingAngular);
		}
		if(writeDampingSpring){
			writer.WriteDataTagFloat("spring", dampingSpring);
		}
		
		writer.WriteClosingTag("damping");
	}
	
	pWriteConstraintDof(writer, constraint.GetDofLinearX(), "linearX", true);
	pWriteConstraintDof(writer, constraint.GetDofLinearY(), "linearY", true);
	pWriteConstraintDof(writer, constraint.GetDofLinearZ(), "linearZ", true);
	pWriteConstraintDof(writer, constraint.GetDofAngularX(), "angularX", false);
	pWriteConstraintDof(writer, constraint.GetDofAngularY(), "angularY", false);
	pWriteConstraintDof(writer, constraint.GetDofAngularZ(), "angularZ", false);
	
	writer.WriteClosingTag("constraint");
}

void deRigModule::pWriteConstraintDof(decXmlWriter &writer, const deColliderConstraintDof &dof,
const char *tagName, bool linearConstraint){
	bool emptyTag = true;
	float value;
	
	// lower
	value = dof.GetLowerLimit();
	if(! linearConstraint){
		value /= DEG2RAD;
	}
	
	if(fabsf(value) > FLOAT_SAFE_EPSILON){ // skip default value 0.0
		if(emptyTag){
			writer.WriteOpeningTag(tagName);
			emptyTag = false;
		}
		writer.WriteDataTagFloat("limitLower", value);
	}
	
	// upper
	value = dof.GetUpperLimit();
	if(! linearConstraint){
		value /= DEG2RAD;
	}
	
	if(fabsf(value) > FLOAT_SAFE_EPSILON){ // skip default value 0.0
		if(emptyTag){
			writer.WriteOpeningTag(tagName);
			emptyTag = false;
		}
		writer.WriteDataTagFloat("limitUpper", value);
	}
	
	// static friction
	value = dof.GetStaticFriction();
	
	if(fabsf(value) > FLOAT_SAFE_EPSILON){ // skip default value 0.0
		if(emptyTag){
			writer.WriteOpeningTag(tagName);
			emptyTag = false;
		}
		writer.WriteDataTagFloat("staticFriction", value);
	}
	
	// static friction
	value = dof.GetKinematicFriction();
	
	if(fabsf(value) > FLOAT_SAFE_EPSILON){ // skip default value 0.0
		if(emptyTag){
			writer.WriteOpeningTag(tagName);
			emptyTag = false;
		}
		writer.WriteDataTagFloat("kinematicFriction", value);
	}
	
	// sprint stiffness
	value = dof.GetSpringStiffness();
	
	if(fabsf(value) > FLOAT_SAFE_EPSILON){ // skip default value 0.0
		if(emptyTag){
			writer.WriteOpeningTag(tagName);
			emptyTag = false;
		}
		writer.WriteDataTagFloat("springStiffness", value);
	}
	
	if(! emptyTag){
		writer.WriteClosingTag(tagName);
	}
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class dermModuleInternal : public deInternalModule{
public:
	dermModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("DERig");
		SetDescription("Handles rigs in the XML Drag[en]gine rig format.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtRig);
		SetDirectoryName("derig");
		GetPatternList().Add(".derig");
		SetDefaultExtension(".derig");
		SetPriority(1);
		SetDefaultLoggingName();
	}
	
	void CreateModule() override{
		SetModule(DERigCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *dermRegisterInternalModule(deModuleSystem *system){
	return new dermModuleInternal(system);
}
#endif
