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

#include "meWPHeightTerrain.h"
#include "meWPHeightTerrainListener.h"
#include "meWindowProperties.h"
#include "../meWindowMain.h"
#include "../../loadsave/meSaveSupport.h"
#include "../../undosys/gui/heightterrain/meUHTImportHeightImage.h"
#include "../../undosys/gui/heightterrain/meUHTImportVisibilityImage.h"
#include "../../undosys/properties/heightterrain/meUHTSetPathHT.h"
#include "../../undosys/properties/heightterrain/meUHTSetPathHI.h"
#include "../../undosys/properties/heightterrain/meUHTSetPathVI.h"
#include "../../undosys/properties/heightterrain/meUHTSetPathPFCache.h"
#include "../../undosys/properties/heightterrain/meUHTSetSectorSize.h"
#include "../../undosys/properties/heightterrain/meUHTSetSectorResolution.h"
#include "../../undosys/properties/heightterrain/meUHTSetBaseHeight.h"
#include "../../undosys/properties/heightterrain/meUHTSetHeightScaling.h"
#include "../../undosys/properties/heightterrain/texture/meUHTAddTexture.h"
#include "../../undosys/properties/heightterrain/texture/meUHTRemoveTexture.h"
#include "../../undosys/properties/heightterrain/texture/meUHTSetTexSkin.h"
#include "../../undosys/properties/heightterrain/texture/meUHTSetTexMask.h"
#include "../../undosys/properties/heightterrain/texture/meUHTSetTexUVOffset.h"
#include "../../undosys/properties/heightterrain/texture/meUHTSetTexUVScaling.h"
#include "../../undosys/properties/heightterrain/texture/meUHTSetTexUVRotation.h"
#include "../../undosys/properties/heightterrain/texture/meUHTTexSetTypeNum.h"
#include "../../undosys/properties/heightterrain/vegetation/meUHTVLayerAdd.h"
#include "../../undosys/properties/heightterrain/vegetation/meUHTVLayerRemove.h"
#include "../../undosys/properties/heightterrain/vegetation/meUHTVLayerUp.h"
#include "../../undosys/properties/heightterrain/vegetation/meUHTVLayerDown.h"
#include "../../undosys/properties/heightterrain/vegetation/meUHTVVariationAdd.h"
#include "../../undosys/properties/heightterrain/vegetation/meUHTVVariationRemove.h"
#include "../../undosys/properties/heightterrain/vegetation/meUHTVVarSetModel.h"
#include "../../undosys/properties/heightterrain/vegetation/meUHTVVarSetSkin.h"
#include "../../undosys/properties/heightterrain/vegetation/meUHTVVarSetRotPerForce.h"
#include "../../undosys/properties/heightterrain/vegetation/meUHTVVarSetRestitution.h"
#include "../../undosys/properties/heightterrain/navspace/meUHTNavSpaceAdd.h"
#include "../../undosys/properties/heightterrain/navspace/meUHTNavSpaceRemove.h"
#include "../../undosys/properties/heightterrain/navspace/meUHTNavSpaceSetLayer.h"
#include "../../undosys/properties/heightterrain/navspace/meUHTNavSpaceSetSnapDist.h"
#include "../../undosys/properties/heightterrain/navspace/meUHTNavSpaceSetSnapAngle.h"
#include "../../undosys/properties/heightterrain/navspace/type/meUHTNavSpaceTypeAdd.h"
#include "../../undosys/properties/heightterrain/navspace/type/meUHTNavSpaceTypeRemove.h"
#include "../../undosys/properties/heightterrain/navspace/type/meUHTNavSpaceTypeSetName.h"
#include "../../undosys/properties/heightterrain/navspace/type/meUHTNavSpaceTypeSetColor.h"
#include "../../undosys/properties/heightterrain/navspace/type/meUHTNavSpaceTypeSetType.h"
#include "../../undosys/properties/heightterrain/navspace/type/face/meUHTNavSpaceFaceAdd.h"
#include "../../undosys/properties/heightterrain/navspace/type/face/meUHTNavSpaceFaceRemove.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"
#include "../../world/heightterrain/meHTVegetationLayer.h"
#include "../../world/heightterrain/meHTVVariation.h"
#include "../../world/heightterrain/rules/meHTVRuleResult.h"
#include "../../world/terrain/meHeightTerrain.h"
#include "../../world/terrain/meHeightTerrainSector.h"
#include "../../world/terrain/meHeightTerrainTexture.h"
#include "../../world/terrain/meHeightTerrainNavSpace.h"
#include "../../world/terrain/meHeightTerrainNavSpaceType.h"
#include "../../world/terrain/meHeightTerrainNavSpaceFace.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/property/igdeGDProperty.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeToggleButton.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/composed/igdeEditSliderTextListener.h>
#include <deigde/gui/composed/igdeEditVector2.h>
#include <deigde/gui/composed/igdeEditVector2Listener.h>
#include <deigde/gui/event/igdeColorBoxListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeSpinTextFieldListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCommand.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>



// Actions
///////////

static const decString vEmptyString;

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	meWPHeightTerrain &pPanel;
	
public:
	cBaseTextFieldListener(meWPHeightTerrain &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(textField, world)));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld *world) = 0;
};

class cBaseAction : public igdeAction{
protected:
	meWPHeightTerrain &pPanel;
	
public:
	cBaseAction(meWPHeightTerrain &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(world)));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(meWorld *world) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	meWPHeightTerrain &pPanel;
	
public:
	cBaseComboBoxListener(meWPHeightTerrain &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(comboBox, world)));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, meWorld *world) = 0;
};

class cBaseEditVector2Listener : public igdeEditVector2Listener{
protected:
	meWPHeightTerrain &pPanel;
	
public:
	cBaseEditVector2Listener(meWPHeightTerrain &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector2 *editVector2){
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(editVector2->GetVector2(), world)));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(const decVector2 &vector, meWorld *world) = 0;
};

class cBaseEditPathListener : public igdeEditPathListener{
protected:
	meWPHeightTerrain &pPanel;
	
public:
	cBaseEditPathListener(meWPHeightTerrain &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(editPath, world)));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeEditPath *editPath, meWorld *world) = 0;
};

class cBaseEditSliderTextListener : public igdeEditSliderTextListener{
protected:
	meWPHeightTerrain &pPanel;
	
public:
	cBaseEditSliderTextListener(meWPHeightTerrain &panel) : pPanel(panel){}
	
	virtual void OnSliderTextValueChanged(igdeEditSliderText *sliderText){
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(sliderText, world)));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeEditSliderText *sliderText, meWorld *world) = 0;
};


class cEditPathHT : public cBaseEditPathListener{
public:
	cEditPathHT(meWPHeightTerrain &panel) : cBaseEditPathListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeEditPath *editPath, meWorld *world){
		return world->GetHeightTerrain()->GetPathHT() != editPath->GetPath()
			? new meUHTSetPathHT(world, world->GetHeightTerrain(), editPath->GetPath()) : NULL;
	}
};

class cEditSectorSize : public cBaseTextFieldListener{
public:
	cEditSectorSize(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld *world){
		const float value = textField->GetFloat();
		return fabsf(value - world->GetHeightTerrain()->GetSectorSize()) > FLOAT_SAFE_EPSILON
			? new meUHTSetSectorSize(world, world->GetHeightTerrain(), value) : NULL;
	}
};

class cEditSectorResolution : public cBaseTextFieldListener{
public:
	cEditSectorResolution(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld *world){
		const int value = textField->GetInteger();
		return value != world->GetHeightTerrain()->GetSectorResolution()
			? new meUHTSetSectorResolution(world, world->GetHeightTerrain(), value) : NULL;
	}
};

class cEditBaseHeight : public cBaseTextFieldListener{
public:
	cEditBaseHeight(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld *world){
		const float value = textField->GetFloat();
		return fabsf(value - world->GetHeightTerrain()->GetBaseHeight()) > FLOAT_SAFE_EPSILON
			? new meUHTSetBaseHeight(world, world->GetHeightTerrain(), value) : NULL;
	}
};

class cEditHeightScale : public cBaseTextFieldListener{
public:
	cEditHeightScale(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld *world){
		const float value = textField->GetFloat();
		return fabsf(value - world->GetHeightTerrain()->GetHeightScaling()) > FLOAT_SAFE_EPSILON
			? new meUHTSetHeightScaling(world, world->GetHeightTerrain(), value) : NULL;
	}
};


