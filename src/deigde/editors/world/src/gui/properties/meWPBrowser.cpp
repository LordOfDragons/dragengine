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

#include "meWPBrowser.h"
#include "meWPBrowserListener.h"
#include "meWindowProperties.h"
#include "../meWindowMain.h"
#include "../../undosys/properties/decal/meUDecalSkin.h"
#include "../../undosys/properties/object/meUSetObjectClass.h"
#include "../../undosys/properties/object/property/meUObjectAddProperty.h"
#include "../../undosys/properties/object/property/meUObjectSetProperty.h"
#include "../../undosys/properties/object/texture/meUObjectTextureSetSkin.h"
#include "../../undosys/properties/object/texture/meUObjectAddTexture.h"
#include "../../utils/meHelpers.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"
#include "../../world/decal/meDecal.h"
#include "../../world/decal/meDecal.h"
#include "../../world/decal/meDecalSelection.h"
#include "../../world/object/meObject.h"
#include "../../world/object/meObject.h"
#include "../../world/object/meObjectSelection.h"
#include "../../world/object/texture/meObjectTexture.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGDCategory.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gamedefinition/property/igdeGDProperty.h>
#include <deigde/gamedefinition/skin/igdeGDSkin.h>
#include <deigde/gamedefinition/skin/igdeGDSkinManager.h>
#include <deigde/gamedefinition/sky/igdeGDSky.h>
#include <deigde/gamedefinition/sky/igdeGDSkyManager.h>
#include <deigde/gamedefinition/preview/igdeGDPreviewManager.h>
#include <deigde/gamedefinition/visitor/igdeGDAddToListVisitor.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeIconListBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/browse/igdeBrowseItemGDPreviewListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeIconListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeTreeListListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerSplitted.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCommand.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/model/igdeTreeItem.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/gui/wrapper/igdeWSky.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>



// Actions
///////////

namespace{

class cComboType : public igdeComboBoxListener{
	meWPBrowser &pPanel;
	
public:
	typedef deTObjectReference<cComboType> Ref;
	cComboType(meWPBrowser &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		if(pPanel.GetWorld()){
			pPanel.UpdateCategoryList();
			pPanel.UpdateItemList();
		}
	}
};

class cActionSelectionMode : public igdeAction{
	meWPBrowser &pPanel;
	const meWPBrowser::eSelectionMode pMode;
	
public:
	typedef deTObjectReference<cActionSelectionMode> Ref;
	cActionSelectionMode(meWPBrowser &panel, meWPBrowser::eSelectionMode mode,
	const char *text, igdeIcon *icon, const char *description) :
		igdeAction(text, icon, description), pPanel(panel), pMode(mode){}
	
	void OnAction() override{
		pPanel.SetSelectionMode(pMode);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld());
		SetSelected(pPanel.GetSelectionMode() == pMode);
	}
};

class cTreeCategories : public igdeTreeListListener{
	meWPBrowser &pPanel;
	
public:
	typedef deTObjectReference<cTreeCategories> Ref;
	cTreeCategories(meWPBrowser &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeTreeList*){
		if(pPanel.GetWorld()){
			pPanel.UpdateItemList();
		}
	}
};

class cTextFilter : public igdeTextFieldListener{
	meWPBrowser &pPanel;
	
public:
	typedef deTObjectReference<cTextFilter> Ref;
	cTextFilter(meWPBrowser &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField*){
		pPanel.UpdateItemList();
	}
	
	virtual void OnTextChanging(igdeTextField*){
		pPanel.UpdateItemList();
	}
};

class cActionSetSkinObjTex : public igdeAction{
	meWPBrowser &pPanel;
	const decString pName;
	
public:
	typedef deTObjectReference<cActionSetSkinObjTex> Ref;
	cActionSetSkinObjTex(meWPBrowser &panel, const decString &name) : igdeAction(
		decString("Active Object Set Texture: ") + name, nullptr,
		decString("Set texture '") + name + "' of active object"), pPanel(panel), pName(name){}
	
