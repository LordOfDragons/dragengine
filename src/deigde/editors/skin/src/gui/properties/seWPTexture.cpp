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

#include "seWPTexture.h"
#include "seWPTextureListener.h"
#include "seWindowProperties.h"
#include "../seWindowMain.h"
#include "../../skin/seSkin.h"
#include "../../skin/texture/seTexture.h"
#include "../../skin/property/node/sePropertyNodeGroup.h"
#include "../../undosys/texture/seUTextureSetName.h"
#include "../../undosys/property/seUPropertySetValueType.h"
#include "../../undosys/property/seUPropertySetValue.h"
#include "../../undosys/property/seUPropertySetColor.h"
#include "../../undosys/property/seUPropertySetImagePath.h"
#include "../../undosys/property/seUPropertySetVideoPath.h"
#include "../../undosys/property/seUPropertySetMappedComponent.h"
#include "../../undosys/property/seUPropertyToggleVideoSharedTime.h"
#include "../../undosys/property/seUPropertySetConstructedColor.h"
#include "../../undosys/property/seUPropertyConstructedToggleTileX.h"
#include "../../undosys/property/seUPropertyConstructedToggleTileY.h"
#include "../../undosys/property/seUPropertySetRenderableName.h"
#include "../../undosys/property/seUPropertySetConstructedSize.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditPoint.h>
#include <deigde/gui/composed/igdeEditPointListener.h>
#include <deigde/gui/composed/igdeEditPoint3.h>
#include <deigde/gui/composed/igdeEditPoint3Listener.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionSelectFile.h>
#include <deigde/gui/event/igdeColorBoxListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeSpinTextFieldListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/video/deVideo.h>



// Actions
////////////

