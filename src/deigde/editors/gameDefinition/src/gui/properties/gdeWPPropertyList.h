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

#ifndef _GDEWPPROPERTYLIST_H_
#define _GDEWPPROPERTYLIST_H_

#include "../../gamedef/property/gdeProperty.h"

#include <dragengine/common/math/decMath.h>

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeTextAreaReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeSwitcherReference.h>
#include <deigde/gui/composed/igdeEditPropertyValueReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionContextMenuReference.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/resources/igdeIconReference.h>


class gdeGameDefinition;
class gdePropertyList;
class gdeFilePattern;

class igdeUndo;
class igdeUndoSystem;
class igdeClipboard;


/**
 * \brief Propert list panel.
 * 
 * Subclass to implement creating undo actions.
 */
class gdeWPPropertyList : public igdeContainerFlow{
private:
	const gdePropertyList *pPropertyList;
	gdeGameDefinition *pGameDefinition;
	igdeClipboard *pClipboard;
	
	igdeIconReference pIconIdentifierUsage;
	igdeIconReference pIconIdentifierNoUsage;
	
	igdeActionContextMenuReference pActionPropertiesMenu;
	igdeActionReference pActionPropertyAdd;
	igdeActionReference pActionPropertyRemove;
	igdeActionReference pActionPropertyCopy;
	igdeActionReference pActionPropertyCut;
	igdeActionReference pActionPropertyPaste;
	
	igdeActionReference pActionOptionAdd;
	igdeActionReference pActionOptionRemove;
	
	igdeActionReference pActionCustomPatternAdd;
	igdeActionReference pActionCustomPatternRemove;
	
	igdeActionContextMenuReference pActionCustomPatternMenu;
	
	igdeComboBoxReference pCBProperties;
	igdeButtonReference pBtnMenuProperties;
	igdeTextFieldReference pEditName;
	igdeTextAreaReference pEditDescription;
	igdeComboBoxReference pCBType;
	
	igdeSwitcherReference pSwiParameters;
	
	igdeTextFieldReference pEditMinimum;
	igdeTextFieldReference pEditMaximum;
	
	igdeListBoxReference pListOptions;
	
	igdeComboBoxReference pCBPathPatternType;
	igdeComboBoxReference pCBCustomPattern;
	igdeButtonReference pBtnCustomPatternMenu;
	igdeTextFieldReference pCustomPatternEditName;
	igdeTextFieldReference pCustomPatternEditPattern;
	igdeTextFieldReference pCustomPatternEditExtension;
	
	igdeComboBoxReference pCBIdentifierGroup;
	igdeCheckBoxReference pChkIdentifierUsage;
	
	igdeEditPropertyValueReference pEditDefault;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPPropertyList( igdeEnvironment &environment );
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPPropertyList();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Property list to edit. */
	inline const gdePropertyList *GetPropertyList() const{ return pPropertyList; }
	
	/** \brief Set property list to edit. */
	void SetPropertyList( const gdePropertyList *propPropertyList );
	
	/** \brief Game definition or NULL. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or NULL. */
	void SetGameDefinition( gdeGameDefinition *gameDefinition );
	
	/** \brief Undo system or NULL. */
	igdeUndoSystem *GetUndoSystem() const;
	
	/** \brief Clipboard or NULL. */
	inline igdeClipboard *GetClipboard() const{ return pClipboard; }
	
	/** \brief Set clipboard or NULL. */
	void SetClipboard( igdeClipboard *clipboard );
	
	
	
	/** \brief Active property or NULL. */
	gdeProperty *GetProperty() const;
	
	/** \brief Selection option or empty string if not found. */
	decString GetOption() const;
	
	/** \brief Active custom pattern or -1. */
	int GetCustomPatternIndex() const;
	
	/** \brief Active custom pattern or NULL. */
	gdeFilePattern *GetCustomPattern() const;
	
	
	
	/** \brief Update property list. */
	void UpdateList();
	
	/** \brief Update property. */
	void UpdateProperty();
	