class cEditPathHeightImage : public cBaseEditPathListener{
public:
	cEditPathHeightImage(meWPHeightTerrain &panel) : cBaseEditPathListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeEditPath *editPath, meWorld *world){
		meHeightTerrainSector * const sector = pPanel.GetSector();
		if(!sector || sector->GetPathHeightImage() == editPath->GetPath()){
			return NULL;
		}
		
		if(pPanel.GetEngine()->GetVirtualFileSystem()->ExistsFile(decPath::CreatePathUnix(editPath->GetPath()))){
			igdeCommonDialogs::eButton answer = igdeCommonDialogs::Question(&pPanel,
				igdeCommonDialogs::ebsYesNoCancel, "Set Height Image Path",
				"Image to save the height image to exists already.\n"
				"Do you want to import image before changing path?\n"
				"[YES] Import image then change path (2 undo actions).\n"
				"[NO] Change path and while saving image will be overwritten.\n"
				"[CANCEL] Do no import image nor change path.");
			
			if(answer == igdeCommonDialogs::ebCancel){
				return NULL;
				
			}else if(answer == igdeCommonDialogs::ebYes){
				const int resolution = world->GetHeightTerrain()->GetSectorResolution();
				deImage::Ref image(deImage::Ref::New(
					 pPanel.GetEngine()->GetImageManager()->LoadImage(editPath->GetPath(), "/")));
				
				if(image->GetComponentCount() != 1){
					image = NULL;
					igdeCommonDialogs::Error(&pPanel, "Import Height Image",
						"The height image does not have exactly 1 color channel. Only\n"
						"images with 1 color channel can be used for height terrains.");
				}
				
				if(image && (image->GetWidth() != resolution || image->GetHeight() != resolution)){
					image = NULL;
					igdeCommonDialogs::Error(&pPanel, "Import Height Image",
						"The image does not match the height image dimension set in the height terrain.");
					// NOTE allow the user to scale the height image... maybe.. scaling is tricky
				}
				
				if(image){
					world->GetUndoSystem()->Add(
						meUHTImportHeightImage::Ref::NewWith(world, sector, image));
				}
			}
		}
		
		return new meUHTSetPathHI(world, sector, editPath->GetPath());
	}
};

class cEditPathVisibilityImage : public cBaseEditPathListener{
public:
	cEditPathVisibilityImage(meWPHeightTerrain &panel) : cBaseEditPathListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeEditPath *editPath, meWorld *world){
		meHeightTerrainSector * const sector = pPanel.GetSector();
		if(!sector || sector->GetPathVisibilityImage() == editPath->GetPath()){
			return NULL;
		}
		
		if(pPanel.GetEngine()->GetVirtualFileSystem()->ExistsFile(decPath::CreatePathUnix(editPath->GetPath()))){
			igdeCommonDialogs::eButton answer = igdeCommonDialogs::Question(&pPanel,
				igdeCommonDialogs::ebsYesNoCancel, "Set Visibility Image Path",
				"Image to save the visibility image to exists already.\n"
				"Do you want to import image before changing path?\n"
				"[YES] Import image then change path (2 undo actions).\n"
				"[NO] Change path and while saving image will be overwritten.\n"
				"[CANCEL] Do no import image nor change path.");
			
			if(answer == igdeCommonDialogs::ebCancel){
				return NULL;
				
			}else if(answer == igdeCommonDialogs::ebYes){
				const int resolution = world->GetHeightTerrain()->GetSectorResolution();
				deImage::Ref image;
				
				image.TakeOver(pPanel.GetEngine()->GetImageManager()->LoadImage(editPath->GetPath(), "/"));
				
				if(image->GetComponentCount() != 1){
					image = NULL;
					igdeCommonDialogs::Error(&pPanel, "Import Visibility Image",
						"The visibility image does not have exactly 1 color channel. Only\n"
						"images with 1 color channel can be used for visibility terrains.");
				}
				
				if(image && (image->GetWidth() != resolution || image->GetHeight() != resolution)){
					image = NULL;
					igdeCommonDialogs::Error(&pPanel, "Import Visibility Image",
						"The image does not match the visibility image dimension set in the visibility terrain.");
					// NOTE allow the user to scale the visibility image... maybe.. scaling is tricky
				}
				
				if(image){
					meUHTImportVisibilityImage::Ref undo(meUHTImportVisibilityImage::Ref::NewWith(
						world, sector, image));
					world->GetUndoSystem()->Add(undo);
				}
			}
		}
		
		return new meUHTSetPathHI(world, sector, editPath->GetPath());
	}
};


class cComboTexture : public cBaseComboBoxListener{
public:
	cComboTexture(meWPHeightTerrain &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, meWorld*){
		meHeightTerrainSector * const sector = pPanel.GetSector();
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(sector){
			sector->SetActiveTexture(selection ? (meHeightTerrainTexture*)selection->GetData() : NULL);
		}
		return NULL;
	}
};

class cActionMenuTexture : public igdeActionContextMenu{
	meWPHeightTerrain &pPanel;
	
public:
	cActionMenuTexture(meWPHeightTerrain &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Texture menu"),
	pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		if(!pPanel.GetSector()){
			return;
		}
		
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, pPanel.GetActionTextureAdd());
		helper.MenuCommand(contextMenu, pPanel.GetActionTextureRemove());
	}
};

class cActionTextureAdd : public cBaseAction{
public:
	cActionTextureAdd(meWPHeightTerrain &panel) : cBaseAction(panel, "Add...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add texture"){}
	
	virtual igdeUndo *OnAction(meWorld *world){
		meHeightTerrainSector * const sector = pPanel.GetSector();
		if(!sector){
			return NULL;
		}
		
		decString name("texture");
		while(igdeCommonDialogs::GetString(&pPanel, "Add Texture", "Enter the name of the new texture", name)){
			if(sector->HasTextureNamed(name)){
				igdeCommonDialogs::Error(&pPanel, "Invalid Texture Name", "A texture with this name exists already.");
				continue;
			}
			
			const meHeightTerrainTexture::Ref texture(meHeightTerrainTexture::Ref::NewWith(world->GetEngine(), name));
			return new meUHTAddTexture(world, sector, texture);
		}
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetSector());
	}
};

class cActionTextureRemove : public cBaseAction{
public:
	cActionTextureRemove(meWPHeightTerrain &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove texture"){}
	
	virtual igdeUndo *OnAction(meWorld *world){
		meHeightTerrainTexture * const texture = pPanel.GetTexture();
		return texture ? new meUHTRemoveTexture(world, pPanel.GetSector(), texture) : NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetTexture());
	}
};

class cTextTexTypeNumber : public cBaseTextFieldListener{
public:
	cTextTexTypeNumber(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld*){
		meHeightTerrainTexture * const texture = pPanel.GetTexture();
		const int value = textField->GetInteger();
		return texture && value != texture->GetTypeNumber() ? new meUHTTexSetTypeNum(texture, value) : NULL;
	}
};

class cPathTexSkin : public cBaseEditPathListener{
public:
	cPathTexSkin(meWPHeightTerrain &panel) : cBaseEditPathListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeEditPath *editPath, meWorld *world){
		meHeightTerrainTexture * const texture = pPanel.GetTexture();
		return texture && editPath->GetPath() != texture->GetPathSkin()
			? new meUHTSetTexSkin(world, pPanel.GetSector(), texture, editPath->GetPath()) : NULL;
	}
};

class cPathTexMask : public cBaseEditPathListener{
public:
	cPathTexMask(meWPHeightTerrain &panel) : cBaseEditPathListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeEditPath *editPath, meWorld *world){
		meHeightTerrainTexture * const texture = pPanel.GetTexture();
		return texture && editPath->GetPath() != texture->GetPathMask()
			? new meUHTSetTexMask(world, pPanel.GetSector(), texture, editPath->GetPath()) : NULL;
	}
};

class cEditTexUVOffset : public cBaseEditVector2Listener{
public:
	cEditTexUVOffset(meWPHeightTerrain &panel) : cBaseEditVector2Listener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector2 &vector, meWorld *world){
		meHeightTerrainTexture * const texture = pPanel.GetTexture();
		return texture && !vector.IsEqualTo(decVector2(texture->GetProjectionOffsetU(), texture->GetProjectionOffsetV()))
			? new meUHTSetTexUVOffset(world, pPanel.GetSector(), texture, vector) : NULL;
	}
};

class cEditTexUVScale : public cBaseEditVector2Listener{
public:
	cEditTexUVScale(meWPHeightTerrain &panel) : cBaseEditVector2Listener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector2 &vector, meWorld *world){
		meHeightTerrainTexture * const texture = pPanel.GetTexture();
		return texture && !vector.IsEqualTo(decVector2(texture->GetProjectionScalingU(), texture->GetProjectionScalingV()))
			? new meUHTSetTexUVScaling(world, pPanel.GetSector(), texture, vector) : NULL;
	}
};

