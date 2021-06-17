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

#ifndef _GDEWPSOBJECTCLASS_H_
#define _GDEWPSOBJECTCLASS_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeColorBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeIconListBoxReference.h>
#include <deigde/gui/igdeTextAreaReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditVector2Reference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionContextMenuReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class gdeFilePattern;
class gdeGameDefinition;
class gdeObjectClass;
class gdeProperty;
class gdeOCComponentTexture;
class gdeOCInherit;
class gdeCategoryList;
class gdeWindowProperties;
class gdeWPSObjectClassListener;


/**
 * \brief Object class property panel.
 */
class gdeWPSObjectClass : public igdeContainerScroll{
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSObjectClassListener *pListener;
	
	gdeGameDefinition *pGameDefinition;
	
	igdeActionReference pActionInheritAdd;
	igdeActionReference pActionInheritRemove;
	igdeActionReference pActionInheritRemoveAll;
	igdeActionReference pActionPropertyValueSet;
	igdeActionReference pActionPropertyValueRemove;
	igdeActionReference pActionPropertyValueClear;
	igdeActionReference pActionPropertyValuesFromSubObjects;
	igdeActionContextMenuReference pActionTexturesMenu;
	igdeActionReference pActionTextureAdd;
	igdeActionReference pActionTextureRemove;
	
	igdeTextFieldReference pEditName;
	igdeTextAreaReference pEditDescription;
	igdeComboBoxReference pCBScaleMode;
	igdeCheckBoxReference pChkIsGhost;
	igdeCheckBoxReference pChkCanInstantiate;
	igdeTextFieldReference pEditDefaultInheritPropertyPrefix;
	
	igdeWidgetReference pEditProperties;
	igdeWidgetReference pEditTextureProperties;
	
	igdeComboBoxReference pCBPropertyValuesKeys;
	igdeButtonReference pBtnPropertyValueSet;
	igdeIconListBoxReference pListPropertyValues;
	
	igdeComboBoxFilterReference pCBCategory;
	igdeButtonReference pBtnJumpToCategory;
	
	igdeWidgetReference pListHideTags;
	igdeWidgetReference pListPartialHideTags;
	
	igdeListBoxReference pListInherits;
	igdeComboBoxFilterReference pInheritCBClass;
	igdeButtonReference pBtnJumpToInheritClass;
	igdeTextFieldReference pInheritEditPropertyPrefix;
	igdeButtonReference pBtnInheritPropertyPrefixReset;
	
	igdeComboBoxReference pCBTextures;
	igdeButtonReference pBtnTextures;
	igdeTextFieldReference pTextureEditName;
	igdeEditPathReference pTextureEditPathSkin;
	igdeEditVector2Reference pTextureEditOffset;
	igdeTextFieldReference pTextureEditRotation;
	igdeEditVector2Reference pTextureEditScale;
	igdeColorBoxReference pTextureClrTint;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSObjectClass( gdeWindowProperties &windowMain );
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPSObjectClass();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline gdeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Game definition or \em NULL if not set. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or \em NULL if not set. */
	void SetGameDefinition( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Active object class or \em NULL if absent. */
	gdeObjectClass *GetObjectClass() const;
	
	/** \brief Active property class or \em NULL if absent. */
	gdeProperty *GetProperty() const;
	
	/** \brief Active texture property class or \em NULL if absent. */
	gdeProperty *GetTextureProperty() const;
	
	/** \brief Active inherit or \em NULL if not set. */
	gdeOCInherit *GetInherit() const;
	
	/** \brief Active property key (in combo box). */
	const decString &GetPropertyKey() const;
	
	/** \brief Active property value (in list box). */
	const char *GetPropertyValue() const;
	
	/** \brief Active object class texture or \em NULL if not set. */
	gdeOCComponentTexture *GetTexture() const;
	
	
	
	/** \brief Actions. */
	inline igdeAction *GetActionInheritAdd() const{ return pActionInheritAdd; }
	inline igdeAction *GetActionInheritRemove() const{ return pActionInheritRemove; }
	inline igdeAction *GetActionInheritRemoveAll() const{ return pActionInheritRemoveAll; }
	inline igdeAction *GetActionPropertyValueSet() const{ return pActionPropertyValueSet; }
	inline igdeAction *GetActionPropertyValueRemove() const{ return pActionPropertyValueRemove; }
	inline igdeAction *GetActionPropertyValueClear() const{ return pActionPropertyValueClear; }
	inline igdeAction *GetActionPropertyValuesFromSubObjects() const{ return pActionPropertyValuesFromSubObjects; }
	inline igdeActionContextMenu *GetActionTexturesMenu() const{ return pActionTexturesMenu; }
	inline igdeAction *GetActionTextureAdd() const{ return pActionTextureAdd; }
	inline igdeAction *GetActionTextureRemove() const{ return pActionTextureRemove; }
	
	
	
	/** \brief Udpate used tags. */
	void UpdateUsedTagsList();
	
	/** \brief Update category list. */
	void UpdateCategoryList();
	
	/** \brief Update class list. */
	void UpdateClassLists();
	
	/** \brief Update identifier list. */
	void UpdateIdentifierLists();
	
	
	
	/** \brief Update object class. */
	void UpdateObjectClass();
	
	/** \brief Update properties. */
	void UpdateProperties();
	
	/** \brief Update property. */
	void UpdateProperty();
	
	/** \brief Update property values. */
	void UpdatePropertyValues();
	
	/** \brief Update property value keys. */
	void UpdatePropertyValueKeys();
	
	/** \brief Update texture properties. */
	void UpdateTextureProperties();
	
	/** \brief Update texture property. */
	void UpdateTextureProperty();
	
	/** \brief Update inherits. */
	void UpdateInherits();
	
	/** \brief Update inherit. */
	void UpdateInherit();
	
	/** \brief Select inherit. */
	void SelectInherit( gdeOCInherit *inherit );
	
	/** \brief Update hide tags. */
	void UpdateHideTags();
	
	/** \brief Update partial hide tags. */
	void UpdatePartialHideTags();
	
	/** \brief Update component texture list. */
	void UpdateTextureList();
	
	/** \brief Select active texture. */
	void SelectActiveTexture();
	
	/** \brief Update component texture. */
	void UpdateTexture();
	/*@}*/
	
	
	
private:
	/** \brief Update category list. */
	void pUpdateCategoryList( const gdeCategoryList &list, const char *prefix );
};

#endif
