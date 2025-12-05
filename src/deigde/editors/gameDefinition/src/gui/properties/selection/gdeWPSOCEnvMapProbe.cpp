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

#include "gdeWPSOCEnvMapProbe.h"
#include "gdeWPSOCEnvMapProbeListener.h"
#include "../gdeWindowProperties.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/envmapprobe/gdeOCEnvMapProbe.h"
#include "../../../gamedef/property/gdeProperty.h"
#include "../../../undosys/objectClass/envmapprobe/gdeUOCEnvMapProbeSetPropertyName.h"
#include "../../../undosys/objectClass/envmapprobe/gdeUOCEnvMapProbeSetScaling.h"
#include "../../../undosys/objectClass/envmapprobe/gdeUOCEnvMapProbeSetInfluenceBorderSize.h"
#include "../../../undosys/objectClass/envmapprobe/gdeUOCEnvMapProbeSetPosition.h"
#include "../../../undosys/objectClass/envmapprobe/gdeUOCEnvMapProbeSetShapeInfluence.h"
#include "../../../undosys/objectClass/envmapprobe/gdeUOCEnvMapProbeSetShapeReflection.h"
#include "../../../undosys/objectClass/envmapprobe/gdeUOCEnvMapProbeSetInfluencePriority.h"
#include "../../../undosys/objectClass/envmapprobe/gdeUOCEnvMapProbeSetRotation.h"
#include "../../../undosys/objectClass/envmapprobe/gdeUOCEnvMapProbeSetShapeReflectionMask.h"

#include <deigde/codec/igdeCodecPropertyString.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeWindow.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	gdeWPSOCEnvMapProbe &pPanel;
	