namespace{

class cBaseAction : public igdeAction{
protected:
	seWPTexture &pPanel;
	
public:
	cBaseAction( seWPTexture &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		if( ! skin || ! texture ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnAction( skin, texture ) );
		if( undo ){
			skin->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( seSkin *skin, seTexture *texture ) = 0;
	
	virtual void Update(){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		if( skin && texture ){
			Update( *skin, *texture );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const seSkin &, const seTexture & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cBaseActionProperty : public cBaseAction{
public:
	cBaseActionProperty( seWPTexture &panel, const char *text, igdeIcon *icon,
		const char *description ) : cBaseAction( panel, text, icon, description ){ }
	
	virtual igdeUndo *OnAction( seSkin *skin, seTexture *texture ){
		return pPanel.GetProperty() ? OnActionProperty( skin, texture, pPanel.GetProperty() ) : NULL;
	}
	
	virtual igdeUndo *OnActionProperty( seSkin *skin, seTexture *texture, seProperty *property ) = 0;
	
	
	virtual void Update( const seSkin &skin, const seTexture &texture ){
		seProperty * const property = pPanel.GetProperty();
		if( property ){
			UpdateProperty( skin, texture, *property );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void UpdateProperty( const seSkin &, const seTexture &, const seProperty & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	seWPTexture &pPanel;
	
public:
	cBaseTextFieldListener( seWPTexture &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		if( ! skin || ! texture || ! property ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( *textField, skin, texture, property ) );
		if( undo ){
			skin->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, seSkin *skin,
		seTexture *texture, seProperty *property ) = 0;
};

class cBaseEditPathListener : public igdeEditPathListener{
protected:
	seWPTexture &pPanel;
	
public:
	cBaseEditPathListener( seWPTexture &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		if( ! skin || ! texture || ! property ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( *editPath, skin, texture, property ) );
		if( undo ){
			skin->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeEditPath &editPath, seSkin *skin,
		seTexture *texture, seProperty *property ) = 0;
};

class cBaseColorBoxListener : public igdeColorBoxListener{
protected:
	seWPTexture &pPanel;
	
public:
	cBaseColorBoxListener( seWPTexture &panel ) : pPanel( panel ){ }
	
	virtual void OnColorChanged( igdeColorBox *colorBox ){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		if( ! skin || ! texture || ! property ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( *colorBox, skin, texture, property ) );
		if( undo ){
			skin->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeColorBox &colorBox, seSkin *skin,
		seTexture *texture, seProperty *property ) = 0;
};


class cListTextures : public igdeListBoxListener{
	seWPTexture &pPanel;
public:
	cListTextures( seWPTexture &panel ) : pPanel( panel ){ }
	
	virtual void OnSelectionChanged( igdeListBox *listBox ){
		seSkin * const skin = pPanel.GetSkin();
		if( skin ){
			skin->SetActiveTexture( listBox->GetSelectedItem()
				? ( seTexture* )listBox->GetSelectedItem()->GetData() : NULL );
		}
	}
	
	virtual void AddContextMenuEntries( igdeListBox*, igdeMenuCascade &menu ){
		seWindowMain &windowMain = pPanel.GetWindowProperties().GetWindowMain();
		igdeEnvironment &env = menu.GetEnvironment();
		igdeUIHelper &helper = env.GetUIHelper();
		
		helper.MenuCommand( menu, windowMain.GetActionTextureAdd() );
		helper.MenuCommand( menu, windowMain.GetActionTextureRemove() );
		
		helper.MenuSeparator( menu );
		helper.MenuCommand( menu, windowMain.GetActionTextureImportFromFile() );
		helper.MenuCommand( menu, windowMain.GetActionTextureImportFromGDef() );
	}
};

class cTextTextureName : public igdeTextFieldListener{
	seWPTexture &pPanel;
public:
	cTextTextureName( seWPTexture &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		seTexture * const texture = pPanel.GetTexture();
		const decString &value = textField->GetText();
		if( ! texture || texture->GetName() == value ){
			return;
		}
		
		if( pPanel.GetSkin()->GetTextureList().HasNamed( value ) ){
			igdeCommonDialogs::Error( &pPanel, "Set Texture Name", "Texture name exists already" );
			textField->SetText( texture->GetName() );
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new seUTextureSetName( texture, value ) );
		pPanel.GetSkin()->GetUndoSystem()->Add( undo );
	}
};

class cEditPreviewEditTCOffset : public igdeEditPointListener{
	seWPTexture &pPanel;
public:
	cEditPreviewEditTCOffset( seWPTexture &panel ) : pPanel( panel ){ }
	
	virtual void OnPointChanged( igdeEditPoint *editPoint ){
		seTexture * const texture = pPanel.GetTexture();
		if( texture ){
			texture->SetTexCoordOffset( editPoint->GetPoint() );
		}
	}
};

class cEditPreviewEditTCScale : public igdeEditPointListener{
	seWPTexture &pPanel;
public:
	cEditPreviewEditTCScale( seWPTexture &panel ) : pPanel( panel ){ }
	
	virtual void OnPointChanged( igdeEditPoint *editPoint ){
		seTexture * const texture = pPanel.GetTexture();
		if( texture ){
			texture->SetTexCoordScaling( editPoint->GetPoint() );
		}
	}
};

class cEditPreviewEditTCRotation : public igdeTextFieldListener{
	seWPTexture &pPanel;
public:
	cEditPreviewEditTCRotation( seWPTexture &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		seTexture * const texture = pPanel.GetTexture();
		if( texture ){
			texture->SetTexCoordRotation( textField->GetFloat() );
		}
	}
};


class cListProperties : public igdeListBoxListener{
	seWPTexture &pPanel;
public:
	cListProperties( seWPTexture &panel ) : pPanel( panel ){ }
	
	virtual void OnSelectionChanged( igdeListBox *listBox ){
		seTexture * const texture = pPanel.GetTexture();
		if( texture ){
			texture->SetActiveProperty( listBox->GetSelectedItem()
				? ( seProperty* )listBox->GetSelectedItem()->GetData() : NULL );
		}
	}
	
	virtual void AddContextMenuEntries( igdeListBox*, igdeMenuCascade &menu ){
		seWindowMain &windowMain = pPanel.GetWindowProperties().GetWindowMain();
		igdeEnvironment &env = menu.GetEnvironment();
		igdeUIHelper &helper = env.GetUIHelper();
		
		helper.MenuCommand( menu, windowMain.GetActionTexturePropertyAdd() );
		helper.MenuCommand( menu, windowMain.GetActionTexturePropertyRemove() );
	}
};

class cComboPropertyValueType : public igdeComboBoxListener{
	seWPTexture &pPanel;
public:
	cComboPropertyValueType( seWPTexture &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		seProperty * const property = pPanel.GetProperty();
		if( ! property || ! comboBox->GetSelectedItem() ){
			return;
		}
		
		const seProperty::eValueTypes value = ( seProperty::eValueTypes )( intptr_t )
			comboBox->GetSelectedItem()->GetData();
		if( value == property->GetValueType() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new seUPropertySetValueType( property, value ) );
		pPanel.GetSkin()->GetUndoSystem()->Add( undo );
	}
};

class cTextPropertyRenderable : public cBaseTextFieldListener{
public:
	cTextPropertyRenderable( seWPTexture &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, seSkin*, seTexture*, seProperty *property ){
		return property->GetRenderableName() != textField.GetText()
			? new seUPropertySetRenderableName( property, textField.GetText() ) : NULL;
	}
};


class cTextPropertyValue : public cBaseTextFieldListener{
public:
	cTextPropertyValue( seWPTexture &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, seSkin*, seTexture*, seProperty *property ){
		const float value = textField.GetFloat();
		return fabsf( property->GetValue() - value ) > FLOAT_SAFE_EPSILON
			? new seUPropertySetValue( property, value ) : NULL;
	}
};


class cColorPropertyColor : public cBaseColorBoxListener{
public:
	cColorPropertyColor( seWPTexture &panel ) : cBaseColorBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeColorBox &colorBox, seSkin*, seTexture*, seProperty *property ){
		return ! property->GetColor().IsEqualTo( colorBox.GetColor() )
			? new seUPropertySetColor( property, colorBox.GetColor() ) : NULL;
	}
};


class cPathPropertyImage : public cBaseEditPathListener{
public:
	cPathPropertyImage( seWPTexture &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditPath &editPath, seSkin*, seTexture*, seProperty *property ){
		return property->GetImagePath() != editPath.GetPath()
			? new seUPropertySetImagePath( property, editPath.GetPath() ) : NULL;
	}
};


class cPathPropertyVideo : public cBaseEditPathListener{
public:
	cPathPropertyVideo( seWPTexture &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditPath &editPath, seSkin*, seTexture*, seProperty *property ){
		return property->GetVideoPath() != editPath.GetPath()
			? new seUPropertySetVideoPath( property, editPath.GetPath() ) : NULL;
	}
};

class cActionPropertyVideoUseShared : public cBaseActionProperty{
public:
	cActionPropertyVideoUseShared( seWPTexture &panel ) : cBaseActionProperty( panel,
		"Use shared time", NULL, "Playback time is shared across instances" ){ }
	
	virtual igdeUndo *OnActionProperty( seSkin*, seTexture* , seProperty *property ){
		return new seUPropertyToggleVideoSharedTime( property );
	}
	
	virtual void UpdateProperty( const seSkin &, const seTexture &, const seProperty &property ){
		SetEnabled( true );
		SetSelected( property.GetVideoSharedTime() );
	}
};


class cSpinPropertyMappedComponent : public igdeSpinTextFieldListener{
	seWPTexture &pPanel;
public:
	cSpinPropertyMappedComponent( seWPTexture &panel ) : pPanel( panel ){ }
	
	virtual void OnValueChanged( igdeSpinTextField* ){
		pPanel.UpdatePropertyMappedComponent();
	}
};

class cComboPropertyMappedInputType : public igdeComboBoxListener{
	seWPTexture &pPanel;
public:
	cComboPropertyMappedInputType( seWPTexture &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		if( ! skin || ! texture || ! property || ! comboBox->GetSelectedItem() ){
			return;
		}
		
		const int index = pPanel.GetPropertyMappedComponentIndex();
		deSkinPropertyMapped::cComponent component( property->GetMappedComponent( index ) );
		const deSkinPropertyMapped::eInputTypes type = ( deSkinPropertyMapped::eInputTypes )
			( intptr_t )comboBox->GetSelectedItem()->GetData();
		if( type == component.GetInputType() ){
			return;
		}
		
		component.SetInputType( type );
		igdeUndoReference undo;
		undo.TakeOver( new seUPropertySetMappedComponent( property, index, component ) );
		skin->GetUndoSystem()->Add( undo );
	}
};

class cEditMappedComponentCurve : public igdeViewCurveBezierListener{
	seWPTexture &pPanel;
	igdeUndoReference pUndo;
	
public:
	cEditMappedComponentCurve( seWPTexture &panel ) : pPanel( panel ){ }
	
	virtual void OnCurveChanged( igdeViewCurveBezier *viewCurveBezier ){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		if( ! skin || ! texture || ! property ){
			return;
		}
		
		const int index = pPanel.GetPropertyMappedComponentIndex();
		deSkinPropertyMapped::cComponent component( property->GetMappedComponent( index ) );
		
		if( pUndo ){
			component.GetCurve() = viewCurveBezier->GetCurve();
			( ( seUPropertySetMappedComponent& )( igdeUndo& )pUndo ).SetNewValue( component );
			
		}else{
			if( component.GetCurve() == viewCurveBezier->GetCurve() ){
				return;
			}
			component.GetCurve() = viewCurveBezier->GetCurve();
			pUndo.TakeOver( new seUPropertySetMappedComponent( property, index, component ) );
		}
		
		skin->GetUndoSystem()->Add( pUndo );
		pUndo = NULL;
	}
	
	virtual void OnCurveChanging( igdeViewCurveBezier *viewCurveBezier ){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		if( ! skin || ! texture || ! property ){
			return;
		}
		
		const int index = pPanel.GetPropertyMappedComponentIndex();
		deSkinPropertyMapped::cComponent component( property->GetMappedComponent( index ) );
		
		if( pUndo ){
			component.GetCurve() = viewCurveBezier->GetCurve();
			( ( seUPropertySetMappedComponent& )( igdeUndo& )pUndo ).SetNewValue( component );
			pUndo->Redo();
			
		}else{
			if( component.GetCurve() == viewCurveBezier->GetCurve() ){
				return;
			}
			component.GetCurve() = viewCurveBezier->GetCurve();
			pUndo.TakeOver( new seUPropertySetMappedComponent( property, index, component ) );
		}
	}
};

class cTextPropertyMappedInputLower : public cBaseTextFieldListener{
public:
	cTextPropertyMappedInputLower( seWPTexture &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, seSkin*, seTexture*, seProperty *property ){
		const int index = pPanel.GetPropertyMappedComponentIndex();
		deSkinPropertyMapped::cComponent component( property->GetMappedComponent( index ) );
		const float value = textField.GetFloat();
		if( fabsf( value - component.GetInputLower() ) < FLOAT_SAFE_EPSILON ){
			return NULL;
		}
		component.SetInputLower( value );
		return new seUPropertySetMappedComponent( property, index, component );
	}
};

class cTextPropertyMappedInputUpper : public cBaseTextFieldListener{
public:
	cTextPropertyMappedInputUpper( seWPTexture &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, seSkin*, seTexture*, seProperty *property ){
		const int index = pPanel.GetPropertyMappedComponentIndex();
		deSkinPropertyMapped::cComponent component( property->GetMappedComponent( index ) );
		const float value = textField.GetFloat();
		if( fabsf( value - component.GetInputUpper() ) < FLOAT_SAFE_EPSILON ){
			return NULL;
		}
		component.SetInputUpper( value );
		return new seUPropertySetMappedComponent( property, index, component );
	}
};

class cActionPropertyMappedInputClamped : public cBaseActionProperty{
public:
	cActionPropertyMappedInputClamped( seWPTexture &panel ) : cBaseActionProperty( panel,
		"Input Clamped:", NULL, "Input value is clamperd to input range instead od wrapping around" ){ }
	
	virtual igdeUndo *OnActionProperty( seSkin*, seTexture*, seProperty *property ){
		const int index = pPanel.GetPropertyMappedComponentIndex();
		deSkinPropertyMapped::cComponent component( property->GetMappedComponent( index ) );
		component.SetInputClamped( ! component.GetInputClamped() );
		return new seUPropertySetMappedComponent( property, index, component );
	}
	
	virtual void UpdateProperty( const seSkin &skin, const seTexture &texture, const seProperty &property ){
		cBaseActionProperty::UpdateProperty( skin, texture, property );
		SetSelected( property.GetMappedComponent( pPanel.GetPropertyMappedComponentIndex() ).GetInputClamped() );
	}
};

class cTextPropertyMappedBone : public cBaseTextFieldListener{
public:
	cTextPropertyMappedBone( seWPTexture &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, seSkin*, seTexture*, seProperty *property ){
		const int index = pPanel.GetPropertyMappedComponentIndex();
		deSkinPropertyMapped::cComponent component( property->GetMappedComponent( index ) );
		if( textField.GetText() == component.GetBone() ){
			return NULL;
		}
		component.SetBone( textField.GetText() );
		return new seUPropertySetMappedComponent( property, index, component );
	}
};


class cColorPropertyConstructed : public cBaseColorBoxListener{
public:
	cColorPropertyConstructed( seWPTexture &panel ) : cBaseColorBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeColorBox &colorBox, seSkin*, seTexture*, seProperty *property ){
		return ! property->GetNodeColor().IsEqualTo( colorBox.GetColor() )
			? new seUPropertySetConstructedColor( property, colorBox.GetColor() ) : NULL;
	}
};

class cEditPropertyConstructedSize : public igdeEditPoint3Listener{
	seWPTexture &pPanel;
public:
	cEditPropertyConstructedSize( seWPTexture &panel ) : pPanel( panel ){ }
	
	virtual void OnPoint3Changed( igdeEditPoint3 *editPoint3 ){
		seProperty * const property = pPanel.GetProperty();
		if( ! property || property->GetNodeGroup()->GetSize() == editPoint3->GetPoint3() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new seUPropertySetConstructedSize( property, editPoint3->GetPoint3() ) );
		pPanel.GetSkin()->GetUndoSystem()->Add( undo );
	}
};

class cActionConstructedChkTileX : public cBaseActionProperty{
public:
	cActionConstructedChkTileX( seWPTexture &panel ) : cBaseActionProperty( panel,
		"X-Axis", NULL, "Tile nodes along X axis" ){ }
	
	virtual igdeUndo *OnActionProperty( seSkin*, seTexture* , seProperty *property ){
		return new seUPropertyConstructedToggleTileX( property );
	}
	
	virtual void UpdateProperty( const seSkin &, const seTexture &, const seProperty &property ){
		SetEnabled( true );
		SetSelected( property.GetNodeTileX() );
	}
};

class cActionConstructedChkTileY : public cBaseActionProperty{
public:
	cActionConstructedChkTileY( seWPTexture &panel ) : cBaseActionProperty( panel,
		"Y-Axis", NULL, "Tile nodes along Y axis" ){ }
	
	virtual igdeUndo *OnActionProperty( seSkin*, seTexture* , seProperty *property ){
		return new seUPropertyConstructedToggleTileY( property );
	}
	
	virtual void UpdateProperty( const seSkin &, const seTexture &, const seProperty &property ){
		SetEnabled( true );
		SetSelected( property.GetNodeTileY() );
	}
};

}



// Class seWPTexture
//////////////////////

// Constructor, destructor
////////////////////////////

seWPTexture::seWPTexture( seWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pSkin( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainerReference content, panel, groupBox, form, formLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = new seWPTextureListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	
	// texture
	helper.GroupBoxFlow( content, groupBox, "Texture:" );
	helper.ListBox( groupBox, 4, "Textures", pListTexture, new cListTextures( *this ) );
	pListTexture->SetDefaultSorter();
	
	form.TakeOver( new igdeContainerForm( env ) );
	groupBox->AddChild( form );
	helper.EditString( form, "Name:", "Name of texture", pEditTexName, new cTextTextureName( *this ) );
	
	
	// preview
	helper.GroupBox( content, groupBox, "Texture Preview:", true );
	helper.EditPoint( groupBox, "Translation:", "Texture coordinates translation.",
		pPreviewEditTCOffset, new cEditPreviewEditTCOffset( *this ) );
	helper.EditPoint( groupBox, "Scaling:", "Texture coordinates scaling.",
		pPreviewEditTCScaling, new cEditPreviewEditTCScale( *this ) );
	helper.EditFloat( groupBox, "Rotation:", "Texture coordinates rotation.",
		pPreviewEditTCRotation, new cEditPreviewEditTCRotation( *this ) );
	
	
	// property
	helper.GroupBoxFlow( content, groupBox, "Property:" );
	helper.ListBox( groupBox, 8, "Property", pListProperty, new cListProperties( *this ) );
	pListProperty->SetDefaultSorter();
	
	form.TakeOver( new igdeContainerForm( env ) );
	groupBox->AddChild( form );
	
	helper.EditString( form, "Name:", "Name of property", pEditPropName, NULL );
	pEditPropName->SetEditable( false );
	
	helper.EditString( form, "Renderable:", "Name of the renderable to use or an empty string to use none",
		pEditPropRenderable, new cTextPropertyRenderable( *this ) );
	
	helper.ComboBox( form, "Value Type:", "Type of value to use for the property",
		pCBPropertyType, new cComboPropertyValueType( *this ) );
	pCBPropertyType->AddItem( "Value", NULL, ( void* )( intptr_t )seProperty::evtValue );
	pCBPropertyType->AddItem( "Color", NULL, ( void* )( intptr_t )seProperty::evtColor );
	pCBPropertyType->AddItem( "Image", NULL, ( void* )( intptr_t )seProperty::evtImage );
	pCBPropertyType->AddItem( "Video", NULL, ( void* )( intptr_t )seProperty::evtVideo );
	pCBPropertyType->AddItem( "Constructed", NULL, ( void* )( intptr_t )seProperty::evtConstructed );
	pCBPropertyType->AddItem( "Mapped", NULL, ( void* )( intptr_t )seProperty::evtMapped );
	
	
	// property panel switcher
	pSwitcher.TakeOver( new igdeSwitcher( env ) );
	content->AddChild( pSwitcher );
	
	
	// empty
	helper.Label( pSwitcher, "", "" );
	
	
	// static value
	panel.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	pSwitcher->AddChild( panel );
	helper.GroupBox( panel, groupBox, "Value:" );
	helper.EditFloat( groupBox, "Value:", "Value to use", pEditPvtValue, new cTextPropertyValue( *this ) );
	
	
	// color value
	panel.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	pSwitcher->AddChild( panel );
	helper.GroupBox( panel, groupBox, "Color:" );
	helper.ColorBox( groupBox, "Color:", "Color to use", pClrPvtColor, new cColorPropertyColor( *this ) );
	
	
	// image
	panel.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	pSwitcher->AddChild( panel );
	helper.GroupBox( panel, groupBox, "Image:" );
	helper.EditPath( groupBox, "Path:", "Path to the image to use", igdeEnvironment::efpltImage,
		pEditPvtImagePath, new cPathPropertyImage( *this ) );
	
	helper.EditString( groupBox, "", "Image information", pLabPvtImageInfo, NULL );
	pLabPvtImageInfo->SetEditable( false );
	
	
	// video
	panel.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	pSwitcher->AddChild( panel );
	helper.GroupBox( panel, groupBox, "Video:" );
	helper.EditPath( groupBox, "Path:", "Path to the video to use", igdeEnvironment::efpltVideo,
		pEditPvtVideoPath, new cPathPropertyVideo( *this ) );
	
	helper.EditString( groupBox, "", "Video information", pLabPvtVideoInfo, NULL );
	pLabPvtVideoInfo->SetEditable( false );
	
	helper.CheckBox( groupBox, pChkPvtVideoSharedTime, new cActionPropertyVideoUseShared( *this ), true );
	
	
	// constructed
	panel.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	pSwitcher->AddChild( panel );
	helper.GroupBox( panel, groupBox, "Constructed:" );
	helper.ColorBox( groupBox, "Color:", "Color to use",
		pConstructedClrColor, new cColorPropertyConstructed( *this ) );
	helper.EditPoint3( groupBox, "Size:", "Size of constructed image",
		pConstructedEditSize, new cEditPropertyConstructedSize( *this ) );
	
	helper.FormLine( groupBox, "Tile:", "Tile nodes", formLine );
	helper.CheckBox( formLine, pConstructedChkTileX, new cActionConstructedChkTileX( *this ), true );
	helper.CheckBox( formLine, pConstructedChkTileY, new cActionConstructedChkTileY( *this ), true );
	
	
	// mapped
	panel.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	pSwitcher->AddChild( panel );
	helper.GroupBoxFlow( panel, groupBox, "Mapped:" );
	
	form.TakeOver( new igdeContainerForm( env ) );
	groupBox->AddChild( form );
	
	helper.EditSpinInteger( form, "Component:", "Component to edit (0=red, 1=green, 2=blue, 3=alpha)",
		0, 3, pSpinPvtMappedComponent, new cSpinPropertyMappedComponent( *this ) );
	
	helper.ComboBox( form, "Input Type:", "Type of input to use for curve",
		pCBPvtMappedInputType, new cComboPropertyMappedInputType( *this ) );
	pCBPvtMappedInputType->AddItem( "Time", NULL,
		( void* )( intptr_t )deSkinPropertyMapped::eitTime );
	pCBPvtMappedInputType->AddItem( "BonePositionX", NULL,
		( void* )( intptr_t )deSkinPropertyMapped::eitBonePositionX );
	pCBPvtMappedInputType->AddItem( "BonePositionY", NULL,
		( void* )( intptr_t )deSkinPropertyMapped::eitBonePositionY );
	pCBPvtMappedInputType->AddItem( "BonePositionZ", NULL,
		( void* )( intptr_t )deSkinPropertyMapped::eitBonePositionZ );
	pCBPvtMappedInputType->AddItem( "BoneRotationX", NULL,
		( void* )( intptr_t )deSkinPropertyMapped::eitBoneRotationX );
	pCBPvtMappedInputType->AddItem( "BoneRotationY", NULL,
		( void* )( intptr_t )deSkinPropertyMapped::eitBoneRotationY );
	pCBPvtMappedInputType->AddItem( "BoneRotationZ", NULL,
		( void* )( intptr_t )deSkinPropertyMapped::eitBoneRotationZ );
	pCBPvtMappedInputType->AddItem( "BoneScaleX", NULL,
		( void* )( intptr_t )deSkinPropertyMapped::eitBoneScaleX );
	pCBPvtMappedInputType->AddItem( "BoneScaleY", NULL,
		( void* )( intptr_t )deSkinPropertyMapped::eitBoneScaleY );
	pCBPvtMappedInputType->AddItem( "BoneScaleZ", NULL,
		( void* )( intptr_t )deSkinPropertyMapped::eitBoneScaleZ );
	
	helper.EditFloat( form, "Input Lower:", "Lower input range",
		pEditPvtMappedInputLower, new cTextPropertyMappedInputLower( *this ) );
	helper.EditFloat( form, "Input Upper:", "Upper input range",
		pEditPvtMappedInputUpper, new cTextPropertyMappedInputUpper( *this ) );
	helper.CheckBox( form, pChkPvtMappedInputClamped, new cActionPropertyMappedInputClamped( *this ), true );
	
	helper.EditString( form, "Bone:", "Name of the bone to use if bone related input type is used",
		pEditPvtMappedBone, new cTextPropertyMappedBone( *this ) );
	
	helper.ViewCurveBezier( groupBox, pEditPvtMappedCurve, new cEditMappedComponentCurve( *this ) );
	pEditPvtMappedCurve->SetDefaultSize( decPoint( 200, 250 ) );
	pEditPvtMappedCurve->ClearCurve();
	pEditPvtMappedCurve->SetClamp( true );
	pEditPvtMappedCurve->SetClampMin( decVector2( 0.0f, 0.0f ) );
	pEditPvtMappedCurve->SetClampMax( decVector2( 1.0f, 1.0f ) );
}

seWPTexture::~seWPTexture(){
	SetSkin( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void seWPTexture::SetSkin( seSkin *skin ){
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
	
	UpdateTextureList();
}

seTexture *seWPTexture::GetTexture() const{
	return pSkin ? pSkin->GetActiveTexture() : NULL;
}

seProperty *seWPTexture::GetProperty() const{
	seTexture * const texture = GetTexture();
	return texture ? texture->GetActiveProperty() : NULL;
}



void seWPTexture::UpdateTextureList(){
	seTexture * const selection = GetTexture();
	
	pListTexture->RemoveAllItems();
	
	if( pSkin ){
		const seTextureList &list = pSkin->GetTextureList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			seTexture * const texture = list.GetAt( i );
			pListTexture->AddItem( texture->GetName(), NULL, texture );
		}
		
		pListTexture->SortItems();
	}
	
	pListTexture->SetSelectionWithData( selection );
	UpdateTexture();
}

void seWPTexture::SelectActiveTexture(){
	pListTexture->SetSelectionWithData( GetTexture() );
}

void seWPTexture::UpdateTexture(){
	const seTexture * const texture = GetTexture();
	
	if( texture ){
		pEditTexName->SetText( texture->GetName() );
		
	}else{
		pEditTexName->ClearText();
	}
	
	pEditTexName->SetEnabled( texture );
	
	UpdatePropertyList();
	UpdatePreviewParameters();
}

void seWPTexture::UpdatePreviewParameters(){
	const seTexture * const texture = GetTexture();
	
	if( texture ){
		pPreviewEditTCOffset->SetPoint( texture->GetTexCoordOffset() );
		pPreviewEditTCScaling->SetPoint( texture->GetTexCoordScaling() );
		pPreviewEditTCRotation->SetFloat( texture->GetTexCoordRotation() );
		
	}else{
		pPreviewEditTCOffset->SetPoint( decPoint() );
		pPreviewEditTCScaling->SetPoint( decPoint() );
		pPreviewEditTCRotation->ClearText();
	}
	
	const bool enabled = texture;
	pPreviewEditTCOffset->SetEnabled( enabled );
	pPreviewEditTCScaling->SetEnabled( enabled );
	pPreviewEditTCRotation->SetEnabled( enabled );
}

void seWPTexture::UpdatePropertyList(){
	const seTexture * const texture = GetTexture();
	seProperty * const selection = GetProperty();
	
	pListProperty->RemoveAllItems();
	
	if( texture ){
		const sePropertyList &list = texture->GetPropertyList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			seProperty * const property = list.GetAt( i );
			pListProperty->AddItem( property->GetName(), 0, property );
		}
		
		pListProperty->SortItems();
	}
	
	pListProperty->SetSelectionWithData( selection );
	ShowPropertyPanel();
	UpdateProperty();
}

void seWPTexture::SelectActiveProperty(){
	pListProperty->SetSelectionWithData( GetProperty() );
	
	// catches certain cases
	ShowPropertyPanel();
	UpdateProperty();
}

void seWPTexture::ShowPropertyPanel(){
	const seProperty * const property = GetProperty();
	
	if( property ){
		switch( property->GetValueType() ){
		case seProperty::evtValue:
			pSwitcher->SetCurrent( 1 );
			break;
			
		case seProperty::evtColor:
			pSwitcher->SetCurrent( 2 );
			break;
			
		case seProperty::evtImage:
			pSwitcher->SetCurrent( 3 );
			break;
			
		case seProperty::evtVideo:
			pSwitcher->SetCurrent( 4 );
			break;
			
		case seProperty::evtConstructed:
			pSwitcher->SetCurrent( 5 );
			break;
			
		case seProperty::evtMapped:
			pSwitcher->SetCurrent( 6 );
			break;
			
		default:
			pSwitcher->SetCurrent( 0 ); // empty
		}
		
	}else{
		pSwitcher->SetCurrent( 0 ); // empty
	}
}

void seWPTexture::UpdateProperty(){
	const seProperty * const property = GetProperty();
	
	if( property ){
		pEditPropName->SetText( property->GetName() );
		pEditPropRenderable->SetText( property->GetRenderableName() );
		pCBPropertyType->SetSelectionWithData( ( void* )( intptr_t )property->GetValueType() );
		
		pEditPvtValue->SetFloat( property->GetValue() );
		
		pClrPvtColor->SetColor( property->GetColor() );
		
		pEditPvtImagePath->SetPath( property->GetImagePath() );
		
		if( property->GetEngineImage() ){
			const deImage &image = *property->GetEngineImage();
			const char *type = "?";
			decString text;
			
			if( image.GetComponentCount() == 1 ){
				type = "Gray";
				
			}else if( image.GetComponentCount() == 2 ){
				type = "Gray-Alpha";
				
			}else if( image.GetComponentCount() == 3 ){
				type = "RGB";
				
			}else if( image.GetComponentCount() == 4 ){
				type ="RGBA";
			}
			
			text.Format( "%dx%dx%d, %s, %d-Bit", image.GetWidth(), image.GetHeight(),
				image.GetDepth(), type, image.GetBitCount() );
			pLabPvtImageInfo->SetText( text );
			
		}else{
			pLabPvtImageInfo->ClearText();
		}
		
		pEditPvtVideoPath->SetPath( property->GetVideoPath() );
		if( property->GetEngineVideo() ){
			const deVideo &video = *property->GetEngineVideo();
			const char *type = "?";
			decString text;
			
			switch( video.GetPixelFormat() ){
			case deVideo::epf420:
			case deVideo::epf422:
			case deVideo::epf444:
				type = "RGB";
				break;
				
			case deVideo::epf4444:
				type = "RGBA";
				break;
			}
			
			text.Format( "%dx%d %s, FPS %d, Frames %d, Playtime %.1fs", video.GetWidth(), video.GetHeight(),
				type, video.GetFrameRate(), video.GetFrameCount(), video.GetPlayTime() );
			pLabPvtVideoInfo->SetText( text );
			
		}else{
			pLabPvtVideoInfo->ClearText();
		}
		
		pConstructedClrColor->SetColor( property->GetNodeColor() );
		pConstructedEditSize->SetPoint3( property->GetNodeGroup()->GetSize() );
		
	}else{
		pEditPropName->ClearText();
		pEditPropRenderable->ClearText();
		pCBPropertyType->SetSelectionWithData( ( void* )( intptr_t )seProperty::evtValue );
		
		pEditPvtValue->ClearText();
		
		pClrPvtColor->SetColor( decColor( 1.0f, 1.0f, 1.0f ) );
		
		pEditPvtImagePath->ClearPath();
		pLabPvtImageInfo->ClearText();
		
		pEditPvtVideoPath->ClearPath();
		pLabPvtVideoInfo->ClearText();
		
		pConstructedClrColor->SetColor( decColor( 1.0f, 1.0f, 1.0f ) );
		pConstructedEditSize->SetPoint3( decPoint3() );
	}
	
	const bool enabled = property;
	pSpinPvtMappedComponent->SetEnabled( enabled );
	pEditPropName->SetEnabled( enabled );
	pEditPropRenderable->SetEnabled( enabled );
	
	pEditPvtValue->SetEnabled( enabled );
	
	pClrPvtColor->SetEnabled( enabled );
	
	pEditPvtImagePath->SetEnabled( enabled );
	
	pEditPvtVideoPath->SetEnabled( enabled );
	pChkPvtVideoSharedTime->GetAction()->Update();
	
	pConstructedClrColor->SetEnabled( enabled );
	pConstructedEditSize->SetEnabled( enabled );
	pConstructedChkTileX->GetAction()->Update();
	pConstructedChkTileY->GetAction()->Update();
	
	UpdatePropertyMappedComponent();
}

int seWPTexture::GetPropertyMappedComponentIndex() const{
	return pSpinPvtMappedComponent->GetValue();
}

void seWPTexture::UpdatePropertyMappedComponent(){
	const seProperty * const property = GetProperty();
	const int index = GetPropertyMappedComponentIndex();
	
	if( property ){
		const deSkinPropertyMapped::cComponent &component = property->GetMappedComponent( index );
		pEditPvtMappedCurve->SetCurve( component.GetCurve() );
		pCBPvtMappedInputType->SetSelectionWithData( ( void* )( intptr_t )component.GetInputType() );
		pEditPvtMappedInputLower->SetFloat( component.GetInputLower() );
		pEditPvtMappedInputUpper->SetFloat( component.GetInputUpper() );
		pEditPvtMappedBone->SetText( component.GetBone() );
		
	}else{
		pEditPvtMappedCurve->ClearCurve();
		pCBPvtMappedInputType->SetSelectionWithData( ( void* )( intptr_t )deSkinPropertyMapped::eitTime );
		pEditPvtMappedInputLower->ClearText();
		pEditPvtMappedInputUpper->ClearText();
		pEditPvtMappedBone->ClearText();
	}
	
	pChkPvtMappedInputClamped->GetAction()->Update();
	
	const bool enabled = property;
	pEditPvtMappedCurve->SetEnabled( enabled );
	pCBPvtMappedInputType->SetEnabled( enabled );
	pEditPvtMappedInputLower->SetEnabled( enabled );
	pEditPvtMappedInputUpper->SetEnabled( enabled );
	pEditPvtMappedBone->SetEnabled( enabled );
}
