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
		
		igdeUndo::Ref undo(OnChanged(textField, world));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld *world) = 0;
};

class cBaseAction : public igdeAction{
protected:
	meWPHeightTerrain &pPanel;
	
public:
	cBaseAction(meWPHeightTerrain &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(world));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(meWorld *world) = 0;
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
		
		igdeUndo::Ref undo(OnChanged(comboBox, world));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, meWorld *world) = 0;
};

class cBaseEditVector2Listener : public igdeEditVector2Listener{
protected:
	meWPHeightTerrain &pPanel;
	
public:
	cBaseEditVector2Listener(meWPHeightTerrain &panel) : pPanel(panel){}
	
	void OnVector2Changed(igdeEditVector2 *editVector2) override{
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editVector2->GetVector2(), world));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decVector2 &vector, meWorld *world) = 0;
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
		
		igdeUndo::Ref undo(OnChanged(editPath, world));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeEditPath *editPath, meWorld *world) = 0;
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
		
		igdeUndo::Ref undo(OnChanged(sliderText, world));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeEditSliderText *sliderText, meWorld *world) = 0;
};


class cEditPathHT : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cEditPathHT> Ref;
	cEditPathHT(meWPHeightTerrain &panel) : cBaseEditPathListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditPath *editPath, meWorld *world) override{
		return world->GetHeightTerrain()->GetPathHT() != editPath->GetPath()
			? meUHTSetPathHT::Ref::New(world, world->GetHeightTerrain(), editPath->GetPath()) : igdeUndo::Ref();
	}
};

class cEditSectorSize : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditSectorSize> Ref;
	cEditSectorSize(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld *world) override{
		const float value = textField->GetFloat();
		return fabsf(value - world->GetHeightTerrain()->GetSectorSize()) > FLOAT_SAFE_EPSILON
			? meUHTSetSectorSize::Ref::New(world, world->GetHeightTerrain(), value) : igdeUndo::Ref();
	}
};

class cEditSectorResolution : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditSectorResolution> Ref;
	cEditSectorResolution(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld *world) override{
		const int value = textField->GetInteger();
		return value != world->GetHeightTerrain()->GetSectorResolution()
			? meUHTSetSectorResolution::Ref::New(world, world->GetHeightTerrain(), value) : igdeUndo::Ref();
	}
};

class cEditBaseHeight : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditBaseHeight> Ref;
	cEditBaseHeight(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld *world) override{
		const float value = textField->GetFloat();
		return fabsf(value - world->GetHeightTerrain()->GetBaseHeight()) > FLOAT_SAFE_EPSILON
			? meUHTSetBaseHeight::Ref::New(world, world->GetHeightTerrain(), value) : igdeUndo::Ref();
	}
};

class cEditHeightScale : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditHeightScale> Ref;
	cEditHeightScale(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld *world) override{
		const float value = textField->GetFloat();
		return fabsf(value - world->GetHeightTerrain()->GetHeightScaling()) > FLOAT_SAFE_EPSILON
			? meUHTSetHeightScaling::Ref::New(world, world->GetHeightTerrain(), value) : igdeUndo::Ref();
	}
};


class cEditPathHeightImage : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cEditPathHeightImage> Ref;
	cEditPathHeightImage(meWPHeightTerrain &panel) : cBaseEditPathListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditPath *editPath, meWorld *world) override{
		meHeightTerrainSector * const sector = pPanel.GetSector();
		if(!sector || sector->GetPathHeightImage() == editPath->GetPath()){
			return {};
		}
		
		if(pPanel.GetEngine()->GetVirtualFileSystem()->ExistsFile(decPath::CreatePathUnix(editPath->GetPath()))){
			igdeCommonDialogs::eButton answer = igdeCommonDialogs::Question(pPanel,
				igdeCommonDialogs::ebsYesNoCancel, "@World.WPHeightTerrain.Dialog.SetHeightImagePath",
				"@World.WPHeightTerrain.Dialog.ImageToSaveHeightImageToExistsAlready");
			
			if(answer == igdeCommonDialogs::ebCancel){
				return {};
				
			}else if(answer == igdeCommonDialogs::ebYes){
				const int resolution = world->GetHeightTerrain()->GetSectorResolution();
				deImage::Ref image(pPanel.GetEngine()->GetImageManager()->LoadImage(editPath->GetPath(), "/"));
				
				if(image->GetComponentCount() != 1){
					image = nullptr;
					igdeCommonDialogs::Error(pPanel, "@World.WPHeightTerrain.Dialog.ImportHeightImage",
						"@World.WPHeightTerrain.Dialog.HeightImageDoesNotHaveExactly1ColorChannelOnly");
				}
				
				if(image && (image->GetWidth() != resolution || image->GetHeight() != resolution)){
					image = nullptr;
					igdeCommonDialogs::Error(pPanel, "@World.WPHeightTerrain.Dialog.ImportHeightImage",
						"@World.WPHeightTerrain.Dialog.ImageDoesNotMatchHeightImageDimensionSetInHeightTerrain");
					// NOTE allow the user to scale the height image... maybe.. scaling is tricky
				}
				
				if(image){
					world->GetUndoSystem()->Add(
						meUHTImportHeightImage::Ref::New(world, sector, image));
				}
			}
		}
		
		return meUHTSetPathHI::Ref::New(world, sector, editPath->GetPath());
	}
};

class cEditPathVisibilityImage : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cEditPathVisibilityImage> Ref;
	cEditPathVisibilityImage(meWPHeightTerrain &panel) : cBaseEditPathListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditPath *editPath, meWorld *world) override{
		meHeightTerrainSector * const sector = pPanel.GetSector();
		if(!sector || sector->GetPathVisibilityImage() == editPath->GetPath()){
			return {};
		}
		
		if(pPanel.GetEngine()->GetVirtualFileSystem()->ExistsFile(decPath::CreatePathUnix(editPath->GetPath()))){
			igdeCommonDialogs::eButton answer = igdeCommonDialogs::Question(pPanel,
				igdeCommonDialogs::ebsYesNoCancel, "@World.WPHeightTerrain.Dialog.SetVisibilityImagePath",
				"@World.WPHeightTerrain.Dialog.ImageToSaveVisibilityImageToExistsAlready");
			
			if(answer == igdeCommonDialogs::ebCancel){
				return {};
				
			}else if(answer == igdeCommonDialogs::ebYes){
				const int resolution = world->GetHeightTerrain()->GetSectorResolution();
				deImage::Ref image;
				
				image = pPanel.GetEngine()->GetImageManager()->LoadImage(editPath->GetPath(), "/");
				
				if(image->GetComponentCount() != 1){
					image = nullptr;
					igdeCommonDialogs::Error(pPanel, "@World.WPHeightTerrain.Dialog.ImportVisibilityImage",
						"@World.WPHeightTerrain.Dialog.VisibilityImageDoesNotHaveExactly1ColorChannelOnly");
				}
				
				if(image && (image->GetWidth() != resolution || image->GetHeight() != resolution)){
					image = nullptr;
					igdeCommonDialogs::Error(pPanel, "@World.WPHeightTerrain.Dialog.ImportVisibilityImage",
						"@World.WPHeightTerrain.Dialog.ImageDoesNotMatchVisibilityImageDimensionSetInVisibilityTerrain");
					// NOTE allow the user to scale the visibility image... maybe.. scaling is tricky
				}
				
				if(image){
					meUHTImportVisibilityImage::Ref undo(meUHTImportVisibilityImage::Ref::New(
						world, sector, image));
					world->GetUndoSystem()->Add(undo);
				}
			}
		}
		
		return meUHTSetPathHI::Ref::New(world, sector, editPath->GetPath());
	}
};


