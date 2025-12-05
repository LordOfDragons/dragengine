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

#include "gdeDefaultPropertyValue.h"
#include "../../gamedef/property/gdeProperty.h"

#include <deigde/gamedefinition/property/igdeGDProperty.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/composed/igdeEditPropertyValue.h>
#include <deigde/gui/composed/igdeEditPropertyValueListener.h>

#include <dragengine/deEngine.h>



// Class gdeDefaultPropertyValue
//////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeDefaultPropertyValue::gdeDefaultPropertyValue( igdeUIHelper &helper ) :
igdeEditPropertyValue( helper ),
pGDProperty( NULL )
{
	pGDProperty = new igdeGDProperty( "Property" );
}

gdeDefaultPropertyValue::~gdeDefaultPropertyValue(){
	if( pGDProperty ){
		pGDProperty->FreeReference();
	}
}



// Management
///////////////

void gdeDefaultPropertyValue::CreateAndAdd( igdeContainer &form, igdeUIHelper &helper,
const char *label, const char *description, igdeEditPropertyValue::Ref &widget,
igdeEditPropertyValueListener *listener ){
	helper.Label( form, label, description, igdeLabel::eaLeft | igdeLabel::eaMiddle );
	widget.TakeOver( new gdeDefaultPropertyValue( helper ) );
	if( listener ){
		widget->AddListener( listener );
		listener->FreeReference();
	}
	form.AddChild( widget );
}

void gdeDefaultPropertyValue::CreateAndAdd( igdeContainer &parent, igdeUIHelper &helper,
igdeEditPropertyValue::Ref &widget, igdeEditPropertyValueListener *listener ){
	widget.TakeOver( new gdeDefaultPropertyValue( helper ) );
	if( listener ){
		widget->AddListener( listener );
		listener->FreeReference();
	}
	parent.AddChild( widget );
}



void gdeDefaultPropertyValue::SetValue( const decString &value, const gdeProperty &property ){
	pGDProperty->SetName( property.GetName() );
	
	switch( property.GetType() ){
	case gdeProperty::eptString:
	default:
		pGDProperty->SetType( igdeGDProperty::eptString );
		break;
		
	case gdeProperty::eptInteger:
		pGDProperty->SetType( igdeGDProperty::eptInteger );
		break;
		
	case gdeProperty::eptPoint2:
		pGDProperty->SetType( igdeGDProperty::eptPoint2 );
		break;
		
	case gdeProperty::eptPoint3:
		pGDProperty->SetType( igdeGDProperty::eptPoint3 );
		break;
		
	case gdeProperty::eptFloat:
		pGDProperty->SetType( igdeGDProperty::eptFloat );
		break;
		
	case gdeProperty::eptVector2:
		pGDProperty->SetType( igdeGDProperty::eptVector2 );
		break;
		
	case gdeProperty::eptVector3:
		pGDProperty->SetType( igdeGDProperty::eptVector3 );
		break;
		
	case gdeProperty::eptColor:
		pGDProperty->SetType( igdeGDProperty::eptColor );
		break;
		
	case gdeProperty::eptBoolean:
		pGDProperty->SetType( igdeGDProperty::eptBoolean );
		break;
		
	case gdeProperty::eptPath:
		pGDProperty->SetType( igdeGDProperty::eptPath );
		pGDProperty->SetMinimumValue( property.GetMinimumValue() );
		pGDProperty->SetMaximumValue( property.GetMaximumValue() );
		break;
		
	case gdeProperty::eptRange:
		pGDProperty->SetType( igdeGDProperty::eptRange );
		break;
		
	case gdeProperty::eptSelect:
		pGDProperty->SetType( igdeGDProperty::eptSelect );
		pGDProperty->GetOptions() = property.GetOptions();
		break;
		
	case gdeProperty::eptList:
		pGDProperty->SetType( igdeGDProperty::eptList );
		break;
		
	case gdeProperty::eptTriggerExpression:
		pGDProperty->SetType( igdeGDProperty::eptTriggerExpression );
		break;
		
	case gdeProperty::eptTriggerTarget:
		pGDProperty->SetType( igdeGDProperty::eptTriggerTarget );
		break;
		
	case gdeProperty::eptShape:
		pGDProperty->SetType( igdeGDProperty::eptShape );
		break;
		
	case gdeProperty::eptShapeList:
		pGDProperty->SetType( igdeGDProperty::eptShapeList );
		break;
		
	case gdeProperty::eptIdentifier:
		pGDProperty->SetType( igdeGDProperty::eptIdentifier );
		//pGDProperty->SetIdentifiers();
		break;
	}
	
	switch( property.GetPathPatternType() ){
	case gdeProperty::epptAll:
	default:
		pGDProperty->SetPathPatternType( igdeGDProperty::epptAll );
		break;
		
	case gdeProperty::epptModel:
		pGDProperty->SetPathPatternType( igdeGDProperty::epptModel );
		break;
		
	case gdeProperty::epptSkin:
		pGDProperty->SetPathPatternType( igdeGDProperty::epptSkin );
		break;
		
	case gdeProperty::epptRig:
		pGDProperty->SetPathPatternType( igdeGDProperty::epptRig );
		break;
		
	case gdeProperty::epptAnimation:
		pGDProperty->SetPathPatternType( igdeGDProperty::epptAnimation );
		break;
		
	case gdeProperty::epptAnimator:
		pGDProperty->SetPathPatternType( igdeGDProperty::epptAnimator );
		break;
		
	case gdeProperty::epptImage:
		pGDProperty->SetPathPatternType( igdeGDProperty::epptImage );
		break;
		
	case gdeProperty::epptOcclusionMesh:
		pGDProperty->SetPathPatternType( igdeGDProperty::epptOcclusionMesh );
		break;
		
	case gdeProperty::epptNavigationSpace:
		pGDProperty->SetPathPatternType( igdeGDProperty::epptNavigationSpace );
		break;
		
	case gdeProperty::epptParticleEmitter:
		pGDProperty->SetPathPatternType( igdeGDProperty::epptParticleEmitter );
		break;
		
	case gdeProperty::epptSound:
		pGDProperty->SetPathPatternType( igdeGDProperty::epptSound );
		break;
		
	case gdeProperty::epptSynthesizer:
		pGDProperty->SetPathPatternType( igdeGDProperty::epptSynthesizer );
		break;
		
	case gdeProperty::epptVideo:
		pGDProperty->SetPathPatternType( igdeGDProperty::epptVideo );
		break;
		
	case gdeProperty::epptFont:
		pGDProperty->SetPathPatternType( igdeGDProperty::epptFont );
		break;
		
	case gdeProperty::epptSky:
		pGDProperty->SetPathPatternType( igdeGDProperty::epptSky );
		break;
		
	case gdeProperty::epptCamera:
		pGDProperty->SetPathPatternType( igdeGDProperty::epptCamera );
		break;
		
	case gdeProperty::epptWorld:
		pGDProperty->SetPathPatternType(igdeGDProperty::epptWorld);
		break;
		
	case gdeProperty::epptCustom:
		pGDProperty->SetPathPatternType( igdeGDProperty::epptCustom );
		break;
	}
	
	igdeEditPropertyValue::SetValue( value, pGDProperty );
}
