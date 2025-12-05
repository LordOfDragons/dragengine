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

#ifndef _MEWPPROPERTYLIST_H_
#define _MEWPPROPERTYLIST_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/common/string/decStringSet.h>

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeIconListBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/composed/igdeEditPropertyValue.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/resources/igdeIcon.h>


class igdeUndo;
class igdeUndoSystem;
class igdeClipboard;
class igdeMenuCascade;
class igdeUIHelper;
class igdeGDProperty;
class igdeTriggerTargetList;


/**
 * \brief Propert list panel.
 * 
 * Subclass to implement creating undo actions.
 */
class meWPPropertyList : public igdeContainerFlow{
private:
	igdeUndoSystem *pUndoSystem;
	igdeClipboard *pClipboard;
	decStringDictionary pProperties;
	
	igdeIcon::Ref pIconUnknownKey;
	igdeIcon::Ref pIconInvalidValue;
	
	igdeComboBoxFilter::Ref pCBKeys;
	igdeButton::Ref pBtnKeyAdd;
	
	igdeIconListBox::Ref pListProperties;
	igdeEditPropertyValue::Ref pEditPropertyValue;
	igdeTextArea::Ref pDisplayInfo;
	
	igdeAction::Ref pActionPropertyAdd;
	igdeAction::Ref pActionPropertyRemove;
	igdeAction::Ref pActionPropertyClear;
	igdeAction::Ref pActionPropertyCopy;
	igdeAction::Ref pActionPropertyCopyAll;
	igdeAction::Ref pActionPropertyCut;
	igdeAction::Ref pActionPropertyCutAll;
	igdeAction::Ref pActionPropertyPaste;
	igdeAction::Ref pActionPropertyRename;
	igdeAction::Ref pActionPropertyExport;
	igdeAction::Ref pActionPropertyImport;
	
	bool pEnabled;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	meWPPropertyList( igdeEnvironment &environment );
	
protected:
	/** \brief Clean up panel. */
	virtual ~meWPPropertyList();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo system or NULL. */
	inline igdeUndoSystem *GetUndoSystem() const{ return pUndoSystem; }
	
	/** \brief Set undo system or NULL. */
	void SetUndoSystem( igdeUndoSystem *undoSystem );
	
	/** \brief Clipboard or NULL. */
	inline igdeClipboard *GetClipboard() const{ return pClipboard; }
	
	/** \brief Set clipboard or NULL. */
	void SetClipboard( igdeClipboard *clipboard );
	
	/** \brief Property list to edit. */
	inline const decStringDictionary &GetProperties() const{ return pProperties; }
	
	/** \brief Set property list to edit. */
	void SetProperties( const decStringDictionary &properties );
	
	/** \brief Widget is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Widget is enabled. */
	void SetEnabled( bool enabled );
	
	
	
	/** \brief Selected property key in combo box or empty string. */
	const decString &GetKey() const;
	
	/** \brief Selected property in list or empty string. */
	const decString &GetProperty() const;
	
	/** \brief Value of GetProperty() or empty string. */
	const decString &GetPropertyValue() const;
	
	/** \brief Edit property value. */
	const decString &GetEditPropertyValue() const;
	
	/** \brief Set edit property value. */
	void SetEditPropertyValue( const char *value ) const;
	
	
	
	/** \brief Game definition property for key or NULL. */
	virtual const igdeGDProperty *GetGDProperty( const char *key ) const = 0;
	
	/** \brief Game definition default value for key. */
	virtual decString GetGDDefaultValue( const char *key ) const = 0;
	
	/** \brief List of game definition property keys. */
	virtual decStringSet GetGDPropertyKeys() const = 0;
	
	
	
	/** \brief Update keys combo box. */
	void UpdateKeys();
	
	/** \brief Update property list. */
	void UpdateList();
	
	/** \brief Update selected property. */
	void UpdateProperty();
	
	/** \brief Update enabled state of widgets. */
	void UpdateEnabled();
	
	/** \brief Select key in combo box. */
	void SelectKey( const char *key );
	
	/** \brief Select property. */
	void SelectProperty( const char *property );
	
	/** \brief Edit property value in dialog. */
	void EditPropertyValueInDialog();
	
	/** \brief Verify properties. */
	void VerifyProperties();
	
	/** \brief Update display information. */
	void UpdateInformation( const char *key );
	
	/** \brief Set identifiers. */
	void SetIdentifiers( const decStringSet &identifiers );
	
	/** \brief Set trigger target list with updating. */
	void SetTriggerTargetList( igdeTriggerTargetList *list );
	
	/** \brief Game project game definition changed. */
	void OnGameDefinitionChanged();
	
	
	
	/** \brief Actions. */
	inline igdeAction *GetActionPropertyAdd() const{ return pActionPropertyAdd; }
	inline igdeAction *GetActionPropertyRemove() const{ return pActionPropertyRemove; }
	inline igdeAction *GetActionPropertyClear() const{ return pActionPropertyClear; }
	inline igdeAction *GetActionPropertyCopy() const{ return pActionPropertyCopy; }
	inline igdeAction *GetActionPropertyCopyAll() const{ return pActionPropertyCopyAll; }
	inline igdeAction *GetActionPropertyCut() const{ return pActionPropertyCut; }
	inline igdeAction *GetActionPropertyCutAll() const{ return pActionPropertyCutAll; }
	inline igdeAction *GetActionPropertyPaste() const{ return pActionPropertyPaste; }
	inline igdeAction *GetActionPropertyRename() const{ return pActionPropertyRename; }
	inline igdeAction *GetActionPropertyExport() const{ return pActionPropertyExport; }
	inline igdeAction *GetActionPropertyImport() const{ return pActionPropertyImport; }
	/*@}*/
	
	
	
	/** \name Subclass undo creation */
	/*@{*/
	virtual igdeUndo *UndoAddProperty( const decString &key, const decString &value ) = 0;
	virtual igdeUndo *UndoRemoveProperty( const decString &key ) = 0;
	virtual igdeUndo *UndoSetProperty( const decString &key, const decString &oldValue, const decString &newValue ) = 0;
	virtual igdeUndo *UndoSetProperties( const decStringDictionary &properties ) = 0;
	/*@}*/
	
	
	
	/** \name Subclass notifications */
	/*@{*/
	virtual void OnKeyChanged( const decString &key );
	virtual void OnPropertySelected( const decString &key, const decString &value );
	virtual void AddContextMenuEntries( igdeUIHelper &helper, igdeMenuCascade &menu );
	/*@}*/
};

#endif