public:
	cBaseTextFieldListener( gdeWPSOCEnvMapProbe &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		gdeOCEnvMapProbe * const envprobe = pPanel.GetEnvMapProbe();
		if( ! envprobe ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( OnChanged( *textField, pPanel.GetObjectClass(), envprobe ) );
		if( undo ){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField &textField,
		gdeObjectClass *objectClass, gdeOCEnvMapProbe *envprobe ) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	gdeWPSOCEnvMapProbe &pPanel;
	
public:
	cBaseEditVectorListener( gdeWPSOCEnvMapProbe &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		gdeOCEnvMapProbe * const envprobe = pPanel.GetEnvMapProbe();
		if( ! envprobe ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( OnChanged( editVector->GetVector(), pPanel.GetObjectClass(), envprobe ) );
		if( undo ){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( const decVector &vector, gdeObjectClass *objectClass,
		gdeOCEnvMapProbe *envprobe ) = 0;
};


class cEditPosition : public cBaseEditVectorListener {
public:
	cEditPosition( gdeWPSOCEnvMapProbe &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( const decVector &vector, gdeObjectClass *objectClass,
	gdeOCEnvMapProbe *envprobe ){
		if( envprobe->GetPosition().IsEqualTo( vector ) ){
			return NULL;
		}
		return new gdeUOCEnvMapProbeSetPosition( objectClass, envprobe, vector );
	}
};

class cEditRotation : public cBaseEditVectorListener {
public:
	cEditRotation( gdeWPSOCEnvMapProbe &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( const decVector &vector, gdeObjectClass *objectClass,
	gdeOCEnvMapProbe *envprobe ){
		if( envprobe->GetRotation().IsEqualTo( vector ) ){
			return NULL;
		}
		return new gdeUOCEnvMapProbeSetRotation( objectClass, envprobe, vector );
	}
};

class cEditScaling : public cBaseEditVectorListener {
public:
	cEditScaling( gdeWPSOCEnvMapProbe &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( const decVector &vector, gdeObjectClass *objectClass,
	gdeOCEnvMapProbe *envprobe ){
		if( envprobe->GetScaling().IsEqualTo( vector ) ){
			return NULL;
		}
		return new gdeUOCEnvMapProbeSetScaling( objectClass, envprobe, vector );
	}
};

class cTextShapeInfluence : public cBaseTextFieldListener{
public:
	cTextShapeInfluence( gdeWPSOCEnvMapProbe &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCEnvMapProbe *envprobe ){
		igdeCodecPropertyString codec;
		decString oldEncoded;
		codec.EncodeShapeList( envprobe->GetShapeListInfluence(), oldEncoded );
		const decString encoded( textField.GetText() );
		if( encoded == oldEncoded ){
			return NULL;
		}
		
		decShapeList shapeList;
		try{
			codec.DecodeShapeList( encoded, shapeList );
			
		}catch( const deException & ){
			igdeCommonDialogs::Error( pPanel.GetParentWindow(), "Invalid Input",
				"Input value does not decode to a proper shape list" );
			textField.Focus();
			return NULL;
		}
		
		return new gdeUOCEnvMapProbeSetShapeInfluence( objectClass, envprobe, shapeList );
	}
};

class cTextShapeReflection : public cBaseTextFieldListener{
public:
	cTextShapeReflection( gdeWPSOCEnvMapProbe &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCEnvMapProbe *envprobe ){
		igdeCodecPropertyString codec;
		decShapeList shapeList;
		if( envprobe->GetShapeReflection() ){
			shapeList.Add( envprobe->GetShapeReflection()->Copy() );
		}
		decString oldEncoded;
		codec.EncodeShapeList( shapeList, oldEncoded );
		const decString encoded( textField.GetText() );
		if( encoded == oldEncoded ){
			return NULL;
		}
		
		try{
			codec.DecodeShapeList( encoded, shapeList );
			
		}catch( const deException & ){
			igdeCommonDialogs::Error( pPanel.GetParentWindow(), "Invalid Input",
				"Input value does not decode to a proper shape list" );
			textField.Focus();
			return NULL;
		}
		
		return new gdeUOCEnvMapProbeSetShapeReflection(
			objectClass, envprobe, shapeList.GetAt( 0 )->Copy() );
	}
};

class cTextShapeReflectionMask : public cBaseTextFieldListener{
public:
	cTextShapeReflectionMask( gdeWPSOCEnvMapProbe &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCEnvMapProbe *envprobe ){
		igdeCodecPropertyString codec;
		decString oldEncoded;
		codec.EncodeShapeList( envprobe->GetShapeListReflectionMask(), oldEncoded );
		const decString encoded( textField.GetText() );
		if( encoded == oldEncoded ){
			return NULL;
		}
		
		decShapeList shapeList;
		try{
			codec.DecodeShapeList( encoded, shapeList );
			
		}catch( const deException & ){
			igdeCommonDialogs::Error( pPanel.GetParentWindow(), "Invalid Input",
				"Input value does not decode to a proper shape list" );
			textField.Focus();
			return NULL;
		}
		
		return new gdeUOCEnvMapProbeSetShapeReflectionMask( objectClass, envprobe, shapeList );
	}
};

class cTextInfluenceBorderSize : public cBaseTextFieldListener{
public:
	cTextInfluenceBorderSize( gdeWPSOCEnvMapProbe &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCEnvMapProbe *envprobe ){
		const float value = textField.GetFloat();
		if( fabsf( envprobe->GetInfluenceBorderSize() - value ) < FLOAT_SAFE_EPSILON ){
			return NULL;
		}
		return new gdeUOCEnvMapProbeSetInfluenceBorderSize( objectClass, envprobe, value );
	}
};

class cTextInfluencePriority : public cBaseTextFieldListener{
public:
	cTextInfluencePriority( gdeWPSOCEnvMapProbe &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCEnvMapProbe *envprobe ){
		const int value = textField.GetInteger();
		if( envprobe->GetInfluencePriority() == value ){
			return NULL;
		}
		return new gdeUOCEnvMapProbeSetInfluencePriority( objectClass, envprobe, value );
	}
};


class cComboPropertyNames : public igdeComboBoxListener{
	gdeWPSOCEnvMapProbe &pPanel;
	
public:
	cComboPropertyNames( gdeWPSOCEnvMapProbe &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		if( pPanel.GetEnvMapProbe() ){
			pPanel.UpdatePropertyName();
		}
	}
};

class cComboPropertyNameTarget : public igdeComboBoxListener{
	gdeWPSOCEnvMapProbe &pPanel;
	
public:
	cComboPropertyNameTarget( gdeWPSOCEnvMapProbe &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		gdeOCEnvMapProbe * const envprobe = pPanel.GetEnvMapProbe();
		if( ! envprobe ){
			return;
		}
		
		const gdeOCEnvMapProbe::eProperties propertyName = pPanel.GetPropertyName();
		if( envprobe->GetPropertyName( propertyName ) == comboBox->GetText() ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new gdeUOCEnvMapProbeSetPropertyName(
			pPanel.GetObjectClass(), envprobe, propertyName, comboBox->GetText() ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};

}



// Class gdeWPSOCEnvMapProbe
//////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCEnvMapProbe::gdeWPSOCEnvMapProbe( gdeWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pGameDefinition( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, frameLine;
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	pListener = new gdeWPSOCEnvMapProbeListener( *this );
	
	helper.GroupBox( content, groupBox, "Object Class Environment Map Probe:" );
	helper.EditVector( groupBox, "Position:", "Position relative to object class",
		pEditPosition, new cEditPosition( *this ) );
	helper.EditVector( groupBox, "Rotation:", "Rotation in degrees relative to object class",
		pEditRotation, new cEditRotation( *this ) );
	helper.EditVector( groupBox, "Scaling:", "Scaling in degrees relative to object class",
		pEditScaling, new cEditScaling( *this ) );
	helper.EditString( groupBox, "Influence Shape:", "Influence shape",
		pEditShapeInfluence, new cTextShapeInfluence( *this ) );
	helper.EditString( groupBox, "Reflection Shape:", "Reflection shape",
		pEditShapeReflection, new cTextShapeReflection( *this ) );
	helper.EditString( groupBox, "Reflection Shape Mask:", "Reflection shape mask",
		pEditShapeReflectionMask, new cTextShapeReflectionMask( *this ) );
	helper.EditFloat( groupBox, "Influence Border Size:", "Influence border size",
		pEditInfluenceBorderSize, new cTextInfluenceBorderSize( *this ) );
	helper.EditInteger( groupBox, "Influence Priority:", "Influence priority",
		pEditInfluencePriority, new cTextInfluencePriority( *this ) );
	
	// properties targets
	helper.GroupBox( content, groupBox, "Properties:" );
	helper.ComboBox( groupBox, "Property:", "Property to set target for",
		pCBPropertyNames, new cComboPropertyNames( *this ) );
	pCBPropertyNames->AddItem( "Influence area", NULL,
		( void* )( intptr_t )gdeOCEnvMapProbe::epInfluenceArea );
	pCBPropertyNames->AddItem( "Influence border size", NULL,
		( void* )( intptr_t )gdeOCEnvMapProbe::epInfluenceBorderSize );
	pCBPropertyNames->AddItem( "Influence priority", NULL,
		( void* )( intptr_t )gdeOCEnvMapProbe::epInfluencePriority );
	pCBPropertyNames->AddItem( "Reflection shape", NULL,
		( void* )( intptr_t )gdeOCEnvMapProbe::epReflectionShape );
	pCBPropertyNames->AddItem( "Reflection mask", NULL,
		( void* )( intptr_t )gdeOCEnvMapProbe::epReflectionMask );
	pCBPropertyNames->AddItem( "Attach position", NULL,
		( void* )( intptr_t )gdeOCEnvMapProbe::epAttachPosition );
	pCBPropertyNames->AddItem( "Attach rotation", NULL,
		( void* )( intptr_t )gdeOCEnvMapProbe::epAttachRotation );
	
	helper.ComboBoxFilter( groupBox, "Target:", true, "Object class property to target",
		pCBPropertyNameTarget, new cComboPropertyNameTarget( *this ) );
	pCBPropertyNameTarget->SetEditable( true );
	pCBPropertyNameTarget->SetDefaultSorter();
	pCBPropertyNameTarget->SetFilterCaseInsentive( true );
}

gdeWPSOCEnvMapProbe::~gdeWPSOCEnvMapProbe(){
	SetGameDefinition( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void gdeWPSOCEnvMapProbe::SetGameDefinition( gdeGameDefinition *gameDefinition ){
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
	UpdateEnvMapProbe();
}



gdeObjectClass *gdeWPSOCEnvMapProbe::GetObjectClass() const{
	return pGameDefinition ? pGameDefinition->GetActiveObjectClass() : NULL;
}

gdeOCEnvMapProbe *gdeWPSOCEnvMapProbe::GetEnvMapProbe() const{
	const gdeObjectClass * const objectClass = GetObjectClass();
	return objectClass ? pGameDefinition->GetActiveOCEnvMapProbe() : NULL;
}

const gdeOCEnvMapProbe::eProperties gdeWPSOCEnvMapProbe::GetPropertyName() const{
	return ( gdeOCEnvMapProbe::eProperties )( intptr_t )pCBPropertyNames->GetSelectedItem()->GetData();
}



void gdeWPSOCEnvMapProbe::UpdatePropertyList(){
	const gdeObjectClass * const objectClass = GetObjectClass();
	
	const decString selection( pCBPropertyNameTarget->GetText() );
	pCBPropertyNameTarget->RemoveAllItems();
	
	if( objectClass ){
		const gdePropertyList &properties = objectClass->GetProperties();
		const int count = properties.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			pCBPropertyNameTarget->AddItem( properties.GetAt( i )->GetName() );
		}
	}
	
	pCBPropertyNameTarget->SortItems();
	pCBPropertyNameTarget->SetText( selection );
}

void gdeWPSOCEnvMapProbe::UpdateEnvMapProbe(){
	const gdeOCEnvMapProbe * const envprobe = GetEnvMapProbe();
	
	if( envprobe ){
		igdeCodecPropertyString codec;
		decString encoded;
		
		pEditPosition->SetVector( envprobe->GetPosition() );
		pEditRotation->SetVector( envprobe->GetRotation() );
		pEditScaling->SetVector( envprobe->GetScaling() );
		
		codec.EncodeShapeList( envprobe->GetShapeListInfluence(), encoded );
		pEditShapeInfluence->SetText( encoded );
		
		decShapeList shapeList;
		if( envprobe->GetShapeReflection() ){
			shapeList.Add( envprobe->GetShapeReflection()->Copy() );
		}
		codec.EncodeShapeList( shapeList, encoded );
		pEditShapeReflection->SetText( encoded );
		
		codec.EncodeShapeList( envprobe->GetShapeListReflectionMask(), encoded );
		pEditShapeReflectionMask->SetText( encoded );
		
		pEditInfluenceBorderSize->SetFloat( envprobe->GetInfluenceBorderSize() );
		pEditInfluencePriority->SetInteger( envprobe->GetInfluencePriority() );
		
	}else{
		pEditPosition->SetVector( decVector() );
		pEditRotation->SetVector( decVector() );
		pEditScaling->SetVector( decVector() );
		pEditShapeInfluence->ClearText();
		pEditShapeReflection->ClearText();
		pEditShapeReflectionMask->ClearText();
		pEditInfluenceBorderSize->ClearText();
		pEditInfluencePriority->ClearText();
	}
	
	const bool enabled = envprobe;
	pEditPosition->SetEnabled( enabled );
	pEditRotation->SetEnabled( enabled );
	pEditScaling->SetEnabled( enabled );
	pEditShapeInfluence->SetEnabled( enabled );
	pEditShapeReflection->SetEnabled( enabled );
	pEditShapeReflectionMask->SetEnabled( enabled );
	pEditInfluenceBorderSize->SetEnabled( enabled );
	pEditInfluencePriority->SetEnabled( enabled );
	
	UpdatePropertyName();
}

void gdeWPSOCEnvMapProbe::UpdatePropertyName(){
	const gdeOCEnvMapProbe * const envprobe = GetEnvMapProbe();
	
	if( envprobe ){
		pCBPropertyNameTarget->SetText( envprobe->GetPropertyName( GetPropertyName() ) );
		
	}else{
		pCBPropertyNameTarget->ClearText();
	}
	
	pCBPropertyNameTarget->SetEnabled( envprobe );
}