	void OnAction() override{
		igdeGDSkin * const gdskin = pPanel.GetSelectedSkin();
		if(!gdskin){
			return;
		}
		
		meObject * const object = pPanel.GetWorld()->GetSelectionObject().GetActive();
		if(!object){
			return;
		}
		
		meObjectTexture *texture = object->GetTextureNamed(pName);
		const decString &newskin = gdskin->GetPath();
		igdeUndo::Ref undo;
		
		if(texture){
			if(newskin.Equals(texture->GetSkinPath())){
				return;
			}
			
			undo = meUObjectTextureSetSkin::Ref::New(texture, newskin);
			
		}else{
			const meObjectTexture::Ref refTexture(meObjectTexture::Ref::New(object->GetEnvironment(), pName));
			texture = refTexture;
			texture->SetSkinPath(newskin);
			
			undo = meUObjectAddTexture::Ref::New(object, texture);
		}
		pPanel.GetWorld()->GetUndoSystem()->Add(undo);
	}
};

class cActionSetSkinObjProp : public igdeAction{
	meWPBrowser &pPanel;
	const decString pName;
	
public:
	typedef deTObjectReference<cActionSetSkinObjProp> Ref;
	cActionSetSkinObjProp(meWPBrowser &panel, const decString &name) : igdeAction(
		decString("Active Object Set Property: ") + name, nullptr,
		decString("Set property '") + name + "' of active object"), pPanel(panel), pName(name){}
	
	void OnAction() override{
		igdeGDSkin * const gdskin = pPanel.GetSelectedSkin();
		if(!gdskin){
			return;
		}
		
		meObject * const object = pPanel.GetWorld()->GetSelectionObject().GetActive();
		if(!object){
			return;
		}
		
		const decString &newValue = gdskin->GetPath();
		igdeUndo::Ref undo;
		
		if(object->GetProperties().Has(pName)){
			const decString &oldValue = object->GetProperties().GetAt(pName);
			if(newValue == oldValue){
				return;
			}
			
			undo = meUObjectSetProperty::Ref::New(object, pName, oldValue, newValue);
			
		}else{
			undo = meUObjectAddProperty::Ref::New(object, pName, newValue);
		}
		pPanel.GetWorld()->GetUndoSystem()->Add(undo);
	}
};

class cListItems : public igdeIconListBoxListener{
	meWPBrowser &pPanel;
	
public:
	typedef deTObjectReference<cListItems> Ref;
	cListItems(meWPBrowser &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeIconListBox*){
		pPanel.CurrentItemChanged();
	}
	
	virtual void OnDoubleClickItem(igdeIconListBox*, int){
		if(!pPanel.GetWorld()){
			return;
		}
		
		switch(pPanel.GetPreviewItemType()){
		case meWPBrowser::epitObjectClass:
			pPanel.GetActionSetClass()->OnAction();
			break;
			
		case meWPBrowser::epitSkin:
			switch(pPanel.GetWorld()->GetGuiParameters().GetElementMode()){
			case meWorldGuiParameters::eemObject:
				pPanel.GetActionSetSkin()->OnAction();
				break;
				
			case meWorldGuiParameters::eemDecal:
				pPanel.GetActionSetDecal()->OnAction();
				break;
				
			default:
				break;
			}
			break;
			
		case meWPBrowser::epitSky:
			break;
		}
	}
	
	virtual void AddContextMenuEntries(igdeIconListBox*, igdeMenuCascade &menu){
		if(!pPanel.GetWorld()){
			return;
		}
		
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		
		switch(pPanel.GetPreviewItemType()){
		case meWPBrowser::epitObjectClass:
			helper.MenuCommand(menu, pPanel.GetActionSetClass());
			break;
			
		case meWPBrowser::epitSkin:
			helper.MenuCommand(menu, pPanel.GetActionSetSkin());
			
			switch(pPanel.GetWorld()->GetGuiParameters().GetElementMode()){
			case meWorldGuiParameters::eemObject:{
				const meObject * const object = pPanel.GetWorld()->GetSelectionObject().GetActive();
				if(!object){
					break;
				}
				
				// object textures
				decStringList names;
				object->GetModelTextureNameList(names);
				names.SortAscending();
				
				if(names.IsNotEmpty()){
					helper.MenuSeparator(menu);
					names.Visit([&](const decString &name){
						helper.MenuCommand(menu, cActionSetSkinObjTex::Ref::New(pPanel, name));
					});
				}
				
				// skin based properties
				if(object->GetGDClass()){
					names.RemoveAll();
					meHelpers::GetPatternTypePropertyNames(*object->GetGDClass(), igdeGDProperty::epptSkin, names);
					names.SortAscending();
					
					if(names.IsNotEmpty()){
						helper.MenuSeparator(menu);
						names.Visit([&](const decString &name){
							helper.MenuCommand(menu, cActionSetSkinObjProp::Ref::New(pPanel, name));
						});
					}
				}
				
				}break;
				
			case meWorldGuiParameters::eemDecal:
				helper.MenuCommand(menu, pPanel.GetActionSetDecal());
				break;
				
			default:
				break;
			}
			break;
			
		case meWPBrowser::epitSky:
			helper.MenuCommand(menu, pPanel.GetActionSetSky());
			break;
		}
		
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, pPanel.GetActionPIRebuild());
		
