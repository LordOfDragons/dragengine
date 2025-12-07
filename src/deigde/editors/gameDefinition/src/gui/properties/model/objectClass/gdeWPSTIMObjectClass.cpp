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

#include "gdeWPSTIMObjectClass.h"
#include "gdeWPSTIMOCBillboard.h"
#include "gdeWPSTIMOCCamera.h"
#include "gdeWPSTIMOCComponent.h"
#include "gdeWPSTIMOCEnvMapProbe.h"
#include "gdeWPSTIMOCLight.h"
#include "gdeWPSTIMOCNavBlocker.h"
#include "gdeWPSTIMOCNavSpace.h"
#include "gdeWPSTIMOCParticleEmitter.h"
#include "gdeWPSTIMOCForceField.h"
#include "gdeWPSTIMOCSnapPoint.h"
#include "gdeWPSTIMOCSpeaker.h"
#include "gdeWPSTIMOCWorld.h"
#include "../gdeWPSTreeModel.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/billboard/gdeOCBillboard.h"
#include "../../../../gamedef/objectClass/camera/gdeOCCamera.h"
#include "../../../../gamedef/objectClass/component/gdeOCComponent.h"
#include "../../../../gamedef/objectClass/envmapprobe/gdeOCEnvMapProbe.h"
#include "../../../../gamedef/objectClass/inherit/gdeOCInherit.h"
#include "../../../../gamedef/objectClass/light/gdeOCLight.h"
#include "../../../../gamedef/objectClass/navblocker/gdeOCNavigationBlocker.h"
#include "../../../../gamedef/objectClass/navspace/gdeOCNavigationSpace.h"
#include "../../../../gamedef/objectClass/particleemitter/gdeOCParticleEmitter.h"
#include "../../../../gamedef/objectClass/forceField/gdeOCForceField.h"
#include "../../../../gamedef/objectClass/snappoint/gdeOCSnapPoint.h"
#include "../../../../gamedef/objectClass/speaker/gdeOCSpeaker.h"
#include "../../../../gamedef/objectClass/world/gdeOCWorld.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItem.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMObjectClass::gdeWPSTIMObjectClass(gdeWPSTreeModel &tree, gdeObjectClass *objectClass) :
gdeWPSTreeItemModel(tree, etObjectClass),
pObjectClass(NULL)
{
	if(!objectClass){
		DETHROW(deeInvalidParam);
	}
	
	SetText(objectClass->GetName());
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeWPSTIMObjectClass::~gdeWPSTIMObjectClass(){
	if(pObjectClass){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

gdeWPSTIMOCBillboard *gdeWPSTIMObjectClass::GetChildWith(gdeOCBillboard *billboard) const{
	gdeWPSTIMOCSubObject *child = (gdeWPSTIMOCSubObject*)GetFirstChild();
	
	while(child){
		if(child->GetType() == etObjectClassBillboard &&
		pObjectClass->GetBillboards().GetAt(child->GetIndex()) == billboard){
			return (gdeWPSTIMOCBillboard*)child;
		}
		child = (gdeWPSTIMOCSubObject*)child->GetNext();
	}
	
	return NULL;
}

gdeWPSTIMOCCamera *gdeWPSTIMObjectClass::GetChildWith(gdeOCCamera *camera) const{
	gdeWPSTIMOCSubObject *child = (gdeWPSTIMOCSubObject*)GetFirstChild();
	
	while(child){
		if(child->GetType() == etObjectClassCamera &&
		pObjectClass->GetCameras().GetAt(child->GetIndex()) == camera){
			return (gdeWPSTIMOCCamera*)child;
		}
		child = (gdeWPSTIMOCSubObject*)child->GetNext();
	}
	
	return NULL;
}

gdeWPSTIMOCComponent *gdeWPSTIMObjectClass::GetChildWith(gdeOCComponent *component) const{
	gdeWPSTIMOCSubObject *child = (gdeWPSTIMOCSubObject*)GetFirstChild();
	
	while(child){
		if(child->GetType() == etObjectClassComponent &&
		pObjectClass->GetComponents().GetAt(child->GetIndex()) == component){
			return (gdeWPSTIMOCComponent*)child;
		}
		child = (gdeWPSTIMOCSubObject*)child->GetNext();
	}
	
	return NULL;
}

gdeWPSTIMOCEnvMapProbe *gdeWPSTIMObjectClass::GetChildWith(gdeOCEnvMapProbe *envMapProbe) const{
	gdeWPSTIMOCSubObject *child = (gdeWPSTIMOCSubObject*)GetFirstChild();
	
	while(child){
		if(child->GetType() == etObjectClassEnvMapProbe &&
		pObjectClass->GetEnvMapProbes().GetAt(child->GetIndex()) == envMapProbe){
			return (gdeWPSTIMOCEnvMapProbe*)child;
		}
		child = (gdeWPSTIMOCSubObject*)child->GetNext();
	}
	
	return NULL;
}

gdeWPSTIMOCLight *gdeWPSTIMObjectClass::GetChildWith(gdeOCLight *light) const{
	gdeWPSTIMOCSubObject *child = (gdeWPSTIMOCSubObject*)GetFirstChild();
	
	while(child){
		if(child->GetType() == etObjectClassLight &&
		pObjectClass->GetLights().GetAt(child->GetIndex()) == light){
			return (gdeWPSTIMOCLight*)child;
		}
		child = (gdeWPSTIMOCSubObject*)child->GetNext();
	}
	
	return NULL;
}

gdeWPSTIMOCNavBlocker *gdeWPSTIMObjectClass::GetChildWith(gdeOCNavigationBlocker *navblocker) const{
	gdeWPSTIMOCSubObject *child = (gdeWPSTIMOCSubObject*)GetFirstChild();
	
	while(child){
		if(child->GetType() == etObjectClassNavigationBlocker &&
		pObjectClass->GetNavigationBlockers().GetAt(child->GetIndex()) == navblocker){
			return (gdeWPSTIMOCNavBlocker*)child;
		}
		child = (gdeWPSTIMOCSubObject*)child->GetNext();
	}
	
	return NULL;
}

gdeWPSTIMOCNavSpace *gdeWPSTIMObjectClass::GetChildWith(gdeOCNavigationSpace *navspace) const{
	gdeWPSTIMOCSubObject *child = (gdeWPSTIMOCSubObject*)GetFirstChild();
	
	while(child){
		if(child->GetType() == etObjectClassNavigationSpace &&
		pObjectClass->GetNavigationSpaces().GetAt(child->GetIndex()) == navspace){
			return (gdeWPSTIMOCNavSpace*)child;
		}
		child = (gdeWPSTIMOCSubObject*)child->GetNext();
	}
	
	return NULL;
}

gdeWPSTIMOCParticleEmitter *gdeWPSTIMObjectClass::GetChildWith(
gdeOCParticleEmitter *particleEmitter) const{
	gdeWPSTIMOCSubObject *child = (gdeWPSTIMOCSubObject*)GetFirstChild();
	
	while(child){
		if(child->GetType() == etObjectClassParticleEmitter &&
		pObjectClass->GetParticleEmitters().GetAt(child->GetIndex()) == particleEmitter){
			return (gdeWPSTIMOCParticleEmitter*)child;
		}
		child = (gdeWPSTIMOCSubObject*)child->GetNext();
	}
	
	return NULL;
}

gdeWPSTIMOCForceField *gdeWPSTIMObjectClass::GetChildWith(
gdeOCForceField *forceField) const{
	gdeWPSTIMOCSubObject *child = (gdeWPSTIMOCSubObject*)GetFirstChild();
	
	while(child){
		if(child->GetType() == etObjectClassForceField &&
		pObjectClass->GetForceFields().GetAt(child->GetIndex()) == forceField){
			return (gdeWPSTIMOCForceField*)child;
		}
		child = (gdeWPSTIMOCSubObject*)child->GetNext();
	}
	
	return NULL;
}

gdeWPSTIMOCSnapPoint *gdeWPSTIMObjectClass::GetChildWith(gdeOCSnapPoint *snapPoint) const{
	gdeWPSTIMOCSubObject *child = (gdeWPSTIMOCSubObject*)GetFirstChild();
	
	while(child){
		if(child->GetType() == etObjectClassSnapPoint &&
		pObjectClass->GetSnapPoints().GetAt(child->GetIndex()) == snapPoint){
			return (gdeWPSTIMOCSnapPoint*)child;
		}
		child = (gdeWPSTIMOCSubObject*)child->GetNext();
	}
	
	return NULL;
}

gdeWPSTIMOCSpeaker *gdeWPSTIMObjectClass::GetChildWith(gdeOCSpeaker *speaker) const{
	gdeWPSTIMOCSubObject *child = (gdeWPSTIMOCSubObject*)GetFirstChild();
	
	while(child){
		if(child->GetType() == etObjectClassSpeaker &&
		pObjectClass->GetSpeakers().GetAt(child->GetIndex()) == speaker){
			return (gdeWPSTIMOCSpeaker*)child;
		}
		child = (gdeWPSTIMOCSubObject*)child->GetNext();
	}
	
	return NULL;
}

gdeWPSTIMOCWorld *gdeWPSTIMObjectClass::GetChildWith(gdeOCWorld *world) const{
	gdeWPSTIMOCSubObject *child = (gdeWPSTIMOCSubObject*)GetFirstChild();
	
	while(child){
		if(child->GetType() == etObjectClassWorld &&
		pObjectClass->GetWorlds().GetAt(child->GetIndex()) == world){
			return (gdeWPSTIMOCWorld*)child;
		}
		child = (gdeWPSTIMOCSubObject*)child->GetNext();
	}
	
	return nullptr;
}


void gdeWPSTIMObjectClass::NameChanged(){
	SetText(pObjectClass->GetName());
	ParentSortItems();
}

void gdeWPSTIMObjectClass::SubObjectsChanged(){
	const gdeWPSTreeModel::HelperIgnoreSelection ignoreSelection(GetTree());
	
	RemoveAllModels();
	pAddSubObjects();
	Validate();
}

bool gdeWPSTIMObjectClass::IsValid() const{
	// category is valid
	const gdeCategoryList &categories = GetGameDefinition().GetCategoriesObjectClass();
	if(!pObjectClass->GetCategory().IsEmpty() &&
	!categories.GetWithPath(pObjectClass->GetCategory())){
		return false;
	}
	
	// inherits are valid
	const gdeOCInheritList inherits = pObjectClass->GetInherits();
	const int inheritCount = inherits.GetCount();
	int i;
	for(i=0; i<inheritCount; i++){
		const decString &name = inherits.GetAt(i)->GetName();
		if(!name.IsEmpty() && !GetGameDefinition().FindObjectClass(name)){
			return false;
		}
	}
	
	// sub elements are valid. for this we need to check the models
	gdeWPSTIMOCSubObject *child = (gdeWPSTIMOCSubObject*)GetFirstChild();
	while(child){
		if(!child->IsValid()){
			return false;
		}
		child = (gdeWPSTIMOCSubObject*)child->GetNext();
	}
	
	// all valid
	return true;
}

void gdeWPSTIMObjectClass::Validate(){
	SetIcon(GetWindowMain().GetEnvironment().GetStockIcon(
		IsValid() ? igdeEnvironment::esiNew : igdeEnvironment::esiWarning));
	ItemChanged();
}

void gdeWPSTIMObjectClass::ValidateObjectClassName(){
	gdeWPSTIMOCSubObject *child = (gdeWPSTIMOCSubObject*)GetFirstChild();
	while(child){
		child->ValidateObjectClassName();
		child = (gdeWPSTIMOCSubObject*)child->GetNext();
	}
	
	Validate();
}

void gdeWPSTIMObjectClass::ValidateCategoryName(){
	Validate();
}



int gdeWPSTIMObjectClass::Compare(const gdeWPSTreeItemModel &item) const{
	const gdeWPSTIMObjectClass &other = (const gdeWPSTIMObjectClass &)item;
	return pObjectClass->GetName().Compare(other.GetObjectClass()->GetName());
}

void gdeWPSTIMObjectClass::OnAddedToTree(){
	pAddSubObjects();
	Validate();
}

void gdeWPSTIMObjectClass::OnSelected(){
	GetGameDefinition().SetActiveObjectClass(pObjectClass);
	GetGameDefinition().SetSelectedObjectType(gdeGameDefinition::eotObjectClass);
}

void gdeWPSTIMObjectClass::OnContextMenu(igdeMenuCascade &contextMenu){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(contextMenu, windowMain.GetActionObjectClassAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionObjectClassRemove());
	helper.MenuCommand(contextMenu, windowMain.GetActionObjectClassDuplicate());
	helper.MenuCommand(contextMenu, windowMain.GetActionObjectClassSubclass());
	helper.MenuCommand(contextMenu, windowMain.GetActionObjectClassExportXmlEClass());
	
	helper.MenuSeparator(contextMenu);
	helper.MenuCommand(contextMenu, windowMain.GetActionObjectClassCopy());
	helper.MenuCommand(contextMenu, windowMain.GetActionObjectClassCut());
	helper.MenuCommand(contextMenu, windowMain.GetActionObjectClassPaste());
	
	helper.MenuSeparator(contextMenu);
	helper.MenuCommand(contextMenu, windowMain.GetActionOCBillboardAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCBillboardPaste());
	
	helper.MenuSeparator(contextMenu);
	helper.MenuCommand(contextMenu, windowMain.GetActionOCCameraAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCCameraPaste());
	
	helper.MenuSeparator(contextMenu);
	helper.MenuCommand(contextMenu, windowMain.GetActionOCComponentAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCComponentPaste());
	
	helper.MenuSeparator(contextMenu);
	helper.MenuCommand(contextMenu, windowMain.GetActionOCEnvMapProbeAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCEnvMapProbePaste());
	
	helper.MenuSeparator(contextMenu);
	helper.MenuCommand(contextMenu, windowMain.GetActionOCLightAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCLightPaste());
	
	helper.MenuSeparator(contextMenu);
	helper.MenuCommand(contextMenu, windowMain.GetActionOCNavSpaceAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCNavSpacePaste());
	
	helper.MenuSeparator(contextMenu);
	helper.MenuCommand(contextMenu, windowMain.GetActionOCNavBlockerAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCNavBlockerPaste());
	
	helper.MenuSeparator(contextMenu);
	helper.MenuCommand(contextMenu, windowMain.GetActionOCParticleEmitterAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCParticleEmitterPaste());
	
	helper.MenuSeparator(contextMenu);
	helper.MenuCommand(contextMenu, windowMain.GetActionOCForceFieldAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCForceFieldPaste());
	
	helper.MenuSeparator(contextMenu);
	helper.MenuCommand(contextMenu, windowMain.GetActionOCSnapPointAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCSnapPointPaste());
	
	helper.MenuSeparator(contextMenu);
	helper.MenuCommand(contextMenu, windowMain.GetActionOCSpeakerAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCSpeakerPaste());
	
	helper.MenuSeparator(contextMenu);
	helper.MenuCommand(contextMenu, windowMain.GetActionOCWorldAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCWorldPaste());
}

void gdeWPSTIMObjectClass::SelectBestMatching(const char *string){
	if(GetParent()){
		((gdeWPSTreeItemModel*)GetParent())->SelectBestMatching(string);
	}
}



// Private Functions
//////////////////////

void gdeWPSTIMObjectClass::pAddSubObjects(){
	igdeTreeItem::Ref item;
	int i;
	
	const gdeOCBillboardList &billboards = pObjectClass->GetBillboards();
	const int billboardCount = billboards.GetCount();
	for(i=0; i<billboardCount; i++){
		item.TakeOver(new gdeWPSTIMOCBillboard(GetTree(), pObjectClass, billboards.GetAt(i), i));
		AppendModel(item);
	}
	
	const gdeOCCameraList &cameras = pObjectClass->GetCameras();
	const int cameraCount = cameras.GetCount();
	for(i=0; i<cameraCount; i++){
		item.TakeOver(new gdeWPSTIMOCCamera(GetTree(), pObjectClass, cameras.GetAt(i), i));
		AppendModel(item);
	}
	
	const gdeOCComponentList &components = pObjectClass->GetComponents();
	const int componentCount = components.GetCount();
	for(i=0; i<componentCount; i++){
		item.TakeOver(new gdeWPSTIMOCComponent(GetTree(), pObjectClass, components.GetAt(i), i));
		AppendModel(item);
	}
	
	const gdeOCEnvMapProbeList &envMapProbes = pObjectClass->GetEnvMapProbes();
	const int envMapProbeCount = envMapProbes.GetCount();
	for(i=0; i<envMapProbeCount; i++){
		item.TakeOver(new gdeWPSTIMOCEnvMapProbe(GetTree(), pObjectClass, envMapProbes.GetAt(i), i));
		AppendModel(item);
	}
	
	const gdeOCLightList &lights = pObjectClass->GetLights();
	const int lightCount = lights.GetCount();
	for(i=0; i<lightCount; i++){
		item.TakeOver(new gdeWPSTIMOCLight(GetTree(), pObjectClass, lights.GetAt(i), i));
		AppendModel(item);
	}
	
	const gdeOCNavigationBlockerList &navBlockers = pObjectClass->GetNavigationBlockers();
	const int navBlockerCount = navBlockers.GetCount();
	for(i=0; i<navBlockerCount; i++){
		item.TakeOver(new gdeWPSTIMOCNavBlocker(GetTree(), pObjectClass, navBlockers.GetAt(i), i));
		AppendModel(item);
	}
	
	const gdeOCNavigationSpaceList &navSpaces = pObjectClass->GetNavigationSpaces();
	const int navSpaceCount = navSpaces.GetCount();
	for(i=0; i<navSpaceCount; i++){
		item.TakeOver(new gdeWPSTIMOCNavSpace(GetTree(), pObjectClass, navSpaces.GetAt(i), i));
		AppendModel(item);
	}
	
	const gdeOCParticleEmitterList &particleEmitters = pObjectClass->GetParticleEmitters();
	const int particleEmitterCount = particleEmitters.GetCount();
	for(i=0; i<particleEmitterCount; i++){
		item.TakeOver(new gdeWPSTIMOCParticleEmitter(GetTree(), pObjectClass, particleEmitters.GetAt(i), i));
		AppendModel(item);
	}
	
	const gdeOCForceFieldList &forceFields = pObjectClass->GetForceFields();
	const int forceFieldCount = forceFields.GetCount();
	for(i=0; i<forceFieldCount; i++){
		item.TakeOver(new gdeWPSTIMOCForceField(GetTree(), pObjectClass, forceFields.GetAt(i), i));
		AppendModel(item);
	}
	
	const gdeOCSnapPointList &snapPoints = pObjectClass->GetSnapPoints();
	const int snapPointCount = snapPoints.GetCount();
	for(i=0; i<snapPointCount; i++){
		item.TakeOver(new gdeWPSTIMOCSnapPoint(GetTree(), pObjectClass, snapPoints.GetAt(i), i));
		AppendModel(item);
	}
	
	const gdeOCSpeakerList &speakers = pObjectClass->GetSpeakers();
	const int speakerCount = speakers.GetCount();
	for(i=0; i<speakerCount; i++){
		item.TakeOver(new gdeWPSTIMOCSpeaker(GetTree(), pObjectClass, speakers.GetAt(i), i));
		AppendModel(item);
	}
	
	const gdeOCWorldList &worlds = pObjectClass->GetWorlds();
	const int worldCount = worlds.GetCount();
	for(i=0; i<worldCount; i++){
		item.TakeOver(new gdeWPSTIMOCWorld(GetTree(), pObjectClass, worlds.GetAt(i), i));
		AppendModel(item);
	}
}