class cEditTexUVRotation : public cBaseTextFieldListener{
public:
	cEditTexUVRotation(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld *world){
		meHeightTerrainTexture * const texture = pPanel.GetTexture();
		const float value = textField->GetFloat();
		return texture && fabsf(value - texture->GetProjectionRotation()) > FLOAT_SAFE_EPSILON
			? new meUHTSetTexUVRotation(world, pPanel.GetSector(), texture, value) : NULL;
	}
};


class cComboNavSpace : public cBaseComboBoxListener{
public:
	cComboNavSpace(meWPHeightTerrain &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, meWorld *world){
		meHeightTerrainSector * const sector = pPanel.GetSector();
		if(sector){
			const igdeListItem * const selection = comboBox->GetSelectedItem();
			sector->SetActiveNavSpace(selection ? (meHeightTerrainNavSpace*)selection->GetData() : NULL);
		}
		return NULL;
	}
};

class cActionMenuNavSpace : public igdeActionContextMenu{
	meWPHeightTerrain &pPanel;
	
public:
	cActionMenuNavSpace(meWPHeightTerrain &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Navigation Space menu"),
	pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		if(!pPanel.GetSector()){
			return;
		}
		
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, pPanel.GetActionNavSpaceAdd());
		helper.MenuCommand(contextMenu, pPanel.GetActionNavSpaceRemove());
	}
};

class cActionNavSpaceAdd : public cBaseAction{
public:
	cActionNavSpaceAdd(meWPHeightTerrain &panel) : cBaseAction(panel, "Add...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add Navigation Space"){}
	
	virtual igdeUndo *OnAction(meWorld *world){
		meHeightTerrainSector * const sector = pPanel.GetSector();
		if(sector){
			decString name("navspace");
			while(igdeCommonDialogs::GetString(&pPanel, "Add Navigation Space",
			"Enter the name of the new navigation space", name)){
				if(sector->HasNavSpaceNamed(name)){
					igdeCommonDialogs::Error(&pPanel, "Invalid Navigation Space Name",
						"A navigation space with this name exists already.");
					continue;
				}
				
				const meHeightTerrainNavSpace::Ref navspace(meHeightTerrainNavSpace::Ref::NewWith(*world->GetEngine(), name));
				return new meUHTNavSpaceAdd(sector, navspace);
			}
		}
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetWorld() && pPanel.GetWorld()->GetHeightTerrain());
	}
};

class cActionNavSpaceRemove : public cBaseAction{
public:
	cActionNavSpaceRemove(meWPHeightTerrain &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove Navigation Space"){}
	
	virtual igdeUndo *OnAction(meWorld *world){
		meHeightTerrainNavSpace * const navspace = pPanel.GetActiveNavSpace();
		return navspace ? new meUHTNavSpaceRemove(navspace->GetHTSector(), navspace) : NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetActiveNavSpace());
	}
};

class cTextNavSpaceLayer : public cBaseTextFieldListener{
public:
	cTextNavSpaceLayer(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld*){
		meHeightTerrainNavSpace * const navspace = pPanel.GetActiveNavSpace();
		const int value = textField->GetInteger();
		return navspace && value != navspace->GetLayer() ? new meUHTNavSpaceSetLayer(navspace, value) : NULL;
	}
};

class cTextNavSpaceSnapDistance : public cBaseTextFieldListener{
public:
	cTextNavSpaceSnapDistance(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld*){
		meHeightTerrainNavSpace * const navspace = pPanel.GetActiveNavSpace();
		const float value = textField->GetFloat();
		return navspace && fabsf(value - navspace->GetSnapDistance()) > FLOAT_SAFE_EPSILON
			? new meUHTNavSpaceSetSnapDist(navspace, value) : NULL;
	}
};

class cTextNavSpaceSnapAngle : public cBaseTextFieldListener{
public:
	cTextNavSpaceSnapAngle(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld*){
		meHeightTerrainNavSpace * const navspace = pPanel.GetActiveNavSpace();
		const float value = textField->GetFloat();
		return navspace && fabsf(value - navspace->GetSnapAngle()) > FLOAT_SAFE_EPSILON
			? new meUHTNavSpaceSetSnapAngle(navspace, value) : NULL;
	}
};


class cComboNavSpaceType : public cBaseComboBoxListener{
public:
	cComboNavSpaceType(meWPHeightTerrain &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, meWorld*){
		meHeightTerrainNavSpace * const navspace = pPanel.GetActiveNavSpace();
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(navspace){
			navspace->SetActiveType(selection ? (meHeightTerrainNavSpaceType*)selection->GetData() : NULL);
		}
		return NULL;
	}
};

class cActionMenuNavSpaceType : public igdeActionContextMenu{
	meWPHeightTerrain &pPanel;
	
public:
	cActionMenuNavSpaceType(meWPHeightTerrain &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Navigation Space Type menu"),
	pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		if(!pPanel.GetSector()){
			return;
		}
		
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, pPanel.GetActionNavSpaceTypeAdd());
		helper.MenuCommand(contextMenu, pPanel.GetActionNavSpaceTypeRemove());
	}
};

class cActionNavSpaceTypeAdd : public cBaseAction{
public:
	cActionNavSpaceTypeAdd(meWPHeightTerrain &panel) : cBaseAction(panel, "Add...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add Navigation Space Type"){}
	
	virtual igdeUndo *OnAction(meWorld*){
		meHeightTerrainNavSpace * const navspace = pPanel.GetActiveNavSpace();
		decString name("type");
		while(igdeCommonDialogs::GetString(&pPanel, "Add Navigation Space Type",
		"Enter the name of the new navigation space type", name)){
			if(navspace->HasTypeNamed(name)){
				igdeCommonDialogs::Error(&pPanel, "Invalid Navigation Space Type Name",
					"A navigation space type with this name exists already.");
				continue;
			}
			
			const meHeightTerrainNavSpaceType::Ref type(meHeightTerrainNavSpaceType::Ref::NewWith(name));
			return new meUHTNavSpaceTypeAdd(navspace, type);
		}
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetActiveNavSpace());
	}
};

class cActionNavSpaceTypeRemove : public cBaseAction{
public:
	cActionNavSpaceTypeRemove(meWPHeightTerrain &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove Navigation Space Type"){}
	
	virtual igdeUndo *OnAction(meWorld*){
		meHeightTerrainNavSpaceType * const type = pPanel.GetActiveNavSpaceType();
		return type ? new meUHTNavSpaceTypeRemove(type) : NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetActiveNavSpaceType());
	}
};

class cTextNavSpaceTypeName : public cBaseTextFieldListener{
public:
	cTextNavSpaceTypeName(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld*){
		meHeightTerrainNavSpaceType * const type = pPanel.GetActiveNavSpaceType();
		if(!type || textField->GetText() == type->GetName()){
			return NULL;
		}
		if(pPanel.GetActiveNavSpace()->HasTypeNamed(textField->GetText())){
			igdeCommonDialogs::Error(&pPanel, "Invalid Name", "A navigation space type with this name exists already.");
			pPanel.UpdateNavSpaceType();
			return NULL;
		}
		return new meUHTNavSpaceTypeSetName(type, textField->GetText());
	}
};

class cTextNavSpaceTypeType : public cBaseTextFieldListener{
public:
	cTextNavSpaceTypeType(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld*){
		meHeightTerrainNavSpaceType * const type = pPanel.GetActiveNavSpaceType();
		const int value = textField->GetInteger();
		return type && type->GetType() != value ? new meUHTNavSpaceTypeSetType(type, value) : NULL;
	}
};

class cTextNavSpaceTypeColor : public igdeColorBoxListener{
	meWPHeightTerrain &pPanel;
	
public:
	cTextNavSpaceTypeColor(meWPHeightTerrain &panel) : pPanel(panel){}
	
	virtual void OnColorChanged(igdeColorBox *colorBox){
		meHeightTerrainNavSpaceType * const type = pPanel.GetActiveNavSpaceType();
		if(!type || colorBox->GetColor().IsEqualTo(type->GetColor())){
			return;
		}
		pPanel.GetWorld()->GetUndoSystem()->Add(meUHTNavSpaceTypeSetColor::Ref::NewWith(
			type, colorBox->GetColor()));
	}
};