		// view
		igdeMenuCascade::Ref menuView(igdeMenuCascade::Ref::New(helper.GetEnvironment(), "View"));
		
		helper.MenuOption(menuView, pPanel.GetActionPISizeSmall());
		helper.MenuOption(menuView, pPanel.GetActionPISizeMedium());
		helper.MenuOption(menuView, pPanel.GetActionPISizeLarge());
		
		helper.MenuSeparator(menuView);
		helper.MenuOption(menuView, pPanel.GetActionPIViewList());
		helper.MenuOption(menuView, pPanel.GetActionPIViewPreview());
		
		helper.MenuSeparator(menu);
		menu.AddChild(menuView);
	}
};


class cActionSetClass : public igdeAction{
	meWPBrowser &pPanel;
	
public:
	typedef deTObjectReference<cActionSetClass> Ref;
	cActionSetClass(meWPBrowser &panel) : igdeAction("Set Class", nullptr,
		"Set class of selected objects"), pPanel(panel){}
	
	void OnAction() override{
		const igdeGDClass * const gdclass = pPanel.GetSelectedObjectClass();
		if(!gdclass || pPanel.GetWorld()->GetGuiParameters().GetElementMode() != meWorldGuiParameters::eemObject){
			return;
		}
		
		const meObject::List &selection = pPanel.GetWorld()->GetSelectionObject().GetSelected();
		const decString &cname = gdclass->GetName();
		meObject::List list;
		
		selection.Visit([&](meObject * const object){
			if(!cname.Equals(object->GetClassName())){
				list.Add(object);
			}
		});
		
		if(list.IsEmpty()){
			return;
		}
		
		pPanel.GetWorld()->GetUndoSystem()->Add(meUSetObjectClass::Ref::New(list, cname));
	}
	
	void Update() override{
		SetEnabled(pPanel.GetSelectedObjectClass() && pPanel.GetWorld()->GetSelectionObject().GetSelected().IsNotEmpty()
			&& pPanel.GetWorld()->GetGuiParameters().GetElementMode() == meWorldGuiParameters::eemObject);
	}
};

class cActionSetSkin : public igdeAction{
	meWPBrowser &pPanel;
	
public:
	typedef deTObjectReference<cActionSetSkin> Ref;
	cActionSetSkin(meWPBrowser &panel) : igdeAction("Objects Set Active Texture Skin", nullptr,
		"Set skin of active texture of selected objects"), pPanel(panel){}
	
	void OnAction() override{
		const igdeGDSkin * const gdskin = pPanel.GetSelectedSkin();
		if(!gdskin || pPanel.GetWorld()->GetGuiParameters().GetElementMode() != meWorldGuiParameters::eemObject){
			return;
		}
		
		const meObject::List &selection = pPanel.GetWorld()->GetSelectionObject().GetSelected();
		const decString &newskin = gdskin->GetPath();
		meObjectTexture::List list;
		
		selection.Visit([&](meObject * const object){
			meObjectTexture * const texture = object->GetActiveTexture();
			if(texture && newskin != texture->GetSkinPath()){
				list.Add(texture);
			}
		});
		
		if(list.IsEmpty()){
			return;
		}
		
		pPanel.GetWorld()->GetUndoSystem()->Add(meUObjectTextureSetSkin::Ref::New(list, newskin));
	}
	
	void Update() override{
		SetEnabled(pPanel.GetSelectedSkin() && pPanel.GetWorld()->GetSelectionObject().GetSelected().IsNotEmpty()
			&& pPanel.GetWorld()->GetGuiParameters().GetElementMode() == meWorldGuiParameters::eemObject);
	}
};

class cActionSetDecal : public igdeAction{
	meWPBrowser &pPanel;
	
public:
	typedef deTObjectReference<cActionSetDecal> Ref;
	cActionSetDecal(meWPBrowser &panel) : igdeAction("Decals Set Skin", nullptr,
		"Set skin of selected decals"), pPanel(panel){}
	
