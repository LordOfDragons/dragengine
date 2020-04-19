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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "gdeWPSOCSnapPoint.h"
#include "gdeWPSOCSnapPointListener.h"
#include "../gdeWindowProperties.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/snappoint/gdeOCSnapPoint.h"
#include "../../../undosys/objectClass/snappoint/gdeUOCSnapPointSetName.h"
#include "../../../undosys/objectClass/snappoint/gdeUOCSnapPointSetPosition.h"
#include "../../../undosys/objectClass/snappoint/gdeUOCSnapPointSetRotation.h"
#include "../../../undosys/objectClass/snappoint/gdeUOCSnapPointSetSnapDistance.h"
#include "../../../undosys/objectClass/snappoint/gdeUOCSnapPointToggleSnapToRotation.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

namespace{

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	gdeWPSOCSnapPoint &pPanel;
	
public:
	cBaseEditVectorListener( gdeWPSOCSnapPoint &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		gdeOCSnapPoint * const snapPoint = pPanel.GetSnapPoint();
		if( ! snapPoint ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( editVector->GetVector(), pPanel.GetObjectClass(), snapPoint ) );
		if( undo ){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( const decVector &vector, gdeObjectClass *objectClass,
		gdeOCSnapPoint *snapPoint ) = 0;
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	gdeWPSOCSnapPoint &pPanel;
	
public:
	cBaseTextFieldListener( gdeWPSOCSnapPoint &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		gdeOCSnapPoint * const snapPoint = pPanel.GetSnapPoint();
		if( ! snapPoint ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( *textField, pPanel.GetObjectClass(), snapPoint ) );
		if( undo ){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeObjectClass *objectClass,
		gdeOCSnapPoint *snapPoint ) = 0;
};


class cTextName : public cBaseTextFieldListener{
public:
	cTextName( gdeWPSOCSnapPoint &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCSnapPoint *snapPoint ){
		if( snapPoint->GetName() == textField.GetText() ){
			return NULL;
		}
		return new gdeUOCSnapPointSetName( objectClass, snapPoint, textField.GetText() );
	}
};

class cEditPosition : public cBaseEditVectorListener {
public:
	cEditPosition( gdeWPSOCSnapPoint &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( const decVector &vector, gdeObjectClass *objectClass,
	gdeOCSnapPoint *snapPoint ){
		if( snapPoint->GetPosition().IsEqualTo( vector ) ){
			return NULL;
		}
		return new gdeUOCSnapPointSetPosition( objectClass, snapPoint, vector );
	}
};

class cEditRotation : public cBaseEditVectorListener {
public:
	cEditRotation( gdeWPSOCSnapPoint &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( const decVector &vector, gdeObjectClass *objectClass,
	gdeOCSnapPoint *snapPoint ){
		if( snapPoint->GetRotation().IsEqualTo( vector ) ){
			return NULL;
		}
		return new gdeUOCSnapPointSetRotation( objectClass, snapPoint, vector );
	}
};

class cTextSnapDistance : public cBaseTextFieldListener{
public:
	cTextSnapDistance( gdeWPSOCSnapPoint &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCSnapPoint *snapPoint ){
		const float value = textField.GetFloat();
		if( fabsf( snapPoint->GetSnapDistance() - value ) < FLOAT_SAFE_EPSILON ){
			return NULL;
		}
		return new gdeUOCSnapPointSetSnapDistance( objectClass, snapPoint, value );
	}
};

class cActionSnapToRotation : public igdeAction{
	gdeWPSOCSnapPoint &pPanel;
	
public:
	cActionSnapToRotation( gdeWPSOCSnapPoint &panel ) : igdeAction( "Snap to rotation",
		"Object rotation is snapped to snap point rotation" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		gdeOCSnapPoint * const snapPoint = pPanel.GetSnapPoint();
		if( ! snapPoint ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUOCSnapPointToggleSnapToRotation( pPanel.GetObjectClass(), snapPoint ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};

}



// Class gdeWPSOCSnapPoint
////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCSnapPoint::gdeWPSOCSnapPoint( gdeWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pGameDefinition( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference content, groupBox, frameLine;
	
	pListener = new gdeWPSOCSnapPointListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	helper.GroupBox( content, groupBox, "Object Class Particle Emitter:" );
	
	helper.EditString( groupBox, "Name:", "Name to show next to snap points",
		pEditName, new cTextName( *this ) );
	helper.EditVector( groupBox, "Position:", "Position relative to object class",
		pEditPosition, new cEditPosition( *this ) );
	helper.EditVector( groupBox, "Rotation:", "Rotation in degrees relative to object class", 4, 1,
		pEditRotation, new cEditRotation( *this ) );
	helper.EditFloat( groupBox, "Snap Distance:", "Snap distance in meters",
		pEditSnapDistance, new cTextSnapDistance( *this ) );
	helper.CheckBox( groupBox, pChkSnapToRotation, new cActionSnapToRotation( *this ), true );
}

gdeWPSOCSnapPoint::~gdeWPSOCSnapPoint(){
	SetGameDefinition( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void gdeWPSOCSnapPoint::SetGameDefinition( gdeGameDefinition *gameDefinition ){
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
	
	UpdateSnapPoint();
}



gdeObjectClass *gdeWPSOCSnapPoint::GetObjectClass() const{
	return pGameDefinition ? pGameDefinition->GetActiveObjectClass() : NULL;
}

gdeOCSnapPoint *gdeWPSOCSnapPoint::GetSnapPoint() const{
	const gdeObjectClass * const objectClass = GetObjectClass();
	return objectClass ? pGameDefinition->GetActiveOCSnapPoint() : NULL;
}



void gdeWPSOCSnapPoint::UpdateSnapPoint(){
	const gdeOCSnapPoint * const snapPoint = GetSnapPoint();
	
	if( snapPoint ){
		pEditName->SetText( snapPoint->GetName() );
		pEditPosition->SetVector( snapPoint->GetPosition() );
		pEditRotation->SetVector( snapPoint->GetRotation() );
		pEditSnapDistance->SetFloat( snapPoint->GetSnapDistance() );
		pChkSnapToRotation->SetChecked( snapPoint->GetSnapToRotation() );
		
	}else{
		pEditName->ClearText();
		pEditPosition->SetVector( decVector() );
		pEditRotation->SetVector( decVector() );
		pEditSnapDistance->ClearText();
		pChkSnapToRotation->SetChecked( false );
	}
	
	const bool enabled = snapPoint;
	pEditName->SetEnabled( enabled );
	pEditPosition->SetEnabled( enabled );
	pEditRotation->SetEnabled( enabled );
	pEditSnapDistance->SetEnabled( enabled );
	pChkSnapToRotation->SetEnabled( enabled );
}
