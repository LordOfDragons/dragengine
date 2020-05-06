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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "seWPDynamicSkin.h"
#include "seWPDynamicSkinListener.h"
#include "../seWindowProperties.h"
#include "../../seWindowMain.h"
#include "../../../skin/seSkin.h"
#include "../../../skin/dynamicskin/seDynamicSkin.h"
#include "../../../skin/dynamicskin/seDynamicSkinRenderable.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/composed/igdeEditSliderTextListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeActionContextMenuReference.h>
#include <deigde/gui/event/igdeColorBoxListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/model/igdeTreeItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace{

class cBaseAction : public igdeAction{
protected:
	seWPDynamicSkin &pPanel;
	
public:
	cBaseAction( seWPDynamicSkin &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		seSkin * const skin = pPanel.GetSkin();
		seDynamicSkinRenderable * const renderable = pPanel.GetRenderable();
		if( skin && renderable ){
			OnAction( skin, renderable );
		}
	}
	
	virtual void OnAction( seSkin *skin, seDynamicSkinRenderable *renderable ) = 0;
	
	virtual void Update(){
		seSkin * const skin = pPanel.GetSkin();
		seDynamicSkinRenderable * const renderable = pPanel.GetRenderable();
		if( skin && renderable ){
			Update( *skin, *renderable );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const seSkin &, const seDynamicSkinRenderable & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	seWPDynamicSkin &pPanel;
	
public:
	cBaseTextFieldListener( seWPDynamicSkin &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		seSkin * const skin = pPanel.GetSkin();
		seDynamicSkinRenderable * const renderable = pPanel.GetRenderable();
		if( skin && renderable ){
			OnChanged( *textField, skin, renderable );
		}
	}
	
	virtual void OnChanged( igdeTextField &textField, seSkin *skin, seDynamicSkinRenderable *renderable ) = 0;
};

class cBaseEditPathListener : public igdeEditPathListener{
protected:
	seWPDynamicSkin &pPanel;
	
public:
	cBaseEditPathListener( seWPDynamicSkin &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		seSkin * const skin = pPanel.GetSkin();
		seDynamicSkinRenderable * const renderable = pPanel.GetRenderable();
		if( skin && renderable ){
			OnChanged( *editPath, skin, renderable );
		}
	}
	
	virtual void OnChanged( igdeEditPath &editPath, seSkin *skin, seDynamicSkinRenderable *renderable ) = 0;
};

class cBaseColorBoxListener : public igdeColorBoxListener{
protected:
	seWPDynamicSkin &pPanel;
	
public:
	cBaseColorBoxListener( seWPDynamicSkin &panel ) : pPanel( panel ){ }
	
	virtual void OnColorChanged( igdeColorBox *colorBox ){
		seSkin * const skin = pPanel.GetSkin();
		seDynamicSkinRenderable * const renderable = pPanel.GetRenderable();
		if( skin && renderable ){
			OnChanged( *colorBox, skin, renderable );
		}
	}
	
	virtual void OnChanged( igdeColorBox &colorBox, seSkin *skin, seDynamicSkinRenderable *renderable ) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	seWPDynamicSkin &pPanel;
	
public:
	cBaseComboBoxListener( seWPDynamicSkin &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		seSkin * const skin = pPanel.GetSkin();
		seDynamicSkinRenderable * const renderable = pPanel.GetRenderable();
		if( skin && renderable ){
			OnChanged( *comboBox, skin, renderable );
		}
	}
	
	virtual void OnChanged( igdeComboBox &comboBox, seSkin *skin, seDynamicSkinRenderable *renderable ) = 0;
};

class cBaseEditSliderTextListener : public igdeEditSliderTextListener{
protected:
	seWPDynamicSkin &pPanel;
	
public:
	cBaseEditSliderTextListener( seWPDynamicSkin &panel ) : pPanel( panel ){ }
	
	virtual void OnSliderTextValueChanged( igdeEditSliderText *sliderText ){
		seSkin * const skin = pPanel.GetSkin();
		seDynamicSkinRenderable * const renderable = pPanel.GetRenderable();
		if( skin && renderable ){
			OnChanged( *sliderText, skin, renderable );
		}
	}
	
	virtual void OnSliderTextValueChanging( igdeEditSliderText *sliderText ){
		OnSliderTextValueChanged( sliderText );
	}
	
	virtual void OnChanged( igdeEditSliderText &sliderText, seSkin *skin, seDynamicSkinRenderable *renderable ) = 0;
};


class cActionAddRenderable : public igdeAction{
	seWPDynamicSkin &pPanel;
public:
	cActionAddRenderable( seWPDynamicSkin &panel ) : igdeAction( "Add...",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add Renderable" ),
		pPanel( panel ){ }
	
	virtual void OnAction(){
		if( ! pPanel.GetSkin() ){
			return;
		}
		
		decString name( "Renderable" );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Add Renderable", "Name:", name ) ){
			return;
		}
		
		seDynamicSkin &dynamicSkin = pPanel.GetSkin()->GetDynamicSkin();
		if( dynamicSkin.GetRenderableList().HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Add Renderable", "A renderable with this name exists already." );
			return;
		}
		
		deObjectReference renderable;
		renderable.TakeOver( new seDynamicSkinRenderable( pPanel.GetSkin()->GetEngine(), name ) );
		dynamicSkin.AddRenderable( ( seDynamicSkinRenderable* )( deObject* )renderable );
		dynamicSkin.SetActiveRenderable( ( seDynamicSkinRenderable* )( deObject* )renderable );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetSkin() );
	}
};

class cActionRemoveRenderable : public cBaseAction{
public:
	cActionRemoveRenderable( seWPDynamicSkin &panel ) : cBaseAction( panel, "Remove",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove renderable" ){ }
	
	virtual void OnAction( seSkin *skin, seDynamicSkinRenderable *renderable ){
		skin->GetDynamicSkin().RemoveRenderable( renderable );
	}
};

class cListRenderable : public igdeListBoxListener{
	seWPDynamicSkin &pPanel;
public:
	cListRenderable( seWPDynamicSkin &panel ) : pPanel( panel ){ }
	