	/** \brief Update property identifier list. */
	void UpdatePropertyIdentifierList();
	
	/** \brief Update custom pattern list. */
	void UpdateCustomPatternList();
	
	/** \brief Update custom pattern. */
	void UpdateCustomPattern();
	
	/** \brief Update enabled state of widgets. */
	void UpdateEnabled();
	
	/** \brief Select property. */
	void SelectProperty( gdeProperty *property );
	
	/** \brief Select custom file pattern. */
	void SelectCustomPattern( gdeFilePattern *filePattern );
	
	/** \brief Set default value from type specific default value. */
	void SetDefaultValueFromType();
	
	
	
	/** \brief Actions. */
	inline igdeActionContextMenu *GetActionPropertiesMenu() const{ return pActionPropertiesMenu; }
	inline igdeAction *GetActionPropertyAdd() const{ return pActionPropertyAdd; }
	inline igdeAction *GetActionPropertyRemove() const{ return pActionPropertyRemove; }
	inline igdeAction *GetActionPropertyCopy() const{ return pActionPropertyCopy; }
	inline igdeAction *GetActionPropertyCut() const{ return pActionPropertyCut; }
	inline igdeAction *GetActionPropertyPaste() const{ return pActionPropertyPaste; }
	
	inline igdeAction *GetActionOptionAdd() const{ return pActionOptionAdd; }
	inline igdeAction *GetActionOptionRemove() const{ return pActionOptionRemove; }
	
	inline igdeAction *GetActionCustomPatternAdd() const{ return pActionCustomPatternAdd; }
	inline igdeAction *GetActionCustomPatternRemove() const{ return pActionCustomPatternRemove; }
	
	inline igdeActionContextMenu *GetActionCustomPatternMenu() const{ return pActionCustomPatternMenu; }
	/*@}*/
	
	
	
	/** \name Subclass undo creation */
	/*@{*/
	virtual igdeUndo *UndoAdd( gdeProperty *property ) = 0;
	virtual igdeUndo *UndoRemove( gdeProperty *property ) = 0;
	virtual igdeUndo *UndoPaste( gdeProperty *property ) = 0;
	virtual igdeUndo *UndoName( gdeProperty *property, const decString &name ) = 0;
	virtual igdeUndo *UndoDescription( gdeProperty *property, const decString &description ) = 0;
	virtual igdeUndo *UndoType( gdeProperty *property, gdeProperty::ePropertyTypes type ) = 0;
	virtual igdeUndo *UndoMinimumValue( gdeProperty *property, float value ) = 0;
	virtual igdeUndo *UndoMaximumValue( gdeProperty *property, float value ) = 0;
	virtual igdeUndo *UndoDefaultValue( gdeProperty *property, const decString &newValue, const decString &oldValue ) = 0;
	virtual igdeUndo *UndoOptions( gdeProperty *property, const decStringList &options ) = 0;
	virtual igdeUndo *UndoPathPatternType( gdeProperty *property, gdeProperty::ePathPatternTypes type ) = 0;
	virtual igdeUndo *UndoIdentifierGroup( gdeProperty *property, const decString &identifier ) = 0;
	virtual igdeUndo *UndoIdentifierUsage( gdeProperty *property ) = 0;
	virtual igdeUndo *UndoCustomFilePatternAdd( gdeProperty *property, gdeFilePattern *filePattern ) = 0;
	virtual igdeUndo *UndoCustomFilePatternRemove( gdeProperty *property, gdeFilePattern *filePattern ) = 0;
	virtual igdeUndo *UndoCustomFilePatternName( gdeProperty *property,
		gdeFilePattern *filePattern, const decString &name ) = 0;
	virtual igdeUndo *UndoCustomFilePatternPattern( gdeProperty *property,
		gdeFilePattern *filePattern, const decString &pattern ) = 0;
	virtual igdeUndo *UndoCustomFilePatternExtension( gdeProperty *property,
		gdeFilePattern *filePattern, const decString &extension ) = 0;
	/*@}*/
};

#endif