class cActionNavSpaceFaceAdd : public cBaseAction{
public:
	cActionNavSpaceFaceAdd(meWPHeightTerrain &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add face with selected navigation points"){}
	
	bool CanUse(const meWorld &world) const{
		const meHeightTerrainNavSpace * const navspace = pPanel.GetActiveNavSpace();
		if(!navspace || world.GetGuiParameters().GetWorkMode() != meWorldGuiParameters::ewmNavSpaceEdit){
			return false;
		}
		
		const decIntList &selNavPoints = navspace->GetHTSector()->GetSelectedNavPoints();
		if(selNavPoints.GetCount() < 3){
			return false;
		}
		
		const int typeCount = navspace->GetTypeCount();
		int i, j;
		
		for(i=0; i<typeCount; i++){
			const meHeightTerrainNavSpaceType &checkType = *navspace->GetTypeAt(i);
			const int faceCount = checkType.GetFaceCount();
			
			for(j=0; j<faceCount; j++){
				if(checkType.GetFaceAt(j)->NavPointsMatch(selNavPoints)){
					return false;
				}
			}
		}
		
		return true;
	}
	
	virtual igdeUndo *OnAction(meWorld *world){
		if(!CanUse(*world)){
			return NULL;
		}
		
		const meHeightTerrainNavSpaceFace::Ref face(meHeightTerrainNavSpaceFace::Ref::NewWith());
		face->GetNavPoints() = pPanel.GetSector()->GetSelectedNavPoints();
		face->OrderClockwise(world->GetHeightTerrain()->GetSectorResolution());
		return new meUHTNavSpaceFaceAdd(pPanel.GetActiveNavSpaceType(), face);
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetWorld() && CanUse(*pPanel.GetWorld()));
	}
};

class cActionNavSpaceFaceRemove : public cBaseAction{
public:
	cActionNavSpaceFaceRemove(meWPHeightTerrain &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove face matching selected navigation points"){}
	
	bool CanUse(const meWorld &world) const{
		return pPanel.GetActiveNavSpace()
			&& world.GetGuiParameters().GetWorkMode() == meWorldGuiParameters::ewmNavSpaceEdit
			&& pPanel.GetSector()->GetSelectedNavPoints().GetCount() > 0;
	}
	
	void FindFaces(const meWorld &world, decObjectOrderedSet &faces){
		const decIntList &selNavPoints = pPanel.GetSector()->GetSelectedNavPoints();
		const meHeightTerrainNavSpace &navspace = *pPanel.GetActiveNavSpace();
		const int typeCount = navspace.GetTypeCount();
		int i, j;
		
		if(selNavPoints.GetCount() == 1){
			const int navpoint = selNavPoints.GetAt(0);
			
			for(i=0; i<typeCount; i++){
				const meHeightTerrainNavSpaceType &type = *navspace.GetTypeAt(i);
				const int faceCount = type.GetFaceCount();
				
				for(j=0; j<faceCount; j++){
					meHeightTerrainNavSpaceFace * const face = type.GetFaceAt(j);
					if(face->HasNavPoint(navpoint)){
						faces.Add(face);
					}
				}
			}
			
		}else if(selNavPoints.GetCount() == 2){
			const int navpoint1 = selNavPoints.GetAt(0);
			const int navpoint2 = selNavPoints.GetAt(1);
			
			for(i=0; i<typeCount; i++){
				const meHeightTerrainNavSpaceType &type = *navspace.GetTypeAt(i);
				const int faceCount = type.GetFaceCount();
				
				for(j=0; j<faceCount; j++){
					meHeightTerrainNavSpaceFace * const face = type.GetFaceAt(j);
					if(face->HasNavEdge(navpoint1, navpoint2)){
						faces.Add(face);
					}
				}
			}
			
		}else{ //selNavPoints.GetCount() > 2
			for(i=0; i<typeCount; i++){
				const meHeightTerrainNavSpaceType &type = *navspace.GetTypeAt(i);
				const int faceCount = type.GetFaceCount();
				
				for(j=0; j<faceCount; j++){
					meHeightTerrainNavSpaceFace * const face = type.GetFaceAt(j);
					if(face->HasAllNavPointsIn(selNavPoints)){
						faces.Add(face);
					}
				}
			}
		}
	}
	
	virtual igdeUndo *OnAction(meWorld *world){
		if(!CanUse(*world)){
			return NULL;
		}
		
		decObjectOrderedSet faces;
		FindFaces(*world, faces);
		return faces.GetCount() > 0 ? new meUHTNavSpaceFaceRemove(faces) : NULL;
	}
	
	virtual void Update(){
		bool enabled = pPanel.GetWorld() && CanUse(*pPanel.GetWorld());
		if(enabled){
			decObjectOrderedSet faces;
			FindFaces(*pPanel.GetWorld(), faces);
			enabled = faces.GetCount() > 0;
		}
		SetEnabled(enabled);
	}
};



class cActionUpdateVegetation : public cBaseAction{
public:
	cActionUpdateVegetation(meWPHeightTerrain &panel) : cBaseAction(panel, "Visible",
		NULL, "Updates the visible vegetation system (can take some time)"){}
	
	virtual igdeUndo *OnAction(meWorld *world){
		world->ForceUpdateVegetation(false);
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetWorld());
	}
};

class cActionUpdateVegetationAll : public cBaseAction{
public:
	cActionUpdateVegetationAll(meWPHeightTerrain &panel) : cBaseAction(panel, "All",
		NULL, "Updates entire the vegetation system (lengthy operation!)"){}
	
	virtual igdeUndo *OnAction(meWorld *world){
		world->ForceUpdateVegetation(true);
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetWorld());
	}
};

class cActionClearVegetation : public cBaseAction{
public:
	cActionClearVegetation(meWPHeightTerrain &panel) : cBaseAction(panel, "Clear",
		NULL, "Clear vegetation (hides all)"){}
	
	virtual igdeUndo *OnAction(meWorld *world){
		world->ClearVegetation();
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetWorld());
	}
};

class cPathPathPFCache : public cBaseEditPathListener{
public:
	cPathPathPFCache(meWPHeightTerrain &panel) : cBaseEditPathListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeEditPath *editPath, meWorld *world){
		meHeightTerrainSector * const sector = pPanel.GetSector();
		return sector && editPath->GetPath() != sector->GetPathPFCacheImage()
			? new meUHTSetPathPFCache(world, sector, editPath->GetPath()) : NULL;
	}
};


class cComboVLayer : public cBaseComboBoxListener{
public:
	cComboVLayer(meWPHeightTerrain &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, meWorld *world){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		world->GetHeightTerrain()->SetActiveVLayer(selection ? (meHTVegetationLayer*)selection->GetData() : NULL);
		return NULL;
	}
};

class cActionMenuVLayer : public igdeActionContextMenu{
	meWPHeightTerrain &pPanel;
	
public:
	cActionMenuVLayer(meWPHeightTerrain &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Vegetation Layer menu"),
	pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		if(!pPanel.GetSector()){
			return;
		}
		
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, pPanel.GetActionVLayerAdd());
		helper.MenuCommand(contextMenu, pPanel.GetActionVLayerUp());
		helper.MenuCommand(contextMenu, pPanel.GetActionVLayerDown());
		helper.MenuCommand(contextMenu, pPanel.GetActionVLayerRemove());
	}
};

class cActionVLayerAdd : public cBaseAction{
public:
	cActionVLayerAdd(meWPHeightTerrain &panel) : cBaseAction(panel, "Add...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add vegetation layer"){}
	
	virtual igdeUndo *OnAction(meWorld *world){
		decString name("vlayer");
		if(!igdeCommonDialogs::GetString(&pPanel, "Add Vegetation Layer", "Enter name of new layer", name)){
			return NULL;
		}
		
		const meHTVegetationLayer::Ref vlayer(meHTVegetationLayer::Ref::NewWith(world->GetEngine(), name));
		
		vlayer->AddRule(meHTVRuleResult::Ref::NewWith());
		
		return new meUHTVLayerAdd(world, world->GetHeightTerrain(), vlayer);
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetSector());
	}
};

class cActionVLayerRemove : public cBaseAction{
public:
	cActionVLayerRemove(meWPHeightTerrain &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove vegetation layer"){}
	
	virtual igdeUndo *OnAction(meWorld *world){
		meHTVegetationLayer * const vlayer = pPanel.GetVLayer();
		return vlayer ? new meUHTVLayerRemove(world, world->GetHeightTerrain(), vlayer) : NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetVLayer());
	}
};

class cActionVLayerUp : public cBaseAction{
public:
	cActionVLayerUp(meWPHeightTerrain &panel) : cBaseAction(panel, "Move Up",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp), "Move vegetation layer up"){}
	
	virtual igdeUndo *OnAction(meWorld *world){
		meHTVegetationLayer * const vlayer = pPanel.GetVLayer();
		return vlayer && world->GetHeightTerrain()->GetVLayerCount() > 1
			? new meUHTVLayerUp(world, world->GetHeightTerrain(), vlayer) : NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetVLayer() && pPanel.GetWorld()->GetHeightTerrain()->GetVLayerCount() > 1);
	}
};