class cComboTexture : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboTexture> Ref;
	cComboTexture(meWPHeightTerrain &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, meWorld*) override{
		meHeightTerrainSector * const sector = pPanel.GetSector();
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(sector){
			sector->SetActiveTexture(selection ? (meHeightTerrainTexture*)selection->GetData() : nullptr);
		}
		return {};
	}
};

class cActionMenuTexture : public igdeActionContextMenu{
	meWPHeightTerrain &pPanel;
	
public:
	typedef deTObjectReference<cActionMenuTexture> Ref;
	cActionMenuTexture(meWPHeightTerrain &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "@World.WPHeightTerrain.Action.TextureMenu"),
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
	typedef deTObjectReference<cActionTextureAdd> Ref;
	cActionTextureAdd(meWPHeightTerrain &panel) : cBaseAction(panel, "@World.WPHeightTerrain.Action.TextureAdd2",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "@World.WPHeightTerrain.Action.TextureAdd"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		meHeightTerrainSector * const sector = pPanel.GetSector();
		if(!sector){
			return {};
		}
		
		decString name(pPanel.Translate("World.WPHeightTerrain.DefaultName.Texture").ToUTF8());
		while(igdeCommonDialogs::GetString(pPanel, "@World.WPHeightTerrain.Dialog.AddTexture", "@World.WPHeightTerrain.Dialog.EnterNameOfNewTexture", name)){
			if(sector->GetTextures().HasNamed(name)){
				igdeCommonDialogs::Error(pPanel, "@World.WPHeightTerrain.Dialog.InvalidTextureNameError", "@World.WPHeightTerrain.Dialog.DuplicateTextureName");
				continue;
			}
			
			const meHeightTerrainTexture::Ref texture(meHeightTerrainTexture::Ref::New(world->GetEngine(), name));
			return meUHTAddTexture::Ref::New(world, sector, texture);
		}
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetSector());
	}
};

class cActionTextureRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionTextureRemove> Ref;
	cActionTextureRemove(meWPHeightTerrain &panel) : cBaseAction(panel, "@World.WPHeightTerrain.Action.TextureRemove2",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "@World.WPHeightTerrain.Action.TextureRemove"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		meHeightTerrainTexture * const texture = pPanel.GetTexture();
		return texture ? meUHTRemoveTexture::Ref::New(world, pPanel.GetSector(), texture) : meUHTRemoveTexture::Ref();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetTexture());
	}
};

class cTextTexTypeNumber : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextTexTypeNumber> Ref;
	cTextTexTypeNumber(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld*) override{
		meHeightTerrainTexture * const texture = pPanel.GetTexture();
		const int value = textField->GetInteger();
		return texture && value != texture->GetTypeNumber() ? meUHTTexSetTypeNum::Ref::New(texture, value) : meUHTTexSetTypeNum::Ref();
	}
};

class cPathTexSkin : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cPathTexSkin> Ref;
	cPathTexSkin(meWPHeightTerrain &panel) : cBaseEditPathListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditPath *editPath, meWorld *world) override{
		meHeightTerrainTexture * const texture = pPanel.GetTexture();
		return texture && editPath->GetPath() != texture->GetPathSkin()
			? meUHTSetTexSkin::Ref::New(world, pPanel.GetSector(), texture, editPath->GetPath()) : igdeUndo::Ref();
	}
};

class cPathTexMask : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cPathTexMask> Ref;
	cPathTexMask(meWPHeightTerrain &panel) : cBaseEditPathListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditPath *editPath, meWorld *world) override{
		meHeightTerrainTexture * const texture = pPanel.GetTexture();
		return texture && editPath->GetPath() != texture->GetPathMask()
			? meUHTSetTexMask::Ref::New(world, pPanel.GetSector(), texture, editPath->GetPath()) : igdeUndo::Ref();
	}
};

class cEditTexUVOffset : public cBaseEditVector2Listener{
public:
	typedef deTObjectReference<cEditTexUVOffset> Ref;
	cEditTexUVOffset(meWPHeightTerrain &panel) : cBaseEditVector2Listener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector2 &vector, meWorld *world) override{
		meHeightTerrainTexture * const texture = pPanel.GetTexture();
		return texture && !vector.IsEqualTo(decVector2(texture->GetProjectionOffsetU(), texture->GetProjectionOffsetV()))
			? meUHTSetTexUVOffset::Ref::New(world, pPanel.GetSector(), texture, vector) : igdeUndo::Ref();
	}
};

class cEditTexUVScale : public cBaseEditVector2Listener{
public:
	typedef deTObjectReference<cEditTexUVScale> Ref;
	cEditTexUVScale(meWPHeightTerrain &panel) : cBaseEditVector2Listener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector2 &vector, meWorld *world) override{
		meHeightTerrainTexture * const texture = pPanel.GetTexture();
		return texture && !vector.IsEqualTo(decVector2(texture->GetProjectionScalingU(), texture->GetProjectionScalingV()))
			? meUHTSetTexUVScaling::Ref::New(world, pPanel.GetSector(), texture, vector) : igdeUndo::Ref();
	}
};

class cEditTexUVRotation : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditTexUVRotation> Ref;
	cEditTexUVRotation(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld *world) override{
		meHeightTerrainTexture * const texture = pPanel.GetTexture();
		const float value = textField->GetFloat();
		return texture && fabsf(value - texture->GetProjectionRotation()) > FLOAT_SAFE_EPSILON
			? meUHTSetTexUVRotation::Ref::New(world, pPanel.GetSector(), texture, value) : igdeUndo::Ref();
	}
};


class cComboNavSpace : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboNavSpace> Ref;
	cComboNavSpace(meWPHeightTerrain &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, meWorld *world) override{
		meHeightTerrainSector * const sector = pPanel.GetSector();
		if(sector){
			const igdeListItem * const selection = comboBox->GetSelectedItem();
			sector->SetActiveNavSpace(selection ? (meHeightTerrainNavSpace*)selection->GetData() : nullptr);
		}
		return {};
	}
};

class cActionMenuNavSpace : public igdeActionContextMenu{
	meWPHeightTerrain &pPanel;
	
public:
	typedef deTObjectReference<cActionMenuNavSpace> Ref;
	cActionMenuNavSpace(meWPHeightTerrain &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "@World.WPHeightTerrain.Action.NavSpaceMenu"),
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
	typedef deTObjectReference<cActionNavSpaceAdd> Ref;
	cActionNavSpaceAdd(meWPHeightTerrain &panel) : cBaseAction(panel, "@World.WPHeightTerrain.Action.NavSpaceAdd2",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "@World.WPHeightTerrain.Action.NavSpaceAdd"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		meHeightTerrainSector * const sector = pPanel.GetSector();
		if(sector){
			decString name(pPanel.Translate("World.WPHeightTerrain.DefaultName.NavSpace").ToUTF8());
			while(igdeCommonDialogs::GetString(pPanel, "@World.UAddNavSpace.AddNavigationSpace",
			"@World.WPHeightTerrain.Dialog.EnterNameOfNewNavigationSpace", name)){
				if(sector->GetNavSpaces().HasNamed(name)){
					igdeCommonDialogs::Error(pPanel, "@World.WPHeightTerrain.Dialog.InvalidNavigationSpaceNameError",
						"@World.WPHeightTerrain.Dialog.ANavigationSpaceWithThisNameExistsAlready");
					continue;
				}
				
				const meHeightTerrainNavSpace::Ref navspace(meHeightTerrainNavSpace::Ref::New(*world->GetEngine(), name));
				return meUHTNavSpaceAdd::Ref::New(sector, navspace);
			}
		}
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld() && pPanel.GetWorld()->GetHeightTerrain());
	}
};

class cActionNavSpaceRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionNavSpaceRemove> Ref;
	cActionNavSpaceRemove(meWPHeightTerrain &panel) : cBaseAction(panel, "@World.WPHeightTerrain.Action.NavSpaceRemove2",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "@World.WPHeightTerrain.Action.NavSpaceRemove"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		meHeightTerrainNavSpace * const navspace = pPanel.GetActiveNavSpace();
		return navspace ? meUHTNavSpaceRemove::Ref::New(navspace->GetHTSector(), navspace) : meUHTNavSpaceRemove::Ref();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveNavSpace());
	}
};

class cTextNavSpaceLayer : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextNavSpaceLayer> Ref;
	cTextNavSpaceLayer(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld*) override{
		meHeightTerrainNavSpace * const navspace = pPanel.GetActiveNavSpace();
		const int value = textField->GetInteger();
		return navspace && value != navspace->GetLayer() ? meUHTNavSpaceSetLayer::Ref::New(navspace, value) : meUHTNavSpaceSetLayer::Ref();
	}
};

class cTextNavSpaceSnapDistance : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextNavSpaceSnapDistance> Ref;
	cTextNavSpaceSnapDistance(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld*) override{
		meHeightTerrainNavSpace * const navspace = pPanel.GetActiveNavSpace();
		const float value = textField->GetFloat();
		return navspace && fabsf(value - navspace->GetSnapDistance()) > FLOAT_SAFE_EPSILON
			? meUHTNavSpaceSetSnapDist::Ref::New(navspace, value) : igdeUndo::Ref();
	}
};

class cTextNavSpaceSnapAngle : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextNavSpaceSnapAngle> Ref;
	cTextNavSpaceSnapAngle(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld*) override{
		meHeightTerrainNavSpace * const navspace = pPanel.GetActiveNavSpace();
		const float value = textField->GetFloat();
		return navspace && fabsf(value - navspace->GetSnapAngle()) > FLOAT_SAFE_EPSILON
			? meUHTNavSpaceSetSnapAngle::Ref::New(navspace, value) : igdeUndo::Ref();
	}
};


class cComboNavSpaceType : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboNavSpaceType> Ref;
	cComboNavSpaceType(meWPHeightTerrain &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, meWorld*) override{
		meHeightTerrainNavSpace * const navspace = pPanel.GetActiveNavSpace();
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(navspace){
			navspace->SetActiveType(selection ? (meHeightTerrainNavSpaceType*)selection->GetData() : nullptr);
		}
		return {};
	}
};

class cActionMenuNavSpaceType : public igdeActionContextMenu{
	meWPHeightTerrain &pPanel;
	
public:
	typedef deTObjectReference<cActionMenuNavSpaceType> Ref;
	cActionMenuNavSpaceType(meWPHeightTerrain &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "@World.WPHeightTerrain.Action.NavSpaceTypeMenu"),
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
	typedef deTObjectReference<cActionNavSpaceTypeAdd> Ref;
	cActionNavSpaceTypeAdd(meWPHeightTerrain &panel) : cBaseAction(panel, "@World.WPHeightTerrain.Action.NavSpaceTypeAdd2",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "@World.WPHeightTerrain.Action.NavSpaceTypeAdd"){}
	
	igdeUndo::Ref OnAction(meWorld*) override{
		meHeightTerrainNavSpace * const navspace = pPanel.GetActiveNavSpace();
		decString name(pPanel.Translate("World.WPHeightTerrain.DefaultName.Type").ToUTF8());
		while(igdeCommonDialogs::GetString(pPanel, "@World.WPHeightTerrain.Dialog.AddNavigationSpaceType",
		"@World.WPHeightTerrain.Dialog.EnterNameOfNewNavigationSpaceType", name)){
			if(navspace->GetTypes().HasNamed(name)){
				igdeCommonDialogs::Error(pPanel, "@World.WPHeightTerrain.Dialog.InvalidNavigationSpaceTypeNameError",
					"@World.WPHeightTerrain.Dialog.ANavigationSpaceTypeWithThisNameExistsAlready");
				continue;
			}
			
			const meHeightTerrainNavSpaceType::Ref type(meHeightTerrainNavSpaceType::Ref::New(name));
			return meUHTNavSpaceTypeAdd::Ref::New(navspace, type);
		}
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveNavSpace());
	}
};

class cActionNavSpaceTypeRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionNavSpaceTypeRemove> Ref;
	cActionNavSpaceTypeRemove(meWPHeightTerrain &panel) : cBaseAction(panel, "@World.WPHeightTerrain.Action.NavSpaceTypeRemove2",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "@World.WPHeightTerrain.Action.NavSpaceTypeRemove"){}
	
	igdeUndo::Ref OnAction(meWorld*) override{
		meHeightTerrainNavSpaceType * const type = pPanel.GetActiveNavSpaceType();
		return type ? meUHTNavSpaceTypeRemove::Ref::New(type) : meUHTNavSpaceTypeRemove::Ref();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveNavSpaceType());
	}
};

class cTextNavSpaceTypeName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextNavSpaceTypeName> Ref;
	cTextNavSpaceTypeName(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld*) override{
		meHeightTerrainNavSpaceType * const type = pPanel.GetActiveNavSpaceType();
		if(!type || textField->GetText() == type->GetName()){
			return {};
		}
		if(pPanel.GetActiveNavSpace()->GetTypes().HasNamed(textField->GetText())){
			igdeCommonDialogs::Error(pPanel, "@World.WPHeightTerrain.Dialog.InvalidNavigationSpaceTypeNameError", "@World.WPHeightTerrain.Dialog.ANavigationSpaceTypeWithThisNameExistsAlready");
			pPanel.UpdateNavSpaceType();
			return {};
		}
		return meUHTNavSpaceTypeSetName::Ref::New(type, textField->GetText());
	}
};

class cTextNavSpaceTypeType : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextNavSpaceTypeType> Ref;
	cTextNavSpaceTypeType(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld*) override{
		meHeightTerrainNavSpaceType * const type = pPanel.GetActiveNavSpaceType();
		const int value = textField->GetInteger();
		return type && type->GetType() != value ? meUHTNavSpaceTypeSetType::Ref::New(type, value) : meUHTNavSpaceTypeSetType::Ref();
	}
};

class cTextNavSpaceTypeColor : public igdeColorBoxListener{
	meWPHeightTerrain &pPanel;
	
public:
	typedef deTObjectReference<cTextNavSpaceTypeColor> Ref;
	cTextNavSpaceTypeColor(meWPHeightTerrain &panel) : pPanel(panel){}
	
	virtual void OnColorChanged(igdeColorBox *colorBox){
		meHeightTerrainNavSpaceType * const type = pPanel.GetActiveNavSpaceType();
		if(!type || colorBox->GetColor().IsEqualTo(type->GetColor())){
			return;
		}
		pPanel.GetWorld()->GetUndoSystem()->Add(meUHTNavSpaceTypeSetColor::Ref::New(
			type, colorBox->GetColor()));
	}
};

class cActionNavSpaceFaceAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionNavSpaceFaceAdd> Ref;
	cActionNavSpaceFaceAdd(meWPHeightTerrain &panel) : cBaseAction(panel, "@World.WPHeightTerrain.Action.NavSpaceFaceAdd2",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "@World.WPHeightTerrain.Action.NavSpaceFaceAdd"){}
	
	bool CanUse(const meWorld &world) const{
		const meHeightTerrainNavSpace * const navspace = pPanel.GetActiveNavSpace();
		if(!navspace || world.GetGuiParameters().GetWorkMode() != meWorldGuiParameters::ewmNavSpaceEdit){
			return false;
		}
		
		const decTList<int> &selNavPoints = navspace->GetHTSector()->GetSelectedNavPoints();
		if(selNavPoints.GetCount() < 3){
			return false;
		}
		
		for(const auto &type : navspace->GetTypes()){
			for(const auto &face : type->GetFaces()){
				if(face->NavPointsMatch(selNavPoints)){
					return false;
				}
			}
		}
		
		return true;
	}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		if(!CanUse(*world)){
			return {};
		}
		
		const meHeightTerrainNavSpaceFace::Ref face(meHeightTerrainNavSpaceFace::Ref::New());
		face->GetNavPoints() = pPanel.GetSector()->GetSelectedNavPoints();
		face->OrderClockwise(world->GetHeightTerrain()->GetSectorResolution());
		return meUHTNavSpaceFaceAdd::Ref::New(pPanel.GetActiveNavSpaceType(), face);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld() && CanUse(*pPanel.GetWorld()));
	}
};

class cActionNavSpaceFaceRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionNavSpaceFaceRemove> Ref;
	cActionNavSpaceFaceRemove(meWPHeightTerrain &panel) : cBaseAction(panel, "@World.WPHeightTerrain.Action.NavSpaceFaceRemove2",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"@World.WPHeightTerrain.Action.NavSpaceFaceRemove"){}
	
	bool CanUse(const meWorld &world) const{
		return pPanel.GetActiveNavSpace()
			&& world.GetGuiParameters().GetWorkMode() == meWorldGuiParameters::ewmNavSpaceEdit
			&& pPanel.GetSector()->GetSelectedNavPoints().IsNotEmpty();
	}
	
	void FindFaces(const meWorld &world, meHeightTerrainNavSpaceFace::List &faces){
		const decTList<int> &selNavPoints = pPanel.GetSector()->GetSelectedNavPoints();
		const meHeightTerrainNavSpace &navspace = *pPanel.GetActiveNavSpace();
		
		if(selNavPoints.GetCount() == 1){
			const int navpoint = selNavPoints.First();
			navspace.GetTypes().Visit([&](const meHeightTerrainNavSpaceType &type){
				type.GetFaces().Visit([&](meHeightTerrainNavSpaceFace * const face){
					if(face->HasNavPoint(navpoint)){
						faces.Add(face);
					}
				});
			});
			
		}else if(selNavPoints.GetCount() == 2){
			const int navpoint1 = selNavPoints.First();
			const int navpoint2 = selNavPoints.GetAt(1);
			
			navspace.GetTypes().Visit([&](const meHeightTerrainNavSpaceType &type){
				type.GetFaces().Visit([&](meHeightTerrainNavSpaceFace * const face){
					if(face->HasNavEdge(navpoint1, navpoint2)){
						faces.Add(face);
					}
				});
			});
			
		}else{ //selNavPoints.GetCount() > 2
			navspace.GetTypes().Visit([&](const meHeightTerrainNavSpaceType &type){
				type.GetFaces().Visit([&](meHeightTerrainNavSpaceFace * const face){
					if(face->HasAllNavPointsIn(selNavPoints)){
						faces.Add(face);
					}
				});
			});
		}
	}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		if(!CanUse(*world)){
			return {};
		}
		
		meHeightTerrainNavSpaceFace::List faces;
		FindFaces(*world, faces);
		return faces.IsNotEmpty() ? meUHTNavSpaceFaceRemove::Ref::New(faces) : meUHTNavSpaceFaceRemove::Ref();
	}
	
	void Update() override{
		bool enabled = pPanel.GetWorld() && CanUse(*pPanel.GetWorld());
		if(enabled){
			meHeightTerrainNavSpaceFace::List faces;
			FindFaces(*pPanel.GetWorld(), faces);
			enabled = faces.IsNotEmpty();
		}
		SetEnabled(enabled);
	}
};



class cActionUpdateVegetation : public cBaseAction{
public:
	typedef deTObjectReference<cActionUpdateVegetation> Ref;
	cActionUpdateVegetation(meWPHeightTerrain &panel) : cBaseAction(panel, "@World.WPHeightTerrain.Action.UpdateVegetation2",
		nullptr, "@World.WPHeightTerrain.Action.UpdateVegetationVisible"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->ForceUpdateVegetation(false);
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld());
	}
};

class cActionUpdateVegetationAll : public cBaseAction{
public:
	typedef deTObjectReference<cActionUpdateVegetationAll> Ref;
	cActionUpdateVegetationAll(meWPHeightTerrain &panel) : cBaseAction(panel, "@World.WPHeightTerrain.Action.UpdateVegetationAll2",
		nullptr, "@World.WPHeightTerrain.Action.UpdateVegetationAll"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->ForceUpdateVegetation(true);
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld());
	}
};

class cActionClearVegetation : public cBaseAction{
public:
	typedef deTObjectReference<cActionClearVegetation> Ref;
	cActionClearVegetation(meWPHeightTerrain &panel) : cBaseAction(panel, "@World.WPHeightTerrain.Action.ClearVegetation2",
		nullptr, "@World.WPHeightTerrain.Action.ClearVegetation"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->ClearVegetation();
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld());
	}
};

class cPathPathPFCache : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cPathPathPFCache> Ref;
	cPathPathPFCache(meWPHeightTerrain &panel) : cBaseEditPathListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditPath *editPath, meWorld *world) override{
		meHeightTerrainSector * const sector = pPanel.GetSector();
		return sector && editPath->GetPath() != sector->GetPathPFCacheImage()
			? meUHTSetPathPFCache::Ref::New(world, sector, editPath->GetPath()) : igdeUndo::Ref();
	}
};


class cComboVLayer : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboVLayer> Ref;
	cComboVLayer(meWPHeightTerrain &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, meWorld *world) override{
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		world->GetHeightTerrain()->SetActiveVLayer(selection ? (meHTVegetationLayer*)selection->GetData() : nullptr);
		return {};
	}
};

class cActionMenuVLayer : public igdeActionContextMenu{
	meWPHeightTerrain &pPanel;
	
public:
	typedef deTObjectReference<cActionMenuVLayer> Ref;
	cActionMenuVLayer(meWPHeightTerrain &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "@World.WPHeightTerrain.Action.VLayerMenu"),
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
	typedef deTObjectReference<cActionVLayerAdd> Ref;
	cActionVLayerAdd(meWPHeightTerrain &panel) : cBaseAction(panel, "@World.WPHeightTerrain.Action.VLayerAdd2",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "@World.WPHeightTerrain.Action.VLayerAdd"){}
	
	virtual igdeUndo::Ref OnAction(meWorld *world){
		decString name(pPanel.Translate("World.WPHeightTerrain.DefaultName.VLayer").ToUTF8());
		if(!igdeCommonDialogs::GetString(pPanel, "@World.WPHeightTerrain.Dialog.AddVegetationLayer", "@World.WPHeightTerrain.Dialog.EnterNameOfNewLayer", name)){
			return {};
		}
		
		const meHTVegetationLayer::Ref vlayer(meHTVegetationLayer::Ref::New(world->GetEngine(), name));
		
		vlayer->AddRule(meHTVRuleResult::Ref::New());
		
		return meUHTVLayerAdd::Ref::New(world, world->GetHeightTerrain(), vlayer);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetSector());
	}
};

class cActionVLayerRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionVLayerRemove> Ref;
	cActionVLayerRemove(meWPHeightTerrain &panel) : cBaseAction(panel, "@World.WPHeightTerrain.Action.VLayerRemove2",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "@World.WPHeightTerrain.Action.VLayerRemove"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		meHTVegetationLayer * const vlayer = pPanel.GetVLayer();
		return vlayer ? meUHTVLayerRemove::Ref::New(world, world->GetHeightTerrain(), vlayer) : meUHTVLayerRemove::Ref();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetVLayer());
	}
};

class cActionVLayerUp : public cBaseAction{
public:
	typedef deTObjectReference<cActionVLayerUp> Ref;
	cActionVLayerUp(meWPHeightTerrain &panel) : cBaseAction(panel, "@World.WPHeightTerrain.Action.VLayerUp2",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp), "@World.WPHeightTerrain.Action.VLayerUp"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		meHTVegetationLayer * const vlayer = pPanel.GetVLayer();
		return vlayer && world->GetHeightTerrain()->GetVLayers().GetCount() > 1
			? meUHTVLayerUp::Ref::New(world, world->GetHeightTerrain(), vlayer) : igdeUndo::Ref();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetVLayer() && pPanel.GetWorld()->GetHeightTerrain()->GetVLayers().GetCount() > 1);
	}
};

class cActionVLayerDown : public cBaseAction{
public:
	typedef deTObjectReference<cActionVLayerDown> Ref;
	cActionVLayerDown(meWPHeightTerrain &panel) : cBaseAction(panel, "@World.WPHeightTerrain.Action.VLayerDown2",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp), "@World.WPHeightTerrain.Action.VLayerDown"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		meHTVegetationLayer * const vlayer = pPanel.GetVLayer();
		return vlayer && world->GetHeightTerrain()->GetVLayers().IndexOf(vlayer)
			< world->GetHeightTerrain()->GetVLayers().GetCount() - 1
				? meUHTVLayerUp::Ref::New(world, world->GetHeightTerrain(), vlayer) : igdeUndo::Ref();
	}
	
	void Update() override{
		meHTVegetationLayer * const vlayer = pPanel.GetVLayer();
		SetEnabled(vlayer && pPanel.GetWorld()->GetHeightTerrain()->GetVLayers().IndexOf(vlayer)
			< pPanel.GetWorld()->GetHeightTerrain()->GetVLayers().GetCount() - 1);
	}
};


class cComboVVariation : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboVVariation> Ref;
	cComboVVariation(meWPHeightTerrain &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, meWorld*) override{
		meHTVegetationLayer * const vlayer = pPanel.GetVLayer();
		if(vlayer){
			vlayer->SetActiveVariation(static_cast<meHTVVariation*>(comboBox->GetSelectedItemData()));
		}
		return {};
	}
};

class cActionMenuVVariation : public igdeActionContextMenu{
	meWPHeightTerrain &pPanel;
	
public:
	typedef deTObjectReference<cActionMenuVVariation> Ref;
	cActionMenuVVariation(meWPHeightTerrain &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "@World.WPHeightTerrain.Action.VVariationMenu"),
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
	typedef deTObjectReference<cActionVVariationAdd> Ref;
	cActionVVariationAdd(meWPHeightTerrain &panel) : cBaseAction(panel, "@World.WPHeightTerrain.Action.VVariationAdd2",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "@World.WPHeightTerrain.Action.VVariationAddDesc"){}
	
	virtual igdeUndo::Ref OnAction(meWorld *world){
		meHTVegetationLayer * const vlayer = pPanel.GetVLayer();
		if(!vlayer){
			return {};
		}
		
		const meHTVVariation::Ref variation(meHTVVariation::Ref::New(world->GetEngine()));
		return meUHTVVariationAdd::Ref::New(vlayer, variation);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetVLayer());
	}
};

class cActionVVariationRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionVVariationRemove> Ref;
	cActionVVariationRemove(meWPHeightTerrain &panel) : cBaseAction(panel, "@World.WPHeightTerrain.Action.VVariationRemove2",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "@World.WPHeightTerrain.Action.VVariationRemoveDesc"){}
	
	igdeUndo::Ref OnAction(meWorld*) override{
		meHTVVariation * const variation = pPanel.GetVVariation();
		return variation ? meUHTVVariationRemove::Ref::New(pPanel.GetVLayer(), variation) : meUHTVVariationRemove::Ref();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetVVariation());
	}
};

class cPathVVModel : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cPathVVModel> Ref;
	cPathVVModel(meWPHeightTerrain &panel) : cBaseEditPathListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditPath *editPath, meWorld*) override{
		meHTVVariation * const variation = pPanel.GetVVariation();
		return variation && editPath->GetPath() != variation->GetPathModel()
			? meUHTVVarSetModel::Ref::New(variation, editPath->GetPath()) : igdeUndo::Ref();
	}
};

class cPathVVSkin : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cPathVVSkin> Ref;
	cPathVVSkin(meWPHeightTerrain &panel) : cBaseEditPathListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditPath *editPath, meWorld*) override{
		meHTVVariation * const variation = pPanel.GetVVariation();
		return variation && editPath->GetPath() != variation->GetPathSkin()
			? meUHTVVarSetSkin::Ref::New(variation, editPath->GetPath()) : igdeUndo::Ref();
	}
};

class cTextVVRotPerForce : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextVVRotPerForce> Ref;
	cTextVVRotPerForce(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld*) override{
		meHTVVariation * const variation = pPanel.GetVVariation();
		const float value = textField->GetFloat();
		return variation && fabsf(value - variation->GetRotationPerForce()) > FLOAT_SAFE_EPSILON
			? meUHTVVarSetRotPerForce::Ref::New(variation, value) : igdeUndo::Ref();
	}
};

class cTextVVRestitution : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextVVRestitution> Ref;
	cTextVVRestitution(meWPHeightTerrain &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld*) override{
		meHTVVariation * const variation = pPanel.GetVVariation();
		const float value = textField->GetFloat();
		return variation && fabsf(value - variation->GetRestitution()) > FLOAT_SAFE_EPSILON
			? meUHTVVarSetRestitution::Ref::New(variation, value) : igdeUndo::Ref();
	}
};


class cActionHPMode : public cBaseAction{
	const meWorldGuiParameters::eHPDrawModes pMode;
	
public:
	typedef deTObjectReference<cActionHPMode> Ref;
	cActionHPMode(meWPHeightTerrain &panel, meWorldGuiParameters::eHPDrawModes mode,
	const char *text, igdeIcon *icon, const char *description) :
		cBaseAction(panel, text, icon, description), pMode(mode){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetGuiParameters().SetHPDrawMode(pMode);
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld());
		SetSelected(pPanel.GetWorld() && pPanel.GetWorld()->GetGuiParameters().GetHPDrawMode() == pMode);
	}
};

class cSliderHPRadius : public cBaseEditSliderTextListener{
public:
	typedef deTObjectReference<cSliderHPRadius> Ref;
	cSliderHPRadius(meWPHeightTerrain &panel) : cBaseEditSliderTextListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditSliderText *sliderText, meWorld *world) override{
		world->GetGuiParameters().SetHPRadius(sliderText->GetValue());
		return {};
	}
};

class cSliderHPStrength : public cBaseEditSliderTextListener{
public:
	typedef deTObjectReference<cSliderHPStrength> Ref;
	cSliderHPStrength(meWPHeightTerrain &panel) : cBaseEditSliderTextListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditSliderText *sliderText, meWorld *world) override{
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
		return {};
	}
};


