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

#include "peeWindowCurves.h"
#include "peeWindowCurvesListener.h"
#include "../peeWindowMain.h"
#include "../../emitter/peeEmitter.h"
#include "../../emitter/peeType.h"
#include "../../emitter/peeParameter.h"
#include "../../undosys/parameter/peeUParameterSetCurveBeam.h"
#include "../../undosys/parameter/peeUParameterSetCurveProgress.h"
#include "../../undosys/parameter/peeUParameterSetCurveSpread.h"
#include "../../undosys/parameter/peeUParameterSetCurveValue.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Events
///////////

namespace{

class cListCurves : public igdeListBoxListener{
	peeWindowCurves &pPanel;
public:
	cListCurves( peeWindowCurves &panel ) : pPanel( panel ){ }
	
	virtual void OnSelectionChanged( igdeListBox* ){
		if( pPanel.GetParameter() ){
			pPanel.DropProgressiveUndo();
			pPanel.UpdateCurve();
		}
	}
};

class cViewCurve : public igdeViewCurveBezierListener{
	peeWindowCurves &pPanel;
	igdeUndo::Ref &pUndo;
public:
	cViewCurve( peeWindowCurves &panel, igdeUndo::Ref &undo ) :
	pPanel( panel ),
	pUndo( undo ){
	}
	
	virtual void OnCurveChanged( igdeViewCurveBezier *view ){
		OnCurveChanging( view );
		pUndo = NULL;
	}
	
	virtual void OnCurveChanging( igdeViewCurveBezier *view ){
		peeType * const type = pPanel.GetType();
		peeParameter * const parameter = pPanel.GetParameter();
		if( ! type || ! parameter ){
			pUndo = NULL;
			return;
		}
		
		if( pUndo ){
			( ( peeUParameterSetCurve& )( igdeUndo& )pUndo ).SetNewCurve( view->GetCurve() );
			pUndo->Redo();
			return;
		}
		
		switch( pPanel.GetCurve() ){
		case peeWindowCurves::ecValue:
			pUndo.TakeOver( new peeUParameterSetCurveValue( type, parameter, view->GetCurve() ) );
			break;
			
		case peeWindowCurves::ecSpread:
			pUndo.TakeOver( new peeUParameterSetCurveSpread( type, parameter, view->GetCurve() ) );
			break;
			
		case peeWindowCurves::ecBeam:
			pUndo.TakeOver( new peeUParameterSetCurveBeam( type, parameter, view->GetCurve() ) );
			break;
			
		case peeWindowCurves::ecProgress:
		default:
			pUndo.TakeOver( new peeUParameterSetCurveProgress( type, parameter, view->GetCurve() ) );
			break;
		}
		
		pPanel.GetEmitter()->GetUndoSystem()->Add( pUndo );
	}
};

}



// Class peeWindowCurves
//////////////////////////

// Constructor, destructor
////////////////////////////

peeWindowCurves::peeWindowCurves( peeWindowMain &windowMain ) :
igdeContainerSplitted(windowMain.GetEnvironment(), igdeContainerSplitted::espRight,
	igdeApplication::app().DisplayScaled(200)),
pWindowMain( windowMain ),
pListener( NULL ),
pEmitter( NULL )
{
	igdeEnvironment &env = windowMain.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pIconCurveEmpty = env.GetStockIcon( igdeEnvironment::esiSmallMinus );
	pIconCurveUsed = env.GetStockIcon( igdeEnvironment::esiSmallPlus );
	
	pListener = new peeWindowCurvesListener( *this );
	
	helper.ListBox( 4, "Curve to edit", pListCurves, new cListCurves( *this ) );
	pListCurves->AddItem( "Value", pIconCurveEmpty, ( void* )( intptr_t )ecValue );
	pListCurves->AddItem( "Spread", pIconCurveEmpty, ( void* )( intptr_t )ecSpread );
	pListCurves->AddItem( "Progress", pIconCurveEmpty, ( void* )( intptr_t )ecProgress );
	pListCurves->AddItem( "Beam", pIconCurveEmpty, ( void* )( intptr_t )ecBeam );
	AddChild( pListCurves, igdeContainerSplitted::eaSide );
	
	helper.ViewCurveBezier( pEditCurve, new cViewCurve( *this, pUndoSetCurve ) );
	pEditCurve->SetClampMin( decVector2( 0.0f, 0.0f ) );
	pEditCurve->SetClampMax( decVector2( 1.0f, 1.0f ) );
	pEditCurve->SetClamp( true );
	AddChild( pEditCurve, igdeContainerSplitted::eaCenter );
}