class cActionVLayerDown : public cBaseAction{
public:
	cActionVLayerDown(meWPHeightTerrain &panel) : cBaseAction(panel, "Move Down",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp), "Move vegetation layer down"){}
	
	virtual igdeUndo *OnAction(meWorld *world){
		meHTVegetationLayer * const vlayer = pPanel.GetVLayer();
		return vlayer && world->GetHeightTerrain()->IndexOfVLayer(vlayer)
			< world->GetHeightTerrain()->GetVLayerCount() - 1
				? new meUHTVLayerUp(world, world->GetHeightTerrain(), vlayer) : NULL;
	}
	
	virtual void Update(){
		meHTVegetationLayer * const vlayer = pPanel.GetVLayer();
		SetEnabled(vlayer && pPanel.GetWorld()->GetHeightTerrain()->IndexOfVLayer(vlayer)
			< pPanel.GetWorld()->GetHeightTerrain()->GetVLayerCount() - 1);
	}
};


class cComboVVariation : public cBaseComboBoxListener{
public:
	cComboVVariation(meWPHeightTerrain &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, meWorld*){
		meHTVegetationLayer * const vlayer = pPanel.GetVLayer();
		if(vlayer){
			const igdeListItem * const selection = comboBox->GetSelectedItem();
			vlayer->SetActiveVariation(selection ? (meHTVVariation*)selection->GetData() : NULL);
		}
		return NULL;
	}
};

class cActionMenuVVariation : public igdeActionContextMenu{
	meWPHeightTerrain &pPanel;
	
public:
	cActionMenuVVariation(meWPHeightTerrain &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Vegetation Variation menu"),
	pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		if(!pPanel.GetVLayer()){
			return;
		}
		
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, pPanel.GetActionVVariationAdd());
		helper.MenuCommand(contextMenu, pPanel.GetActionVVariationRemove());
	}
};

class cActionVVariationAdd : public cBaseAction{
public:
	cActionVVariationAdd(meWPHeightTerrain &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add variation"){}
	
	virtual igdeUndo *OnAction(meWorld *world){
		meHTVegetationLayer * const vlayer = pPanel.GetVLayer();
		if(!vlayer){
			return NULL;
		}
		
		const meHTVVariation::Ref variation(meHTVVariation::Ref::NewWith(world->GetEngine()));
		return new meUHTVVariationAdd(vlayer, variation);
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetVLayer());
	}
};

class cActionVVariationRemove : public cBaseAction{
public:
	cActionVVariationRemove(meWPHeightTerrain &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove variation"){}
	
	virtual igdeUndo *OnAction(meWorld*){
		meHTVVariation * const variation = pPanel.GetVVariation();
		return variation ? new meUHTVVariationRemove(pPanel.GetVLayer(), variation) : NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetVVariation());
	}
};

class cPathVVModel : public cBaseEditPathListener{
public:
	cPathVVModel(meWPHeightTerrain &panel) : cBaseEditPathListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeEditPath *editPath, meWorld*){
		meHTVVariation * const variation = pPanel.GetVVariation();
		return variation && editPath->GetPath() != variation->GetPathModel()
			? new meUHTVVarSetModel(variation, editPath->GetPath()) : NULL;
	}
};

class cPathVVSkin : public cBaseEditPathListener{
public:
	cPathVVSkin(meWPHeightTerrain &panel) : cBaseEditPathListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeEditPath *editPath, meWorld*){
		meHTVVariation * const variation = pPanel.GetVVariation();
		return variation && editPath->GetPath() != variation->GetPathSkin()
			? new meUHTVVarSetSkin(variation, editPath->GetPath()) : NULL;
	}
};

class cTextVVRotPerForce : public cBaseTextFieldListener{
public:
	cTextVVRotPerForce(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld*){
		meHTVVariation * const variation = pPanel.GetVVariation();
		const float value = textField->GetFloat();
		return variation && fabsf(value - variation->GetRotationPerForce()) > FLOAT_SAFE_EPSILON
			? new meUHTVVarSetRotPerForce(variation, value) : NULL;
	}
};

class cTextVVRestitution : public cBaseTextFieldListener{
public:
	cTextVVRestitution(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld*){
		meHTVVariation * const variation = pPanel.GetVVariation();
		const float value = textField->GetFloat();
		return variation && fabsf(value - variation->GetRestitution()) > FLOAT_SAFE_EPSILON
			? new meUHTVVarSetRestitution(variation, value) : NULL;
	}
};


class cActionHPMode : public cBaseAction{
	const meWorldGuiParameters::eHPDrawModes pMode;
	
public:
	cActionHPMode(meWPHeightTerrain &panel, meWorldGuiParameters::eHPDrawModes mode,
	const char *text, igdeIcon *icon, const char *description) :
		cBaseAction(panel, text, icon, description), pMode(mode){}
	
	virtual igdeUndo *OnAction(meWorld *world){
		world->GetGuiParameters().SetHPDrawMode(pMode);
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetWorld());
		SetSelected(pPanel.GetWorld() && pPanel.GetWorld()->GetGuiParameters().GetHPDrawMode() == pMode);
	}
};

class cSliderHPRadius : public cBaseEditSliderTextListener{
public:
	cSliderHPRadius(meWPHeightTerrain &panel) : cBaseEditSliderTextListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeEditSliderText *sliderText, meWorld *world){
		world->GetGuiParameters().SetHPRadius(sliderText->GetValue());
		return NULL;
	}
};

class cSliderHPStrength : public cBaseEditSliderTextListener{
public:
	cSliderHPStrength(meWPHeightTerrain &panel) : cBaseEditSliderTextListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeEditSliderText *sliderText, meWorld *world){
		switch(world->GetGuiParameters().GetHPDrawMode()){
		case meWorldGuiParameters::ehpdmRaise:
		case meWorldGuiParameters::ehpdmLower:
			world->GetGuiParameters().SetHPDrawStrength(sliderText->GetValue());
			break;
			
		case meWorldGuiParameters::ehpdmLevel:
			world->GetGuiParameters().SetHPLevelStrength(sliderText->GetValue());
			break;
			
		case meWorldGuiParameters::ehpdmSmooth:
			world->GetGuiParameters().SetHPSmoothStrength(sliderText->GetValue());
			break;
		}
		return NULL;
	}
};


class cActionMPMode : public cBaseAction{
	const meWorldGuiParameters::eMPDrawModes pMode;
	
public:
	cActionMPMode(meWPHeightTerrain &panel, meWorldGuiParameters::eMPDrawModes mode,
	const char *text, igdeIcon *icon, const char *description) :
		cBaseAction(panel, text, icon, description), pMode(mode){}
	
	virtual igdeUndo *OnAction(meWorld *world){
		world->GetGuiParameters().SetMPDrawMode(pMode);
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetWorld());
		SetSelected(pPanel.GetWorld() && pPanel.GetWorld()->GetGuiParameters().GetMPDrawMode() == pMode);
	}
};

class cSliderMPRadius : public cBaseEditSliderTextListener{
public:
	cSliderMPRadius(meWPHeightTerrain &panel) : cBaseEditSliderTextListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeEditSliderText *sliderText, meWorld *world){
		world->GetGuiParameters().SetMPRadius(sliderText->GetValue());
		return NULL;
	}
};


class cActionVPMode : public cBaseAction{
	const meWorldGuiParameters::eVPDrawModes pMode;
	
public:
	cActionVPMode(meWPHeightTerrain &panel, meWorldGuiParameters::eVPDrawModes mode,
	const char *text, igdeIcon *icon, const char *description) :
		cBaseAction(panel, text, icon, description), pMode(mode){}
	
	virtual igdeUndo *OnAction(meWorld *world){
		world->GetGuiParameters().SetVPDrawMode(pMode);
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetWorld());
		SetSelected(pPanel.GetWorld() && pPanel.GetWorld()->GetGuiParameters().GetVPDrawMode() == pMode);
	}
};

class cSliderVPRadius : public cBaseEditSliderTextListener{
public:
	cSliderVPRadius(meWPHeightTerrain &panel) : cBaseEditSliderTextListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeEditSliderText *sliderText, meWorld *world){
		world->GetGuiParameters().SetVPRadius(sliderText->GetValue());
		return NULL;
	}
};

}



// Class meWPHeightTerrain
////////////////////////////

// Constructor, destructor
////////////////////////////

