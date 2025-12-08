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

#ifndef _GDEWPSOCCOMPONENT_H_
#define _GDEWPSOCCOMPONENT_H_

#include "../../../gamedef/objectClass/component/gdeOCComponent.h"

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/string/decStringList.h>

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeIconListBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVector2.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class gdeGameDefinition;
class gdeObjectClass;
class gdeOCComponent;
class gdeOCComponentTexture;
class gdeWindowProperties;
class gdeWPSOCComponentListener;



/**
 * \brief Object class component property panel.
 */
class gdeWPSOCComponent : public igdeContainerScroll{
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSOCComponentListener::Ref pListener;
	
	gdeGameDefinition::Ref pGameDefinition;
	
	decStringList pEngModelTexNames;
	decString pEngModelPath;
	bool pDirtyEngModelTexNames;
	
	igdeActionContextMenu::Ref pActionTexturesMenu;
	igdeAction::Ref pActionTextureAdd;
	igdeAction::Ref pActionTextureRemove;
	decObjectList pActionsTextureAddFromModel;
	igdeAction::Ref pActionTexturePropertyValueSet;
	igdeAction::Ref pActionTexturePropertyValueRemove;
	igdeAction::Ref pActionTexturePropertyValueClear;
	
	igdeEditPath::Ref pEditPathModel;
	igdeEditPath::Ref pEditPathSkin;
	igdeEditPath::Ref pEditPathRig;
	igdeEditPath::Ref pEditPathAnimator;
	igdeEditPath::Ref pEditPathAnimation;
	igdeTextField::Ref pEditMove;
	igdeEditPath::Ref pEditPathOcclusionMesh;
	igdeEditPath::Ref pEditPathAudioModel;
	igdeTextField::Ref pEditPlaybackController;
	igdeCheckBox::Ref pChkDoNotScale;
	igdeCheckBox::Ref pChkStatic;
	igdeCheckBox::Ref pChkRenderEnvMap;
	igdeCheckBox::Ref pChkAffectsAudio;
	igdeCheckBox::Ref pChkPartialHide;
	igdeCheckBox::Ref pChkAttachTarget;
	igdeCheckBox::Ref pChkLightShadowIgnore;
	igdeComboBox::Ref pCBCollisionResponseType;
	igdeEditVector::Ref pEditPosition;
	igdeEditVector::Ref pEditRotation;
	igdeTextField::Ref pEditBoneName;
	
	igdeComboBox::Ref pCBPropertyNames;
	igdeComboBoxFilter::Ref pCBPropertyNameTarget;
	
	igdeComboBox::Ref pCBTextures;
	igdeButton::Ref pBtnTextures;
	igdeTextField::Ref pTextureEditName;
	igdeEditPath::Ref pTextureEditPathSkin;
	igdeEditVector2::Ref pTextureEditOffset;
	igdeTextField::Ref pTextureEditRotation;
	igdeEditVector2::Ref pTextureEditScale;
	igdeColorBox::Ref pTextureClrTint;
	
	igdeComboBox::Ref pTextureCBPropertyKeys;
	igdeButton::Ref pTextureBtnPropertyValueSet;
	igdeIconListBox::Ref pTextureListProperties;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSOCComponent(gdeWindowProperties &windowMain);
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPSOCComponent();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Game definition or \em NULL if not set. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or \em NULL if not set. */
	void SetGameDefinition(gdeGameDefinition *gameDefinition);
	
	
	
	/** \brief Active object class or \em NULL if absent. */
	gdeObjectClass *GetObjectClass() const;
	
	/** \brief Active object class component or \em NULL if not set. */
	gdeOCComponent *GetComponent() const;
	
	/** \brief Active object class component texture or \em NULL if not set. */
	gdeOCComponentTexture *GetTexture() const;
	
	/** \brief Active object class component texture property or empty string if not set. */
	const decString &GetTextureProperty() const;
	
	/** \brief Selected property name. */
	const gdeOCComponent::eProperties GetPropertyName() const;
	
	/** \brief Active property key (in combo box). */
	const decString &GetTexturePropertyKey() const;
	
	/** \brief Active property value (in list box). */
	const char *GetTexturePropertyValue() const;
	
	
	
	/** \brief Update object class property list. */
	void UpdatePropertyList();
	
	/** \brief Update object class texture property list. */
	void UpdateTexturePropertyList();
	
	
	
	/** \brief Update component. */
	void UpdateComponent();
	
	/** \brief Update component property name. */
	void UpdatePropertyName();
	
	/** \brief Update component texture list. */
	void UpdateTextureList();
	
	/** \brief Select active texture. */
	void SelectActiveTexture();
	
	/** \brief Update component texture. */
	void UpdateTexture();
	
	/** \brief Update component texture properties. */
	void UpdateTextureProperties();
	
	/** \brief Select active component texture property. */
	void SelectActiveTextureProperty();
	
	/**
	 * \brief Set engine model texture list.
	 * 
	 * Marks the list dirty if the path changed. List will be update only when
	 * PrepareEngineModelPath() is called to keep performance up.
	 */
	void SetEngineModelPath(const char *path);
	
	/**
	 * \brief Prepare engine model texture list if dirty.
	 * 
	 * Call before accessing pEngModelTexNames to make sure the list is ready.
	 */
	void PrepareEngineModelPath();
	
	
	
	/** \brief Actions. */
	inline igdeActionContextMenu *GetActionTexturesMenu() const{ return pActionTexturesMenu; }
	inline igdeAction *GetActionTextureAdd() const{ return pActionTextureAdd; }
	inline igdeAction *GetActionTextureRemove() const{ return pActionTextureRemove; }
	inline const decObjectList &GetActionsTextureAddFromModel() const{ return pActionsTextureAddFromModel; }
	inline igdeAction *GetActionTexturePropertyValueSet() const{ return pActionTexturePropertyValueSet; }
	inline igdeAction *GetActionTexturePropertyValueRemove() const{ return pActionTexturePropertyValueRemove; }
	inline igdeAction *GetActionTexturePropertyValueClear() const{ return pActionTexturePropertyValueClear; }
	/*@}*/
};

#endif