class cActionMPMode : public cBaseAction{
	const meWorldGuiParameters::eMPDrawModes pMode;
	
public:
	typedef deTObjectReference<cActionMPMode> Ref;
	cActionMPMode(meWPHeightTerrain &panel, meWorldGuiParameters::eMPDrawModes mode,
	const char *text, igdeIcon *icon, const char *description) :
		cBaseAction(panel, text, icon, description), pMode(mode){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetGuiParameters().SetMPDrawMode(pMode);
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld());
		SetSelected(pPanel.GetWorld() && pPanel.GetWorld()->GetGuiParameters().GetMPDrawMode() == pMode);
	}
};

class cSliderMPRadius : public cBaseEditSliderTextListener{
public:
	typedef deTObjectReference<cSliderMPRadius> Ref;
	cSliderMPRadius(meWPHeightTerrain &panel) : cBaseEditSliderTextListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditSliderText *sliderText, meWorld *world) override{
		world->GetGuiParameters().SetMPRadius(sliderText->GetValue());
		return {};
	}
};


class cActionVPMode : public cBaseAction{
	const meWorldGuiParameters::eVPDrawModes pMode;
	
public:
	typedef deTObjectReference<cActionVPMode> Ref;
	cActionVPMode(meWPHeightTerrain &panel, meWorldGuiParameters::eVPDrawModes mode,
	const char *text, igdeIcon *icon, const char *description) :
		cBaseAction(panel, text, icon, description), pMode(mode){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetGuiParameters().SetVPDrawMode(pMode);
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld());
		SetSelected(pPanel.GetWorld() && pPanel.GetWorld()->GetGuiParameters().GetVPDrawMode() == pMode);
	}
};

class cSliderVPRadius : public cBaseEditSliderTextListener{
public:
	typedef deTObjectReference<cSliderVPRadius> Ref;
	cSliderVPRadius(meWPHeightTerrain &panel) : cBaseEditSliderTextListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeEditSliderText *sliderText, meWorld *world) override{
		world->GetGuiParameters().SetVPRadius(sliderText->GetValue());
		return {};
	}
};

}



// Class meWPHeightTerrain
////////////////////////////

// Constructor, destructor
////////////////////////////

