/* 
 * Drag[en]gine IGDE Skin Editor
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

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "seViewConstructed.h"
#include "seViewConstructedListener.h"
#include "seViewConstructedView.h"
#include "../seWindowMain.h"
#include "../../skin/seSkin.h"
#include "../../skin/property/seProperty.h"
#include "../../skin/property/node/sePropertyNodeGroup.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeScrollBar.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeScrollBarListener.h>
#include <deigde/gui/event/igdeSpinTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/model/igdeListItem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cScrollView : public igdeScrollBarListener{
	seViewConstructed &pView;
	
public:
	cScrollView( seViewConstructed &view ) : pView( view ){}
	
	virtual void OnValueChanged( igdeScrollBar* ){
		if( pView.GetSkin() ){
			pView.OnScrolled();
		}
	}
};

class cSpinLayer : public igdeSpinTextFieldListener{
	seViewConstructed &pView;
	
public:
	cSpinLayer( seViewConstructed &view ) : pView( view ){}
	
	virtual void OnValueChanged( igdeSpinTextField *textField ){
		if( ! pView.GetSkin() ){
			return;
		}
		
		seProperty * const property = pView.GetViewNode().GetActiveProperty();
		if( property ){
			property->SetActiveNodeLayer( textField->GetValue() );
		}
	}
};

class cComboZoom : public igdeComboBoxListener{
	seViewConstructed &pView;
	
public:
	cComboZoom( seViewConstructed &view ) : pView( view ){}
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		if( ! pView.GetSkin() || ! comboBox->GetSelectedItem() ){
			return;
		}
		
		const int zoom = ( int )( intptr_t )comboBox->GetSelectedItem()->GetData();
		if( zoom == pView.GetViewNode().GetZoom() ){
			return;
		}
		
		pView.GetViewNode().SetZoom( zoom );
		pView.UpdateScrollbarRanges();
	}
};

}



// Class seViewConstructed
////////////////////////////

// Constructor, destructor
////////////////////////////

seViewConstructed::seViewConstructed( seWindowMain &windowMain ) :
igdeContainerBorder( windowMain.GetEnvironment() ),
pWindowMain( windowMain ),
pListener( NULL ),
pViewNode( NULL )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	pListener = new seViewConstructedListener( *this );
	
	
	igdeContainerReference bottomLine;
	bottomLine.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst ) );
	helper.ScrollBar( bottomLine, true, 0, 0, 1, 0, pSBHorizontal, new cScrollView( *this ) );
	helper.EditSpinInteger( bottomLine, "Select layer to edit", 0, 0, pSpinLayer, new cSpinLayer( *this ) );
	
	helper.ComboBox( bottomLine, "Select zoom factor", pCBZoom, new cComboZoom( *this ) );
	pCBZoom->AddItem( "800 %", NULL, ( void* )( intptr_t )800 );
	pCBZoom->AddItem( "600 %", NULL, ( void* )( intptr_t )600 );
	pCBZoom->AddItem( "400 %", NULL, ( void* )( intptr_t )400 );
	pCBZoom->AddItem( "300 %", NULL, ( void* )( intptr_t )300 );
	pCBZoom->AddItem( "200 %", NULL, ( void* )( intptr_t )200 );
	pCBZoom->AddItem( "150 %", NULL, ( void* )( intptr_t )150 );
	pCBZoom->AddItem( "100 %", NULL, ( void* )( intptr_t )100 );
	pCBZoom->AddItem( "50 %", NULL, ( void* )( intptr_t )50 );
	pCBZoom->AddItem( "25 %", NULL, ( void* )( intptr_t )25 );
	pCBZoom->SetSelectionWithData( ( void* )( intptr_t )100 );
	
	AddChild( bottomLine, igdeContainerBorder::eaBottom );
	
	
	helper.ScrollBar( false, 0, 0, 1, 0, pSBVertical, new cScrollView( *this ) );
	AddChild( pSBVertical, igdeContainerBorder::eaRight );
	
	
	igdeWidgetReference view;
	view.TakeOver( pViewNode = new seViewConstructedView( windowMain ) );
	AddChild( view, igdeContainerBorder::eaCenter );
}

seViewConstructed::~seViewConstructed(){
	SetSkin( NULL );
	
	if( pListener ){
		delete pListener;
	}
}



// Management
///////////////

seSkin *seViewConstructed::GetSkin() const{
	return pViewNode ? pViewNode->GetSkin() : NULL;
}

void seViewConstructed::SetSkin( seSkin *skin ){
	if( skin == pViewNode->GetSkin() ){
		return;
	}
	
	if( pViewNode->GetSkin() ){
		pViewNode->GetSkin()->RemoveListener( pListener );
	}
	
	pViewNode->SetSkin( skin );
	
	if( skin ){
		skin->AddListener( pListener );
	}
	
	UpdateScrollbarRanges();
	UpdateLayerRanges();
}



void seViewConstructed::SetEnableRendering( bool enable ){
	pViewNode->SetEnableRendering( enable );
}



void seViewConstructed::OnAfterEngineStart(){
	pViewNode->OnAfterEngineStart();
}

void seViewConstructed::OnBeforeEngineStop(){
	pViewNode->OnBeforeEngineStop();
}

void seViewConstructed::ResetView(){
	pViewNode->ResetView();
}

void seViewConstructed::OnFrameUpdate( float elapsed ){
	pViewNode->OnFrameUpdate( elapsed );
}

void seViewConstructed::OnResize(){
	UpdateScrollbarRanges();
}

void seViewConstructed::OnScrolled(){
	pViewNode->SetOffset( pScrollOffset
		- decPoint( pSBHorizontal->GetValue(), pSBVertical->GetValue() ) );
}



void seViewConstructed::UpdateScrollbarRanges(){
	pContentSize = pViewNode->GetContentSize();
	
	const decPoint size( pViewNode->GetRenderAreaSize() );
	const decPoint range( decPoint().Largest( pContentSize - size ) );
	const decPoint pageSize( size / 4 );
	
	pScrollOffset = range / 2;
	
	pSBHorizontal->SetUpper( range.x + pageSize.x );
	pSBVertical->SetUpper( range.y + pageSize.y );
	
	pSBHorizontal->SetPageSize( pageSize.x );
	pSBVertical->SetPageSize( pageSize.y );
	
	pSBHorizontal->SetValue( pScrollOffset.x - pViewNode->GetOffset().x );
	pSBVertical->SetValue( pScrollOffset.y - pViewNode->GetOffset().y );
	
	OnScrolled();
}

void seViewConstructed::UpdateLayerRanges(){
	const seProperty * const property = pViewNode->GetActiveProperty();
	
	if( property ){
		pSpinLayer->SetRange( 0, property->GetNodeGroup()->GetSize().z - 1 );
		pSpinLayer->SetValue( property->GetActiveNodeLayer() );
		pSpinLayer->SetEnabled( true );
		
	}else{
		pSpinLayer->SetRange( 0, 0 );
		pSpinLayer->SetEnabled( false );
	}
}

void seViewConstructed::UpdateActiveLayer(){
	const seProperty * const property = pViewNode->GetActiveProperty();
	if( property ){
		pSpinLayer->SetValue( property->GetActiveNodeLayer() );
	}
}