	void OnAction() override{
		const igdeGDSkin * const gdskin = pPanel.GetSelectedSkin();
		if(!gdskin || pPanel.GetWorld()->GetGuiParameters().GetElementMode() != meWorldGuiParameters::eemDecal){
			return;
		}
		
		const meDecal::List &selection = pPanel.GetWorld()->GetSelectionDecal().GetSelected();
		const decString &newskin = gdskin->GetPath();
		meDecal::List list;
		
		selection.Visit([&](meDecal * const decal){
			if(!newskin.Equals(decal->GetSkinPath())){
				list.Add(decal);
			}
		});
		
		if(list.IsEmpty()){
			return;
		}
		
		pPanel.GetWorld()->GetUndoSystem()->Add(meUDecalSkin::Ref::New(list, newskin));
	}
	
	void Update() override{
		SetEnabled(pPanel.GetSelectedSkin() && pPanel.GetWorld()->GetSelectionDecal().GetSelected().IsNotEmpty()
			&& pPanel.GetWorld()->GetGuiParameters().GetElementMode() == meWorldGuiParameters::eemDecal);
	}
};

class cActionSetSky : public igdeAction{
	meWPBrowser &pPanel;
	
public:
	typedef deTObjectReference<cActionSetSky> Ref;
	cActionSetSky(meWPBrowser &panel) : igdeAction("Set Sky", nullptr,
		"Change preview sky of world to the selected sky"), pPanel(panel){}
	
	void OnAction() override{
		igdeGDSky * const gdsky = pPanel.GetSelectedSky();
		if(gdsky){
			pPanel.GetWorld()->GetSky()->SetGDSky(gdsky);
		}
	}
	
	void Update() override{
		SetEnabled(pPanel.GetSelectedSky());
	}
};

class cActionPreviewSize : public igdeAction{
	meWPBrowser &pPanel;
	const meWPBrowser::ePreviewSize pSize;
	
public:
	typedef deTObjectReference<cActionPreviewSize> Ref;
	cActionPreviewSize(meWPBrowser &panel, meWPBrowser::ePreviewSize size,
	const char *text, igdeIcon *icon, const char *description) :
		igdeAction(text, icon, description), pPanel(panel), pSize(size){}
	
	void OnAction() override{
		pPanel.SetPreviewSize(pSize);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld());
		SetSelected(pPanel.GetPreviewSize() == pSize);
	}
};

class cActionViewMode : public igdeAction{
	meWPBrowser &pPanel;
	const meWPBrowser::eViewModes pMode;
	
public:
	typedef deTObjectReference<cActionViewMode> Ref;
	cActionViewMode(meWPBrowser &panel, meWPBrowser::eViewModes mode,
	const char *text, igdeIcon *icon, const char *description) :
		igdeAction(text, icon, description), pPanel(panel), pMode(mode){}
	
	void OnAction() override{
		pPanel.SetViewMode(pMode);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld());
		SetSelected(pPanel.GetViewMode() == pMode);
	}
};

class cActionPIRebuild : public igdeAction{
	meWPBrowser &pPanel;
	
public:
	typedef deTObjectReference<cActionPIRebuild> Ref;
	cActionPIRebuild(meWPBrowser &panel) : igdeAction("Rebuild Preview", nullptr,
		"Rebuild Preview"), pPanel(panel){}
	
	void OnAction() override{
		pPanel.RebuildPISelectedItem();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld() && pPanel.GetSelectedListItem());
	}
};

}



// Class meWPBrowser
//////////////////////

// Constructor, destructor
////////////////////////////

