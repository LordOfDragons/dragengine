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

#ifndef _GDEWPSOBJECTCLASS_H_
#define _GDEWPSOBJECTCLASS_H_

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeIconListBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditVector2.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
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
	gdeWPSObjectClassListener::Ref pListener;
	
	gdeGameDefinition::Ref pGameDefinition;
	
	igdeAction::Ref pActionInheritAdd;
	igdeAction::Ref pActionInheritRemove;
	igdeAction::Ref pActionInheritRemoveAll;
	igdeAction::Ref pActionPropertyValueSet;
	igdeAction::Ref pActionPropertyValueRemove;
	igdeAction::Ref pActionPropertyValueClear;
	igdeAction::Ref pActionPropertyValuesFromSubObjects;
	igdeActionContextMenu::Ref pActionTexturesMenu;
	igdeAction::Ref pActionTextureAdd;
	igdeAction::Ref pActionTextureRemove;
	
	igdeTextField::Ref pEditName;
	igdeTextArea::Ref pEditDescription;
	igdeComboBox::Ref pCBScaleMode;
	igdeCheckBox::Ref pChkIsGhost;
	igdeCheckBox::Ref pChkCanInstantiate;
	igdeCheckBox::Ref pChkIsAttachableBehavior;
	igdeCheckBox::Ref pChkInheritSOBillboards;
	igdeCheckBox::Ref pChkInheritSOComponents;
	igdeCheckBox::Ref pChkInheritSOLights;
	igdeCheckBox::Ref pChkInheritSOSnapPoints;
	igdeCheckBox::Ref pChkInheritSOParticleEmitters;
	igdeCheckBox::Ref pChkInheritSOForceFields;
	igdeCheckBox::Ref pChkInheritSOEnvMapProbes;
	igdeCheckBox::Ref pChkInheritSOSpeakers;
	igdeCheckBox::Ref pChkInheritSONavigationSpaces;
	igdeCheckBox::Ref pChkInheritSONavigationBlockers;
	igdeCheckBox::Ref pChkInheritSOWorlds;
	igdeTextField::Ref pEditDefaultInheritPropertyPrefix;
	
	igdeWidget::Ref pEditProperties;
	igdeWidget::Ref pEditTextureProperties;
	
	igdeComboBox::Ref pCBPropertyValuesKeys;
	igdeButton::Ref pBtnPropertyValueSet;
	igdeIconListBox::Ref pListPropertyValues;
	
	igdeComboBoxFilter::Ref pCBCategory;
	igdeButton::Ref pBtnJumpToCategory;
	
	igdeWidget::Ref pListHideTags;
	igdeWidget::Ref pListPartialHideTags;
	
	igdeListBox::Ref pListInherits;
	igdeComboBoxFilter::Ref pInheritCBClass;
	igdeButton::Ref pBtnJumpToInheritClass;
	igdeTextField::Ref pInheritEditPropertyPrefix;
	igdeButton::Ref pBtnInheritPropertyPrefixReset;
	
	igdeComboBox::Ref pCBTextures;
	igdeButton::Ref pBtnTextures;
	igdeTextField::Ref pTextureEditName;
	igdeEditPath::Ref pTextureEditPathSkin;
	igdeEditVector2::Ref pTextureEditOffset;
	igdeTextField::Ref pTextureEditRotation;
	igdeEditVector2::Ref pTextureEditScale;
	igdeColorBox::Ref pTextureClrTint;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSObjectClass(gdeWindowProperties &windowMain);
	
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
	void SetGameDefinition(gdeGameDefinition::Ref gameDefinition);
	
	
	
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
	void SelectInherit(gdeOCInherit *inherit);
	
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
	void pUpdateCategoryList(const gdeCategoryList &list, const char *prefix);
};

#endif
