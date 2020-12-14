/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEWPPROPERTYLIST_H_
#define _MEWPPROPERTYLIST_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/common/string/decStringSet.h>

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeIconListBoxReference.h>
#include <deigde/gui/igdeTextAreaReference.h>
#include <deigde/gui/composed/igdeEditPropertyValueReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionContextMenuReference.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/resources/igdeIconReference.h>


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
	
	igdeIconReference pIconUnknownKey;
	igdeIconReference pIconInvalidValue;
	
	igdeComboBoxReference pCBKeys;
	igdeButtonReference pBtnKeyAdd;
	
	igdeIconListBoxReference pListProperties;
	igdeEditPropertyValueReference pEditPropertyValue;
	igdeTextAreaReference pDisplayInfo;
	
	igdeActionReference pActionPropertyAdd;
	igdeActionReference pActionPropertyRemove;
	igdeActionReference pActionPropertyClear;
	igdeActionReference pActionPropertyCopy;
	igdeActionReference pActionPropertyCopyAll;
	igdeActionReference pActionPropertyCut;
	igdeActionReference pActionPropertyCutAll;
	igdeActionReference pActionPropertyPaste;
	igdeActionReference pActionPropertyRename;
	
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