peeWindowCurves::~peeWindowCurves(){
	SetEmitter( NULL );
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void peeWindowCurves::SetEmitter( peeEmitter *emitter ){
	if( emitter == pEmitter ){
		return;
	}
	
	pUndoSetCurve = NULL;
	
	if( pEmitter ){
		pEmitter->RemoveListener( pListener );
		pEmitter->FreeReference();
	}
	
	pEmitter = emitter;
	
	if( emitter ){
		emitter->AddListener( pListener );
		emitter->AddReference();
	}
	
	UpdateCurve();
}



peeType *peeWindowCurves::GetType() const{
	if( pEmitter ){
		return pEmitter->GetActiveType();
	}
	
	return NULL;
}

peeParameter *peeWindowCurves::GetParameter() const{
	peeType * const activeType = GetType();
	
	if( activeType ){
		return activeType->GetActiveParameter();
	}
	
	return NULL;
}

peeWindowCurves::eCurves peeWindowCurves::GetCurve() const{
	const igdeListItem * const selection = pListCurves->GetSelectedItem();
	if( selection ){
		return ( eCurves )( intptr_t )selection->GetData();
		
	}else{
		return ecProgress;
	}
}

void peeWindowCurves::UpdateCurve(){
	const peeParameter * const parameter = GetParameter();
	
	if( parameter ){
		switch( GetCurve() ){
		case ecValue:
			pEditCurve->SetCurve( parameter->GetCurveValue() );
			pEditCurve->SetEnabled( true );
			break;
			
		case ecSpread:
			pEditCurve->SetCurve( parameter->GetCurveSpread() );
			pEditCurve->SetEnabled( true );
			break;
			
		case ecProgress:
			pEditCurve->SetCurve( parameter->GetCurveProgress() );
			pEditCurve->SetEnabled( true );
			break;
			
		case ecBeam:
			pEditCurve->SetCurve( parameter->GetCurveBeam() );
			pEditCurve->SetEnabled( true );
			break;
			
		default:
			pEditCurve->SetDefaultBezier();
			pEditCurve->SetEnabled( false );
		}
		
	}else{
		pEditCurve->SetDefaultBezier();
		pEditCurve->SetEnabled( false );
	}
	
	pUpdateCurveListIcons();
}

void peeWindowCurves::DropProgressiveUndo(){
	pUndoSetCurve = NULL;
}



// Private Functions
////////////////////////

void peeWindowCurves::pUpdateCurveListIcons(){
	const peeParameter * const parameter = GetParameter();
	const int count = pListCurves->GetItemCount();
	int i;
	
	if( parameter ){
		for( i=0; i<count; i++ ){
			igdeListItem &item = *pListCurves->GetItemAt( i );
			int pointCount;
			
			switch( ( eCurves )( intptr_t )item.GetData() ){
			case ecValue:
				pointCount = parameter->GetCurveValue().GetPointCount();
				break;
				
			case ecSpread:
				pointCount = parameter->GetCurveSpread().GetPointCount();
				break;
				
			case ecProgress:
				pointCount = parameter->GetCurveProgress().GetPointCount();
				break;
				
			case ecBeam:
				pointCount = parameter->GetCurveBeam().GetPointCount();
				break;
				
			default:
				pointCount = 0;
			}
			
			item.SetIcon( pointCount > 0 ? pIconCurveUsed : pIconCurveEmpty );
			pListCurves->ItemChangedAt( i );
		}
		
	}else{
		for( i=0; i<count; i++ ){
			pListCurves->GetItemAt( i )->SetIcon( pIconCurveEmpty );
			pListCurves->ItemChangedAt( i );
		}
	}
}