meWPHeightTerrain::meWPHeightTerrain(meWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties),
pListener(NULL),
pWorld(NULL),
pTexture(NULL),
pVLayer(NULL),
pVVariation(NULL)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener.TakeOverWith(*this);
	
	content.TakeOverWith(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	pActionMenuTexture.TakeOverWith(*this);
	pActionTextureAdd.TakeOverWith(*this);
	pActionTextureRemove.TakeOverWith(*this);
	
	pActionMenuNavSpace.TakeOverWith(*this);
	pActionNavSpaceAdd.TakeOverWith(*this);
	pActionNavSpaceRemove.TakeOverWith(*this);
	
	pActionMenuNavSpaceType.TakeOverWith(*this);
	pActionNavSpaceTypeAdd.TakeOverWith(*this);
	pActionNavSpaceTypeRemove.TakeOverWith(*this);
	
	pActionNavSpaceFaceAdd.TakeOverWith(*this);
	pActionNavSpaceFaceRemove.TakeOverWith(*this);
	
	pActionMenuVLayer.TakeOverWith(*this);
	pActionVLayerAdd.TakeOverWith(*this);
	pActionVLayerRemove.TakeOverWith(*this);
	pActionVLayerUp.TakeOverWith(*this);
	pActionVLayerDown.TakeOverWith(*this);
	
	pActionMenuVVariation.TakeOverWith(*this);
	pActionVVariationAdd.TakeOverWith(*this);
	pActionVVariationRemove.TakeOverWith(*this);
	
	
	// height terrain
	helper.GroupBox(content, groupBox, "Height Terrain:");
	
	igdeFilePatternList filePatterns;
	filePatterns.AddFilePattern(new igdeFilePattern(
		"Drag[en]gine Height Terrain", "*.deterrain", ".deterrain"));
	helper.EditPath(groupBox, "Height Terrain:", "File to save height terrain to",
		filePatterns, pEditPathHTerrain, new cEditPathHT(*this));
	helper.EditFloat(groupBox, "Sector Size:", "Size of sectors along X and Z axis",
		pEditSectorSize, new cEditSectorSize(*this));
	helper.EditInteger(groupBox, "Sector Resolution:", "Resolution of height images in sectors (width and height)",
		pEditSectorResolution, new cEditSectorResolution(*this));
	helper.EditFloat(groupBox, "Base Height:", "Set base height of height image values",
		pEditBaseHeight, new cEditBaseHeight(*this));
	helper.EditFloat(groupBox, "Height Scaling:", "Set height scaling of height image values",
		pEditHeightScale, new cEditHeightScale(*this));
	
	
	// sector
	helper.GroupBox(content, groupBox, "Sector:");
	
	helper.EditPath(groupBox, "Height Image:", "File to save height image to",
		igdeEnvironment::efpltImage, pEditPathHeightImage, new cEditPathHeightImage(*this));
	helper.EditPath(groupBox, "Visibility Image:", "File to save visibility image to",
		igdeEnvironment::efpltImage, pEditPathVisImage, new cEditPathVisibilityImage(*this));
	
	
	// texture
	helper.GroupBox(content, groupBox, "Texture:");
	
	helper.FormLineStretchFirst(groupBox, "Texture:", "Texture to edit", formLine);
	helper.ComboBox(formLine, "Texture to edit", pCBTexture, new cComboTexture(*this));
	helper.Button(formLine, pBtnTexture, pActionMenuTexture);
	pActionMenuTexture->SetWidget(pBtnTexture);
	
	helper.EditInteger(groupBox, "Type Number:", "Type number of the texture.",
		pEditTexTypeNum, new cTextTexTypeNumber(*this));
	helper.EditPath(groupBox, "Skin:", "Skin to use for the texture.",
		igdeEnvironment::efpltSkin, pEditTexSkin, new cPathTexSkin(*this));
	helper.EditPath(groupBox, "Mask:", "Mask to use for the texture.",
		igdeEnvironment::efpltSkin, pEditTexMask, new cPathTexMask(*this));
	helper.EditVector2(groupBox, "Offset:", "Offsets the projected texture relative to the world origin.",
		pEditTexUVOffset, new cEditTexUVOffset(*this));
	helper.EditVector2(groupBox, "Scaling:", "Scaling of the projected texture.",
		pEditTexUVScale, new cEditTexUVScale(*this));
	helper.EditFloat(groupBox, "Rotation:", "Rotation of the projected texture.",
		pEditTexUVRot, new cEditTexUVRotation(*this));
	
	
	// navigation space
	helper.GroupBox(content, groupBox, "Navigation-Space:", true);
	
	helper.FormLineStretchFirst(groupBox, "Nav-Space:", "Nav-Space", formLine);
	helper.ComboBox(formLine, "Nav-Space", pCBNavSpace, new cComboNavSpace(*this));
	helper.Button(formLine, pBtnNavSpace, pActionMenuNavSpace);
	pActionMenuNavSpace->SetWidget(pBtnNavSpace);
	
	helper.EditInteger(groupBox, "Layer:", "Navigation layer.",
		pEditNavSpaceLayer, new cTextNavSpaceLayer(*this));
	helper.EditFloat(groupBox, "Snap Distance:", "Snap distance.",
		pEditNavSpaceSnapDist, new cTextNavSpaceSnapDistance(*this));
	helper.EditFloat(groupBox, "Snap Angle:", "Snap angle.",
		pEditNavSpaceSnapAngle, new cTextNavSpaceSnapAngle(*this));
	
	
	// navigation space type
	helper.GroupBox(content, groupBox, "Navigation Type:", true);
	
	helper.FormLineStretchFirst(groupBox, "Type:", "Navigation type to edit", formLine);
	helper.ComboBox(formLine, "Navigation type to edit", pCBNavSpaceType, new cComboNavSpaceType(*this));
	helper.Button(formLine, pBtnNavSpaceType, pActionMenuNavSpaceType);
	pActionMenuNavSpaceType->SetWidget(pBtnNavSpaceType);
	
	helper.EditString(groupBox, "Name:", "Navigation space type name.",
		pEditNavSpaceTypeName, new cTextNavSpaceTypeName(*this));
	helper.EditInteger(groupBox, "Type:", "Navigation space type number.",
		pEditNavSpaceTypeType, new cTextNavSpaceTypeType(*this));
	helper.ColorBox(groupBox, "Color:", "Navigation space type color for visualization purpose.",
		pEditNavSpaceTypeColor, new cTextNavSpaceTypeColor(*this));
	
	helper.FormLine(groupBox, "Faces:", "Navigation space faces", formLine);
	helper.Button(formLine, pBtnNavSpaceFaceAdd, pActionNavSpaceFaceAdd);
	helper.Button(formLine, pBtnNavSpaceFaceRemove, pActionNavSpaceFaceRemove);
	
	
	// vegetation
	helper.GroupBox(content, groupBox, "Vegetation:", true);
	
	helper.FormLine(groupBox, "Update:", "Update vegetation", formLine);
	helper.Button(formLine, pBtnUpdateVegetation, new cActionUpdateVegetation(*this), true);
	helper.Button(formLine, pBtnUpdateVegetationAll, new cActionUpdateVegetationAll(*this), true);
	helper.Button(formLine, pBtnClearVegetation, new cActionClearVegetation(*this), true);
	
	filePatterns.RemoveAllFilePatterns();
	filePatterns.AddFilePattern(new igdeFilePattern(
		"Drag[en]gine Prop Field Cache", "*.depfc", ".depfc"));
	helper.EditPath(groupBox, "Skin:", "Skin to use for the texture.",
		filePatterns, pEditPathPFCache, new cPathPathPFCache(*this));
	
	helper.FormLineStretchFirst(groupBox, "Layer:", "Layer to edit", formLine);
	helper.ComboBox(formLine, "Layer to edit", pCBVLayer, new cComboVLayer(*this));
	helper.Button(formLine, pBtnMenuVLayer, pActionMenuVLayer);
	pActionMenuVLayer->SetWidget(pBtnMenuVLayer);
	
	helper.FormLineStretchFirst(groupBox, "Variation:", "Variation to edit", formLine);
	helper.ComboBox(formLine, "Variation to edit", pCBVVariation, new cComboVVariation(*this));
	helper.Button(formLine, pBtnMenuVVariation, pActionMenuVVariation);
	pActionMenuVVariation->SetWidget(pBtnMenuVVariation);
	
	helper.EditPath(groupBox, "Model:", "Model to use for the variation",
		igdeEnvironment::efpltModel, pEditVVModel, new cPathVVModel(*this));
	helper.EditPath(groupBox, "Skin:", "Skin to use for the variation",
		igdeEnvironment::efpltSkin, pEditVVSkin, new cPathVVSkin(*this));
	helper.EditFloat(groupBox, "Rotation/Force:", "Rotation per Force.",
		pEditVVRotPerForce, new cTextVVRotPerForce(*this));
	helper.EditFloat(groupBox, "Restition:", "Restition.",
		pEditVVRestitution, new cTextVVRestitution(*this));
	
	
	// height paint
	helper.GroupBox(content, groupBox, "Height Painting:");
	
	helper.FormLine(groupBox, "Mode:", "Paint mode", formLine);
	helper.ToggleButton(formLine, pBtnHPModeRaise, new cActionHPMode(*this,
		meWorldGuiParameters::ehpdmRaise, "Raise", NULL, "Raise"), true);
	helper.ToggleButton(formLine, pBtnHPModeLower, new cActionHPMode(*this,
		meWorldGuiParameters::ehpdmLower, "Lower", NULL, "Lower"), true);
	helper.ToggleButton(formLine, pBtnHPModeLevel, new cActionHPMode(*this,
		meWorldGuiParameters::ehpdmLevel, "Level", NULL, "Level"), true);
	helper.ToggleButton(formLine, pBtnHPModeSmooth, new cActionHPMode(*this,
		meWorldGuiParameters::ehpdmSmooth, "Smooth", NULL, "Smooth"), true);
	
	helper.EditSliderText(groupBox, "Radius:", "Sets the radius of influence.",
		0.0f, 10.0f, 6, 3, 1.0f, pSldHPRadius, new cSliderHPRadius(*this));
	helper.EditSliderText(groupBox, "Strength:", "Strength of the influence.",
		0.0f, 1.0f, 6, 3, 0.1f, pSldHPStrength, new cSliderHPStrength(*this));
	
	
	// mask paint
	helper.GroupBox(content, groupBox, "Mask Painting:", true);
	
	helper.FormLine(groupBox, "Mode:", "Paint mode", formLine);
	helper.ToggleButton(formLine, pBtnMPModeDraw, new cActionMPMode(*this,
		meWorldGuiParameters::empdmDraw, "Draw", NULL, "Draw"), true);
	helper.ToggleButton(formLine, pBtnMPModeErase, new cActionMPMode(*this,
		meWorldGuiParameters::empdmErase, "Erase", NULL, "Erase"), true);
	
	helper.EditSliderText(groupBox, "Radius:", "Sets the radius of influence.",
		0.0f, 10.0f, 6, 3, 1.0f, pSldMPRadius, new cSliderMPRadius(*this));
	
	
	// visibility paint
	helper.GroupBox(content, groupBox, "Visibility Painting:", true);
	
	helper.FormLine(groupBox, "Mode:", "Paint mode", formLine);
	helper.ToggleButton(formLine, pBtnVPModeVisible, new cActionVPMode(*this,
		meWorldGuiParameters::evpdmVisible, "Visible", NULL, "Visible"), true);
	helper.ToggleButton(formLine, pBtnVPModeInvisible, new cActionVPMode(*this,
		meWorldGuiParameters::evpdmInvisible, "Invisible", NULL, "Invisible"), true);
	
	helper.EditSliderText(groupBox, "Radius:", "Sets the radius of influence.",
		0.0f, 10.0f, 6, 3, 1.0f, pSldVPRadius, new cSliderVPRadius(*this));
}