meWPBrowser::meWPBrowser(meWindowProperties &windowProperties) :
igdeContainerBox(windowProperties.GetEnvironment(), igdeContainerBox::eaY),
pWindowProperties(windowProperties),
pPreviewSize(epsLarge),
pSelectionMode(esmCategory),
pViewMode(evmPreview)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerSplitted::Ref content;
	igdeContainer::Ref groupBox, form, frameLine;
	
	
	pListener = meWPBrowserListener::Ref::New(*this);
	
	
	pActionSetClass = cActionSetClass::Ref::New(*this);
	pActionSetSkin = cActionSetSkin::Ref::New(*this);
	pActionSetDecal = cActionSetDecal::Ref::New(*this);
	pActionSetSky = cActionSetSky::Ref::New(*this);
	
	pActionPISizeSmall = cActionPreviewSize::Ref::New(*this, epsSmall,
		"Small Preview Image", nullptr, "Small Preview Image");
	pActionPISizeMedium = cActionPreviewSize::Ref::New(*this, epsMedium,
		"Medium Preview Image", nullptr, "Medium Preview Image");
	pActionPISizeLarge = cActionPreviewSize::Ref::New(*this, epsLarge,
		"Large Preview Image", nullptr, "Large Preview Image");
	
	pActionPIViewList = cActionViewMode::Ref::New(*this, evmList,
		"List Mode", nullptr, "List Mode");
	pActionPIViewPreview = cActionViewMode::Ref::New(*this, evmPreview,
		"Preview Mode", nullptr, "Preview Mode");
	
	pActionPIRebuild = cActionPIRebuild::Ref::New(*this);
	
	
	content = igdeContainerSplitted::Ref::New(env, igdeContainerSplitted::espTop,
		igdeApplication::app().DisplayScaled(400));
	AddChild(content);
	
	
	// browser
	frameLine = igdeGroupBox::Ref::New(env, "Browser:");
	frameLine.DynamicCast<igdeGroupBox>()->SetCanCollapse(false);
	frameLine.DynamicCast<igdeGroupBox>()->SetStretchLast(true);
	content->AddChild(frameLine, igdeContainerSplitted::eaSide);
	groupBox = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY, igdeContainerFlow::esLast);
	frameLine->AddChild(groupBox);
	
	frameLine = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst);
	groupBox->AddChild(frameLine);
	
	helper.ComboBox(frameLine, "Type of items to browse", pCBTypes, cComboType::Ref::New(*this));
	pCBTypes->AddItem("Object Class", nullptr, (void*)(intptr_t)epitObjectClass);
	pCBTypes->AddItem("Skin", nullptr, (void*)(intptr_t)epitSkin);
	pCBTypes->AddItem("Sky", nullptr, (void*)(intptr_t)epitSky);
	
	helper.Button(frameLine, pBtnSelByCat, cActionSelectionMode::Ref::New(*this, esmCategory,
		"Category", nullptr, "Browse by category"));
	helper.Button(frameLine, pBtnSelByFilter, cActionSelectionMode::Ref::New(*this, esmFilter,
		"Filter", nullptr, "Browse by filtering"));
	
	
	pSwitcherSelBy = igdeSwitcher::Ref::New(env);
	groupBox->AddChild(pSwitcherSelBy);
	
	
	// category tree
	helper.TreeList(pSwitcherSelBy, pTreeCategories, 10, "Categories", cTreeCategories::Ref::New(*this));
	pTreeCategories->SetDefaultSorter();
	
	
	// filter
	form = igdeContainerForm::Ref::New(env);
	pSwitcherSelBy->AddChild(form);
	
	helper.EditString(form, "Filter:", "Show items containing filter case insensitive",
		pEditFilter, cTextFilter::Ref::New(*this));
	
	
	// items
	frameLine = igdeGroupBox::Ref::New(env, "Items:");
	frameLine.DynamicCast<igdeGroupBox>()->SetCanCollapse(false);
	frameLine.DynamicCast<igdeGroupBox>()->SetStretchLast(true);
	content->AddChild(frameLine, igdeContainerSplitted::eaCenter);
	groupBox = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY, igdeContainerFlow::esFirst);
	frameLine->AddChild(groupBox);
	
	const igdeUIHelper::sColumnHeader headers[] = {
		igdeUIHelper::sColumnHeader("Name", nullptr, igdeApplication::app().DisplayScaled(200))
	};
	helper.IconListBox(groupBox, pListItems,
		igdeApplication::app().DisplayScaled(decPoint(100, 150)),
		headers, 1, "Items", cListItems::Ref::New(*this));
	pListItems->SetDefaultSorter();
	pListItems->SetViewMode(igdeIconListBox::evmIconVertical);
	
	helper.EditString(groupBox, "Item information", pEditInfos, 50, 5, {});
	pEditInfos->SetEditable(false);
	
	OnGameDefinitionChanged();
}

meWPBrowser::~meWPBrowser(){
	SetWorld(nullptr);
}



// Management
///////////////

void meWPBrowser::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	if(pWorld){
		pWorld->RemoveNotifier(pListener);
	}
	
	pWorld = world;
	
	if(world){
		world->AddNotifier(pListener);
	}
	
	CurrentItemChanged();
}



