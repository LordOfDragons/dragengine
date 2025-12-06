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
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeScrollBar.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeWidget.h>
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
	
	
	igdeContainerFlow::Ref bottomLine(igdeContainerFlow::Ref::New(
		new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst ) ));
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
	
	
	igdeWidget::Ref view(igdeWidget::Ref::New( pViewNode = new seViewConstructedView( windowMain ) ));
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
	const decPoint pageSize( decPoint( 1, 1 ).Largest( size / 4 ) );
	
	pScrollOffset = range / 2;
	
	pSBHorizontal->SetUpper( decMath::max( range.x + pageSize.x, 0) );
	pSBVertical->SetUpper( decMath::max( range.y + pageSize.y, 0 ) );
	
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