meWPHeightTerrain::meWPHeightTerrain(meWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener = meWPHeightTerrainListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	pActionMenuTexture = cActionMenuTexture::Ref::New(*this);
	pActionTextureAdd = cActionTextureAdd::Ref::New(*this);
	pActionTextureRemove = cActionTextureRemove::Ref::New(*this);
	
	pActionMenuNavSpace = cActionMenuNavSpace::Ref::New(*this);
	pActionNavSpaceAdd = cActionNavSpaceAdd::Ref::New(*this);
	pActionNavSpaceRemove = cActionNavSpaceRemove::Ref::New(*this);
	
	pActionMenuNavSpaceType = cActionMenuNavSpaceType::Ref::New(*this);
	pActionNavSpaceTypeAdd = cActionNavSpaceTypeAdd::Ref::New(*this);
	pActionNavSpaceTypeRemove = cActionNavSpaceTypeRemove::Ref::New(*this);
	
	pActionNavSpaceFaceAdd = cActionNavSpaceFaceAdd::Ref::New(*this);
	pActionNavSpaceFaceRemove = cActionNavSpaceFaceRemove::Ref::New(*this);
	
	pActionMenuVLayer = cActionMenuVLayer::Ref::New(*this);
	pActionVLayerAdd = cActionVLayerAdd::Ref::New(*this);
	pActionVLayerRemove = cActionVLayerRemove::Ref::New(*this);
	pActionVLayerUp = cActionVLayerUp::Ref::New(*this);
	pActionVLayerDown = cActionVLayerDown::Ref::New(*this);
	
	pActionMenuVVariation = cActionMenuVVariation::Ref::New(*this);
	pActionVVariationAdd = cActionVVariationAdd::Ref::New(*this);
	pActionVVariationRemove = cActionVVariationRemove::Ref::New(*this);
	
	
	// height terrain
	helper.GroupBox(content, groupBox, "@World.WPHeightTerrain.HeightTerrain");
	
	igdeFilePattern::List filePatterns;
	filePatterns.Add(igdeFilePattern::Ref::New(
		"@World.WPHeightTerrain.DragEnGineHeightTerrain", "*.deterrain", ".deterrain"));
	helper.EditPath(groupBox, "@World.WPHeightTerrain.HeightTerrain", "@World.WPHeightTerrain.FileToSaveHeightTerrainTo.ToolTip",
		filePatterns, pEditPathHTerrain, cEditPathHT::Ref::New(*this));
	helper.EditFloat(groupBox, "@World.WPHeightTerrain.SectorSize", "@World.WPHeightTerrain.SizeOfSectorsAlongXZAxis.ToolTip",
		pEditSectorSize, cEditSectorSize::Ref::New(*this));
	helper.EditInteger(groupBox, "@World.WPHeightTerrain.SectorResolution", "@World.WPHeightTerrain.SectorResolution.ToolTip",
		pEditSectorResolution, cEditSectorResolution::Ref::New(*this));
	helper.EditFloat(groupBox, "@World.WPHeightTerrain.BaseHeight", "@World.WPHeightTerrain.SetBaseHeightOfHeightImageValues.ToolTip",
		pEditBaseHeight, cEditBaseHeight::Ref::New(*this));
	helper.EditFloat(groupBox, "@World.WPHeightTerrain.HeightScaling", "@World.WPHeightTerrain.SetHeightScalingOfHeightImageValues.ToolTip",
		pEditHeightScale, cEditHeightScale::Ref::New(*this));
	
	
	// sector
	helper.GroupBox(content, groupBox, "@World.WPHeightTerrain.Sector");
	
	helper.EditPath(groupBox, "@World.WPHeightTerrain.HeightImage", "@World.WPHeightTerrain.FileToSaveHeightImageTo.ToolTip",
		igdeEnvironment::efpltImage, pEditPathHeightImage, cEditPathHeightImage::Ref::New(*this));
	helper.EditPath(groupBox, "@World.WPHeightTerrain.VisibilityImage", "@World.WPHeightTerrain.FileToSaveVisibilityImageTo.ToolTip",
		igdeEnvironment::efpltImage, pEditPathVisImage, cEditPathVisibilityImage::Ref::New(*this));
	
	
	// texture
	helper.GroupBox(content, groupBox, "@World.WPSObject.Texture");
	
	helper.FormLineStretchFirst(groupBox, "@World.WPSObject.Texture", "@World.WPHeightTerrain.TextureToEdit.ToolTip", formLine);
	helper.ComboBox(formLine, "@World.WPHeightTerrain.TextureToEdit.ToolTip", pCBTexture, cComboTexture::Ref::New(*this));
	helper.Button(formLine, pBtnTexture, pActionMenuTexture);
	pActionMenuTexture->SetWidget(pBtnTexture);
	
	helper.EditInteger(groupBox, "@World.WPWorld.TypeNumber", "@World.WPHeightTerrain.TypeNumberOfTexture.ToolTip",
		pEditTexTypeNum, cTextTexTypeNumber::Ref::New(*this));
	helper.EditPath(groupBox, "@World.WPSObject.Skin", "@World.WPHeightTerrain.SkinToUseForTexture.ToolTip",
		igdeEnvironment::efpltSkin, pEditTexSkin, cPathTexSkin::Ref::New(*this));
	helper.EditPath(groupBox, "@World.WPHeightTerrain.Mask", "@World.WPHeightTerrain.MaskToUseForTexture.ToolTip",
		igdeEnvironment::efpltSkin, pEditTexMask, cPathTexMask::Ref::New(*this));
	helper.EditVector2(groupBox, "@World.WPHeightTerrain.Offset", "@World.WPHeightTerrain.OffsetsProjectedTextureRelativeToWorldOrigin.ToolTip",
		pEditTexUVOffset, cEditTexUVOffset::Ref::New(*this));
	helper.EditVector2(groupBox, "@World.WPHeightTerrain.Scaling", "@World.WPHeightTerrain.ScalingOfProjectedTexture.ToolTip",
		pEditTexUVScale, cEditTexUVScale::Ref::New(*this));
	helper.EditFloat(groupBox, "@World.WPHeightTerrain.Rotation", "@World.WPHeightTerrain.RotationOfProjectedTexture.ToolTip",
		pEditTexUVRot, cEditTexUVRotation::Ref::New(*this));
	
	
	// navigation space
	helper.GroupBox(content, groupBox, "@World.WPHeightTerrain.NavigationSpace", true);
	
	helper.FormLineStretchFirst(groupBox, "@World.WPHeightTerrain.NavSpace", "@World.WPHeightTerrain.NavSpace.ToolTip", formLine);
	helper.ComboBox(formLine, "@World.WPHeightTerrain.NavSpace.ToolTip", pCBNavSpace, cComboNavSpace::Ref::New(*this));
	helper.Button(formLine, pBtnNavSpace, pActionMenuNavSpace);
	pActionMenuNavSpace->SetWidget(pBtnNavSpace);
	
	helper.EditInteger(groupBox, "@World.WPWorld.Layer", "@World.WPHeightTerrain.NavigationLayer.ToolTip",
		pEditNavSpaceLayer, cTextNavSpaceLayer::Ref::New(*this));
	helper.EditFloat(groupBox, "@World.WPHeightTerrain.SnapDistance", "@World.WPHeightTerrain.SnapDistance.ToolTip",
		pEditNavSpaceSnapDist, cTextNavSpaceSnapDistance::Ref::New(*this));
	helper.EditFloat(groupBox, "@World.WPHeightTerrain.SnapAngle", "@World.WPHeightTerrain.SnapAngle.ToolTip",
		pEditNavSpaceSnapAngle, cTextNavSpaceSnapAngle::Ref::New(*this));
	
	
	// navigation space type
	helper.GroupBox(content, groupBox, "@World.WPHeightTerrain.NavigationType", true);
	
	helper.FormLineStretchFirst(groupBox, "@World.WPHeightTerrain.Type.Label2", "@World.WPHeightTerrain.NavigationTypeToEdit.ToolTip", formLine);
	helper.ComboBox(formLine, "@World.WPHeightTerrain.NavigationTypeToEdit.ToolTip", pCBNavSpaceType, cComboNavSpaceType::Ref::New(*this));
	helper.Button(formLine, pBtnNavSpaceType, pActionMenuNavSpaceType);
	pActionMenuNavSpaceType->SetWidget(pBtnNavSpaceType);
	
	helper.EditString(groupBox, "@World.WPHeightTerrain.Name", "@World.WPHeightTerrain.NavigationSpaceTypeName.ToolTip",
		pEditNavSpaceTypeName, cTextNavSpaceTypeName::Ref::New(*this));
	helper.EditInteger(groupBox, "@World.WPHeightTerrain.Type.Label3", "@World.WPHeightTerrain.NavigationSpaceTypeNumber.ToolTip",
		pEditNavSpaceTypeType, cTextNavSpaceTypeType::Ref::New(*this));
	helper.ColorBox(groupBox, "@World.WPHeightTerrain.Color", "@World.WPHeightTerrain.NavigationSpaceTypeColorForVisualizationPurpose.ToolTip",
		pEditNavSpaceTypeColor, cTextNavSpaceTypeColor::Ref::New(*this));
	
	helper.FormLine(groupBox, "@World.WPHeightTerrain.Faces", "@World.WPHeightTerrain.NavigationSpaceFaces.ToolTip", formLine);
	helper.Button(formLine, pBtnNavSpaceFaceAdd, pActionNavSpaceFaceAdd);
	helper.Button(formLine, pBtnNavSpaceFaceRemove, pActionNavSpaceFaceRemove);
	
	
	// vegetation
	helper.GroupBox(content, groupBox, "@World.WPHeightTerrain.Vegetation", true);
	
	helper.FormLine(groupBox, "@World.WPHeightTerrain.Update", "@World.WPHeightTerrain.UpdateVegetation.ToolTip", formLine);
	helper.Button(formLine, pBtnUpdateVegetation, cActionUpdateVegetation::Ref::New(*this));
	helper.Button(formLine, pBtnUpdateVegetationAll, cActionUpdateVegetationAll::Ref::New(*this));
	helper.Button(formLine, pBtnClearVegetation, cActionClearVegetation::Ref::New(*this));
	
	filePatterns.RemoveAll();
	filePatterns.Add(igdeFilePattern::Ref::New(
		"@World.WPHeightTerrain.DragEnGinePropFieldCache", "*.depfc", ".depfc"));
	helper.EditPath(groupBox, "@World.WPSObject.Skin", "@World.WPHeightTerrain.SkinToUseForTexture.ToolTip",
		filePatterns, pEditPathPFCache, cPathPathPFCache::Ref::New(*this));
	
	helper.FormLineStretchFirst(groupBox, "@World.WPWorld.Layer", "@World.WPHeightTerrain.LayerToEdit.ToolTip", formLine);
	helper.ComboBox(formLine, "@World.WPHeightTerrain.LayerToEdit.ToolTip", pCBVLayer, cComboVLayer::Ref::New(*this));
	helper.Button(formLine, pBtnMenuVLayer, pActionMenuVLayer);
	pActionMenuVLayer->SetWidget(pBtnMenuVLayer);
	
	helper.FormLineStretchFirst(groupBox, "@World.WPHeightTerrain.Variation", "@World.WPHeightTerrain.Variation.ToolTip", formLine);
	helper.ComboBox(formLine, "@World.WPHeightTerrain.Variation.ToolTip", pCBVVariation, cComboVVariation::Ref::New(*this));
	helper.Button(formLine, pBtnMenuVVariation, pActionMenuVVariation);
	pActionMenuVVariation->SetWidget(pBtnMenuVVariation);
	
	helper.EditPath(groupBox, "@World.WPHeightTerrain.Model", "@World.WPHeightTerrain.ModelToUseForVariation.ToolTip",
		igdeEnvironment::efpltModel, pEditVVModel, cPathVVModel::Ref::New(*this));
	helper.EditPath(groupBox, "@World.WPSObject.Skin", "@World.WPHeightTerrain.SkinToUseForVariation.ToolTip",
		igdeEnvironment::efpltSkin, pEditVVSkin, cPathVVSkin::Ref::New(*this));
	helper.EditFloat(groupBox, "@World.WPHeightTerrain.RotationForce", "@World.WPHeightTerrain.RotationPerForce.ToolTip",
		pEditVVRotPerForce, cTextVVRotPerForce::Ref::New(*this));
	helper.EditFloat(groupBox, "@World.WPHeightTerrain.Restition", "@World.WPHeightTerrain.Restition.ToolTip",
		pEditVVRestitution, cTextVVRestitution::Ref::New(*this));
	
	
	// height paint
	helper.GroupBox(content, groupBox, "@World.WPHeightTerrain.HeightPainting");
	
	helper.FormLine(groupBox, "@World.WPHeightTerrain.Mode", "@World.WPHeightTerrain.PaintMode.ToolTip", formLine);
	helper.ToggleButton(formLine, pBtnHPModeRaise, cActionHPMode::Ref::New(*this,
		meWorldGuiParameters::ehpdmRaise, "@World.WPHeightTerrain.Raise", nullptr, "@World.WPHeightTerrain.Raise"));
	helper.ToggleButton(formLine, pBtnHPModeLower, cActionHPMode::Ref::New(*this,
		meWorldGuiParameters::ehpdmLower, "@World.WPHeightTerrain.Lower", nullptr, "@World.WPHeightTerrain.Lower"));
	helper.ToggleButton(formLine, pBtnHPModeLevel, cActionHPMode::Ref::New(*this,
		meWorldGuiParameters::ehpdmLevel, "@World.WPHeightTerrain.Level", nullptr, "@World.WPHeightTerrain.Level"));
	helper.ToggleButton(formLine, pBtnHPModeSmooth, cActionHPMode::Ref::New(*this,
		meWorldGuiParameters::ehpdmSmooth, "@World.WPHeightTerrain.Smooth", nullptr, "@World.WPHeightTerrain.Smooth"));
	
	helper.EditSliderText(groupBox, "@World.WVNodePropCount.Radius", "@World.WPHeightTerrain.SetsRadiusOfInfluence.ToolTip",
		0.0f, 10.0f, 6, 3, 1.0f, pSldHPRadius, cSliderHPRadius::Ref::New(*this));
	helper.EditSliderText(groupBox, "@World.WPHeightTerrain.Strength", "@World.WPHeightTerrain.StrengthOfInfluence.ToolTip",
		0.0f, 1.0f, 6, 3, 0.1f, pSldHPStrength, cSliderHPStrength::Ref::New(*this));
	
	
	// mask paint
	helper.GroupBox(content, groupBox, "@World.WPHeightTerrain.MaskPainting", true);
	
	helper.FormLine(groupBox, "@World.WPHeightTerrain.Mode", "@World.WPHeightTerrain.PaintMode.ToolTip", formLine);
	helper.ToggleButton(formLine, pBtnMPModeDraw, cActionMPMode::Ref::New(*this,
		meWorldGuiParameters::empdmDraw, "@World.WPHeightTerrain.Draw", nullptr, "@World.WPHeightTerrain.Draw"));
	helper.ToggleButton(formLine, pBtnMPModeErase, cActionMPMode::Ref::New(*this,
		meWorldGuiParameters::empdmErase, "@World.WPHeightTerrain.Erase", nullptr, "@World.WPHeightTerrain.Erase"));
	
	helper.EditSliderText(groupBox, "@World.WVNodePropCount.Radius", "@World.WPHeightTerrain.SetsRadiusOfInfluence.ToolTip",
		0.0f, 10.0f, 6, 3, 1.0f, pSldMPRadius, cSliderMPRadius::Ref::New(*this));
	
	
	// visibility paint
	helper.GroupBox(content, groupBox, "@World.WPHeightTerrain.VisibilityPainting", true);
	
	helper.FormLine(groupBox, "@World.WPHeightTerrain.Mode", "@World.WPHeightTerrain.PaintMode.ToolTip", formLine);
	helper.ToggleButton(formLine, pBtnVPModeVisible, cActionVPMode::Ref::New(*this,
		meWorldGuiParameters::evpdmVisible, "@World.WPHeightTerrain.PaintModeVisible", nullptr, "@World.WPHeightTerrain.PaintModeVisible"));
	helper.ToggleButton(formLine, pBtnVPModeInvisible, cActionVPMode::Ref::New(*this,
		meWorldGuiParameters::evpdmInvisible, "@World.WPHeightTerrain.Invisible", nullptr, "@World.WPHeightTerrain.Invisible"));
	
	helper.EditSliderText(groupBox, "@World.WVNodePropCount.Radius", "@World.WPHeightTerrain.SetsRadiusOfInfluence.ToolTip",
		0.0f, 10.0f, 6, 3, 1.0f, pSldVPRadius, cSliderVPRadius::Ref::New(*this));
}