void meWPBrowser::UpdateCategoryList(){
	const igdeGameDefinition * const gameDefinition = GetGameDefinition();
	const decString selection(GetSelectedCategory() ? GetSelectedCategory()->GetFullPathString() : decString());
	
	pTreeCategories->RemoveAllItems();
	
	if(!gameDefinition){
		return;
	}
	
	igdeGDCategory *categories = nullptr;
	
	switch(GetPreviewItemType()){
	case epitObjectClass:
		categories = gameDefinition->GetClassManager()->GetCategories();
		break;
		
	case epitSkin:
		categories = gameDefinition->GetSkinManager()->GetCategories();
		break;
		
	case epitSky:
		categories = gameDefinition->GetSkyManager()->GetCategories();
		break;
	}
	
	UpdateCategoryListWith(categories);
	pTreeCategories->SortAllItems();
	if(!selection.IsEmpty()){
		SelectCategory(categories->GetCategoryWithPath(decPath::CreatePathNative(selection)));
	}
}

void meWPBrowser::UpdateCategoryListWith(igdeGDCategory *category){
	if(!category){
		DETHROW(deeInvalidParam);
	}
	
	pTreeCategories->AppendItem(nullptr, "< No Category >");
	
	category->GetCategories().Visit([&](igdeGDCategory * const child){
		if(!child->GetHidden()){
			AddCategoryToList(child, nullptr);
		}
	});
}

void meWPBrowser::AddCategoryToList(igdeGDCategory *category, igdeTreeItem *parent){
	if(!category){
		DETHROW(deeInvalidParam);
	}
	
	igdeTreeItem::Ref item(igdeTreeItem::Ref::New(category->GetName(), category));
	pTreeCategories->AppendItem(parent, item);
	
	category->GetCategories().Visit([&](igdeGDCategory * const child){
		if(!child->GetHidden()){
			AddCategoryToList(child, item);
		}
	});
}

void meWPBrowser::UpdateItemList(){
	const igdeGameDefinition * const gameDefinition = GetGameDefinition();
	void * const selection = pListItems->GetSelectedItem() ? pListItems->GetSelectedItem()->GetData() : nullptr;
	
	pListItems->RemoveAllItems();
	
	if(gameDefinition){
		igdeGDAddToListVisitor visitor(GetEnvironment(), pListItems, GetPreviewIconSize());
		
		switch(pSelectionMode){
		case esmCategory:
			switch(GetPreviewItemType()){
			case epitObjectClass:
				gameDefinition->GetClassManager()->VisitClassesMatchingCategory(visitor, GetSelectedCategory());
				break;
				
			case epitSkin:
				gameDefinition->GetSkinManager()->VisitSkinsMatchingCategory(visitor, GetSelectedCategory());
				break;
				
			case epitSky:
				gameDefinition->GetSkyManager()->VisitSkiesMatchingCategory(visitor, GetSelectedCategory());
				break;
			}
			break;
			
		case esmFilter:
			switch(GetPreviewItemType()){
			case epitObjectClass:
				gameDefinition->GetClassManager()->VisitMatchingFilter(visitor, pEditFilter->GetText());
				break;
				
			case epitSkin:
				gameDefinition->GetSkinManager()->VisitMatchingFilter(visitor, pEditFilter->GetText());
				break;
				
			case epitSky:
				gameDefinition->GetSkyManager()->VisitMatchingFilter(visitor, pEditFilter->GetText());
				break;
			}
			break;
		}
	}
	
	pListItems->SortItems();
	
	pListItems->SetSelectionWithData(selection);
	if(!pListItems->GetSelectedItem() && pListItems->GetItems().IsNotEmpty()){
		pListItems->SetSelection(0);
	}
}

void meWPBrowser::RebuildPISelectedItem(){
	igdeListItem * const item = GetSelectedListItem();
	if(!item){
		return;
	}
	
	igdeGDPreviewManager &pvmgr = *GetEnvironment().GetGDPreviewManager();
	igdeGDAddToListVisitor visitor(GetEnvironment(), pListItems, GetPreviewIconSize());
	
	igdeIcon::Ref icon;
	const int iconSize = GetPreviewIconSize();
	icon = igdeIcon::Ref::New(*pvmgr.GetImageCreating(), iconSize, iconSize);
	
	const igdeBrowseItemGDPreviewListener::Ref listener(igdeBrowseItemGDPreviewListener::Ref::New(pListItems, item, iconSize));
	
	switch(GetPreviewItemType()){
	case meWPBrowser::epitObjectClass:{
		igdeGDClass * const gdclass = GetSelectedObjectClass();
		if(!gdclass){
			break;
		}
		
		pvmgr.ClearPreviewObjectClass(gdclass);
		item->SetIcon(icon);
		pListItems->ItemChangedAt(pListItems->GetSelection());
		pvmgr.CreatePreviewObjectClass(gdclass, listener);
		}break;
		
	case meWPBrowser::epitSkin:{
		igdeGDSkin * const gdskin = GetSelectedSkin();
		if(!gdskin){
			break;
		}
		
		pvmgr.ClearPreviewSkin(gdskin);
		item->SetIcon(icon);
		pListItems->ItemChangedAt(pListItems->GetSelection());
		pvmgr.CreatePreviewSkin(gdskin, listener);
		}break;
		
	case meWPBrowser::epitSky:
		break;
	}
}