meWPHeightTerrain::~meWPHeightTerrain(){
	SetWorld(NULL);
}



// Management
///////////////

void meWPHeightTerrain::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	SetVLayer(NULL);
	SetTexture(NULL);
	
	if(pWorld){
		pWorld->RemoveNotifier(pListener);
	}
	
	pWorld = world;
	
	if(world){
		world->AddNotifier(pListener);
		world->AddReference();
	}
	
	UpdateHeightTerrain();
	UpdateSector();
	UpdateTextureList();
	UpdateNavSpaceList();
	UpdateVLayerList();
	
	meHeightTerrainSector * const sector = GetSector();
	if(sector){
		SetTexture(sector->GetActiveTexture());
	}
	if(world){
		SetVLayer(world->GetHeightTerrain()->GetActiveVLayer());
	}
	
	UpdateHeightPaint();
	UpdateMaskPaint();
	UpdateVisibilityPaint();
	OnWorldPathChanged();
}

void meWPHeightTerrain::OnWorldPathChanged(){
	if(pWorld){
		pEditPathHTerrain->SetBasePath(pWorld->GetDirectoryPath());
		pEditPathHeightImage->SetBasePath(pWorld->GetDirectoryPath());
		pEditPathVisImage->SetBasePath(pWorld->GetDirectoryPath());
		pEditTexSkin->SetBasePath(pWorld->GetDirectoryPath());
		pEditTexMask->SetBasePath(pWorld->GetDirectoryPath());
		pEditPathPFCache->SetBasePath(pWorld->GetDirectoryPath());
		pEditVVModel->SetBasePath(pWorld->GetDirectoryPath());
		pEditVVSkin->SetBasePath(pWorld->GetDirectoryPath());
		
	}else{
		pEditPathHTerrain->SetBasePath("");
		pEditPathHeightImage->SetBasePath("");
		pEditPathVisImage->SetBasePath("");
		pEditTexSkin->SetBasePath("");
		pEditTexMask->SetBasePath("");
		pEditPathPFCache->SetBasePath("");
		pEditVVModel->SetBasePath("");
		pEditVVSkin->SetBasePath("");
	}
}

meHeightTerrainSector *meWPHeightTerrain::GetSector() const{
	if(pWorld){
		return pWorld->GetHeightTerrain()->GetSectorWith(decPoint());
			// decPoint( pWorld->GetActiveSector().x, pWorld->GetActiveSector().z ) );
		
	}else{
		return NULL;
	}
}

void meWPHeightTerrain::SetTexture(meHeightTerrainTexture *texture){
	if(texture && !GetSector()){
		DETHROW(deeInvalidParam);
	}
	
	if(texture == pTexture){
		return;
	}
	
	
	pTexture = texture;
	
	pCBTexture->SetSelectionWithData(texture);
	
	UpdateTexture();
}

meHeightTerrainNavSpace *meWPHeightTerrain::GetActiveNavSpace() const{
	return GetSector() ? GetSector()->GetActiveNavSpace() : NULL;
}

meHeightTerrainNavSpaceType *meWPHeightTerrain::GetActiveNavSpaceType() const{
	const meHeightTerrainNavSpace * const navspace = GetActiveNavSpace();
	return navspace ? navspace->GetActiveType() : NULL;
}

void meWPHeightTerrain::SetVLayer(meHTVegetationLayer *vlayer){
	if(vlayer && !GetSector()){
		DETHROW(deeInvalidParam);
	}
	
	if(vlayer == pVLayer){
		return;
	}
	
	SetVVariation(NULL);
	
	
	pVLayer = vlayer;
	
	UpdateVVariationList();
	
	pCBVLayer->SetSelectionWithData(vlayer);
	UpdateVLayer();
}

void meWPHeightTerrain::SetVVariation(meHTVVariation *variation){
	if(variation && !pVLayer){
		DETHROW(deeInvalidParam);
	}
	
	if(variation == pVVariation){
		return;
	}
	
	
	pVVariation = variation;
	
	pCBVVariation->SetSelectionWithData(variation);
	UpdateVVariation();
}



void meWPHeightTerrain::UpdateHeightTerrain(){
	if(pWorld){
		const meHeightTerrain &ht = *pWorld->GetHeightTerrain();
		pEditPathHTerrain->SetPath(ht.GetPathHT());
		pEditSectorSize->SetFloat(ht.GetSectorSize());
		pEditSectorResolution->SetInteger(ht.GetSectorResolution());
		pEditBaseHeight->SetFloat(ht.GetBaseHeight());
		pEditHeightScale->SetFloat(ht.GetHeightScaling());
		
	}else{
		pEditPathHTerrain->ClearPath();
		pEditSectorSize->ClearText();
		pEditSectorResolution->ClearText();
		pEditBaseHeight->ClearText();
		pEditHeightScale->ClearText();
	}
}

void meWPHeightTerrain::UpdateSector(){
	meHeightTerrainSector * const sector = GetSector();
	
	if(sector){
		pEditPathHeightImage->SetPath(sector->GetPathHeightImage());
		pEditPathVisImage->SetPath(sector->GetPathVisibilityImage());
		pEditPathPFCache->SetPath(sector->GetPathPFCacheImage());
		
	}else{
		pEditPathHeightImage->ClearPath();
		pEditPathVisImage->ClearPath();
		pEditPathPFCache->ClearPath();
	}
}

void meWPHeightTerrain::UpdateTextureList(){
	meHeightTerrainSector * const sector = GetSector();
	meHeightTerrainTexture *selection = GetTexture();
	
	pCBTexture->RemoveAllItems();
	
	if(sector){
		const int count = sector->GetTextureCount();
		int i;
		
		for(i=0; i<count; i++){
			meHeightTerrainTexture * const texture = sector->GetTextureAt(i);
			pCBTexture->AddItem(texture->GetName(), NULL, texture);
		}
	}
	
	pCBTexture->SortItems();
	
	if(!selection && sector && sector->GetTextureCount() > 0){
		selection = sector->GetTextureAt(0);
	}
	pCBTexture->SetSelectionWithData(selection);
}

