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

#ifndef _MEWPHEIGHTTERRAIN_H_
#define _MEWPHEIGHTTERRAIN_H_

#include <deigde/gamedefinition/class/light/igdeGDCLight.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeToggleButton.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/composed/igdeEditVector2.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/undo/igdeUndo.h>


class meHeightTerrainNavSpaceType;
class meHeightTerrain;
class meHeightTerrainNavSpace;
class meHeightTerrainSector;
class meHeightTerrainTexture;
class meHTVegetationLayer;
class meHTVVariation;
class meWindowProperties;
class meWorld;
class meWPHeightTerrainListener;


/**
 * \brief Height Terrain Properties.
 */
class meWPHeightTerrain : public igdeContainerScroll{
private:
	meWindowProperties &pWindowProperties;
	meWPHeightTerrainListener::Ref pListener;
	
	meWorld::Ref pWorld;
	meHeightTerrainTexture::Ref pTexture;
	meHTVegetationLayer::Ref pVLayer;
	meHTVVariation::Ref pVVariation;
	
	
	igdeActionContextMenu::Ref pActionMenuTexture;
	igdeAction::Ref pActionTextureAdd;
	igdeAction::Ref pActionTextureRemove;
	
	igdeActionContextMenu::Ref pActionMenuNavSpace;
	igdeAction::Ref pActionNavSpaceAdd;
	igdeAction::Ref pActionNavSpaceRemove;
	
	igdeActionContextMenu::Ref pActionMenuNavSpaceType;
	igdeAction::Ref pActionNavSpaceTypeAdd;
	igdeAction::Ref pActionNavSpaceTypeRemove;
	
	igdeAction::Ref pActionNavSpaceFaceAdd;
	igdeAction::Ref pActionNavSpaceFaceRemove;
	
	igdeActionContextMenu::Ref pActionMenuVLayer;
	igdeAction::Ref pActionVLayerAdd;
	igdeAction::Ref pActionVLayerRemove;
	igdeAction::Ref pActionVLayerUp;
	igdeAction::Ref pActionVLayerDown;
	
	igdeActionContextMenu::Ref pActionMenuVVariation;
	igdeAction::Ref pActionVVariationAdd;
	igdeAction::Ref pActionVVariationRemove;
	
	
	igdeEditPath::Ref pEditPathHTerrain;
	igdeTextField::Ref pEditSectorSize;
	igdeTextField::Ref pEditSectorResolution;
	igdeTextField::Ref pEditBaseHeight;
	igdeTextField::Ref pEditHeightScale;
	
	igdeEditPath::Ref pEditPathHeightImage;
	igdeEditPath::Ref pEditPathVisImage;
	
	igdeComboBox::Ref pCBTexture;
	igdeButton::Ref pBtnTexture;
	igdeTextField::Ref pEditTexTypeNum;
	igdeEditPath::Ref pEditTexSkin;
	igdeEditPath::Ref pEditTexMask;
	igdeEditVector2::Ref pEditTexUVOffset;
	igdeEditVector2::Ref pEditTexUVScale;
	igdeTextField::Ref pEditTexUVRot;
	
	igdeComboBox::Ref pCBNavSpace;
	igdeButton::Ref pBtnNavSpace;
	igdeTextField::Ref pEditNavSpaceLayer;
	igdeTextField::Ref pEditNavSpaceSnapDist;
	igdeTextField::Ref pEditNavSpaceSnapAngle;
	
	igdeComboBox::Ref pCBNavSpaceType;
	igdeButton::Ref pBtnNavSpaceType;
	igdeTextField::Ref pEditNavSpaceTypeName;
	igdeColorBox::Ref pEditNavSpaceTypeColor;
	igdeTextField::Ref pEditNavSpaceTypeType;
	igdeButton::Ref pBtnNavSpaceFaceAdd;
	igdeButton::Ref pBtnNavSpaceFaceRemove;
	
	igdeToggleButton::Ref pBtnHPModeRaise;
	igdeToggleButton::Ref pBtnHPModeLower;
	igdeToggleButton::Ref pBtnHPModeLevel;
	igdeToggleButton::Ref pBtnHPModeSmooth;
	igdeEditSliderText::Ref pSldHPRadius;
	igdeEditSliderText::Ref pSldHPStrength;
	
	igdeToggleButton::Ref pBtnMPModeDraw;
	igdeToggleButton::Ref pBtnMPModeErase;
	igdeEditSliderText::Ref pSldMPRadius;
	
	igdeToggleButton::Ref pBtnVPModeVisible;
	igdeToggleButton::Ref pBtnVPModeInvisible;
	igdeEditSliderText::Ref pSldVPRadius;
	