	virtual void OnSelectionChanged( igdeListBox *listBox ){
		if( pPanel.GetSkin() ){
			pPanel.GetSkin()->GetDynamicSkin().SetActiveRenderable( listBox->GetSelectedItem()
				? ( seDynamicSkinRenderable* )listBox->GetSelectedItem()->GetData() : NULL );
		}
	}
	
	virtual void AddContextMenuEntries( igdeListBox*, igdeMenuCascade &menu ){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand( menu, new cActionAddRenderable( pPanel ), true );
		helper.MenuCommand( menu, new cActionRemoveRenderable( pPanel ), true );
	}
};

class cTextRenderableName : public cBaseTextFieldListener{
public:
	cTextRenderableName( seWPDynamicSkin &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField &textField, seSkin *skin, seDynamicSkinRenderable *renderable ){
		if( renderable->GetName() == textField.GetText() ){
			return;
		}
		
		if( skin->GetDynamicSkin().GetRenderableList().HasNamed( textField.GetText() ) ){
			igdeCommonDialogs::Error( &pPanel, "Rename Renderable", "A renderable with this name exists already." );
			textField.SetText( renderable->GetName() );
			
		}else{
			renderable->SetName( textField.GetText() );
		}
	}
};

class cComboRenderableType : public cBaseComboBoxListener{
public:
	cComboRenderableType( seWPDynamicSkin &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual void OnChanged( igdeComboBox &comboBox, seSkin*, seDynamicSkinRenderable *renderable ){
		if( comboBox.GetSelectedItem() ){
			renderable->SetRenderableType( ( seDynamicSkinRenderable::eRenderableTypes )
				( intptr_t )comboBox.GetSelectedItem()->GetData() );
		}
	}
};


class cSliderValue : public cBaseEditSliderTextListener{
public:
	cSliderValue( seWPDynamicSkin &panel ) : cBaseEditSliderTextListener( panel ){ }
	
