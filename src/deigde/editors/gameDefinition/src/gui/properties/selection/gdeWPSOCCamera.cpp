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

#include "gdeWPSOCCamera.h"
#include "gdeWPSOCCameraListener.h"
#include "../gdeWindowProperties.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/camera/gdeOCCamera.h"
#include "../../../gamedef/property/gdeProperty.h"
#include "../../../undosys/objectClass/camera/gdeUOCCameraSetFov.h"
#include "../../../undosys/objectClass/camera/gdeUOCCameraSetFovRatio.h"
#include "../../../undosys/objectClass/camera/gdeUOCCameraSetImageDistance.h"
#include "../../../undosys/objectClass/camera/gdeUOCCameraSetPosition.h"
#include "../../../undosys/objectClass/camera/gdeUOCCameraSetPropertyName.h"
#include "../../../undosys/objectClass/camera/gdeUOCCameraSetPropertyPosition.h"
#include "../../../undosys/objectClass/camera/gdeUOCCameraSetPropertyRotation.h"
#include "../../../undosys/objectClass/camera/gdeUOCCameraSetRotation.h"
#include "../../../undosys/objectClass/camera/gdeUOCCameraSetViewDistance.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	gdeWPSOCCamera &pPanel;
	
public:
	cBaseTextFieldListener( gdeWPSOCCamera &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		gdeOCCamera * const camera = pPanel.GetCamera();
		if( ! camera ){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New( OnChanged( *textField, pPanel.GetObjectClass(), camera ) ));
		if( undo ){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField &textField,
		gdeObjectClass *objectClass, gdeOCCamera *camera ) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	gdeWPSOCCamera &pPanel;
	
public:
	cBaseEditVectorListener( gdeWPSOCCamera &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		gdeOCCamera * const camera = pPanel.GetCamera();
		if( ! camera ){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New( OnChanged( editVector->GetVector(), pPanel.GetObjectClass(), camera ) ));
		if( undo ){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( const decVector &vector, gdeObjectClass *objectClass,
		gdeOCCamera *camera ) = 0;
};


class cTextFov : public cBaseTextFieldListener{
public:
	cTextFov( gdeWPSOCCamera &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCCamera *camera ){
		const float value = textField.GetFloat();
		if( fabsf( camera->GetFov() - value ) < FLOAT_SAFE_EPSILON ){
			return NULL;
		}
		return new gdeUOCCameraSetFov( objectClass, camera, value );
	}
};

class cTextFovRatio : public cBaseTextFieldListener{
public:
	cTextFovRatio( gdeWPSOCCamera &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCCamera *camera ){
		const float value = textField.GetFloat();
		if( fabsf( camera->GetFovRatio() - value ) < FLOAT_SAFE_EPSILON ){
			return NULL;
		}
		return new gdeUOCCameraSetFovRatio( objectClass, camera, value );
	}
};

class cTextImageDistance : public cBaseTextFieldListener{
public:
	cTextImageDistance( gdeWPSOCCamera &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCCamera *camera ){
		const float value = textField.GetFloat();
		if( fabsf( camera->GetImageDistance() - value ) < FLOAT_SAFE_EPSILON ){
			return NULL;
		}
		return new gdeUOCCameraSetImageDistance( objectClass, camera, value );
	}
};

class cTextViewDistance : public cBaseTextFieldListener{
public:
	cTextViewDistance( gdeWPSOCCamera &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCCamera *camera ){
		const float value = textField.GetFloat();
		if( fabsf( camera->GetViewDistance() - value ) < FLOAT_SAFE_EPSILON ){
			return NULL;
		}
		return new gdeUOCCameraSetViewDistance( objectClass, camera, value );
	}
};

class cEditPosition : public cBaseEditVectorListener {
public:
	cEditPosition( gdeWPSOCCamera &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( const decVector &vector, gdeObjectClass *objectClass,
	gdeOCCamera *camera ){
		if( camera->GetPosition().IsEqualTo( vector ) ){
			return NULL;
		}
		return new gdeUOCCameraSetPosition( objectClass, camera, vector );
	}
};

class cEditRotation : public cBaseEditVectorListener {
public:
	cEditRotation( gdeWPSOCCamera &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( const decVector &vector, gdeObjectClass *objectClass,
	gdeOCCamera *camera ){
		if( camera->GetRotation().IsEqualTo( vector ) ){
			return NULL;
		}
		return new gdeUOCCameraSetRotation( objectClass, camera, vector );
	}
};

class cComboPropertyName : public igdeComboBoxListener{
	gdeWPSOCCamera &pPanel;
	
public:
	cComboPropertyName( gdeWPSOCCamera &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		gdeOCCamera * const camera = pPanel.GetCamera();
		if( ! camera || camera->GetPropName() == comboBox->GetText() ){
			return;
		}
		
		gdeUOCCameraSetPropertyName::Ref undo(gdeUOCCameraSetPropertyName::Ref::New(new gdeUOCCameraSetPropertyName(
			pPanel.GetObjectClass(), camera, comboBox->GetText() )));
		if( undo ){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
		}
	}
};

class cComboPropertyPosition : public igdeComboBoxListener{
	gdeWPSOCCamera &pPanel;
	
public:
	cComboPropertyPosition(gdeWPSOCCamera &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		gdeOCCamera * const camera = pPanel.GetCamera();
		if(!camera || camera->GetPropPosition() == comboBox->GetText()){
			return;
		}
		
		gdeUOCCameraSetPropertyPosition::Ref undo(gdeUOCCameraSetPropertyPosition::Ref::New(new gdeUOCCameraSetPropertyPosition(
			pPanel.GetObjectClass(), camera, comboBox->GetText())));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
};

class cComboPropertyRotation : public igdeComboBoxListener{
	gdeWPSOCCamera &pPanel;
	
public:
	cComboPropertyRotation(gdeWPSOCCamera &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		gdeOCCamera * const camera = pPanel.GetCamera();
		if(!camera || camera->GetPropRotation() == comboBox->GetText()){
			return;
		}
		
		gdeUOCCameraSetPropertyRotation::Ref undo(gdeUOCCameraSetPropertyRotation::Ref::New(new gdeUOCCameraSetPropertyRotation(
			pPanel.GetObjectClass(), camera, comboBox->GetText())));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
};

}



// Class gdeWPSOCCamera
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCCamera::gdeWPSOCCamera( gdeWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pGameDefinition( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, frameLine;
	
	pListener = new gdeWPSOCCameraListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	helper.GroupBox( content, groupBox, "Object Class Camera:" );
	helper.EditFloat( groupBox, "FOV:", "Field of view (FOV) in degrees", 4, 1,
		pEditFov, new cTextFov( *this ) );
	helper.EditFloat( groupBox, "FOV Ratio:", "Field of view Ratio", 4, 2,
		pEditFovRatio, new cTextFovRatio( *this ) );
	helper.EditFloat( groupBox, "Image Distance:", "Image distance in meters",
		pEditImageDistance, new cTextImageDistance( *this ) );
	helper.EditFloat( groupBox, "View Distance:", "View distance in meters", 4, 1,
		pEditViewDistance, new cTextViewDistance( *this ) );
	helper.EditVector( groupBox, "Position:", "Position relative to object class",
		pEditPosition, new cEditPosition( *this ) );
	helper.EditVector( groupBox, "Rotation:", "Rotation in degrees relative to object class",
		pEditRotation, new cEditRotation( *this ) );
	helper.ComboBox(groupBox, "ID Property:", true,
		"Object class property containing camera identifier",
		pCBPropertyName, new cComboPropertyName(*this));
	pCBPropertyName->SetDefaultSorter();
	helper.ComboBox(groupBox, "Position Property:", true,
		"Object class property containing camera position",
		pCBPropertyPosition, new cComboPropertyPosition(*this));
	pCBPropertyPosition->SetDefaultSorter();
	helper.ComboBox(groupBox, "Rotation Property:", true,
		"Object class property containing camera rotation",
		pCBPropertyRotation, new cComboPropertyRotation(*this));
	pCBPropertyRotation->SetDefaultSorter();
}

gdeWPSOCCamera::~gdeWPSOCCamera(){
	SetGameDefinition( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void gdeWPSOCCamera::SetGameDefinition( gdeGameDefinition *gameDefinition ){
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
	UpdateCamera();
}



gdeObjectClass *gdeWPSOCCamera::GetObjectClass() const{
	return pGameDefinition ? pGameDefinition->GetActiveObjectClass() : NULL;
}

gdeOCCamera *gdeWPSOCCamera::GetCamera() const{
	const gdeObjectClass * const objectClass = GetObjectClass();
	return objectClass ? pGameDefinition->GetActiveOCCamera() : NULL;
}



void gdeWPSOCCamera::UpdatePropertyList(){
	const gdeObjectClass *objectClass = GetObjectClass();
	int i;
	
	decStringSet properties;
	if( objectClass ){
		objectClass->AddPropertyNamesTo( properties, true );
	}
	
	const decString selectionName(pCBPropertyName->GetText());
	const decString selectionPosition(pCBPropertyPosition->GetText());
	const decString selectionRotation(pCBPropertyRotation->GetText());
	pCBPropertyName->RemoveAllItems();
	pCBPropertyPosition->RemoveAllItems();
	pCBPropertyRotation->RemoveAllItems();
	
	const int count = properties.GetCount();
	for(i=0; i<count; i++){
		const decString &property = properties.GetAt(i);
		pCBPropertyName->AddItem(property);
		pCBPropertyPosition->AddItem(property);
		pCBPropertyRotation->AddItem(property);
	}
	
	pCBPropertyName->SortItems();
	pCBPropertyPosition->SortItems();
	pCBPropertyRotation->SortItems();
	
	pCBPropertyName->SetText(selectionName);
	pCBPropertyPosition->SetText(selectionPosition);
	pCBPropertyRotation->SetText(selectionRotation);
}

void gdeWPSOCCamera::UpdateCamera(){
	const gdeOCCamera * const camera = GetCamera();
	
	if( camera ){
		pEditFov->SetFloat( camera->GetFov() );
		pEditFovRatio->SetFloat( camera->GetFovRatio() );
		pEditImageDistance->SetFloat( camera->GetImageDistance() );
		pEditViewDistance->SetFloat( camera->GetViewDistance() );
		pEditPosition->SetVector( camera->GetPosition() );
		pEditRotation->SetVector( camera->GetRotation() );
		pCBPropertyName->SetText(camera->GetPropName());
		pCBPropertyPosition->SetText(camera->GetPropPosition());
		pCBPropertyRotation->SetText(camera->GetPropRotation());
		
	}else{
		pEditFov->ClearText();
		pEditFovRatio->ClearText();
		pEditImageDistance->ClearText();
		pEditViewDistance->ClearText();
		pEditPosition->SetVector( decVector() );
		pEditRotation->SetVector( decVector() );
		pCBPropertyName->ClearText();
		pCBPropertyPosition->ClearText();
		pCBPropertyRotation->ClearText();
	}
	
	const bool enabled = camera;
	pEditFov->SetEnabled( enabled );
	pEditFovRatio->SetEnabled( enabled );
	pEditImageDistance->SetEnabled( enabled );
	pEditViewDistance->SetEnabled( enabled );
	pEditPosition->SetEnabled( enabled );
	pEditRotation->SetEnabled( enabled );
	pCBPropertyName->SetEnabled(enabled);
	pCBPropertyPosition->SetEnabled(enabled);
	pCBPropertyRotation->SetEnabled(enabled);
}