	igdeButton::Ref pBtnUpdateVegetation;
	igdeButton::Ref pBtnUpdateVegetationAll;
	igdeButton::Ref pBtnClearVegetation;
	igdeEditPath::Ref pEditPathPFCache;
	igdeComboBox::Ref pCBVLayer;
	igdeButton::Ref pBtnMenuVLayer;
	igdeComboBox::Ref pCBVVariation;
	igdeButton::Ref pBtnMenuVVariation;
	igdeEditPath::Ref pEditVVModel;
	igdeEditPath::Ref pEditVVSkin;
	igdeTextField::Ref pEditVVRotPerForce;
	igdeTextField::Ref pEditVVRestitution;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	meWPHeightTerrain(meWindowProperties &windowProperties);
	
protected:
	/** \brief Clean up panel. */
	virtual ~meWPHeightTerrain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties panel. */
	inline meWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief World. */
	inline const meWorld::Ref &GetWorld() const{ return pWorld; }
	
	/** \brief Set world. */
	void SetWorld(meWorld *world);
	
	/** \brief World path changed. */
	void OnWorldPathChanged();
	
	/** \brief Current sector. */
	meHeightTerrainSector *GetSector() const;
	
	/** \brief Texture. */
	inline const meHeightTerrainTexture::Ref &GetTexture() const{ return pTexture; }
	
	/** \brief Set texture. */
	void SetTexture(meHeightTerrainTexture *texture);
	
	/** \brief Active navigation space or \em NULL. */
	meHeightTerrainNavSpace *GetActiveNavSpace() const;
	
	/** \brief Active navigation space type or \em NULL. */
	meHeightTerrainNavSpaceType *GetActiveNavSpaceType() const;
	
	/** \brief Vegetation layer. */
	inline const meHTVegetationLayer::Ref &GetVLayer() const{ return pVLayer; }
	
	/** \brief Set vegetation layer. */
	void SetVLayer(meHTVegetationLayer *vlayer);
	
	/** \brief Vegetation layer variation. */
	inline const meHTVVariation::Ref &GetVVariation() const{ return pVVariation; }
	
	/** \brief Set vegetation layer variation. */
	void SetVVariation(meHTVVariation *vvariation);
	
	/** \brief Update height terrain. */
	void UpdateHeightTerrain();
	
	/** \brief Update height terrain sector. */
	void UpdateSector();
	
	/** \brief Update texture list. */
	void UpdateTextureList();
	
	/** \brief Update texture. */
	void UpdateTexture();
	
	/** \brief Update navigation space list. */
	void UpdateNavSpaceList();
	
	/** \brief Select active navigation space. */
	void SelectActiveNavSpace();
	
	/** \brief Update navigation space. */
	void UpdateNavSpace();
	
	/** \brief Update navigation space type list. */
	void UpdateNavSpaceTypeList();
	
	/** \brief Select active navigation space type. */
	void SelectActiveNavSpaceType();
	
	/** \brief Update navigation space type. */
	void UpdateNavSpaceType();
	
	/** \brief Update height paint properties. */
	void UpdateHeightPaint();
	
	/** \brief Update mask paint properties. */
	void UpdateMaskPaint();
	
	/** \brief Update visibility paint properties. */
	void UpdateVisibilityPaint();
	
	/** \brief Selected points changed. */
	void UpdateSelectedPoints();
	
	/** \brief Update vegetation layer list. */
	void UpdateVLayerList();
	
	/** \brief Update vegetation layer. */
	void UpdateVLayer();
	
	/** \brief Update vegetation layer variation list. */
	void UpdateVVariationList();
	
	/** \brief Update vegetation layer variation. */
	void UpdateVVariation();
	
	
	inline igdeAction *GetActionTextureAdd() const{ return pActionTextureAdd; }
	inline igdeAction *GetActionTextureRemove() const{ return pActionTextureRemove; }
	
	inline igdeAction *GetActionNavSpaceAdd() const{ return pActionNavSpaceAdd; }
	inline igdeAction *GetActionNavSpaceRemove() const{ return pActionNavSpaceRemove; }
	
	inline igdeAction *GetActionNavSpaceTypeAdd() const{ return pActionNavSpaceTypeAdd; }
	inline igdeAction *GetActionNavSpaceTypeRemove() const{ return pActionNavSpaceTypeRemove; }
	
	inline igdeAction *GetActionNavSpaceFaceAdd() const{ return pActionNavSpaceFaceAdd; }
	inline igdeAction *GetActionNavSpaceFaceRemove() const{ return pActionNavSpaceFaceRemove; }
	
	inline igdeAction *GetActionVLayerAdd() const{ return pActionVLayerAdd; }
	inline igdeAction *GetActionVLayerRemove() const{ return pActionVLayerRemove; }
	inline igdeAction *GetActionVLayerUp() const{ return pActionVLayerUp; }
	inline igdeAction *GetActionVLayerDown() const{ return pActionVLayerDown; }
	
	inline igdeAction *GetActionVVariationAdd() const{ return pActionVVariationAdd; }
	inline igdeAction *GetActionVVariationRemove() const{ return pActionVVariationRemove; }
	/*@}*/
};

#endif
