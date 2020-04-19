/* 
 * Drag[en]gine IGDE
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

#include "igdeEditPropertyValue.h"
#include "igdeEditPropertyValueListener.h"
#include "igdeEditPath.h"
#include "igdeEditPathListener.h"
#include "igdeEditPoint.h"
#include "igdeEditPointListener.h"
#include "igdeEditPoint3.h"
#include "igdeEditPoint3Listener.h"
#include "igdeEditSliderText.h"
#include "igdeEditSliderTextListener.h"
#include "igdeEditVector.h"
#include "igdeEditVectorListener.h"
#include "igdeEditVector2.h"
#include "igdeEditVector2Listener.h"
#include "../igdeCommonDialogs.h"
#include "../igdeUIHelper.h"
#include "../igdeContainerReference.h"
#include "../igdeComboBox.h"
#include "../igdeComboBoxFilter.h"
#include "../igdeTextField.h"
#include "../igdeButton.h"
#include "../igdeColorBox.h"
#include "../igdeCheckBox.h"
#include "../igdeSwitcher.h"
#include "../dialog/igdeDialogReference.h"
#include "../event/igdeAction.h"
#include "../event/igdeColorBoxListener.h"
#include "../event/igdeComboBoxListener.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerFlow.h"
#include "../trigexpredit/igdeTriggerExpressionDialog.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gamedefinition/property/igdeGDProperty.h"
#include "../../triggersystem/igdeTriggerTarget.h"
#include "../../triggersystem/igdeTriggerTargetList.h"

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

class igdeEditPropertyValue_TextField : public igdeTextFieldListener {
protected:
	igdeEditPropertyValue &pWidget;
	
public:
	igdeEditPropertyValue_TextField( igdeEditPropertyValue &widget ) : pWidget( widget ){ }
	
	virtual void OnTextChanged( igdeTextField* ){
		pWidget.EditWidgetValueChanged( false );
	}
};

class igdeEditPropertyValue_EditPoint : public igdeEditPointListener {
protected:
	igdeEditPropertyValue &pWidget;
	
public:
	igdeEditPropertyValue_EditPoint( igdeEditPropertyValue &widget ) : pWidget( widget ){ }
	
	virtual void OnPointChanged( igdeEditPoint* ){
		pWidget.EditWidgetValueChanged( false );
	}
};

class igdeEditPropertyValue_EditPoint3 : public igdeEditPoint3Listener {
protected:
	igdeEditPropertyValue &pWidget;
	
public:
	igdeEditPropertyValue_EditPoint3( igdeEditPropertyValue &widget ) : pWidget( widget ){ }
	
	virtual void OnPoint3Changed( igdeEditPoint3* ){
		pWidget.EditWidgetValueChanged( false );
	}
};

class igdeEditPropertyValue_EditVector : public igdeEditVectorListener {
protected:
	igdeEditPropertyValue &pWidget;
	
public:
	igdeEditPropertyValue_EditVector( igdeEditPropertyValue &widget ) : pWidget( widget ){ }
	
	virtual void OnVectorChanged( igdeEditVector* ){
		pWidget.EditWidgetValueChanged( false );
	}
};

class igdeEditPropertyValue_EditVector2 : public igdeEditVector2Listener {
protected:
	igdeEditPropertyValue &pWidget;
	
public:
	igdeEditPropertyValue_EditVector2( igdeEditPropertyValue &widget ) : pWidget( widget ){ }
	
	virtual void OnVector2Changed( igdeEditVector2* ){
		pWidget.EditWidgetValueChanged( false );
	}
};

class igdeEditPropertyValue_ColorBox : public igdeColorBoxListener {
protected:
	igdeEditPropertyValue &pWidget;
	
public:
	igdeEditPropertyValue_ColorBox( igdeEditPropertyValue &widget ) : pWidget( widget ){ }
	
	virtual void OnColorChanged( igdeColorBox* ){
		pWidget.EditWidgetValueChanged( false );
	}
};

class igdeEditPropertyValue_ActionBoolean : public igdeAction {
protected:
	igdeEditPropertyValue &pWidget;
	
public:
	igdeEditPropertyValue_ActionBoolean( igdeEditPropertyValue &widget ) : 
	igdeAction( "Value", "Boolean property value" ), pWidget( widget ){ }
	
	virtual void OnAction(){
		pWidget.EditWidgetValueChanged( false );
	}
	
	virtual void Update(){
		SetEnabled( pWidget.GetEnabled() );
		SetSelected( pWidget.GetValue() != "0" );
	}
};

class igdeEditPropertyValue_EditPath : public igdeEditPathListener {
protected:
	igdeEditPropertyValue &pWidget;
	
public:
	igdeEditPropertyValue_EditPath( igdeEditPropertyValue &widget ) : pWidget( widget ){ }
	
	virtual void OnEditPathChanged( igdeEditPath* ){
		pWidget.EditWidgetValueChanged( false );
	}
};

class igdeEditPropertyValue_EditSliderText : public igdeEditSliderTextListener {
protected:
	igdeEditPropertyValue &pWidget;
	
public:
	igdeEditPropertyValue_EditSliderText( igdeEditPropertyValue &widget ) : pWidget( widget ){ }
	
	virtual void OnSliderTextValueChanged( igdeEditSliderText* ){
		pWidget.EditWidgetValueChanged( false );
	}
	
	virtual void OnSliderTextValueChanging( igdeEditSliderText* ){
		pWidget.EditWidgetValueChanged( true );
	}
};

class igdeEditPropertyValue_ComboBox : public igdeComboBoxListener {
protected:
	igdeEditPropertyValue &pWidget;
	
public:
	igdeEditPropertyValue_ComboBox( igdeEditPropertyValue &widget ) : pWidget( widget ){ }
	
	virtual void OnTextChanged( igdeComboBox* ){
		pWidget.EditWidgetValueChanged( false );
	}
};


class igdeEditPropertyValue_ActionEditRawValue : public igdeAction {
protected:
	igdeEditPropertyValue &pWidget;
	
public:
	igdeEditPropertyValue_ActionEditRawValue( igdeEditPropertyValue &widget ) : 
	igdeAction( "!", "Edit raw property value" ), pWidget( widget ){ }
	
	virtual void OnAction(){
		decString value( pWidget.GetValue() );
		if( igdeCommonDialogs::GetMultilineString( &pWidget, "Edit Raw Property Value",
			"Raw property value. Values entered here can violate the\n"
			"property type so be careful what you enter", value ) ){
				pWidget.SetValue( value, pWidget.GetGDProperty() );
				pWidget.EditWidgetValueChanged( false );
		}
	}
	
	virtual void Update(){
		SetEnabled( pWidget.GetEnabled() );
	}
};

class igdeEditPropertyValue_ActionEditList : public igdeAction {
protected:
	igdeEditPropertyValue &pWidget;
	
public:
	igdeEditPropertyValue_ActionEditList( igdeEditPropertyValue &widget ) :
		igdeAction( "...", "Edit list" ), pWidget( widget ){ }
	
	virtual void OnAction(){
		pWidget.GetActionEditRawValue()->OnAction();
	}
	
	virtual void Update(){
		SetEnabled( pWidget.GetEnabled() );
	}
};

class igdeEditPropertyValue_ActionEditTriggerExpression : public igdeAction {
protected:
	igdeEditPropertyValue &pWidget;
	
public:
	igdeEditPropertyValue_ActionEditTriggerExpression( igdeEditPropertyValue &widget ) :
		igdeAction( "...", "Edit trigger expression" ), pWidget( widget ){ }
	
	virtual void OnAction(){
		igdeTriggerTargetList *triggerTargetList = pWidget.GetTriggerTargets();
		if( ! triggerTargetList ){
			static igdeTriggerTargetList dummyList;
			dummyList.RemoveAll();
			triggerTargetList = &dummyList;
		}
		
		igdeDialogReference refDialog;
		refDialog.TakeOver( new igdeTriggerExpressionDialog( pWidget.GetEnvironment(),
			*triggerTargetList, pWidget.GetTriggerExpressionParser() ) );
		igdeTriggerExpressionDialog &dialog = ( igdeTriggerExpressionDialog& )( igdeDialog& )refDialog;
		
		dialog.SetExpression( pWidget.GetValue() );
		if( ! dialog.Run( &pWidget ) || dialog.GetExpression() == pWidget.GetValue() ){
			return;
		}
		
		pWidget.SetValue( dialog.GetExpression(), pWidget.GetGDProperty() );
		pWidget.EditWidgetValueChanged( false );
	}
	
	virtual void Update(){
		SetEnabled( pWidget.GetEnabled() );
	}
};

class igdeEditPropertyValue_ActionEditShape : public igdeAction {
protected:
	igdeEditPropertyValue &pWidget;
	
public:
	igdeEditPropertyValue_ActionEditShape( igdeEditPropertyValue &widget ) :
		igdeAction( "...", "Edit shape" ), pWidget( widget ){ }
	
	virtual void OnAction(){
		pWidget.GetActionEditRawValue()->OnAction();
	}
	
	virtual void Update(){
		SetEnabled( pWidget.GetEnabled() );
	}
};

class igdeEditPropertyValue_ActionEditShapeList : public igdeAction {
protected:
	igdeEditPropertyValue &pWidget;
	
public:
	igdeEditPropertyValue_ActionEditShapeList( igdeEditPropertyValue &widget ) :
		igdeAction( "...", "Edit shape list" ), pWidget( widget ){ }
	
	virtual void OnAction(){
		pWidget.GetActionEditRawValue()->OnAction();
	}
	
	virtual void Update(){
		SetEnabled( pWidget.GetEnabled() );
	}
};



// Class igdeEditPropertyValue
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditPropertyValue::igdeEditPropertyValue( igdeUIHelper &helper ) :
igdeContainerFlow( helper.GetEnvironment(), eaX, esFirst ),
pGDProperty( NULL ),
pEnabled( true ),
pPreventEditing( false ),
pTriggerTargets( NULL )
{
	pCreateContent( helper );
}

igdeEditPropertyValue::~igdeEditPropertyValue(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeEditPropertyValue::SetValue( const char *value, const igdeGDProperty *gdProperty ){
	if( value == pValue && gdProperty == pGDProperty ){
		return;
	}
	
	pValue = value;
	pGDProperty = gdProperty;
	pUpdateEditWidgets();
}

void igdeEditPropertyValue::ClearValue(){
	if( pValue.IsEmpty() ){
		return;
	}
	
	pValue.Empty();
	pGDProperty = NULL;
	pUpdateEditWidgets();
}



void igdeEditPropertyValue::SetEnabled( bool enabled ){
	if( enabled == pEnabled ){
		return;
	}
	
	pEnabled = enabled;
	
	pString->SetEnabled( enabled );
	pInteger->SetEnabled( enabled );
	pPoint->SetEnabled( enabled );
	pPoint3->SetEnabled( enabled );
	pFloat->SetEnabled( enabled );
	pVector->SetEnabled( enabled );
	pVector2->SetEnabled( enabled );
	pColor->SetEnabled( enabled );
	pBoolean->SetEnabled( enabled );
	pPath->SetEnabled( enabled );
	pRange->SetEnabled( enabled );
	pSelect->SetEnabled( enabled );
	pList->SetEnabled( enabled );
	pTriggerExpression->SetEnabled( enabled );
	pTriggerTarget->SetEnabled( enabled );
	pShape->SetEnabled( enabled );
	pShapeList->SetEnabled( enabled );
	pIdentifier->SetEnabled( enabled );
	
	pActionEditRawValue->Update();
	pActionBooleanValue->Update();
	pActionEditList->Update();
	pActionEditTriggerExpression->Update();
	pActionEditShape->Update();
	pActionEditShapeList->Update();
}

void igdeEditPropertyValue::Focus(){
	if( ! pGDProperty ){
		pString->Focus();
		return;
	}
	
	switch( pGDProperty->GetType() ){
	case igdeGDProperty::eptString:
	default:
		pString->Focus();
		break;
		
	case igdeGDProperty::eptInteger:
		pInteger->Focus();
		break;
		
	case igdeGDProperty::eptPoint2:
		pPoint->Focus();
		break;
		
	case igdeGDProperty::eptPoint3:
		pPoint3->Focus();
		break;
		
	case igdeGDProperty::eptFloat:
		pFloat->Focus();
		break;
		
	case igdeGDProperty::eptVector2:
		pVector2->Focus();
		break;
		
	case igdeGDProperty::eptVector3:
		pVector->Focus();
		break;
		
	case igdeGDProperty::eptColor:
		pColor->Focus();
		break;
		
	case igdeGDProperty::eptBoolean:
		pBoolean->Focus();
		break;
		
	case igdeGDProperty::eptPath:
		pPath->Focus();
		break;
		
	case igdeGDProperty::eptRange:
		pRange->Focus();
		break;
		
	case igdeGDProperty::eptSelect:
		pSelect->Focus();
		break;
		
	case igdeGDProperty::eptList:
		pList->Focus();
		break;
		
	case igdeGDProperty::eptTriggerExpression:
		pTriggerExpression->Focus();
		break;
		
	case igdeGDProperty::eptTriggerTarget:
		pTriggerTarget->Focus();
		break;
		
	case igdeGDProperty::eptShape:
		pShape->Focus();
		break;
		
	case igdeGDProperty::eptShapeList:
		pShapeList->Focus();
		break;
		
	case igdeGDProperty::eptIdentifier:
		pIdentifier->Focus();
		break;
	}
}



void igdeEditPropertyValue::SetIdentifiers( const decStringSet &identifiers ){
	const decString selection( pIdentifier->GetText() );
	
	pIdentifier->RemoveAllItems();
	
	pIdentifiers = identifiers;
	
	const int count = identifiers.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pIdentifier->AddItem( identifiers.GetAt( i ) );
	}
	pIdentifier->SortItems();
	pIdentifier->StoreFilterItems();
	
	pIdentifier->SetText( selection );
}

void igdeEditPropertyValue::SetTriggerTargets( igdeTriggerTargetList *triggerTargets ){
	const decString selection( pTriggerTarget->GetText() );
	
	pTriggerTarget->RemoveAllItems();
	
	pTriggerTargets = triggerTargets;
	
	if( triggerTargets ){
		const int count = triggerTargets->GetCount();
		int i;
		for( i=0; i<count; i++ ){
			pTriggerTarget->AddItem( triggerTargets->GetAt( i )->GetName() );
		}
		pTriggerTarget->SortItems();
		pTriggerTarget->StoreFilterItems();
	}
	
	pTriggerTarget->SetText( selection );
}



void igdeEditPropertyValue::AddListener( igdeEditPropertyValueListener* listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeEditPropertyValue::RemoveListener( igdeEditPropertyValueListener* listener ){
	pListeners.Remove( listener );
}

void igdeEditPropertyValue::NotifyPropertyValueChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeEditPropertyValueListener* )listeners.GetAt( i ) )->OnPropertyValueChanged( this );
	}
}

void igdeEditPropertyValue::NotifyPropertyValueChanging(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeEditPropertyValueListener* )listeners.GetAt( i ) )->OnPropertyValueChanging( this );
	}
}



void igdeEditPropertyValue::EditWidgetValueChanged( bool changing ){
	if( pPreventEditing ){
		return;
	}
	
	if( ! pGDProperty ){
		if( pValue == pString->GetText() ){
			return;
		}
		pValue = pString->GetText();
		if( changing ){
			NotifyPropertyValueChanging();
			
		}else{
			NotifyPropertyValueChanged();
		}
		return;
	}
	
	decString newValue;
	
	switch( pGDProperty->GetType() ){
	case igdeGDProperty::eptString:
	default:
		newValue = pString->GetText();
		break;
		
	case igdeGDProperty::eptInteger:
		newValue.Format( "%d", pInteger->GetInteger() );
		break;
		
	case igdeGDProperty::eptPoint2:
		newValue.Format( "%d %d", pPoint->GetPoint().x, pPoint->GetPoint().y );
		break;
		
	case igdeGDProperty::eptPoint3:
		newValue.Format( "%d %d %d", pPoint3->GetPoint3().x, pPoint3->GetPoint3().y, pPoint3->GetPoint3().z );
		break;
		
	case igdeGDProperty::eptFloat:
		newValue.Format( "%g", pFloat->GetFloat() );
		break;
		
	case igdeGDProperty::eptVector2:
		pCodec.EncodeVector2( pVector2->GetVector2(), newValue );
		break;
		
	case igdeGDProperty::eptVector3:
		pCodec.EncodeVector( pVector->GetVector(), newValue );
		break;
		
	case igdeGDProperty::eptColor:
		pCodec.EncodeColor3( pColor->GetColor(), newValue );
		break;
		
	case igdeGDProperty::eptBoolean:
		newValue = pBoolean->GetChecked() ? "1" : "0";
		break;
		
	case igdeGDProperty::eptPath:
		newValue = pPath->GetPath();
		break;
		
	case igdeGDProperty::eptRange:
		newValue.Format( "%g", pRange->GetValue() );
		break;
		
	case igdeGDProperty::eptSelect:
		newValue = pSelect->GetText();
		break;
		
	case igdeGDProperty::eptList:
		newValue = pList->GetText();
		break;
		
	case igdeGDProperty::eptTriggerExpression:
		newValue = pTriggerExpression->GetText();
		break;
		
	case igdeGDProperty::eptTriggerTarget:
		newValue = pTriggerTarget->GetText();
		break;
		
	case igdeGDProperty::eptShape:
		newValue = pShape->GetText();
		break;
		
	case igdeGDProperty::eptShapeList:
		newValue = pShapeList->GetText();
		break;
		
	case igdeGDProperty::eptIdentifier:
		newValue = pIdentifier->GetText();
		break;
	}
	
	pValue = newValue;
	
	if( changing ){
		NotifyPropertyValueChanging();
		
	}else{
		NotifyPropertyValueChanged();
	}
}



// Private Functions
//////////////////////

void igdeEditPropertyValue::pCreateContent( igdeUIHelper &helper ){
	igdeEnvironment &env = helper.GetEnvironment();
	igdeContainerReference frameLine;
	
	pActionEditRawValue.TakeOver( new igdeEditPropertyValue_ActionEditRawValue( *this ) );
	pActionBooleanValue.TakeOver( new igdeEditPropertyValue_ActionBoolean( *this ) );
	pActionEditList.TakeOver( new igdeEditPropertyValue_ActionEditList( *this ) );
	pActionEditTriggerExpression.TakeOver( new igdeEditPropertyValue_ActionEditTriggerExpression( *this ) );
	pActionEditShape.TakeOver( new igdeEditPropertyValue_ActionEditShape( *this ) );
	pActionEditShapeList.TakeOver( new igdeEditPropertyValue_ActionEditShapeList( *this ) );
	
	pSwitcher.TakeOver( new igdeSwitcher( env ) );
	AddChild( pSwitcher );
	helper.Button( *this, pActionEditRawValue );
	
	helper.EditString( pSwitcher, "Property value", pString,
		new igdeEditPropertyValue_TextField( *this ) );
	
	helper.EditInteger( pSwitcher, "Integer property value", pInteger,
		new igdeEditPropertyValue_TextField( *this ) );
	
	helper.EditPoint( pSwitcher, "Point property value", pPoint,
		new igdeEditPropertyValue_EditPoint( *this ) );
	
	helper.EditPoint3( pSwitcher, "3-Component point property value", pPoint3,
		new igdeEditPropertyValue_EditPoint3( *this ) );
	
	helper.EditFloat( pSwitcher, "Floating point property value", pFloat,
		new igdeEditPropertyValue_TextField( *this ) );
	
	helper.EditVector2( pSwitcher, "2-Component vector property value", pVector2,
		new igdeEditPropertyValue_EditVector2( *this ) );
	
	helper.EditVector( pSwitcher, "Vector property value", pVector,
		new igdeEditPropertyValue_EditVector( *this ) );
	
	helper.ColorBox( pSwitcher, "Color property value", pColor,
		new igdeEditPropertyValue_ColorBox( *this ) );
	
	helper.CheckBoxOnly( pSwitcher, pBoolean, pActionBooleanValue );
	
	helper.EditPath( pSwitcher, "Path property value", igdeEnvironment::efpltAll, pPath,
		new igdeEditPropertyValue_EditPath( *this ) );
	
	helper.EditSliderText( pSwitcher, "Range property value", 0.0f, 0.0f, 6, 3, 1.0f, pRange,
		new igdeEditPropertyValue_EditSliderText( *this ) );
	
	helper.ComboBox( pSwitcher, "Selection property value", pSelect,
		new igdeEditPropertyValue_ComboBox( *this ) );
	
	frameLine.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst ) );
	helper.EditString( frameLine, "List property value", pList,
		new igdeEditPropertyValue_TextField( *this ) );
	helper.Button( frameLine, pActionEditList );
	pSwitcher->AddChild( frameLine );
	
	frameLine.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst ) );
	helper.EditString( frameLine, "Trigger expression property value", pTriggerExpression,
		new igdeEditPropertyValue_TextField( *this ) );
	helper.Button( frameLine, pActionEditTriggerExpression );
	pSwitcher->AddChild( frameLine );
	
	helper.ComboBoxFilter( pSwitcher, true, "Trigger target property value", pTriggerTarget,
		new igdeEditPropertyValue_ComboBox( *this ) );
	pTriggerTarget->SetDefaultSorter();
	
	frameLine.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst ) );
	helper.EditString( frameLine, "Shape property value", pShape,
		new igdeEditPropertyValue_TextField( *this ) );
	helper.Button( frameLine, pActionEditShape );
	pSwitcher->AddChild( frameLine );
	
	frameLine.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst ) );
	helper.EditString( frameLine, "Shape list property value", pShapeList,
		new igdeEditPropertyValue_TextField( *this ) );
	helper.Button( frameLine, pActionEditShapeList );
	pSwitcher->AddChild( frameLine );
	
	helper.ComboBoxFilter( pSwitcher, true, "Identifier property value", pIdentifier,
		new igdeEditPropertyValue_ComboBox( *this ) );
	pIdentifier->SetDefaultSorter();
}



void igdeEditPropertyValue::pUpdateEditWidgets(){
	if( ! pGDProperty ){
		pSwitcher->SetCurrent( 0 );
		pString->SetText( pValue );
		return;
	}
	
	pPreventEditing = true;
	
	switch( pGDProperty->GetType() ){
	case igdeGDProperty::eptString:
	default:
		pSwitcher->SetCurrent( 0 );
		pString->SetText( pValue );
		break;
		
	case igdeGDProperty::eptInteger:
		pSwitcher->SetCurrent( 1 );
		pInteger->SetInteger( pValue.ToInt() );
		break;
		
	case igdeGDProperty::eptPoint2:{
		pSwitcher->SetCurrent( 2 );
		int values[ 2 ] = { 0, 0 };
		pCodec.DecodeFixedIntList( pValue, values, 2 );
		pPoint->SetPoint( decPoint( values[ 0 ], values[ 1 ] ) );
		}break;
		
	case igdeGDProperty::eptPoint3:{
		pSwitcher->SetCurrent( 3 );
		int values[ 3 ] = { 0, 0, 0 };
		pCodec.DecodeFixedIntList( pValue, values, 3 );
		pPoint3->SetPoint3( decPoint3( values[ 0 ], values[ 1 ], values[ 2 ] ) );
		}break;
		
	case igdeGDProperty::eptFloat:
		pSwitcher->SetCurrent( 4 );
		pFloat->SetFloat( pValue.ToFloat() );
		break;
		
	case igdeGDProperty::eptVector2:{
		pSwitcher->SetCurrent( 5 );
		float values[ 2 ] = { 0.0f, 0.0f };
		pCodec.DecodeFixedFloatList( pValue, values, 2 );
		pVector2->SetVector2( decVector2( values[ 0 ], values[ 1 ] ) );
		}break;
		
	case igdeGDProperty::eptVector3:{
		pSwitcher->SetCurrent( 6 );
		float values[ 3 ] = { 0.0f, 0.0f, 0.0f };
		pCodec.DecodeFixedFloatList( pValue, values, 3 );
		pVector->SetVector( decVector( values[ 0 ], values[ 1 ], values[ 2 ] ) );
		}break;
		
	case igdeGDProperty::eptColor:{
		pSwitcher->SetCurrent( 7 );
		float values[ 3 ] = { 0.0f, 0.0f, 0.0f };
		pCodec.DecodeFixedFloatList( pValue, values, 3 );
		pColor->SetColor( decColor( values[ 0 ], values[ 1 ], values[ 2 ] ) );
		}break;
		
	case igdeGDProperty::eptBoolean:
		pSwitcher->SetCurrent( 8 );
		pBoolean->SetChecked( pValue != "0" );
		break;
		
	case igdeGDProperty::eptPath:
		pSwitcher->SetCurrent( 9 );
		
		switch( pGDProperty->GetPathPatternType() ){
		case igdeGDProperty::epptAll:
			pPath->SetResourceType( igdeEnvironment::efpltAll );
			break;
			
		case igdeGDProperty::epptModel:
		case igdeGDProperty::epptAudioModel:
			pPath->SetResourceType( igdeEnvironment::efpltModel );
			break;
			
		case igdeGDProperty::epptSkin:
			pPath->SetResourceType( igdeEnvironment::efpltSkin );
			break;
			
		case igdeGDProperty::epptRig:
			pPath->SetResourceType( igdeEnvironment::efpltRig );
			break;
			
		case igdeGDProperty::epptAnimation:
			pPath->SetResourceType( igdeEnvironment::efpltAnimation );
			break;
			
		case igdeGDProperty::epptAnimator:
			pPath->SetResourceType( igdeEnvironment::efpltAnimator );
			break;
			
		case igdeGDProperty::epptImage:
			pPath->SetResourceType( igdeEnvironment::efpltImage );
			break;
			
		case igdeGDProperty::epptOcclusionMesh:
			pPath->SetResourceType( igdeEnvironment::efpltOcclusionMesh );
			break;
			
		case igdeGDProperty::epptNavigationSpace:
			pPath->SetResourceType( igdeEnvironment::efpltNavigationSpace );
			break;
			
		case igdeGDProperty::epptParticleEmitter:
			pPath->SetResourceType( igdeEnvironment::efpltParticleEmitter );
			break;
			
		case igdeGDProperty::epptSound:
			pPath->SetResourceType( igdeEnvironment::efpltSound );
			break;
			
		case igdeGDProperty::epptSynthesizer:
			pPath->SetResourceType( igdeEnvironment::efpltSynthesizer );
			break;
			
		case igdeGDProperty::epptVideo:
			pPath->SetResourceType( igdeEnvironment::efpltVideo );
			break;
			
		case igdeGDProperty::epptFont:
			pPath->SetResourceType( igdeEnvironment::efpltFont );
			break;
			
		case igdeGDProperty::epptSky:
			pPath->SetResourceType( igdeEnvironment::efpltSky );
			break;
			
		case igdeGDProperty::epptCustom:
		default:
			pPath->SetResourceType( igdeEnvironment::efpltAll );
			break;
		}
		
		pPath->SetPath( pValue );
		break;
		
	case igdeGDProperty::eptRange:
		pSwitcher->SetCurrent( 10 );
		pRange->SetRange( pGDProperty->GetMinimumValue(), pGDProperty->GetMaximumValue() );
		pRange->SetTickSpacing( ( pGDProperty->GetMaximumValue() - pGDProperty->GetMinimumValue() ) * 0.1f );
		pRange->SetValue( pValue.ToFloat() );
		break;
		
	case igdeGDProperty::eptSelect:{
		pSwitcher->SetCurrent( 11 );
		
		const decStringList &options = pGDProperty->GetOptions();
		const int count = options.GetCount();
		int i;
		
		pSelect->RemoveAllItems();
		for( i=0; i<count; i++ ){
			pSelect->AddItem( options.GetAt( i ) );
		}
		pSelect->SetText( pValue );
		}break;
		
	case igdeGDProperty::eptList:
		pSwitcher->SetCurrent( 12 );
		pList->SetText( pValue );
		break;
		
	case igdeGDProperty::eptTriggerExpression:
		pSwitcher->SetCurrent( 13 );
		pTriggerExpression->SetText( pValue );
		break;
		
	case igdeGDProperty::eptTriggerTarget:
		pSwitcher->SetCurrent( 14 );
		pTriggerTarget->SetText( pValue );
		break;
		
	case igdeGDProperty::eptShape:
		pSwitcher->SetCurrent( 15 );
		pShape->SetText( pValue );
		break;
		
	case igdeGDProperty::eptShapeList:
		pSwitcher->SetCurrent( 16 );
		pShapeList->SetText( pValue );
		break;
		
	case igdeGDProperty::eptIdentifier:
		pSwitcher->SetCurrent( 17 );
		pIdentifier->SetText( pValue );
		break;
	}
	
	pPreventEditing = false;
}

/*
void igdeEditPropertyValue::pMinifyFloat( decString &value ) const{
	const int len = value.GetLength();
	int cut;
	
	for( cut=len-1; cut>=0; cut-- ){
		if( value[ cut ] != '0' ){
			break;
		}
	}
	
	if( cut >= 0 && value[ cut ] == '.' ){
		cut--;
	}
	
	value = value.GetLeft( cut + 1 );
}
*/