	virtual void OnChanged( igdeEditSliderText &sliderText, seSkin*, seDynamicSkinRenderable *renderable ){
		renderable->SetValue( sliderText.GetValue() );
	}
};

class cTextValueRangeLower : public cBaseTextFieldListener{
public:
	cTextValueRangeLower( seWPDynamicSkin &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField &textField, seSkin*, seDynamicSkinRenderable *renderable ){
		renderable->SetSliderLowerRange( textField.GetFloat() );
	}
};

class cTextValueRangeUpper : public cBaseTextFieldListener{
public:
	cTextValueRangeUpper( seWPDynamicSkin &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField &textField, seSkin*, seDynamicSkinRenderable *renderable ){
		renderable->SetSliderUpperRange( textField.GetFloat() );
	}
};


class cColorColor : public cBaseColorBoxListener{
public:
	cColorColor( seWPDynamicSkin &panel ) : cBaseColorBoxListener( panel ){ }
	
	virtual void OnChanged( igdeColorBox &colorBox, seSkin*, seDynamicSkinRenderable *renderable ){
		renderable->SetColor( colorBox.GetColor() );
	}
};

class cSliderColorRed : public cBaseEditSliderTextListener{
public:
	cSliderColorRed( seWPDynamicSkin &panel ) : cBaseEditSliderTextListener( panel ){ }
	
	virtual void OnChanged( igdeEditSliderText &sliderText, seSkin*, seDynamicSkinRenderable *renderable ){
		decColor color = renderable->GetColor();
		color.r = sliderText.GetValue();
		renderable->SetColor( color );
	}
};

class cSliderColorGreen : public cBaseEditSliderTextListener{
public:
	cSliderColorGreen( seWPDynamicSkin &panel ) : cBaseEditSliderTextListener( panel ){ }
	
	virtual void OnChanged( igdeEditSliderText &sliderText, seSkin*, seDynamicSkinRenderable *renderable ){
		decColor color = renderable->GetColor();
		color.g = sliderText.GetValue();
		renderable->SetColor( color );
	}
};

class cSliderColorBlue : public cBaseEditSliderTextListener{
public:
	cSliderColorBlue( seWPDynamicSkin &panel ) : cBaseEditSliderTextListener( panel ){ }
	
	virtual void OnChanged( igdeEditSliderText &sliderText, seSkin*, seDynamicSkinRenderable *renderable ){
		decColor color = renderable->GetColor();
		color.b = sliderText.GetValue();
		renderable->SetColor( color );
	}
};

class cSliderColorAlpha : public cBaseEditSliderTextListener{
public:
	cSliderColorAlpha( seWPDynamicSkin &panel ) : cBaseEditSliderTextListener( panel ){ }
	