void meWPBrowser::CurrentItemChanged(){
	const igdeGameDefinition * const gameDefinition = GetGameDefinition();
	if(!gameDefinition){
		pEditInfos->ClearText();
		return;
	}
	
	const igdeListItem * const selection = pListItems->GetSelectedItem();
	decString text;
	
	if(pWorld){
		switch(GetPreviewItemType()){
		case epitObjectClass:
			if(selection){
				const igdeGDClass * const gdclass = (igdeGDClass*)selection->GetData();
				pWorld->GetGuiParameters().SetBrowseClass(gdclass->GetName());
				text.Format("%s:\n%s", gdclass->GetName().GetString(), gdclass->GetDescription().GetString());
				
			}else{
				pWorld->GetGuiParameters().SetBrowseClass("");
			}
			break;
			
		case epitSkin:
			if(selection){
				const igdeGDSkin * const gdskin = (igdeGDSkin*)selection->GetData();
				pWorld->GetGuiParameters().SetBrowseSkin(gdskin->GetPath());
				text.Format("%s:\n%s", gdskin->GetName().GetString(), gdskin->GetDescription().GetString());
				
			}else{
				pWorld->GetGuiParameters().SetBrowseSkin("");
			}
			break;
			
		case epitSky:
			break;
		}
	}
	
	pEditInfos->SetText(text);
}

meWPBrowser::ePreviewItemType meWPBrowser::GetPreviewItemType() const{
	const igdeListItem * const selection = pCBTypes->GetSelectedItem();
	return selection ? (ePreviewItemType)(intptr_t)selection->GetData() : epitObjectClass;
}

void meWPBrowser::SetPreviewItemType(ePreviewItemType type){
	pCBTypes->SetSelectionWithData((void*)(intptr_t)type);
}

igdeGDCategory *meWPBrowser::GetSelectedCategory() const{
	const igdeTreeItem * const selection = pTreeCategories->GetSelection();
	return selection ? (igdeGDCategory*)selection->GetData() : nullptr;
}

void meWPBrowser::SelectCategory(igdeGDCategory *category){
	pTreeCategories->SetSelectionWithData(category);
	if(!pTreeCategories->GetSelection() && pTreeCategories->GetFirstChild()){
		pTreeCategories->SetSelection(pTreeCategories->GetFirstChild());
	}
}

void meWPBrowser::SetSelectionMode(eSelectionMode mode){
	if(mode == pSelectionMode){
		return;
	}
	
	pSelectionMode = mode;
	
	switch(mode){
	case esmCategory:
		pSwitcherSelBy->SetCurrent(0);
		UpdateItemList();
		break;
		
	case esmFilter:
		pSwitcherSelBy->SetCurrent(1);
		UpdateItemList();
		break;
	}
}

igdeListItem *meWPBrowser::GetSelectedListItem() const{
	return pWorld ? pListItems->GetSelectedItem() : nullptr;
}

igdeGDClass *meWPBrowser::GetSelectedObjectClass() const{
	return pWorld && GetPreviewItemType() == epitObjectClass && pListItems->GetSelectedItem()
		? (igdeGDClass*)pListItems->GetSelectedItem()->GetData() : nullptr;
}

igdeGDSkin *meWPBrowser::GetSelectedSkin() const{
	return pWorld && GetPreviewItemType() == epitSkin && pListItems->GetSelectedItem()
		? (igdeGDSkin*)pListItems->GetSelectedItem()->GetData() : nullptr;
}

igdeGDSky *meWPBrowser::GetSelectedSky() const{
	return pWorld && GetPreviewItemType() == epitSky && pListItems->GetSelectedItem()
		? (igdeGDSky*)pListItems->GetSelectedItem()->GetData() : nullptr;
}

