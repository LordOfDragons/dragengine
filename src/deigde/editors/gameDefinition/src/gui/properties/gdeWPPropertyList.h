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