	virtual void OnChanged( igdeEditSliderText &sliderText, seSkin*, seDynamicSkinRenderable *renderable ){
		decColor color = renderable->GetColor();
		color.a = sliderText.GetValue();
		renderable->SetColor( color );
	}
};


class cPathImage : public cBaseEditPathListener{
public:
	cPathImage( seWPDynamicSkin &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual void OnChanged( igdeEditPath &editPath, seSkin*, seDynamicSkinRenderable *renderable ){
		renderable->SetImagePath( editPath.GetPath() );
	}
};


class cPathVideo : public cBaseEditPathListener{
public:
	cPathVideo( seWPDynamicSkin &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual void OnChanged( igdeEditPath &editPath, seSkin*, seDynamicSkinRenderable *renderable ){
		renderable->SetVideoPath( editPath.GetPath() );
	}
};

}


// Class seWPDynamicSkin
//////////////////////////

// Constructor, destructor
////////////////////////////

seWPDynamicSkin::seWPDynamicSkin( seWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pSkin( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainerReference content, panel, groupBox, form, formLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = new seWPDynamicSkinListener( *this );
	
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	
	// renderable
	helper.GroupBoxFlow( content, groupBox, "Renderable:" );
	
	helper.ListBox( groupBox, 6, "Select renderable to edit", pListRenderable, new cListRenderable( *this ) );
	pListRenderable->SetDefaultSorter();
	
	form.TakeOver( new igdeContainerForm( env ) );
	groupBox->AddChild( form );
	helper.EditString( form, "Name:", "Unique name of the renderable", pEditName, new cTextRenderableName( *this ) );
	
	helper.ComboBox( form, "Type:", "Type of renderable", pCBRenderableType, new cComboRenderableType( *this ) );
	pCBRenderableType->AddItem( "Value", NULL, ( void* )( intptr_t )seDynamicSkinRenderable::ertValue );
	pCBRenderableType->AddItem( "Color", NULL, ( void* )( intptr_t )seDynamicSkinRenderable::ertColor );
	pCBRenderableType->AddItem( "Image", NULL, ( void* )( intptr_t )seDynamicSkinRenderable::ertImage );
	pCBRenderableType->AddItem( "Video Frame", NULL, ( void* )( intptr_t )seDynamicSkinRenderable::ertVideoFrame );
	pCBRenderableType->AddItem( "Canvas", NULL, ( void* )( intptr_t )seDynamicSkinRenderable::ertCanvas );
	pCBRenderableType->AddItem( "Camera", NULL, ( void* )( intptr_t )seDynamicSkinRenderable::ertCamera );
	
	
	// type specific panels
	pSwitcher.TakeOver( new igdeSwitcher( env ) );
	content->AddChild( pSwitcher );
	
	
	// empty
	helper.Label( pSwitcher, "", "" );
	
	
	// value
	panel.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	pSwitcher->AddChild( panel );
	helper.GroupBox( panel, groupBox, "Value:" );
	helper.EditSliderText( groupBox, "Value:", "Value", 0.0f, 1.0f, 6, 3, 0.1f, pSldValue, new cSliderValue( *this ) );
	helper.EditFloat( groupBox, "Lower:", "Lower range", pEditValueLower, new cTextValueRangeLower( *this ) );
	helper.EditFloat( groupBox, "Upper:", "Upper range", pEditValueUpper, new cTextValueRangeUpper( *this ) );
	
	
	// color
	panel.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	pSwitcher->AddChild( panel );
	helper.GroupBox( panel, groupBox, "Color:" );
	helper.ColorBox( groupBox, "Color:", "Color", pClrColor, new cColorColor( *this ) );
	helper.EditSliderText( groupBox, "Red:", "Red color value", 0.0f, 1.0f, 6, 3, 0.1f,
		pSldColorRed, new cSliderColorRed( *this ) );
	helper.EditSliderText( groupBox, "Green:", "Green color value", 0.0f, 1.0f, 6, 3, 0.1f,
		pSldColorGreen, new cSliderColorGreen( *this ) );
	helper.EditSliderText( groupBox, "Blue:", "Blue color value", 0.0f, 1.0f, 6, 3, 0.1f,
		pSldColorBlue, new cSliderColorBlue( *this ) );
	helper.EditSliderText( groupBox, "ALpha:", "ALpha color value", 0.0f, 1.0f, 6, 3, 0.1f,
		pSldColorAlpha, new cSliderColorAlpha( *this ) );
	
	
	// image
	panel.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	pSwitcher->AddChild( panel );
	helper.GroupBox( panel, groupBox, "Image:" );
	helper.EditPath( groupBox, "Path:", "Path to image", igdeEnvironment::efpltImage,
		pEditImagePath, new cPathImage( *this ) );
	
	
	// video
	panel.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	pSwitcher->AddChild( panel );
	helper.GroupBox( panel, groupBox, "Video:" );
	helper.EditPath( groupBox, "Path:", "Path to video", igdeEnvironment::efpltVideo,
		pEditVideoPath, new cPathVideo( *this ) );
	
	
	// canvas
	helper.Label( pSwitcher, "", "" );
	
	
	// camera
	helper.Label( pSwitcher, "", "" );
}

seWPDynamicSkin::~seWPDynamicSkin(){
	SetSkin( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void seWPDynamicSkin::SetSkin( seSkin *skin ){
	if( skin == pSkin ){
		return;
	}
	
	if( pSkin ){
		pSkin->RemoveListener( pListener );
		pSkin->FreeReference();
	}
	
	pSkin = skin;
	
	if( skin ){
		skin->AddListener( pListener );
		skin->AddReference();
	}
	
	UpdateRenderableList();
}

seDynamicSkinRenderable *seWPDynamicSkin::GetRenderable() const{
	return pSkin ? pSkin->GetDynamicSkin().GetActiveRenderable() : NULL;
}



void seWPDynamicSkin::UpdateRenderableList(){
	seDynamicSkinRenderable * const selection = GetRenderable();
	
	pListRenderable->RemoveAllItems();
	
	if( pSkin ){
		const seDynamicSkinRenderableList &list = pSkin->GetDynamicSkin().GetRenderableList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			seDynamicSkinRenderable * const renderable = list.GetAt( i );
			pListRenderable->AddItem( renderable->GetName(), NULL, renderable );
		}
		
		pListRenderable->SortItems();
	}
	
	pListRenderable->SetSelectionWithData( selection );
	ShowValuePanel();
	UpdateRenderable();
}

void seWPDynamicSkin::UpdateRenderable(){
	const seDynamicSkinRenderable * const renderable = GetRenderable();
	
	if( renderable ){
		pEditName->SetText( renderable->GetName() );
		pCBRenderableType->SetSelectionWithData( ( void* )( intptr_t )renderable->GetRenderableType() );
		
		const float sliderLower = renderable->GetSliderValueLower();
		const float sliderUpper = renderable->GetSliderValueUpper();
		pEditValueLower->SetFloat( sliderLower );
		pEditValueUpper->SetFloat( sliderUpper );
		pSldValue->SetRange( sliderLower, sliderUpper );
		pSldValue->SetTickSpacing( ( sliderUpper - sliderLower ) * 0.1f );
		pSldValue->SetValue( renderable->GetValue() );
		
		pClrColor->SetColor( renderable->GetColor() );
		pSldColorRed->SetValue( renderable->GetColor().r );
		pSldColorGreen->SetValue( renderable->GetColor().g );
		pSldColorBlue->SetValue( renderable->GetColor().b );
		pSldColorAlpha->SetValue( renderable->GetColor().a );
		
		pEditImagePath->SetPath( renderable->GetImagePath() );
		
		pEditVideoPath->SetPath( renderable->GetVideoPath() );
		
	}else{
		pEditName->ClearText();
		
		pSldValue->SetRange( 0.0f, 1.0f );
		pSldValue->SetTickSpacing( 0.1f );
		pSldValue->SetValue( 0.0f );
		
		pClrColor->SetColor( decColor( 1.0f, 1.0f, 1.0f ) );
		pSldColorRed->SetValue( 1.0f );
		pSldColorGreen->SetValue( 1.0f );
		pSldColorBlue->SetValue( 1.0f );
		pSldColorAlpha->SetValue( 1.0f );
		
		pEditImagePath->ClearPath();
		
		pEditVideoPath->ClearPath();
	}
	
	const bool enabled = renderable;
	pEditName->SetEnabled( enabled );
}

void seWPDynamicSkin::SelectActiveRenderable(){
	pListRenderable->SetSelectionWithData( GetRenderable() );
	UpdateRenderable();
}

void seWPDynamicSkin::ShowValuePanel(){
	const seDynamicSkinRenderable * const renderable = GetRenderable();
	if( renderable ){
		switch( renderable->GetRenderableType() ){
		case seDynamicSkinRenderable::ertValue:
			pSwitcher->SetCurrent( 1 );
			break;
			
		case seDynamicSkinRenderable::ertColor:
			pSwitcher->SetCurrent( 2 );
			break;
			
		case seDynamicSkinRenderable::ertImage:
			pSwitcher->SetCurrent( 3 );
			break;
			
		case seDynamicSkinRenderable::ertVideoFrame:
			pSwitcher->SetCurrent( 4 );
			break;
			
		case seDynamicSkinRenderable::ertCanvas:
			pSwitcher->SetCurrent( 5 );
			break;
			
		case seDynamicSkinRenderable::ertCamera:
			pSwitcher->SetCurrent( 6 );
			break;
			
		default:
			pSwitcher->SetCurrent( 0 ); // empty
		}
		
	}else{
		pSwitcher->SetCurrent( 0 ); // empty
	}
}