void meWPBrowser::SetPreviewSize(ePreviewSize size){
	if(size == pPreviewSize){
		return;
	}
	
	pPreviewSize = size;
	UpdateItemList();
}

void meWPBrowser::SetViewMode(eViewModes viewMode){
	if(viewMode == pViewMode){
		return;
	}
	
	pViewMode = viewMode;
	
	switch(viewMode){
	case evmList:
		pListItems->SetViewMode(igdeIconListBox::evmList);
		break;
		
	case evmPreview:
		pListItems->SetViewMode(igdeIconListBox::evmIconVertical);
		break;
		
	default:
		pListItems->SetViewMode(igdeIconListBox::evmIconVertical);
	}
}



void meWPBrowser::OnGameProjectChanged(){
	OnGameDefinitionChanged();
}

void meWPBrowser::OnGameDefinitionChanged(){
	decString selection;
	
	switch(GetPreviewItemType()){
	case epitObjectClass:
		if(GetSelectedObjectClass()){
			selection = GetSelectedObjectClass()->GetName();
		}
		break;
		
	case epitSkin:
		if(GetSelectedSkin()){
			selection = GetSelectedSkin()->GetPath();
		}
		break;
		
	case epitSky:
		if(GetSelectedSky()){
			selection = GetSelectedSky()->GetPath();
		}
		break;
	}
	
	UpdateCategoryList();
	UpdateItemList();
	
	if(!selection.IsEmpty() && GetGameDefinition()){
		switch(GetPreviewItemType()){
		case epitObjectClass:
			pListItems->SetSelectionWithData(GetGameDefinition()->GetClassManager()->GetNamed(selection));
			break;
			
		case epitSkin:
			pListItems->SetSelectionWithData(GetGameDefinition()->GetSkinManager()->GetSkinWithPath(selection));
			break;
			
		case epitSky:
			pListItems->SetSelectionWithData(GetGameDefinition()->GetSkyManager()->GetSkyWithPath(selection));
			break;
		}
	}
	
	if(!pListItems->GetSelectedItem() && pListItems->GetItems().IsNotEmpty()){
		pListItems->SetSelection(0);
	}
}



void meWPBrowser::SelectObjectClass(igdeGDClass *gdclass){
	if(!gdclass){
		DETHROW(deeInvalidParam);
	}
	
	// find object class category
	const igdeGameDefinition * const gameDefinition = GetGameDefinition();
	if(!gameDefinition){
		return;
	}
	
	igdeGDCategory * const category = gameDefinition->GetClassManager()->GetCategories()
		->GetCategoryWithPath(decPath::CreatePathUnix(gdclass->GetCategory()));
	
	// select category
	SetPreviewItemType(epitObjectClass);
	SelectCategory(category);
	
	// select object class
	pListItems->SetSelectionWithData(gdclass);
}

void meWPBrowser::SelectSkin(igdeGDSkin *gdskin){
	if(!gdskin){
		DETHROW(deeInvalidParam);
	}
	
	// find skin category
	const igdeGameDefinition * const gameDefinition = GetGameDefinition();
	if(!gameDefinition){
		return;
	}
	
	igdeGDCategory * const category = gameDefinition->GetSkinManager()->GetCategories()
		->GetCategoryWithPath(decPath::CreatePathUnix(gdskin->GetCategory()));
	
	// select category
	SetPreviewItemType(epitSkin);
	SelectCategory(category);
	
	// select object class
	pListItems->SetSelectionWithData(gdskin);
}

void meWPBrowser::SelectSky(igdeGDSky *gdsky){
	if(!gdsky){
		DETHROW(deeInvalidParam);
	}
	
	// find sky category
	const igdeGameDefinition * const gameDefinition = GetGameDefinition();
	if(!gameDefinition){
		return;
	}
	
	igdeGDCategory * const category = gameDefinition->GetSkyManager()->GetCategories()
		->GetCategoryWithPath(decPath::CreatePathUnix(gdsky->GetCategory()));
	
	// select category
	SetPreviewItemType(epitSky);
	SelectCategory(category);
	
	// select object class
	pListItems->SetSelectionWithData(gdsky);
}



int meWPBrowser::GetPreviewIconSize() const{
	switch(pPreviewSize){
	case epsLarge:
		return 128;
		
	case epsMedium:
		return 96;
		
	case epsSmall:
		return 64;
	}
	
	return 128;
}
