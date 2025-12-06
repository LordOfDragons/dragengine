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
#include <stdint.h>

#include "gdeWPSOCParticleEmitter.h"
#include "gdeWPSOCParticleEmitterListener.h"
#include "../gdeWindowProperties.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../undosys/objectClass/particleemitter/gdeUOCParticleEmitterSetPath.h"
#include "../../../undosys/objectClass/particleemitter/gdeUOCParticleEmitterSetPosition.h"
#include "../../../undosys/objectClass/particleemitter/gdeUOCParticleEmitterSetRotation.h"
#include "../../../undosys/objectClass/particleemitter/gdeUOCParticleEmitterSetBoneName.h"
#include "../../../undosys/objectClass/particleemitter/gdeUOCParticleEmitterToggleCasting.h"
#include "../../../undosys/objectClass/particleemitter/gdeUOCParticleEmitterSetPropertyName.h"
#include "../../../undosys/objectClass/particleemitter/gdeUOCParticleEmitterSetTriggerName.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

namespace{

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	gdeWPSOCParticleEmitter &pPanel;
	
public:
	cBaseEditVectorListener( gdeWPSOCParticleEmitter &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		gdeOCParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if( ! particleEmitter ){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New( OnChanged( editVector->GetVector(), pPanel.GetObjectClass(), particleEmitter ) ));
		if( undo ){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( const decVector &vector, gdeObjectClass *objectClass,
		gdeOCParticleEmitter *particleEmitter ) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	gdeWPSOCParticleEmitter &pPanel;
	
public:
	cBaseComboBoxListener( gdeWPSOCParticleEmitter &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		gdeOCParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if( ! particleEmitter ){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New( OnChanged( *comboBox, pPanel.GetObjectClass(), particleEmitter ) ));
		if( undo ){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox,
		gdeObjectClass *objectClass, gdeOCParticleEmitter *particleEmitter ) = 0;
};

class cBaseAction : public igdeAction{
protected:
	gdeWPSOCParticleEmitter &pPanel;
	
public:
	cBaseAction( gdeWPSOCParticleEmitter &panel, const char *text, const char *description ) :
		igdeAction( text, description ), pPanel( panel ){ }
	
	virtual void OnAction(){
		gdeOCParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if( ! particleEmitter ){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New( OnActionParticleEmitter( pPanel.GetObjectClass(), particleEmitter ) ));
		if( undo ){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnActionParticleEmitter( gdeObjectClass *objectClass, gdeOCParticleEmitter *particleEmitter ) = 0;
	
	virtual void Update(){
		gdeOCParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if( particleEmitter ){
			Update( *pPanel.GetObjectClass(), *particleEmitter );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const gdeObjectClass &, const gdeOCParticleEmitter & ){
	}
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	gdeWPSOCParticleEmitter &pPanel;
	
public:
	cBaseTextFieldListener( gdeWPSOCParticleEmitter &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		gdeOCParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if( ! particleEmitter ){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New( OnChanged( *textField, pPanel.GetObjectClass(), particleEmitter ) ));
		if( undo ){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField &textField,
		gdeObjectClass *objectClass, gdeOCParticleEmitter *particleEmitter ) = 0;
};


class cEditPath : public igdeEditPathListener{
	gdeWPSOCParticleEmitter &pPanel;
	
public:
	cEditPath( gdeWPSOCParticleEmitter &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		gdeOCParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if( ! particleEmitter || particleEmitter->GetPath() == editPath->GetPath() ){
			return;
		}
		
		gdeUOCParticleEmitterSetPath::Ref undo(gdeUOCParticleEmitterSetPath::Ref::New(new gdeUOCParticleEmitterSetPath(
			pPanel.GetObjectClass(), particleEmitter, editPath->GetPath() )));
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};

class cEditPosition : public cBaseEditVectorListener {
public:
	cEditPosition( gdeWPSOCParticleEmitter &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( const decVector &vector, gdeObjectClass *objectClass,
	gdeOCParticleEmitter *particleEmitter ){
		if( particleEmitter->GetPosition().IsEqualTo( vector ) ){
			return NULL;
		}
		return new gdeUOCParticleEmitterSetPosition( objectClass, particleEmitter, vector );
	}
};

class cEditRotation : public cBaseEditVectorListener {
public:
	cEditRotation( gdeWPSOCParticleEmitter &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( const decVector &vector, gdeObjectClass *objectClass,
	gdeOCParticleEmitter *particleEmitter ){
		if( particleEmitter->GetRotation().IsEqualTo( vector ) ){
			return NULL;
		}
		return new gdeUOCParticleEmitterSetRotation( objectClass, particleEmitter, vector );
	}
};

class cTextBoneName : public cBaseTextFieldListener{
public:
	cTextBoneName( gdeWPSOCParticleEmitter &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCParticleEmitter *particleEmitter ){
		if( particleEmitter->GetBoneName() == textField.GetText() ){
			return NULL;
		}
		return new gdeUOCParticleEmitterSetBoneName( objectClass, particleEmitter, textField.GetText() );
	}
};

class cActionCasting : public cBaseAction{
public:
	cActionCasting( gdeWPSOCParticleEmitter &panel ) :
	cBaseAction( panel, "Casting Particles", "Particle emitter is casting particles" ){ }
	
	virtual igdeUndo *OnActionParticleEmitter( gdeObjectClass *objectClass, gdeOCParticleEmitter *particleEmitter ){
		return new gdeUOCParticleEmitterToggleCasting( objectClass, particleEmitter );
	}
	
	virtual void Update( const gdeObjectClass &, const gdeOCParticleEmitter &particleEmitter ){
		SetEnabled( true );
		SetSelected( particleEmitter.GetCasting() );
	}
};


class cComboPropertyNames : public igdeComboBoxListener{
	gdeWPSOCParticleEmitter &pPanel;
	
public:
	cComboPropertyNames( gdeWPSOCParticleEmitter &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox* ){
		if( pPanel.GetParticleEmitter() ){
			pPanel.UpdatePropertyName();
		}
	}
};

class cComboPropertyNameTarget : public igdeComboBoxListener{
	gdeWPSOCParticleEmitter &pPanel;
	
public:
	cComboPropertyNameTarget( gdeWPSOCParticleEmitter &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		gdeOCParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if( ! particleEmitter ){
			return;
		}
		
		const gdeOCParticleEmitter::eProperties propertyName = pPanel.GetPropertyName();
		if( particleEmitter->GetPropertyName( propertyName ) == comboBox->GetText() ){
			return;
		}
		
		gdeUOCParticleEmitterSetPropertyName::Ref undo(gdeUOCParticleEmitterSetPropertyName::Ref::New(new gdeUOCParticleEmitterSetPropertyName(
			pPanel.GetObjectClass(), particleEmitter, propertyName, comboBox->GetText() )));
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};

class cComboTriggerNames : public igdeComboBoxListener{
	gdeWPSOCParticleEmitter &pPanel;
	
public:
	cComboTriggerNames( gdeWPSOCParticleEmitter &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox* ){
		if( pPanel.GetParticleEmitter() ){
			pPanel.UpdateTriggerName();
		}
	}
};

class cComboTriggerNameTarget : public igdeComboBoxListener{
	gdeWPSOCParticleEmitter &pPanel;
	
public:
	cComboTriggerNameTarget( gdeWPSOCParticleEmitter &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		if( ! pPanel.GetParticleEmitter() ){
			return;
		}
		
		const gdeOCParticleEmitter::eTriggers triggerName = pPanel.GetTriggerName();
		if( pPanel.GetParticleEmitter()->GetTriggerName( triggerName ) == comboBox->GetText() ){
			return;
		}
		
		gdeUOCParticleEmitterSetTriggerName::Ref undo(gdeUOCParticleEmitterSetTriggerName::Ref::New(new gdeUOCParticleEmitterSetTriggerName(
			pPanel.GetObjectClass(), pPanel.GetParticleEmitter(), triggerName, comboBox->GetText() )));
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};

}



// Class gdeWPSOCParticleEmitter
//////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCParticleEmitter::gdeWPSOCParticleEmitter( gdeWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pGameDefinition( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, frameLine;
	
	pListener = new gdeWPSOCParticleEmitterListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	helper.GroupBox( content, groupBox, "Object Class Particle Emitter:" );
	
	helper.EditPath( groupBox, "Path:", "Path to particle emitter",
		igdeEnvironment::efpltParticleEmitter, pEditPath, new cEditPath( *this ) );
	helper.EditVector( groupBox, "Position:", "Position relative to object class",
		pEditPosition, new cEditPosition( *this ) );
	helper.EditVector( groupBox, "Rotation:", "Rotation in degrees relative to object class", 4, 1,
		pEditRotation, new cEditRotation( *this ) );
	helper.CheckBox( groupBox, pChkCasting, new cActionCasting( *this ), true );
	helper.EditString( groupBox, "Bone:", "Bone name or empty string if not used",
		pEditBoneName, new cTextBoneName( *this ) );
	
	// property targets
	helper.GroupBox( content, groupBox, "Properties:" );
	helper.ComboBox( groupBox, "Property:", "Property to set target for",
		pCBPropertyNames, new cComboPropertyNames( *this ) );
	pCBPropertyNames->AddItem( "Path", NULL, ( void* )( intptr_t )gdeOCParticleEmitter::epPath );
	pCBPropertyNames->AddItem( "Attach position", NULL, ( void* )( intptr_t )gdeOCParticleEmitter::epAttachPosition );
	pCBPropertyNames->AddItem( "Attach rotation", NULL, ( void* )( intptr_t )gdeOCParticleEmitter::epAttachRotation );
	pCBPropertyNames->AddItem( "Casting", NULL, ( void* )( intptr_t )gdeOCParticleEmitter::epCasting );
	
	helper.ComboBoxFilter( groupBox, "Target:", true, "Object class property to target",
		pCBPropertyNameTarget, new cComboPropertyNameTarget( *this ) );
	pCBPropertyNameTarget->SetEditable( true );
	pCBPropertyNameTarget->SetDefaultSorter();
	pCBPropertyNameTarget->SetFilterCaseInsentive( true );
	
	// trigger targets
	helper.GroupBox( content, groupBox, "Triggers:" );
	helper.ComboBox( groupBox, "Trigger:", "Trigger to set target for",
		pCBTriggerNames, new cComboTriggerNames( *this ) );
	pCBTriggerNames->AddItem( "Casting", NULL, ( void* )( intptr_t )gdeOCParticleEmitter::etCasting );
	
	helper.ComboBoxFilter( groupBox, "Target:", true, "Object class property to target",
		pCBTriggerNameTarget, new cComboTriggerNameTarget( *this ) );
	pCBTriggerNameTarget->SetEditable( true );
	pCBTriggerNameTarget->SetDefaultSorter();
	pCBTriggerNameTarget->SetFilterCaseInsentive( true );
}

gdeWPSOCParticleEmitter::~gdeWPSOCParticleEmitter(){
	SetGameDefinition( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void gdeWPSOCParticleEmitter::SetGameDefinition( gdeGameDefinition *gameDefinition ){
	if( gameDefinition == pGameDefinition ){
		return;
	}
	
	if( pGameDefinition ){
		pGameDefinition->RemoveListener( pListener );
		pGameDefinition->FreeReference();
	}
	
	pGameDefinition = gameDefinition;
	
	if( gameDefinition ){
		gameDefinition->AddListener( pListener );
		gameDefinition->AddReference();
	}
	
	UpdatePropertyList();
	UpdateParticleEmitter();
}



gdeObjectClass *gdeWPSOCParticleEmitter::GetObjectClass() const{
	return pGameDefinition ? pGameDefinition->GetActiveObjectClass() : NULL;
}

gdeOCParticleEmitter *gdeWPSOCParticleEmitter::GetParticleEmitter() const{
	const gdeObjectClass * const objectClass = GetObjectClass();
	return objectClass ? pGameDefinition->GetActiveOCParticleEmitter() : NULL;
}

const gdeOCParticleEmitter::eProperties gdeWPSOCParticleEmitter::GetPropertyName() const{
	return ( gdeOCParticleEmitter::eProperties )( intptr_t )pCBPropertyNames->GetSelectedItem()->GetData();
}

const gdeOCParticleEmitter::eTriggers gdeWPSOCParticleEmitter::GetTriggerName() const{
	return ( gdeOCParticleEmitter::eTriggers )( intptr_t )pCBTriggerNames->GetSelectedItem()->GetData();
}



void gdeWPSOCParticleEmitter::UpdatePropertyList(){
	const gdeObjectClass *objectClass = GetObjectClass();
	int i;
	
	decStringSet properties;
	if( objectClass ){
		objectClass->AddPropertyNamesTo( properties, true );
	}
	
	const decString selectionProperty( pCBPropertyNameTarget->GetText() );
	const decString selectionTrigger( pCBTriggerNameTarget->GetText() );
	pCBPropertyNameTarget->RemoveAllItems();
	pCBTriggerNameTarget->RemoveAllItems();
	
	const int count = properties.GetCount();
	for( i=0; i<count; i++ ){
		pCBPropertyNameTarget->AddItem( properties.GetAt( i ) );
		pCBTriggerNameTarget->AddItem( properties.GetAt( i ) );
	}
	
	pCBPropertyNameTarget->SortItems();
	pCBPropertyNameTarget->SetText( selectionProperty );
	
	pCBTriggerNameTarget->SortItems();
	pCBTriggerNameTarget->SetText( selectionTrigger );
}



void gdeWPSOCParticleEmitter::UpdateParticleEmitter(){
	const gdeOCParticleEmitter * const particleEmitter = GetParticleEmitter();
	
	if( particleEmitter ){
		pEditPath->SetPath( particleEmitter->GetPath() );
		pEditPosition->SetVector( particleEmitter->GetPosition() );
		pEditRotation->SetVector( particleEmitter->GetRotation() );
		pEditBoneName->SetText( particleEmitter->GetBoneName() );
		
		
	}else{
		pEditPath->ClearPath();
		pEditPosition->SetVector( decVector() );
		pEditRotation->SetVector( decVector() );
		pEditBoneName->ClearText();
	}
	
	const bool enabled = particleEmitter;
	pEditPath->SetEnabled( enabled );
	pEditPosition->SetEnabled( enabled );	
	pEditRotation->SetEnabled( enabled );
	pEditBoneName->SetEnabled( enabled );
	
	pChkCasting->GetAction()->Update();
	
	UpdatePropertyName();
	UpdateTriggerName();
}

void gdeWPSOCParticleEmitter::UpdatePropertyName(){
	const gdeOCParticleEmitter * const particleEmitter = GetParticleEmitter();
	
	if( particleEmitter ){
		pCBPropertyNameTarget->SetText( particleEmitter->GetPropertyName( GetPropertyName() ) );
		
	}else{
		pCBPropertyNameTarget->ClearText();
	}
	
	pCBPropertyNameTarget->SetEnabled( particleEmitter );
}

void gdeWPSOCParticleEmitter::UpdateTriggerName(){
	const gdeOCParticleEmitter * const particleEmitter = GetParticleEmitter();
	
	if( particleEmitter ){
		pCBTriggerNameTarget->SetText( particleEmitter->GetTriggerName( GetTriggerName() ) );
		
	}else{
		pCBTriggerNameTarget->ClearText();
	}
	
	pCBTriggerNameTarget->SetEnabled( particleEmitter );
}