meWPHeightTerrain::~meWPHeightTerrain(){
	SetWorld(nullptr);
}



// Management
///////////////

void meWPHeightTerrain::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	SetVLayer(nullptr);
	SetTexture(nullptr);
	
	if(pWorld){
		pWorld->RemoveNotifier(pListener);
	}
	
	pWorld = world;
	
	if(world){
		world->AddNotifier(pListener);
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
		return nullptr;
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
	return GetSector() ? GetSector()->GetActiveNavSpace().Pointer() : nullptr;
}

meHeightTerrainNavSpaceType *meWPHeightTerrain::GetActiveNavSpaceType() const{
	const meHeightTerrainNavSpace * const navspace = GetActiveNavSpace();
	return navspace ? navspace->GetActiveType().Pointer() : nullptr;
}

void meWPHeightTerrain::SetVLayer(meHTVegetationLayer *vlayer){
	if(vlayer && !GetSector()){
		DETHROW(deeInvalidParam);
	}
	
	if(vlayer == pVLayer){
		return;
	}
	
	SetVVariation(nullptr);
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
	pCBTexture->UpdateRestoreSelection([&](){
		meHeightTerrainSector * const sector = GetSector();
		
		pCBTexture->RemoveAllItems();
		
		if(sector){
			sector->GetTextures().Visit([&](meHeightTerrainTexture *texture){
				pCBTexture->AddItem(texture->GetName(), nullptr, texture);
			});
			
			pCBTexture->SortItems();
		}
	}, 0);
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
	pCBNavSpace->UpdateRestoreSelection([&](){
		pCBNavSpace->RemoveAllItems();
		
		meHeightTerrainSector * const sector = GetSector();
		if(sector){
			sector->GetNavSpaces().Visit([&](meHeightTerrainNavSpace *navspace){
				pCBNavSpace->AddItem(navspace->GetName(), nullptr, navspace);
			});
		}
		
		pCBNavSpace->SortItems();
	}, 0);
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
	pCBNavSpaceType->UpdateRestoreSelection([&](){
		const meHeightTerrainNavSpace * const navspace = GetActiveNavSpace();
		
		pCBNavSpaceType->RemoveAllItems();
		
		if(navspace){
			navspace->GetTypes().Visit([&](meHeightTerrainNavSpaceType *type){
				pCBNavSpaceType->AddItem(type->GetName(), nullptr, type);
			});
			
			pCBNavSpaceType->SortItems();
		}
	}, 0);
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
	pCBVLayer->UpdateRestoreSelection([&](){
		pCBVLayer->RemoveAllItems();
		
		if(pWorld){
			pWorld->GetHeightTerrain()->GetVLayers().VisitIndexed([&](int i, meHTVegetationLayer *vlayer){
				decString text;
				text.Format("%d: %s", i + 1, vlayer->GetName().GetString());
				pCBVLayer->AddItem(text, nullptr, vlayer);
			});
		}
	}, 0);
}

void meWPHeightTerrain::UpdateVLayer(){
}

void meWPHeightTerrain::UpdateVVariationList(){
	pCBVVariation->UpdateRestoreSelection([&](){
		pCBVVariation->RemoveAllItems();
		
		if(pVLayer){
			pVLayer->GetVariations().VisitIndexed([&](int i, meHTVVariation *variation){
				pCBVVariation->AddItem(decString::Formatted(
					Translate("World.WPHeightTerrain.DefaultName.Variation").ToUTF8(), i),
					nullptr, variation);
			});
		}
	}, 0);
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
