/* 
 * Drag[en]gine IGDE Game Definition Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _GDEWPSOCCOMPONENT_H_
#define _GDEWPSOCCOMPONENT_H_

#include "../../../gamedef/objectClass/component/gdeOCComponent.h"

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/string/decStringList.h>

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeColorBoxReference.h>
#include <deigde/gui/igdeIconListBoxReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/composed/igdeEditVector2Reference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionContextMenuReference.h>
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
	gdeWPSOCComponentListener *pListener;
	
	gdeGameDefinition *pGameDefinition;
	
	decStringList pEngModelTexNames;
	decString pEngModelPath;
	bool pDirtyEngModelTexNames;
	
	igdeActionContextMenuReference pActionTexturesMenu;
	igdeActionReference pActionTextureAdd;
	igdeActionReference pActionTextureRemove;
	decObjectList pActionsTextureAddFromModel;
	igdeActionReference pActionTexturePropertyValueSet;
	igdeActionReference pActionTexturePropertyValueRemove;
	igdeActionReference pActionTexturePropertyValueClear;
	
	igdeEditPathReference pEditPathModel;
	igdeEditPathReference pEditPathSkin;
	igdeEditPathReference pEditPathRig;
	igdeEditPathReference pEditPathAnimator;
	igdeEditPathReference pEditPathOcclusionMesh;
	igdeEditPathReference pEditPathAudioModel;
	igdeTextFieldReference pEditPlaybackController;
	igdeCheckBoxReference pChkDoNotScale;
	igdeCheckBoxReference pChkStatic;
	igdeCheckBoxReference pChkRenderEnvMap;
	igdeCheckBoxReference pChkAffectsAudio;
	igdeCheckBoxReference pChkPartialHide;
	igdeCheckBoxReference pChkAttachTarget;
	igdeComboBoxReference pCBCollisionResponseType;
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	igdeTextFieldReference pEditBoneName;
	
	igdeComboBoxReference pCBPropertyNames;
	igdeComboBoxFilterReference pCBPropertyNameTarget;
	
	igdeComboBoxReference pCBTextures;
	igdeButtonReference pBtnTextures;
	igdeTextFieldReference pTextureEditName;
	igdeEditPathReference pTextureEditPathSkin;
	igdeEditVector2Reference pTextureEditOffset;
	igdeTextFieldReference pTextureEditRotation;
	igdeEditVector2Reference pTextureEditScale;
	igdeColorBoxReference pTextureClrTint;
	
	igdeComboBoxReference pTextureCBPropertyKeys;
	igdeButtonReference pTextureBtnPropertyValueSet;
	igdeIconListBoxReference pTextureListProperties;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSOCComponent( gdeWindowProperties &windowMain );
	
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
	void SetGameDefinition( gdeGameDefinition *gameDefinition );
	
	
	
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
	void SetEngineModelPath( const char *path );
	
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