void meWPHeightTerrain::UpdateTexture(){
	if(pTexture){
		pEditTexSkin->SetPath(pTexture->GetPathSkin());
		pEditTexMask->SetPath(pTexture->GetPathMask());
		pEditTexTypeNum->SetInteger(pTexture->GetTypeNumber());
		pEditTexUVOffset->SetVector2(decVector2(
			pTexture->GetProjectionOffsetU(), pTexture->GetProjectionOffsetV()));
		pEditTexUVScale->SetVector2(decVector2(
			pTexture->GetProjectionScalingU(), pTexture->GetProjectionScalingV()));
		pEditTexUVRot->SetFloat(pTexture->GetProjectionRotation());
		
	}else{
		pEditTexSkin->ClearPath();
		pEditTexMask->ClearPath();
		pEditTexUVOffset->SetVector2(decVector2());
		pEditTexUVScale->SetVector2(decVector2());
		pEditTexUVRot->ClearText();
	}
}

void meWPHeightTerrain::UpdateNavSpaceList(){
	pCBNavSpace->RemoveAllItems();
	
	meHeightTerrainSector * const sector = GetSector();
	if(sector){
		const int count = sector->GetNavSpaceCount();
		int i;
		for(i=0; i<count; i++){
			meHeightTerrainNavSpace * const navspace = sector->GetNavSpaceAt(i);
			pCBNavSpace->AddItem(navspace->GetName(), NULL, navspace);
		}
	}
	
	pCBNavSpace->SortItems();
	SelectActiveNavSpace();
}

void meWPHeightTerrain::SelectActiveNavSpace(){
	pCBNavSpace->SetSelectionWithData(GetActiveNavSpace());
	UpdateNavSpace();
}

void meWPHeightTerrain::UpdateNavSpace(){
	const meHeightTerrainNavSpace * const navspace = GetActiveNavSpace();
	
	if(navspace){
		pEditNavSpaceLayer->SetInteger(navspace->GetLayer());
		pEditNavSpaceSnapDist->SetFloat(navspace->GetSnapDistance());
		pEditNavSpaceSnapAngle->SetFloat(navspace->GetSnapAngle());
		
	}else{
		pEditNavSpaceLayer->ClearText();
		pEditNavSpaceSnapDist->ClearText();
		pEditNavSpaceSnapAngle->ClearText();
	}
	
	UpdateNavSpaceTypeList();
}

void meWPHeightTerrain::UpdateNavSpaceTypeList(){
	const meHeightTerrainNavSpace * const navspace = GetActiveNavSpace();
	
	pCBNavSpaceType->RemoveAllItems();
	
	if(navspace){
		const int count = navspace->GetTypeCount();
		int i;
		
		for(i=0; i<count; i++){
			meHeightTerrainNavSpaceType * const type = navspace->GetTypeAt(i);
			pCBNavSpaceType->AddItem(type->GetName(), NULL, type);
		}
	}
	
	pCBNavSpaceType->SortItems();
	SelectActiveNavSpaceType();
}

void meWPHeightTerrain::SelectActiveNavSpaceType(){
	pCBNavSpaceType->SetSelectionWithData(GetActiveNavSpaceType());
	UpdateNavSpaceType();
}

void meWPHeightTerrain::UpdateNavSpaceType(){
	const meHeightTerrainNavSpaceType * const type = GetActiveNavSpaceType();
	
	if(type){
		pEditNavSpaceTypeColor->SetColor(type->GetColor());
		pEditNavSpaceTypeType->SetInteger(type->GetType());
		
	}else{
		pEditNavSpaceTypeColor->SetColor(decColor(0.5f, 0.5f, 0.5f));
		pEditNavSpaceTypeType->ClearText();
	}
}

void meWPHeightTerrain::UpdateVLayerList(){
	meHTVegetationLayer *selection = pVLayer;
	
	pCBVLayer->RemoveAllItems();
	
	if(pWorld){
		const int count = pWorld->GetHeightTerrain()->GetVLayerCount();
		decString text;
		int i;
		
		for(i=0; i<count; i++){
			meHTVegetationLayer * const vlayer = pWorld->GetHeightTerrain()->GetVLayerAt(i);
			text.Format("%d: %s", i + 1, vlayer->GetName().GetString());
			pCBVLayer->AddItem(text, NULL, vlayer);
		}
	}
	
	pCBVLayer->SortItems();
	
	if(!selection && pWorld && pWorld->GetHeightTerrain()->GetVLayerCount() > 0){
		selection = pWorld->GetHeightTerrain()->GetVLayerAt(0);
	}
	pCBVLayer->SetSelectionWithData(selection);
}

void meWPHeightTerrain::UpdateVLayer(){
}

void meWPHeightTerrain::UpdateVVariationList(){
	meHTVVariation *selection = pVVariation;
	
	pCBVVariation->RemoveAllItems();
	
	if(pVLayer){
		const int count = pVLayer->GetVariationCount();
		decString text;
		int i;
		
		for(i=0; i<count; i++){
			meHTVVariation * const variation = pVLayer->GetVariationAt(i);
			text.Format("Variation %d", i);
			pCBVVariation->AddItem(text, NULL, variation);
		}
	}
	
	pCBVVariation->SortItems();
	
	if(!selection && pVLayer && pVLayer->GetVariationCount() > 0){
		selection = pVLayer->GetVariationAt(0);
	}
	pCBVVariation->SetSelectionWithData(selection);
}

void meWPHeightTerrain::UpdateVVariation(){
	if(pVVariation){
		pEditVVModel->SetPath(pVVariation->GetPathModel());
		pEditVVSkin->SetPath(pVVariation->GetPathSkin());
		pEditVVRotPerForce->SetFloat(pVVariation->GetRotationPerForce());
		pEditVVRestitution->SetFloat(pVVariation->GetRestitution());
		
	}else{
		pEditVVModel->ClearPath();
		pEditVVSkin->ClearPath();
		pEditVVRotPerForce->ClearText();
		pEditVVRestitution->ClearText();
	}
}

void meWPHeightTerrain::UpdateHeightPaint(){
	if(pWorld){
		const meWorldGuiParameters &guiparams = pWorld->GetGuiParameters();
		pSldHPRadius->SetValue(guiparams.GetHPRadius());
		
		switch(guiparams.GetHPDrawMode()){
		case meWorldGuiParameters::ehpdmRaise:
		case meWorldGuiParameters::ehpdmLower:
			pSldHPStrength->SetRange(0.0f, 10.0f);
			pSldHPStrength->SetTickSpacing(1.0f);
			pSldHPStrength->SetValue(guiparams.GetHPDrawStrength());
			break;
			
		case meWorldGuiParameters::ehpdmLevel:
			pSldHPStrength->SetRange(0.0f, 1.0f);
			pSldHPStrength->SetTickSpacing(0.1f);
			pSldHPStrength->SetValue(guiparams.GetHPLevelStrength());
			break;
			
		case meWorldGuiParameters::ehpdmSmooth:
			pSldHPStrength->SetRange(0.0, 1.0f);
			pSldHPStrength->SetTickSpacing(0.1f);
			pSldHPStrength->SetValue(guiparams.GetHPSmoothStrength());
			break;
		}
		
	}else{
		pSldHPRadius->SetValue(0.0f);
		pSldHPStrength->SetValue(0.0f);
	}
	
	pBtnHPModeRaise->GetAction()->Update();
	pBtnHPModeLower->GetAction()->Update();
	pBtnHPModeLevel->GetAction()->Update();
	pBtnHPModeSmooth->GetAction()->Update();
}

void meWPHeightTerrain::UpdateMaskPaint(){
	if(pWorld){
		const meWorldGuiParameters &guiparams = pWorld->GetGuiParameters();
		
		pSldMPRadius->SetValue(guiparams.GetMPRadius());
		
	}else{
		pSldMPRadius->SetValue(0.0f);
	}
	
	pBtnMPModeDraw->GetAction()->Update();
	pBtnMPModeErase->GetAction()->Update();
}

void meWPHeightTerrain::UpdateVisibilityPaint(){
	if(pWorld){
		const meWorldGuiParameters &guiparams = pWorld->GetGuiParameters();
		
		pSldVPRadius->SetValue(guiparams.GetVPRadius());
		
	}else{
		pSldVPRadius->SetValue(0.0f);
	}
	
	pBtnVPModeVisible->GetAction()->Update();
	pBtnVPModeInvisible->GetAction()->Update();
}

void meWPHeightTerrain::UpdateSelectedPoints(){
	pActionNavSpaceFaceAdd->Update();
	pActionNavSpaceFaceRemove->Update();
}
