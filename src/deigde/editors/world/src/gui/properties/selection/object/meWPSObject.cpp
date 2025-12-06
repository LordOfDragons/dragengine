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

#include "meWPSObject.h"
#include "meWPSObjectListener.h"
#include "../meWPSelection.h"
#include "../../meWPBrowser.h"
#include "../../meWPPropertyList.h"
#include "../../meWindowProperties.h"
#include "../../../meWindowMain.h"
#include "../../../../utils/meHelpers.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/meCamera.h"
#include "../../../../world/object/meObject.h"
#include "../../../../world/object/meObjectSelection.h"
#include "../../../../world/object/texture/meObjectTexture.h"
#include "../../../../world/meWorldGuiParameters.h"
#include "../../../../world/idgroup/meIDGroup.h"
#include "../../../../undosys/properties/object/meUSetObjectPosition.h"
#include "../../../../undosys/properties/object/meUSetObjectRotation.h"
#include "../../../../undosys/properties/object/meUSetObjectSize.h"
#include "../../../../undosys/properties/object/meUObjectSetScaling.h"
#include "../../../../undosys/properties/object/meUSetObjectClass.h"
#include "../../../../undosys/properties/object/property/meUObjectAddProperty.h"
#include "../../../../undosys/properties/object/property/meUObjectSetProperty.h"
#include "../../../../undosys/properties/object/property/meUObjectSetProperties.h"
#include "../../../../undosys/properties/object/property/meUObjectRemoveProperty.h"
#include "../../../../undosys/properties/object/property/meUObjectPropertyCopyToSelected.h"
#include "../../../../undosys/properties/object/property/meUObjectPropertyRemoveFromSelected.h"
#include "../../../../undosys/properties/object/property/meUObjectClonePropertiesToSelected.h"
#include "../../../../undosys/properties/object/attachBehavior/meUObjectSetAttachBehaviors.h"
#include "../../../../undosys/properties/object/texture/meUObjectAddTexture.h"
#include "../../../../undosys/properties/object/texture/meUObjectRemoveTexture.h"
#include "../../../../undosys/properties/object/texture/meUObjectTextureSetSkin.h"
#include "../../../../undosys/properties/object/texture/meUObjectTextureTCOffset.h"
#include "../../../../undosys/properties/object/texture/meUObjectTextureTCScaling.h"
#include "../../../../undosys/properties/object/texture/meUObjectTextureTCRotation.h"
#include "../../../../undosys/properties/object/texture/meUObjectTextureColorTint.h"
#include "../../../../undosys/properties/object/texture/meUObjectRemoveAllTextures.h"
#include "../../../../undosys/properties/object/texture/meUObjectRemoveUnusedTextures.h"
#include "../../../../undosys/properties/object/texture/meUObjectAddUsedTextures.h"
#include "../../../../undosys/properties/object/texture/meUObjectTextureCopyToSelected.h"
#include "../../../../undosys/properties/object/texture/meUObjectCloneTexturesToSelected.h"
#include "../../../../undosys/properties/object/texture/property/meUObjTexAddProperty.h"
#include "../../../../undosys/properties/object/texture/property/meUObjTexRemoveProperty.h"
#include "../../../../undosys/properties/object/texture/property/meUObjTexSetProperty.h"
#include "../../../../undosys/properties/object/texture/property/meUObjTexSetProperties.h"

#include <deigde/codec/igdeCodecPropertyString.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gamedefinition/class/igdeGDClassInherit.h>
#include <deigde/gamedefinition/class/component/igdeGDCComponent.h>
#include <deigde/gamedefinition/class/component/igdeGDCCTexture.h>
#include <deigde/gamedefinition/property/igdeGDProperty.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/composed/igdeEditSliderTextListener.h>
#include <deigde/gui/composed/igdeEditDVector.h>
#include <deigde/gui/composed/igdeEditDVectorListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/composed/igdeEditVector2.h>
#include <deigde/gui/composed/igdeEditVector2Listener.h>
#include <deigde/gui/event/igdeColorBoxListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeSpinTextFieldListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCommand.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>



// Actions
///////////

static const decString vEmptyString;

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	meWPSObject &pPanel;
	
public:
	cBaseTextFieldListener( meWPSObject &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		meObject * const object = pPanel.GetActiveObject();
		if( ! object ){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New( OnChanged( textField, object ) ));
		if( undo ){
			object->GetWorld()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, meObject *object ) = 0;
};

class cBaseAction : public igdeAction{
protected:
	meWPSObject &pPanel;
	
public:
	cBaseAction( meWPSObject &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		meObject * const object = pPanel.GetActiveObject();
		if( ! object ){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New( OnAction( object ) ));
		if( undo ){
			object->GetWorld()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( meObject *object ) = 0;
	
	virtual void Update(){
		if( pPanel.GetActiveObject() ){
			Update( *pPanel.GetActiveObject() );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const meObject & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	meWPSObject &pPanel;
	
public:
	cBaseComboBoxListener( meWPSObject &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		meObject * const object = pPanel.GetActiveObject();
		if( ! object ){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New( OnChanged( comboBox, object ) ));
		if( undo ){
			object->GetWorld()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, meObject *object ) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	meWPSObject &pPanel;
	
public:
	cBaseEditVectorListener( meWPSObject &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		meObject * const object = pPanel.GetActiveObject();
		if( ! object ){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New( OnChanged( editVector->GetVector(), object ) ));
		if( undo ){
			object->GetWorld()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( const decVector &vector, meObject *object ) = 0;
};

class cBaseEditDVectorListener : public igdeEditDVectorListener{
protected:
	meWPSObject &pPanel;
	
public:
	cBaseEditDVectorListener( meWPSObject &panel ) : pPanel( panel ){ }
	
	virtual void OnDVectorChanged( igdeEditDVector *editDVector ){
		meObject * const object = pPanel.GetActiveObject();
		if( ! object ){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New( OnChanged( editDVector->GetDVector(), object ) ));
		if( undo ){
			object->GetWorld()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( const decDVector &vector, meObject *object ) = 0;
};



class cSpinActive : public igdeSpinTextFieldListener{
	meWPSObject &pPanel;
	
public:
	cSpinActive( meWPSObject &panel ) : pPanel( panel ){ }
	
	virtual void OnValueChanged( igdeSpinTextField *textField ){
		meObjectSelection &selection = pPanel.GetWorld()->GetSelectionObject();
		const int index = textField->GetValue();
		
		selection.SetActive( index >= 0 && index <= selection.GetSelected().GetCount()
			? selection.GetSelected().GetAt( index ) : NULL );
		
		pPanel.GetWorld()->NotifyObjectSelectionChanged();
	}
};

class cComboClass : public cBaseComboBoxListener{
public:
	cComboClass( meWPSObject &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, meObject *object ){
		return comboBox->GetText() != object->GetClassName()
			? new meUSetObjectClass( object, comboBox->GetText() ) : NULL;
	}
};


class cActionMenuClass : public igdeActionContextMenu{
	meWPSObject &pPanel;
	
public:
	cActionMenuClass( meWPSObject &panel ) : igdeActionContextMenu( "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ), "Class menu" ),
	pPanel( panel ){}
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu ){
		if( ! pPanel.GetActiveObject() ){
			return;
		}
		
		meWindowMain &windowMain = pPanel.GetWPSelection().GetWindowProperties().GetWindowMain();
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, pPanel.GetActionClassBrowse() );
		helper.MenuCommand( contextMenu, pPanel.GetActionClassEdit() );
		helper.MenuSeparator( contextMenu );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectSubclassAsEclass() );
	}
};

class cActionClassBrowse : public cBaseAction{
public:
	cActionClassBrowse( meWPSObject &panel ) : cBaseAction( panel,
		"Browse Class", NULL, "Show class in class browser" ){}
	
	virtual igdeUndo *OnAction( meObject *object ){
		if( object->GetGDClass() ){
			pPanel.GetWPSelection().GetWindowProperties().SwitchToBrowser();
			pPanel.GetWPSelection().GetWindowProperties().GetBrowser().SelectObjectClass( object->GetGDClass() );
		}
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetActiveObject() );
	}
};

class cActionClassEdit : public cBaseAction{
public:
	cActionClassEdit( meWPSObject &panel ) : cBaseAction( panel,
		"Edit Class", NULL, "Edit class in Game Definition Editor" ){}
	
	virtual igdeUndo *OnAction( meObject *object ){
		if( object->GetGDClass() ){
			// TODO
		}
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetActiveObject() );
	}
};


class cActionMenuId : public igdeActionContextMenu{
	meWPSObject &pPanel;
	
public:
	cActionMenuId( meWPSObject &panel ) : igdeActionContextMenu( "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ), "ID menu" ),
	pPanel( panel ){}
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu ){
		if( ! pPanel.GetWorld() ){
			return;
		}
		
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, pPanel.GetActionIdFind() );
	}
};

class cActionIdFind : public igdeAction{
	meWPSObject &pPanel;
	
public:
	cActionIdFind( meWPSObject &panel ) : igdeAction( "Find ID...",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSearch ),
		"Find ID. Activates object if found." ), pPanel( panel ){}
	
	virtual void OnAction() override{
		meWorld * const world = pPanel.GetWorld();
		if( ! world ){
			return;
		}
		
		decString value;
		if( ! igdeCommonDialogs::GetString( &pPanel.GetWPSelection(), "Find ID", "ID:", value ) ){
			return;
		}
		
		value.ToLower();
		if( value.BeginsWith( "0h" ) ){
			value = value.GetMiddle( 2 );
		}
		
		decUniqueID id;
		try{
			id = decUniqueID( value );
			
		}catch( const deException & ){
			igdeCommonDialogs::ErrorFormat( &pPanel.GetWPSelection(),
				"Find ID", "Invalid ID '%s'", value.GetString() );
			return;
		}
		
		meObjectSelection &selection = world->GetSelectionObject();
		const meObject * const active = selection.GetActive();
		const meObjectList &list = world->GetObjects();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			meObject * const object = list.GetAt( i );
			if( object->GetID() != id ){
				continue;
			}
			
			if( object != active ){
				selection.Reset();
				selection.Add( object );
				selection.SetActive( object );
				world->NotifyObjectSelectionChanged();
				
				meCamera * const camera = world->GetActiveCamera();
				if( ! camera->HasHostObject() ){
					camera->SetPosition( object->GetPosition() + camera->GetViewMatrix()
						.TransformNormal( decDVector( 0.0, 0.0, -5.0 ) ) );
				}
			}
			return;
		}
		
		igdeCommonDialogs::ErrorFormat( &pPanel.GetWPSelection(),
			"Find ID", "ID '%s' not found", value.GetString() );
	}
	
	virtual void Update() override{
		SetEnabled( pPanel.GetWorld() != nullptr );
	}
};


class cEditPosition : public cBaseEditDVectorListener{
public:
	cEditPosition( meWPSObject &panel ) : cBaseEditDVectorListener( panel ){}
	
	virtual igdeUndo *OnChanged( const decDVector &vector, meObject *object ){
		if( object->GetPosition().IsEqualTo( vector ) ){
			return NULL;
		}
		return new meUSetObjectPosition( object, vector );
	}
};

class cActionResetPosition : public cBaseAction{
public:
	cActionResetPosition( meWPSObject &panel ) : cBaseAction( panel, "Reset Position", NULL, "Reset position to 0" ){ }
	
	virtual igdeUndo *OnAction( meObject *object ){
		return ! object->GetPosition().IsEqualTo( decVector( 1.0f, 1.0f, 1.0f ) )
			? new meUSetObjectPosition( object, decVector() ) : NULL;
	}
};

class cActionMenuPosition : public igdeActionContextMenu{
	meWPSObject &pPanel;
	
public:
	cActionMenuPosition( meWPSObject &panel ) : igdeActionContextMenu( "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ), "Position menu" ),
	pPanel( panel ){}
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu ){
		if( ! pPanel.GetActiveObject() ){
			return;
		}
		
		meWindowMain &windowMain = pPanel.GetWPSelection().GetWindowProperties().GetWindowMain();
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectCopyPositionX() );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectCopyPositionY() );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectCopyPositionZ() );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectCopyPositionXZ() );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectCopyPositionXYZ() );
		helper.Separator( contextMenu );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectSnapToGrid() );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectDropToGround() );
		helper.Separator( contextMenu );
		helper.MenuCommand( contextMenu, new cActionResetPosition( pPanel ), true );
	}
};

class cEditRotation : public cBaseEditVectorListener{
public:
	cEditRotation( meWPSObject &panel ) : cBaseEditVectorListener( panel ){}
	
	virtual igdeUndo *OnChanged( const decVector &vector, meObject *object ){
		if( object->GetRotation().IsEqualTo( vector ) ){
			return NULL;
		}
		return new meUSetObjectRotation( object, vector );
	}
};

class cActionResetRotation : public cBaseAction{
public:
	cActionResetRotation( meWPSObject &panel ) : cBaseAction( panel, "Reset Rotation", NULL, "Reset rotation to 0" ){ }
	
	virtual igdeUndo *OnAction( meObject *object ){
		return ! object->GetRotation().IsEqualTo( decVector( 1.0f, 1.0f, 1.0f ) )
			? new meUSetObjectRotation( object, decVector() ) : NULL;
	}
};

class cActionMenuRotation : public igdeActionContextMenu{
	meWPSObject &pPanel;
	
public:
	cActionMenuRotation( meWPSObject &panel ) : igdeActionContextMenu( "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ), "Rotation menu" ),
	pPanel( panel ){}
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu ){
		if( ! pPanel.GetActiveObject() ){
			return;
		}
		
		meWindowMain &windowMain = pPanel.GetWPSelection().GetWindowProperties().GetWindowMain();
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectCopyRotationX() );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectCopyRotationY() );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectCopyRotationZ() );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectCopyRotationXYZ() );
		helper.Separator( contextMenu );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectRotateL45() );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectRotateL90() );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectRotateR45() );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectRotateR90() );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectRotate180() );
		helper.Separator( contextMenu );
		helper.MenuCommand( contextMenu, new cActionResetRotation( pPanel ), true );
		helper.Separator( contextMenu );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectDropToGround() );
		helper.Separator( contextMenu );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectSnapToGrid() );
	}
};

static decVector uniformScaleVector( const decVector &oldValue,
const decVector &newValue, igdeGDClass::eScaleModes scaleMode ){
	switch( scaleMode ){
	case igdeGDClass::esmUniform:{
		decVector value( oldValue );
		
		if( fabsf( newValue.x - oldValue.x ) > 1e-3f ){
			if( fabsf( oldValue.x ) > 1e-3f ){
				const float factor = newValue.x / oldValue.x;
				value.x = newValue.x;
				value.y *= factor;
				value.z *= factor;
			}
		}
		
		if( fabsf( newValue.y - oldValue.y ) > 1e-3f ){
			if( fabsf( oldValue.y ) > 1e-3f ){
				const float factor = newValue.y / oldValue.y;
				value.x *= factor;
				value.y = newValue.y;
				value.z *= factor;
			}
		}
		
		if( fabsf( newValue.z - oldValue.z ) > 1e-3f ){
			if( fabsf( oldValue.z ) > 1e-3f ){
				const float factor = newValue.z / oldValue.z;
				value.x *= factor;
				value.y *= factor;
				value.z = newValue.z;
			}
		}
		
		return value;
		}
		
	case igdeGDClass::esmFree:
		return newValue;
		
	default:
		return oldValue;
	}
}

class cEditSize : public cBaseEditVectorListener{
public:
	cEditSize( meWPSObject &panel ) : cBaseEditVectorListener( panel ){}
	
	virtual igdeUndo *OnChanged( const decVector &vector, meObject *object ){
		const decVector size( uniformScaleVector( object->GetSize(), vector, object->GetScaleMode() ) );
		if( ! size.IsEqualTo( object->GetSize() ) ){
			return new meUSetObjectSize( object, size );
			
		}else{
			pPanel.UpdateGeometry();
			return NULL;
		}
	}
};

class cEditScaling : public cBaseEditVectorListener{
public:
	cEditScaling( meWPSObject &panel ) : cBaseEditVectorListener( panel ){}
	
	virtual igdeUndo *OnChanged( const decVector &vector, meObject *object ){
		const decVector scaling( uniformScaleVector( object->GetScaling(), vector, object->GetScaleMode() ) );
		if( ! scaling.IsEqualTo( object->GetScaling() ) ){
			return new meUObjectSetScaling( object, scaling );
			
		}else{
			pPanel.UpdateGeometry();
			return NULL;
		}
	}
};

class cActionResetScaling : public cBaseAction{
public:
	cActionResetScaling( meWPSObject &panel ) : cBaseAction( panel, "Reset Scaling", NULL, "Reset scaling to 1" ){ }
	
	virtual igdeUndo *OnAction( meObject *object ){
		return ! object->GetScaling().IsEqualTo( decVector( 1.0f, 1.0f, 1.0f ) )
			? new meUObjectSetScaling( object, decVector( 1.0f, 1.0f, 1.0f ) ) : NULL;
	}
};

class cActionMenuScale : public igdeActionContextMenu{
	meWPSObject &pPanel;
	
public:
	cActionMenuScale( meWPSObject &panel ) : igdeActionContextMenu( "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ), "Scale menu" ),
	pPanel( panel ){}
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu ){
		if( ! pPanel.GetActiveObject() ){
			return;
		}
		
		meWindowMain &windowMain = pPanel.GetWPSelection().GetWindowProperties().GetWindowMain();
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectCopyScaleX() );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectCopyScaleY() );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectCopyScaleZ() );
		helper.MenuCommand( contextMenu, windowMain.GetActionObjectCopyScaleXYZ() );
		helper.Separator( contextMenu );
		helper.MenuCommand( contextMenu, new cActionResetScaling( pPanel ), true );
	}
};


class cBaseActionTexture : public cBaseAction{
public:
	cBaseActionTexture( meWPSObject &panel, const char *text, igdeIcon *icon, const char *description ) :
	cBaseAction( panel, text, icon, description ){ }
	
	virtual igdeUndo *OnAction( meObject *object ){
		meObjectTexture * const texture = object->GetActiveTexture();
		return texture ? OnActionTexture( object, texture ) : NULL;
	}
	
	virtual igdeUndo *OnActionTexture( meObject *object, meObjectTexture *texture ) = 0;
};

class cBaseTextFieldListenerTexture : public cBaseTextFieldListener{
public:
	cBaseTextFieldListenerTexture( meWPSObject &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, meObject *object ){
		meObjectTexture * const texture = object->GetActiveTexture();
		return texture ? OnChangedTexture( textField, object, texture ) : NULL;
	}
	
	virtual igdeUndo *OnChangedTexture( igdeTextField *textField, meObject *object, meObjectTexture *texture ) = 0;
};

class cBaseEditVector2ListenerTexture : public igdeEditVector2Listener{
protected:
	meWPSObject &pPanel;
	
public:
	cBaseEditVector2ListenerTexture( meWPSObject &panel ) : pPanel( panel ){ }
	
	virtual void OnVector2Changed( igdeEditVector2 *editVector2 ){
		meObjectTexture * const texture = pPanel.GetActiveTexture();
		if( ! texture ){
			return;
		}
		igdeUndo::Ref undo(igdeUndo::Ref::New( OnChangedTexture( editVector2->GetVector2(), pPanel.GetActiveObject(), texture ) ));
		if( undo ){
			pPanel.GetWorld()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChangedTexture( const decVector2 &vector, meObject *object, meObjectTexture *texture ) = 0;
};



class cComboTexture : public cBaseComboBoxListener{
public:
	cComboTexture( meWPSObject &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, meObject *object ){
		if( comboBox->GetSelectedItem() ){
			object->SetActiveTexture( ( meObjectTexture* )comboBox->GetSelectedItem()->GetData() );
		}
		return NULL;
	}
};

class cActionTextureAddModel : public cBaseAction{
	const decString pTextureName;
	
public:
	cActionTextureAddModel( meWPSObject &panel, const char *name ) :
	cBaseAction( panel, name, NULL, "Add model texture" ), pTextureName( name ){ }
	
	virtual igdeUndo *OnAction( meObject *object ){
		meObjectTexture::Ref texture;
		meHelpers::CreateTexture( texture, object, pTextureName );
		
		object->GetWorld()->GetUndoSystem()->Add(meUObjectAddTexture::Ref::NewWith(object, texture));
		
		object->SetActiveTexture( texture );
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetActiveObject() && ! pPanel.GetActiveObject()->HasTextureNamed( pTextureName ) );
	}
};

class cActionTextureRemoveModel : public cBaseAction{
	const decString pTextureName;
	
public:
	cActionTextureRemoveModel( meWPSObject &panel, const char *name ) :
	cBaseAction( panel, name, NULL, "Remove model texture" ), pTextureName( name ){ }
	
	virtual igdeUndo *OnAction( meObject* ){
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetActiveObject() && pPanel.GetActiveObject()->HasTextureNamed( pTextureName ) );
	}
};

class cActionMenuTexture : public igdeActionContextMenu{
	meWPSObject &pPanel;
	
public:
	cActionMenuTexture( meWPSObject &panel ) : igdeActionContextMenu( "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ), "Texture menu" ),
	pPanel( panel ){}
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu ){
		const meObject * const object = pPanel.GetActiveObject();
		if( ! object ){
			return;
		}
		
		decStringList textureNames;
		object->GetModelTextureNameList( textureNames );
		textureNames.SortAscending();
		
		igdeEnvironment &env = pPanel.GetEnvironment();
		igdeUIHelper &helper = env.GetUIHelper();
		igdeMenuCascade::Ref cascade;
		int count, i;
		
		// add texture
		helper.MenuCommand( contextMenu, pPanel.GetActionTextureAddCustom() );
		helper.MenuCommand( contextMenu, pPanel.GetActionTextureAddAllModel() );
		
		cascade.TakeOver( new igdeMenuCascade( env, "Add From Model",
			env.GetStockIcon( igdeEnvironment::esiPlus ) ) );
		contextMenu.AddChild( cascade );
		
		count = textureNames.GetCount();
		for( i=0; i<count; i++ ){
			const decString &texture = textureNames.GetAt( i );
			helper.MenuCommand( cascade, new cActionTextureAddModel( pPanel, texture ), true );
		}
		
		// remove texture
		helper.MenuSeparator( contextMenu );
		helper.MenuCommand( contextMenu, pPanel.GetActionTextureRemove() );
		helper.MenuCommand( contextMenu, pPanel.GetActionTextureRemoveAll() );
		helper.MenuCommand( contextMenu, pPanel.GetActionTextureRemoveAllNotModel() );
		
		cascade.TakeOver( new igdeMenuCascade( env, "Remove From Model",
			env.GetStockIcon( igdeEnvironment::esiMinus ) ) );
		contextMenu.AddChild( cascade );
		
		count = textureNames.GetCount();
		for( i=0; i<count; i++ ){
			const decString &texture = textureNames.GetAt( i );
			helper.MenuCommand( cascade, new cActionTextureRemoveModel( pPanel, texture ), true );
		}
		
		// copy
		helper.MenuSeparator( contextMenu );
		helper.MenuCommand( contextMenu, pPanel.GetActionTextureCopyToSelected() );
		helper.MenuCommand( contextMenu, pPanel.GetActionTextureCloneToSelected() );
	}
};

class cActionTextureAddCustom : public cBaseAction{
public:
	cActionTextureAddCustom( meWPSObject &panel ) : cBaseAction( panel, "Add...",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add texture" ){}
	
	virtual igdeUndo *OnAction( meObject *object ){
		decString name;
		while( igdeCommonDialogs::GetString( &pPanel, "Add Texture", "Name:", name ) ){
			if( object->HasTextureNamed( name ) ){
				igdeCommonDialogs::Error( &pPanel, "Add Texture", "Texture name exists already" );
				continue;
			}
			
			meObjectTexture::Ref texture;
			meHelpers::CreateTexture( texture, object, name );
			
			pPanel.GetWorld()->GetUndoSystem()->Add(meUObjectAddTexture::Ref::NewWith(object, texture));
			object->SetActiveTexture( texture );
			break;
		}
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetActiveObject() );
	}
};

class cActionTextureAddAllModel : public cBaseAction{
public:
	cActionTextureAddAllModel( meWPSObject &panel ) : cBaseAction( panel, "Add All From Model",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add all textures from model" ){}
	
	virtual igdeUndo *OnAction( meObject *object ){
		meUObjectAddUsedTextures * const undo = new meUObjectAddUsedTextures( object );
		if( undo->GetTextureNameList().GetCount() == 0 ){
			undo->FreeReference();
			return NULL;
		}
		return undo;
	}
	
	virtual void Update(){
		const meObject * const object = pPanel.GetActiveObject();
		bool enabled = false;
		
		if( object ){
			decStringList textureNames;
			object->GetModelTextureNameList( textureNames );
			const int count = textureNames.GetCount();
			int i;
			
			for( i=0; i<count; i++ ){
				if( ! object->HasTextureNamed( textureNames.GetAt( i ) ) ){
					enabled = true;
					break;
				}
			}
		}
		
		SetEnabled( enabled );
	}
};

class cActionTextureRemove : public cBaseActionTexture{
public:
	cActionTextureRemove( meWPSObject &panel ) : cBaseActionTexture( panel, "Remove",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove texture" ){}
	
	virtual igdeUndo *OnActionTexture( meObject*, meObjectTexture *texture ){
		return new meUObjectRemoveTexture( texture );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetActiveTexture() );
	}
};

class cActionTextureRemoveAll : public cBaseAction{
public:
	cActionTextureRemoveAll( meWPSObject &panel ) : cBaseAction( panel, "Remove All",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove all textures" ){}
	
	virtual igdeUndo *OnAction( meObject *object ){
		return object->GetTextureCount() > 0 ? new meUObjectRemoveAllTextures( object ) : NULL;
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetActiveObject() && pPanel.GetActiveObject()->GetTextureCount() > 0 );
	}
};

class cActionTextureRemoveAllNotModel : public cBaseAction{
public:
	cActionTextureRemoveAllNotModel( meWPSObject &panel ) : cBaseAction( panel, "Remove All Not From Model",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove all textures not from model" ){}
	
	virtual igdeUndo *OnAction( meObject *object ){
		return new meUObjectRemoveUnusedTextures( object );
	}
	
	virtual void Update(){
		const meObject * const object = pPanel.GetActiveObject();
		bool enabled = false;
		
		if( object ){
			decStringList textureNames;
			object->GetModelTextureNameList( textureNames );
			const int count = textureNames.GetCount();
			int i;
			
			for( i=0; i<count; i++ ){
				if( object->HasTextureNamed( textureNames.GetAt( i ) ) ){
					enabled = true;
					break;
				}
			}
		}
		
		SetEnabled( enabled );
	}
};

class cActionTextureCopyToSelected : public cBaseActionTexture{
public:
	cActionTextureCopyToSelected( meWPSObject &panel ) : cBaseActionTexture( panel,
		"Copy To Selected", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
			"Copy texture from active object to selected objects" ){}
	
	virtual igdeUndo *OnActionTexture( meObject*, meObjectTexture *texture ){
		const meObjectList &list = pPanel.GetWorld()->GetSelectionObject().GetSelected();
		return list.GetCount() > 0 ? new meUObjectTextureCopyToSelected( list, texture ) : NULL;
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetActiveTexture() && pPanel.GetWorld()->GetSelectionObject().GetSelected().GetCount() > 1 );
	}
};

class cActionTextureCloneToSelected : public cBaseAction{
public:
	cActionTextureCloneToSelected( meWPSObject &panel ) : cBaseAction( panel, "Clone To Selected",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
			"Clone textures from active object to selected objects" ){}
	
	virtual igdeUndo *OnAction( meObject *object ){
		meObjectList list( pPanel.GetWorld()->GetSelectionObject().GetSelected() );
		list.RemoveIfPresent( object );
		if( list.GetCount() == 0 ){
			return NULL;
		}
		
		const int textureCount = object->GetTextureCount();
		meObjectTextureList textures;
		int i;
		for( i=0; i<textureCount; i++ ){
			textures.AddTexture( object->GetTextureAt( i ) );
		}
		
		return new meUObjectCloneTexturesToSelected( list, textures );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetActiveObject() && pPanel.GetWorld()->GetSelectionObject().GetSelected().GetCount() > 1 );
	}
};


class cEditObjectProperties : public meWPPropertyList {
	meWPSObject &pPanel;
	
public:
	cEditObjectProperties( meWPSObject &panel ) : meWPPropertyList( panel.GetEnvironment() ), pPanel( panel ){ }
	
	virtual decString GetGDDefaultValue( const char *key ) const{
		const meObject * const object = pPanel.GetActiveObject();
		decString value;
		if( object && object->GetGDClass() ){
			object->GetGDClass()->GetDefaultPropertyValue( key, value );
		}
		return value;
	}
	
	virtual const igdeGDProperty *GetGDProperty( const char *key ) const{
		const meObject * const object = pPanel.GetActiveObject();
		if(!object || !object->GetGDClass()){
			return nullptr;
		}
		
		const igdeGDClassManager &clsmgr = *pPanel.GetGameDefinition()->GetClassManager();
		const decStringList &attachBehaviors = object->GetAttachBehaviors();
		const int count = attachBehaviors.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			const igdeGDClass * const gdclass = clsmgr.GetNamed(attachBehaviors.GetAt(i));
			if(!gdclass){
				continue;
			}
			
			const igdeGDProperty * const property = gdclass->GetPropertyNamed(key);
			if(property){
				return property;
			}
		}
		
		return object->GetGDClass()->GetPropertyNamed(key);
	}
	
	virtual decStringSet GetGDPropertyKeys() const{
		const meObject * const object = pPanel.GetActiveObject();
		decStringSet keys;
		if(object){
			if(object->GetGDClass()){
				object->GetGDClass()->AddPropertyNames(keys, true);
			}
			
			const decStringList &attachBehaviors = object->GetAttachBehaviors();
			const int attachBehaviorCount = attachBehaviors.GetCount();
			if(attachBehaviorCount > 0 && pPanel.GetGameDefinition()){
				const igdeGDClassManager &classManager = *pPanel.GetGameDefinition()->GetClassManager();
				int i;
				
				for(i=0; i<attachBehaviorCount; i++){
					const igdeGDClass * const gdclass = classManager.GetNamed(attachBehaviors.GetAt(i));
					if(gdclass){
						gdclass->AddPropertyNames(keys, true);
					}
				}
			}
		}
		return keys;
	}
	
	virtual void OnPropertySelected( const decString &key, const decString & ){
		meObject * const object = pPanel.GetActiveObject();
		if( object ){
			object->SetActiveProperty( key );
		}
		pPanel.UpdateLight();
		pPanel.UpdateIdentifierLists();
	}
	
	virtual void AddContextMenuEntries( igdeUIHelper &helper, igdeMenuCascade &menu ){
		helper.MenuSeparator( menu );
		helper.MenuCommand( menu, pPanel.GetActionPropCopyToSel() );
		helper.MenuCommand( menu, pPanel.GetActionPropCloneToSel() );
		helper.MenuCommand( menu, pPanel.GetActionPropRemoveFromSel() );
	}
	
	virtual igdeUndo *UndoAddProperty( const decString &key, const decString &value ){
		meObject * const object = pPanel.GetActiveObject();
		return object ? new meUObjectAddProperty( object, key, value ) : NULL;
	}
	
	virtual igdeUndo *UndoRemoveProperty( const decString &key ){
		meObject * const object = pPanel.GetActiveObject();
		return object ? new meUObjectRemoveProperty( object, key, object->GetProperties().GetAt( key ) ) : NULL;
	}
	
	virtual igdeUndo *UndoSetProperty( const decString &key, const decString &oldValue, const decString &newValue ){
		meObject * const object = pPanel.GetActiveObject();
		return object ? new meUObjectSetProperty( object, key, oldValue, newValue ) : NULL;
	}
	
	virtual igdeUndo *UndoSetProperties( const decStringDictionary &properties ){
		meObject * const object = pPanel.GetActiveObject();
		return object ? new meUObjectSetProperties( object, properties ) : NULL;
	}
};

class cActionPropCopyToSel : public cBaseAction{
public:
	cActionPropCopyToSel( meWPSObject &panel ) : cBaseAction( panel, "Copy To Selected",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
			"Copy property from active object to all selected objects" ){}
	
	virtual igdeUndo *OnAction( meObject *object ){
		meObjectList list( pPanel.GetWorld()->GetSelectionObject().GetSelected() );
		const decString &property = pPanel.GetActiveProperty();
		list.RemoveIfPresent( object );
		return ! property.IsEmpty() && list.GetCount() > 0 ? new meUObjectPropertyCopyToSelected(
			list, property, object->GetProperties().GetAt( property ) ) : NULL;
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetActiveObject() && ! pPanel.GetActiveProperty().IsEmpty()
			&& pPanel.GetWorld()->GetSelectionObject().GetSelected().GetCount() > 1 );
	}
};

class cActionPropRemoveFromSel : public cBaseAction{
public:
	cActionPropRemoveFromSel( meWPSObject &panel ) : cBaseAction( panel, "Remove From Selected",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
			"Remove property from all selected objects" ){}
	
	virtual igdeUndo *OnAction( meObject* ){
		const meObjectList &list = pPanel.GetWorld()->GetSelectionObject().GetSelected();
		const decString &property = pPanel.GetActiveProperty();
		return ! property.IsEmpty() && list.GetCount() > 0 ?
			new meUObjectPropertyRemoveFromSelected( list, property ) : NULL;
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetActiveObject() && ! pPanel.GetActiveProperty().IsEmpty()
			&& pPanel.GetWorld()->GetSelectionObject().GetSelected().GetCount() > 0 );
	}
};

class cActionPropCloneToSel : public cBaseAction{
public:
	cActionPropCloneToSel( meWPSObject &panel ) : cBaseAction( panel, "Clone To Selected",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
			"Clone all properties from active object to all selected objects" ){}
	
	virtual igdeUndo *OnAction( meObject *object ){
		meObjectList list( pPanel.GetWorld()->GetSelectionObject().GetSelected() );
		list.RemoveIfPresent( object );
		return list.GetCount() > 0 ? new meUObjectClonePropertiesToSelected( list, object->GetProperties() ) : NULL;
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetActiveObject() && pPanel.GetWorld()->GetSelectionObject().GetSelected().GetCount() > 1 );
	}
};


class cActionAddAttachBehavior : public cBaseAction{
public:
	cActionAddAttachBehavior(meWPSObject &panel) : cBaseAction(panel, "Add Attachable Behavior...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add attachable behavior"){}
	
	igdeUndo *OnAction(meObject *object) override{
		decStringList names;
		if(pPanel.GetGameDefinition()){
			const igdeGDClassManager &classManager = *pPanel.GetGameDefinition()->GetClassManager();
			const int count = classManager.GetCount();
			int i;
			
			for(i=0; i<count; i++){
				const igdeGDClass &objectClass = *classManager.GetAt(i);
				if(objectClass.GetIsAttachableBehavior()){
					names.Add(objectClass.GetName());
				}
			}
			
			names.SortAscending();
		}
		
		decString name;
		if(!igdeCommonDialogs::GetString(&pPanel, "Add Attachable Behavior", "Behavior:", name, names)){
			return nullptr;
		}
		
		decStringList list(object->GetAttachBehaviors());
		list.Add(name);
		return new meUObjectSetAttachBehaviors(object, list);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveObject());
	}
};

class cActionRemoveAttachBehavior : public cBaseAction{
public:
	cActionRemoveAttachBehavior(meWPSObject &panel) : cBaseAction(panel, "Remove Attachable Behavior",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove attachable behavior"){}
	
	igdeUndo *OnAction(meObject *object) override{
		decStringList list(object->GetAttachBehaviors());
		list.RemoveFrom(object->GetActiveAttachBehavior());
		return new meUObjectSetAttachBehaviors(object, list);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveObject()
			&& pPanel.GetActiveObject()->GetActiveAttachBehavior() != -1);
	}
};

class cActionRemoveAllAttachBehaviors : public cBaseAction{
public:
	cActionRemoveAllAttachBehaviors(meWPSObject &panel) : cBaseAction(panel,
		"Remove All Attachable Behaviors",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove all attachable behaviors"){}
	
	igdeUndo *OnAction(meObject *object) override{
		if(pPanel.GetActiveObject()->GetAttachBehaviors().GetCount() == 0){
			return nullptr;
		}
		
		return new meUObjectSetAttachBehaviors(object, decStringList());
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveObject()
			&& pPanel.GetActiveObject()->GetAttachBehaviors().GetCount() > 0);
	}
};

class cListAttachBehavior : public igdeListBoxListener{
	meWPSObject &pPanel;
	bool &pPreventUpdate;
	
public:
	cListAttachBehavior(meWPSObject &panel, bool &preventUpdate) :
		pPanel(panel), pPreventUpdate(preventUpdate){}
	
	void OnSelectionChanged(igdeListBox *listBox) override{
		if(pPreventUpdate){
			return;
		}
		
		meObject * const object = pPanel.GetActiveObject();
		if(object){
			object->SetActiveAttachBehavior(listBox->GetSelection());
		}
	}
	
	void AddContextMenuEntries(igdeListBox *listBox, igdeMenuCascade &menu) override{
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand(menu, new cActionAddAttachBehavior(pPanel), true);
		helper.MenuCommand(menu, new cActionRemoveAttachBehavior(pPanel), true);
		helper.MenuCommand(menu, new cActionRemoveAllAttachBehaviors(pPanel), true);
	};
};


class cEditTextureSkin : public igdeEditPathListener{
	meWPSObject &pPanel;
	
public:
	cEditTextureSkin( meWPSObject &panel ) : pPanel( panel ){}
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		meObjectTexture * const texture = pPanel.GetActiveTexture();
		if( ! texture || texture->GetSkinPath() == editPath->GetPath() ){
			return;
		}
		
		meUObjectTextureSetSkin::Ref undo(meUObjectTextureSetSkin::Ref::New( new meUObjectTextureSetSkin( texture, editPath->GetPath() ) ));
		pPanel.GetWorld()->GetUndoSystem()->Add( undo );
	}
};

class cEditTextureTCOffset : public cBaseEditVector2ListenerTexture{
public:
	cEditTextureTCOffset( meWPSObject &panel ) : cBaseEditVector2ListenerTexture( panel ){}
	
	virtual igdeUndo *OnChangedTexture( const decVector2 &vector, meObject*, meObjectTexture *texture ){
		return ! texture->GetTexCoordOffset().IsEqualTo( vector )
			? new meUObjectTextureTCOffset( texture, vector ) : NULL;
	}
};

class cEditTextureTCScaling : public cBaseEditVector2ListenerTexture{
public:
	cEditTextureTCScaling( meWPSObject &panel ) : cBaseEditVector2ListenerTexture( panel ){}
	
	virtual igdeUndo *OnChangedTexture( const decVector2 &vector, meObject*, meObjectTexture *texture ){
		return ! texture->GetTexCoordScaling().IsEqualTo( vector )
			? new meUObjectTextureTCScaling( texture, vector ) : NULL;
	}
};

class cEditTextureTCRotation : public cBaseTextFieldListenerTexture{
public:
	cEditTextureTCRotation( meWPSObject &panel ) : cBaseTextFieldListenerTexture( panel ){}
	
	virtual igdeUndo *OnChangedTexture( igdeTextField *textField, meObject*, meObjectTexture *texture ){
		const float value = textField->GetFloat();
		return fabsf( texture->GetTexCoordRotation() - value ) > FLOAT_SAFE_EPSILON
			? new meUObjectTextureTCRotation( texture, value ) : NULL;
	}
};

class cEditTextureColorTint : public igdeColorBoxListener{
	meWPSObject &pPanel;
	
public:
	cEditTextureColorTint( meWPSObject &panel ) : pPanel( panel ){}
	
	virtual void OnColorChanged( igdeColorBox *colorBox ){
		meObjectTexture * const texture = pPanel.GetActiveTexture();
		if( ! texture || texture->GetColorTint().IsEqualTo( colorBox->GetColor() ) ){
			return;
		}
		
		meUObjectTextureColorTint::Ref undo(meUObjectTextureColorTint::Ref::New( new meUObjectTextureColorTint( texture, colorBox->GetColor() ) ));
		pPanel.GetWorld()->GetUndoSystem()->Add( undo );
	}
};


class cActionShowMissingTextures : public cBaseAction{
public:
	cActionShowMissingTextures( meWPSObject &panel ) : cBaseAction( panel, "Show Missing Textures",
		NULL, "Show mising textures using placeholder texture" ){}
	
	virtual igdeUndo *OnAction( meObject *object ){
		object->SetShowMissingTextures( ! object->GetShowMissingTextures() );
		return NULL;
	}
};


class cEditTextureProperties : public meWPPropertyList {
	meWPSObject &pPanel;
	
public:
	cEditTextureProperties( meWPSObject &panel ) : meWPPropertyList( panel.GetEnvironment() ), pPanel( panel ){ }
	
	virtual decString GetGDDefaultValue( const char *key ) const{
		const meObjectTexture * const texture = pPanel.GetActiveTexture();
		decString value;
		if( texture && texture->GetObject()->GetGDClass() ){
			texture->GetObject()->GetGDClass()->GetDefaultTexturePropertyValue( key, value );
		}
		return value;
	}
	
	virtual const igdeGDProperty *GetGDProperty( const char *key ) const{
		const meObjectTexture * const texture = pPanel.GetActiveTexture();
		return texture && texture->GetObject()->GetGDClass()
			? texture->GetObject()->GetGDClass()->GetTexturePropertyNamed( key ) : NULL;
	}
	
	virtual decStringSet GetGDPropertyKeys() const{
		const meObjectTexture * const texture = pPanel.GetActiveTexture();
		decStringSet keys;
		if( texture && texture->GetObject()->GetGDClass() ){
			texture->GetObject()->GetGDClass()->AddTexturePropertyNames( keys, true );
		}
		return keys;
	}
	
	virtual void OnPropertySelected( const decString &key, const decString & ){
		meObjectTexture * const texture = pPanel.GetActiveTexture();
		if( texture ){
			texture->SetActiveProperty( key );
		}
	}
	
	virtual igdeUndo *UndoAddProperty( const decString &key, const decString &value ){
		meObjectTexture * const texture = pPanel.GetActiveTexture();
		return texture ? new meUObjTexAddProperty( texture, key, value ) : NULL;
	}
	
	virtual igdeUndo *UndoRemoveProperty( const decString &key ){
		meObjectTexture * const texture = pPanel.GetActiveTexture();
		return texture ? new meUObjTexRemoveProperty( texture, key, texture->GetProperties().GetAt( key ) ) : NULL;
	}
	
	virtual igdeUndo *UndoSetProperty( const decString &key, const decString &oldValue, const decString &newValue ){
		meObjectTexture * const texture = pPanel.GetActiveTexture();
		return texture ? new meUObjTexSetProperty( texture, key, oldValue, newValue ) : NULL;
	}
	
	virtual igdeUndo *UndoSetProperties( const decStringDictionary &properties ){
		meObjectTexture * const texture = pPanel.GetActiveTexture();
		return texture ? new meUObjTexSetProperties( texture, properties ) : NULL;
	}
};


class cLightSlider : public igdeEditSliderTextListener{
protected:
	meWPSObject &pPanel;
	const igdeGDCLight::eProperties pProperty;
	
public:
	cLightSlider( meWPSObject &panel, igdeGDCLight::eProperties property ) :
		pPanel( panel ), pProperty( property ){ }
	
	virtual void OnSliderTextValueChanged( igdeEditSliderText *sliderText ){
		decString value;
		value.Format( "%g", sliderText->GetValue() );
		pPanel.SlideLightProperty( pProperty, value, false );
	}
	
	virtual void OnSliderTextValueChanging( igdeEditSliderText *sliderText ){
		decString value;
		value.Format( "%g", sliderText->GetValue() );
		pPanel.SlideLightProperty( pProperty, value, true );
	}
};

class cLightColor : public igdeColorBoxListener{
protected:
	meWPSObject &pPanel;
	
public:
	cLightColor( meWPSObject &panel ) : pPanel( panel ){ }
	
	virtual void OnColorChanged( igdeColorBox *colorBox ){
		igdeCodecPropertyString codec;
		decString value;
		codec.EncodeColor3( colorBox->GetColor(), value );
		pPanel.SlideLightProperty( igdeGDCLight::epColor, value, false );
	}
};

class cLightIntForDistSlider : public igdeEditSliderTextListener{
protected:
	meWPSObject &pPanel;
	
public:
	cLightIntForDistSlider( meWPSObject &panel ) : pPanel( panel ){ }
	
	virtual void OnSliderTextValueChanged( igdeEditSliderText* ){
		pPanel.UpdateIntensityForDistance();
	}
	
	virtual void OnSliderTextValueChanging( igdeEditSliderText* ){
		pPanel.UpdateIntensityForDistance();
	}
};

}


// Class meWPSObject
/////////////////////

// Constructor, destructor
////////////////////////////

meWPSObject::meWPSObject( meWPSelection &wpselection ) :
igdeContainerScroll( wpselection.GetEnvironment(), false, true ),
pWPSelection( wpselection ),
pListener( NULL ),
pWorld( NULL ),
pPreventUpdate(false)
{
	igdeEnvironment &env = wpselection.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener = new meWPSObjectListener( *this );
	
	
	pActionMenuClass.TakeOver( new cActionMenuClass( *this ) );
	pActionClassBrowse.TakeOver( new cActionClassBrowse( *this ) );
	pActionClassEdit.TakeOver( new cActionClassEdit( *this ) );
	
	pActionIdClass.TakeOver( new cActionMenuId( *this ) );
	pActionIdFind.TakeOver( new cActionIdFind( *this ) );
	
	pActionMenuPosition.TakeOver( new cActionMenuPosition( *this ) );
	pActionMenuRotation.TakeOver( new cActionMenuRotation( *this ) );
	pActionMenuScale.TakeOver( new cActionMenuScale( *this ) );
	
	pActionMenuTexture.TakeOver( new cActionMenuTexture( *this ) );
	pActionTextureAddCustom.TakeOver( new cActionTextureAddCustom( *this ) );
	pActionTextureAddAllModel.TakeOver( new cActionTextureAddAllModel( *this ) );
	pActionTextureRemove.TakeOver( new cActionTextureRemove( *this ) );
	pActionTextureRemoveAll.TakeOver( new cActionTextureRemoveAll( *this ) );
	pActionTextureRemoveAllNotModel.TakeOver( new cActionTextureRemoveAllNotModel( *this ) );
	pActionTextureCopyToSelected.TakeOver( new cActionTextureCopyToSelected( *this ) );
	pActionTextureCloneToSelected.TakeOver( new cActionTextureCloneToSelected( *this ) );
	
	pActionPropCopyToSel.TakeOver( new cActionPropCopyToSel( *this ) );
	pActionPropRemoveFromSel.TakeOver( new cActionPropRemoveFromSel( *this ) );
	pActionPropCloneToSel.TakeOver( new cActionPropCloneToSel( *this ) );
	
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	groupBox.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esLast, 10 ) );
	content->AddChild( groupBox );
	helper.Label( groupBox, "Selected:" );
	helper.EditInteger( groupBox, "Number of selected objects", 3, pEditSelCount, NULL );
	pEditSelCount->SetEditable( false );
	helper.Label( groupBox, "Active:" );
	helper.EditSpinInteger( groupBox, "Active object to edit", 0, 0, pSpinActive, new cSpinActive( *this ) );
	
	
	// geometry
	helper.GroupBox( content, groupBox, "Geometry:" );
	
	helper.FormLineStretchFirst( groupBox, "Class:", "Name of the active object class.", formLine );
	helper.ComboBoxFilter( formLine, true, "Name of the active object class.", pCBClass, new cComboClass( *this ) );
	pCBClass->SetDefaultSorter();
	helper.Button( formLine, pBtnClassMenu, pActionMenuClass );
	pActionMenuClass->SetWidget( pBtnClassMenu );
	
	helper.FormLineStretchFirst( groupBox, "ID:", "Unique object ID.", formLine );
	helper.EditString( formLine, "Unique object ID", pEditID, nullptr );
	pEditID->SetEditable( false );
	helper.Button( formLine, pBtnIdMenu, pActionIdClass );
	pActionIdClass->SetWidget( pBtnIdMenu );
	
	helper.EditString( groupBox, "Attach:", "ID of object to attach to", pEditAttach, nullptr );
	pEditAttach->SetEditable( false );
	
	helper.FormLineStretchFirst( groupBox, "Position:", "Position of the object.", formLine );
	helper.EditDVector( formLine, "Position of the object.", pEditPosition, new cEditPosition( *this ) );
	helper.Button( formLine, pBtnPositionMenu, pActionMenuPosition );
	pActionMenuPosition->SetWidget( pBtnPositionMenu );
	
	helper.FormLineStretchFirst( groupBox, "Rotation:", "Rotation of the object.", formLine );
	helper.EditVector( formLine, "Rotation of the object.", pEditRotation, new cEditRotation( *this ) );
	helper.Button( formLine, pBtnRotationMenu, pActionMenuRotation );
	pActionMenuRotation->SetWidget( pBtnRotationMenu );
	
	helper.FormLineStretchFirst( groupBox, "Scaling:", "Scaling of the object.", formLine );
	helper.EditVector( formLine, "Scaling of the object.", pEditScaling, new cEditScaling( *this ) );
	helper.Button( formLine, pBtnScaleMenu, pActionMenuScale );
	pActionMenuScale->SetWidget( pBtnScaleMenu );
	
	helper.EditVector( groupBox, "Size:", "Size of the object.", pEditSize, new cEditSize( *this ) );
	
	
	// properties
	helper.GroupBoxFlow( content, groupBox, "Properties:" );
	
	pEditProperties.TakeOver( new cEditObjectProperties( *this ) );
	groupBox->AddChild( pEditProperties );
	
	
	// attachable behaviors
	helper.GroupBoxFlow(content, groupBox, "Attachable Behaviors:", true, true);
	
	helper.ListBox(groupBox, 3, "Attachable behaviors of the object.",
		pListAttachBehaviors, new cListAttachBehavior(*this, pPreventUpdate));
	
	
	// display options
	helper.GroupBox( content, groupBox, "Display Options", true );
	
	helper.CheckBox( groupBox, pChkShowMissingTextures, new cActionShowMissingTextures( *this ), true );
	
	
	// texture
	helper.GroupBox( content, groupBox, "Texture:", true );
	
	helper.FormLineStretchFirst( groupBox, "Texture:", "Texture to edit.", formLine );
	helper.ComboBox( formLine, "Texture to edit.", pCBTexture, new cComboTexture( *this ) );
	pCBTexture->SetDefaultSorter();
	helper.Button( formLine, pBtnTextureMenu, pActionMenuTexture );
	pActionMenuTexture->SetWidget( pBtnTextureMenu );
	
	helper.EditPath( groupBox, "Skin:", "Skin to use for the texture",
		igdeEnvironment::efpltSkin, pEditTexSkin, new cEditTextureSkin( *this ) );
	
	
	// texture transform
	helper.GroupBox( content, groupBox, "Texture Transformation:", true );
	
	helper.EditVector2( groupBox, "Translation:", "Texture coordinates translation.",
		pEditTexTCOffset, new cEditTextureTCOffset( *this ) );
	helper.EditVector2( groupBox, "Scaling:", "Texture coordinates scaling.",
		pEditTexTCScaling, new cEditTextureTCScaling( *this ) );
	helper.EditFloat( groupBox, "Rotation:", "Texture coordinates rotation.",
		pEditTexTCRotation, new cEditTextureTCRotation( *this ) );
	helper.ColorBox( groupBox, "Tint:", "Color tint (affects renderable named 'tint').", pClrTexTint, new cEditTextureColorTint( *this ) );
	
	
	// texture properties tables
	helper.GroupBoxFlow( content, groupBox, "Texture Properties:", false, true );
	
	pEditTexProperties.TakeOver( new cEditTextureProperties( *this ) );
	groupBox->AddChild( pEditTexProperties );
	
	
	// light properties
	helper.GroupBox( content, groupBox, "Light:", true );
	
	helper.EditSliderText( groupBox, "Intensity:", "Intensity of the light.", 0.0f, 10.0f,
		5, 2, 1.0f, pSldLigInt, new cLightSlider( *this, igdeGDCLight::epIntensity ) );
	helper.ColorBox( groupBox, "Color:", "Color of the light.", pClrLight, new cLightColor( *this ) );
	helper.EditSliderText( groupBox, "Range:", "Range of the light in meters.", 0.0f, 10.0f,
		5, 2, 1.0f, pSldLigRange, new cLightSlider( *this, igdeGDCLight::epRange ) );
	helper.EditSliderText( groupBox, "Half-Int Dist:",
		"Half intensity distance as percentage of light range.", 0.0f, 1.0f,
		5, 2, 0.1f, pSldLigHID, new cLightSlider( *this, igdeGDCLight::epHalfIntDist ) );
	
	helper.EditSliderText( groupBox, "Distance:",
		"Distance to the light source to evaluate distance for.", 0.0f, 10.0f,
		5, 2, 1.0f, pSldLigDist, new cLightIntForDistSlider( *this ) );
	helper.EditString( groupBox, "Intensity:",
		"Evaluated intensity at distance to light source.", pEditLigDistInt, NULL );
	pEditLigDistInt->SetEditable( false );
}

meWPSObject::~meWPSObject(){
	SetWorld( NULL );
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void meWPSObject::SetWorld( meWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	meWPPropertyList &editProperties = ( meWPPropertyList& )( igdeWidget& )pEditProperties;
	meWPPropertyList &editTextureProperties = ( meWPPropertyList& )( igdeWidget& )pEditTexProperties;
	
	if( pWorld ){
		editProperties.SetClipboard( NULL );
		editProperties.SetUndoSystem( NULL );
		editProperties.SetTriggerTargetList( NULL );
		editProperties.SetIdentifiers( decStringSet() );
		
		editTextureProperties.SetClipboard( NULL );
		editTextureProperties.SetUndoSystem( NULL );
		editTextureProperties.SetTriggerTargetList( NULL );
		editTextureProperties.SetIdentifiers( decStringSet() );
		
		pWorld->RemoveNotifier( pListener );
		pWorld->FreeReference();
	}
	
	pWorld = world;
	
	if( world ){
		world->AddNotifier( pListener );
		world->AddReference();
		
		editProperties.SetClipboard( &pWPSelection.GetWindowProperties().GetWindowMain().GetClipboard() );
		editProperties.SetUndoSystem( world->GetUndoSystem() );
		editProperties.SetTriggerTargetList( &world->GetTriggerTable() );
		
		editTextureProperties.SetClipboard( &pWPSelection.GetWindowProperties().GetWindowMain().GetClipboard() );
		editTextureProperties.SetUndoSystem( world->GetUndoSystem() );
		editTextureProperties.SetTriggerTargetList( &world->GetTriggerTable() );
	}
	
	UpdateClassList();
	
	UpdateSelection();
	UpdateEnabled();
	UpdateObject();
	UpdateGeometry();
	UpdateLight();
	UpdatePropertyKeys();
	UpdateProperties();
	UpdateAttachBehaviors();
	
	UpdateTextureList();
	UpdateTextureEnabled();
	UpdateTexture();
	UpdateTexPropertyKeys();
	UpdateTexProperties();
	
	UpdateIdentifierLists();
}



meObject *meWPSObject::GetActiveObject() const{
	return pWorld ? pWorld->GetSelectionObject().GetActive() : NULL;
}

const decString &meWPSObject::GetActiveProperty() const{
	meObject * const object = GetActiveObject();
	return object ? object->GetActiveProperty() : vEmptyString;
}

meObjectTexture *meWPSObject::GetActiveTexture() const{
	meObject * const object = GetActiveObject();
	return object ? object->GetActiveTexture() : NULL;
}

const decString &meWPSObject::GetActiveTexProperty() const{
	meObjectTexture * const texture = GetActiveTexture();
	return texture ? texture->GetActiveProperty() : vEmptyString;
}



void meWPSObject::UpdateClassList(){
	const decString selection( pCBClass->GetText() );
	
	pCBClass->ReleaseFilterItems();
	pCBClass->RemoveAllItems();
	
	const igdeGDClassManager &classManager = *GetGameDefinition()->GetClassManager();
	const int count = classManager.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const igdeGDClass &objectClass = *classManager.GetAt( i );
		if( objectClass.GetCanInstantiate() ){
			pCBClass->AddItem( objectClass.GetName() );
		}
	}
	
	pCBClass->SortItems();
	pCBClass->StoreFilterItems();
	
	pCBClass->SetText( selection );
}

void meWPSObject::UpdateSelection(){
	int active = 0;
	int count = 0;
	
	pUndoSetProperty = NULL;
	pUndoAddProperty = NULL;
	
	if( pWorld ){
		const meObjectSelection &selection = pWorld->GetSelectionObject();
		count = selection.GetSelected().GetCount();
		if( selection.HasActive() ){
			active = selection.GetSelected().IndexOf( selection.GetActive() );
		}
	}
	
	pEditSelCount->SetInteger( count );
	pSpinActive->SetRange( 0, count - 1 );
	pSpinActive->SetValue( active );
	pSpinActive->SetEnabled( count > 0 );
}

void meWPSObject::UpdateEnabled(){
	const bool enabled = GetActiveObject() != NULL;
	
	pEditPosition->SetEnabled( enabled );
	pEditSize->SetEnabled( enabled );
	pEditScaling->SetEnabled( enabled );
	pEditRotation->SetEnabled( enabled );
	pChkShowMissingTextures->SetEnabled( enabled );
}

void meWPSObject::UpdateObject(){
	const meObject * const object = GetActiveObject();
	
	if( object ){
		pCBClass->SetText( object->GetClassName() );
		
	}else{
		pCBClass->ClearText();
	}
}

void meWPSObject::UpdateGeometry(){
	const meObject * const object = GetActiveObject();
	
	if( object ){
		pEditID->SetText( object->GetID().ToHexString() );
		
		if( object->GetAttachedTo() ){
			pEditAttach->SetText( object->GetAttachedTo()->GetID().ToHexString() );
			
		}else{
			pEditAttach->SetText( "<none>" );
		}
		
		pEditPosition->SetDVector( object->GetPosition() );
		pEditSize->SetVector( object->GetSize() );
		pEditScaling->SetVector( object->GetScaling() );
		pEditRotation->SetVector( object->GetRotation() );
		pChkShowMissingTextures->SetChecked( object->GetShowMissingTextures() );
		
	}else{
		pEditID->ClearText();
		pEditAttach->ClearText();
		pEditPosition->SetDVector( decDVector() );
		pEditSize->SetVector( decVector( 1.0f, 1.0f, 1.0f ) );
		pEditScaling->SetVector( decVector( 1.0f, 1.0f, 1.0f ) );
		pEditRotation->SetVector( decVector() );
		pChkShowMissingTextures->SetChecked( false );
	}
}

void meWPSObject::UpdateLight(){
	const meObject * const object = GetActiveObject();
	igdeGDCLight *gdLight = NULL;
	decString gdpPrefix;
	if( object && object->GetGDClass() ){
		meHelpers::FindFirstLight( *object->GetGDClass(), gdpPrefix, gdLight );
	}
	
	if( gdLight && object ){
		decString value;
		
		// intensity
		const decString &pnameIntensity = gdLight->GetPropertyName( igdeGDCLight::epIntensity );
		if( ! pnameIntensity.IsEmpty() ){
			pSldLigInt->SetEnabled( true );
			
			if( pPropertyValue( *object, gdpPrefix, pnameIntensity, value ) ){
				pSldLigInt->SetValue( value.ToFloat() );
				
			}else{
				pSldLigInt->SetValue( gdLight->GetIntensity() );
			}
			
		}else{
			pSldLigInt->SetEnabled( false );
			pSldLigInt->SetValue( 1.0f );
		}
		
		// color
		const decString &pnameColor = gdLight->GetPropertyName( igdeGDCLight::epColor );
		if( ! pnameColor.IsEmpty() ){
			pClrLight->SetEnabled( true );
			
			if( pPropertyValue( *object, gdpPrefix, pnameColor, value ) ){
				try{
					decColor color;
					igdeCodecPropertyString().DecodeColor( value, color, decColor( 1.0f, 1.0f, 1.0f ) );
					pClrLight->SetColor( color );
					
				}catch( const deException & ){
					GetLogger()->LogInfoFormat( "WorldEditor", "Invalid color value '%s'", value.GetString() );
					pClrLight->SetColor( decColor( 1.0f, 1.0f, 1.0f ) );
				}
				
			}else{
				pClrLight->SetColor( gdLight->GetColor() );
			}
			
		}else{
			pClrLight->SetEnabled( false );
			pClrLight->SetColor( decColor( 1.0f, 1.0f, 1.0f ) );
		}
		
		// range
		const decString &pnameRange = gdLight->GetPropertyName( igdeGDCLight::epRange );
		if( ! pnameRange.IsEmpty() ){
			pSldLigRange->SetEnabled( true );
			
			if( pPropertyValue( *object, gdpPrefix, pnameRange, value ) ){
				pSldLigRange->SetValue( value.ToFloat() );
				
			}else{
				pSldLigRange->SetValue( gdLight->GetRange() );
			}
			
		}else{
			pSldLigRange->SetEnabled( false );
			pSldLigRange->SetValue( 10.0f );
		}
		
		// half intensity distance
		const decString &pnameHalfIntDist = gdLight->GetPropertyName( igdeGDCLight::epHalfIntDist );
		if( ! pnameHalfIntDist.IsEmpty() ){
			pSldLigHID->SetEnabled( true );
			
			if( pPropertyValue( *object, gdpPrefix, pnameHalfIntDist, value ) ){
				pSldLigHID->SetValue( value.ToFloat() );
				
			}else{
				pSldLigHID->SetValue( gdLight->GetHalfIntensityDistance() );
			}
			
		}else{
			pSldLigHID->SetEnabled( false );
			pSldLigHID->SetValue( 0.1f );
		}
		
	}else{
		pSldLigInt->SetValue( 1.0f );
		pClrLight->SetColor( decColor( 1.0f, 1.0f, 1.0f ) );
		pSldLigRange->SetValue( 10.0f );
		pSldLigHID->SetValue( 0.1f );
		
		pSldLigDist->SetValue( 0.0f );
		pEditLigDistInt->ClearText();
	}
	
	const bool enabled = gdLight;
	pSldLigInt->SetEnabled( enabled );
	pClrLight->SetEnabled( enabled );
	pSldLigRange->SetEnabled( enabled );
	pSldLigHID->SetEnabled( enabled );
	pSldLigDist->SetEnabled( enabled );
	pEditLigDistInt->SetEnabled( enabled );
}

void meWPSObject::SelectActiveProperty(){
	if( ! GetActiveProperty().IsEmpty() ){
		( ( meWPPropertyList& )( igdeWidget& )pEditProperties ).SelectProperty( GetActiveProperty() );
	}
}

void meWPSObject::UpdatePropertyKeys(){
	( ( meWPPropertyList& )( igdeWidget& )pEditProperties ).UpdateKeys();
}

void meWPSObject::UpdateProperties(){
	( ( meWPPropertyList& )( igdeWidget& )pEditProperties ).SetProperties(
		GetActiveObject() ? GetActiveObject()->GetProperties() : decStringDictionary() );
	SelectActiveProperty();
}

void meWPSObject::UpdateAttachBehaviors(){
	const meObject * const object = GetActiveObject();
	
	pPreventUpdate = true;
	try{
		pListAttachBehaviors->RemoveAllItems();
		
		if(object){
			const decStringList &list = object->GetAttachBehaviors();
			const int count = list.GetCount();
			int i;
			
			for(i=0; i<count; i++){
				pListAttachBehaviors->AddItem(list.GetAt(i));
			}
		}
		
		SelectActiveAttachBehavior();
		pPreventUpdate = false;
		
	}catch(const deException &){
		pPreventUpdate = false;
		throw;
	}
	
	pListAttachBehaviors->SetEnabled(object);
}

void meWPSObject::SelectActiveAttachBehavior(){
	meObject * const object = GetActiveObject();
	if(object){
		pListAttachBehaviors->SetSelection(object->GetActiveAttachBehavior());
	}
}


void meWPSObject::UpdateIntensityForDistance(){
	const float intensity = pSldLigInt->GetValue();
	const float range = pSldLigRange->GetValue();
	const float halfIntDist = pSldLigHID->GetValue();
	const float distance = pSldLigDist->GetValue();
	const float attquad = ( 1.0f / ( halfIntDist * halfIntDist ) - 2.0f ) / ( range * range );
	const float rangeInt = 1.0f / ( 1.0f + attquad * range * range );
	pEditLigDistInt->SetFloat( decMath::max( 0.0f, intensity
		* ( 1.0f / ( 1.0f + attquad * distance * distance ) - rangeInt ) / ( 1.0f - rangeInt ) ) );
}



void meWPSObject::SelectActiveTexture(){
	pCBTexture->SetSelectionWithData( GetActiveTexture() );
}

void meWPSObject::UpdateTextureList(){
	meObjectTexture * const texture = GetActiveTexture();
	const meObject * const object = GetActiveObject();
	
	pCBTexture->RemoveAllItems();
	
	if( object ){
		const int count = object->GetTextureCount();
		int i;
		
		for( i=0; i<count; i++ ){
			meObjectTexture * const texture2 = object->GetTextureAt( i );
			pCBTexture->AddItem( texture2->GetName(), NULL, texture2 );
		}
		
		pCBTexture->SortItems();
	}
	
	if( texture ){
		pCBTexture->SetSelectionWithData( texture );
	}
}

void meWPSObject::UpdateTextureEnabled(){
	const bool enabled = GetActiveTexture() != NULL;
	
	pEditTexSkin->SetEnabled( enabled );
	pEditTexTCOffset->SetEnabled( enabled );
	pEditTexTCScaling->SetEnabled( enabled );
	pEditTexTCRotation->SetEnabled( enabled );
	pClrTexTint->SetEnabled( enabled );
}

void meWPSObject::UpdateTexture(){
	const meObjectTexture * const texture = GetActiveTexture();
	
	if( texture ){
		pEditTexSkin->SetPath( texture->GetSkinPath() );
		pEditTexTCOffset->SetVector2( texture->GetTexCoordOffset() );
		pEditTexTCScaling->SetVector2( texture->GetTexCoordScaling() );
		pEditTexTCRotation->SetFloat( texture->GetTexCoordRotation() );
		pClrTexTint->SetColor( texture->GetColorTint() );
		
	}else{
		pEditTexSkin->ClearPath();
		pEditTexTCOffset->SetVector2( decVector2() );
		pEditTexTCScaling->SetVector2( decVector2() );
		pEditTexTCRotation->ClearText();
		pClrTexTint->SetColor( decColor( 1.0f, 1.0f, 1.0f ) );
	}
}

void meWPSObject::SelectTexActiveProperty(){
	if( ! GetActiveTexProperty().IsEmpty() ){
		( ( meWPPropertyList& )( igdeWidget& )pEditTexProperties ).SelectProperty( GetActiveTexProperty() );
	}
}

void meWPSObject::UpdateTexPropertyKeys(){
	( ( meWPPropertyList& )( igdeWidget& )pEditTexProperties ).UpdateKeys();
}

void meWPSObject::UpdateTexProperties(){
	( ( meWPPropertyList& )( igdeWidget& )pEditTexProperties ).SetProperties(
		GetActiveTexture() ? GetActiveTexture()->GetProperties() : decStringDictionary() );
	SelectTexActiveProperty();
}



void meWPSObject::UpdateIdentifierLists(){
	meWPPropertyList &editProperties = ( meWPPropertyList& )( igdeWidget& )pEditProperties;
	meWPPropertyList &editTextureProperties = ( meWPPropertyList& )( igdeWidget& )pEditTexProperties;
	
	if( ! pWorld ){
		editProperties.SetIdentifiers( decStringSet() );
		editTextureProperties.SetIdentifiers( decStringSet() );
		return;
	}
	
	// object properties
	decStringSet identifiers;
	const decString &property = GetActiveProperty();
	if( ! property.IsEmpty() ){
		const igdeGDProperty * const gdProperty = editProperties.GetGDProperty( property );
		if( gdProperty && gdProperty->GetType() == igdeGDProperty::eptIdentifier ){
			const meIDGroup * const idgroup = pWorld->GetIDGroupList().GetNamed( gdProperty->GetIdentifierGroup() );
			if( idgroup ){
				const decStringList &list = idgroup->GetIDList();
				const int count = list.GetCount();
				int i;
				
				for( i=0; i<count; i++ ){
					identifiers.Add( list.GetAt( i ) );
				}
			}
		}
	}
	editProperties.SetIdentifiers( identifiers );
	
	// texture properties
	identifiers.RemoveAll();
	const decString &texProperty = GetActiveTexProperty();
	if( ! texProperty.IsEmpty() ){
		const igdeGDProperty * const gdProperty = editTextureProperties.GetGDProperty( texProperty );
		if( gdProperty && gdProperty->GetType() == igdeGDProperty::eptIdentifier ){
			const meIDGroup * const idgroup = pWorld->GetIDGroupList().GetNamed( gdProperty->GetIdentifierGroup() );
			if( idgroup ){
				const decStringList &list = idgroup->GetIDList();
				const int count = list.GetCount();
				int i;
				
				for( i=0; i<count; i++ ){
					identifiers.Add( list.GetAt( i ) );
				}
			}
		}
	}
	editTextureProperties.SetIdentifiers( identifiers );
}

void meWPSObject::UpdateTriggerTargetLists(){
	igdeTriggerTargetList * const list = pWorld ? &pWorld->GetTriggerTable() : NULL;
	( ( meWPPropertyList& )( igdeWidget& )pEditProperties ).SetTriggerTargetList( list );
	( ( meWPPropertyList& )( igdeWidget& )pEditTexProperties ).SetTriggerTargetList( list );
}



void meWPSObject::SetDefaultSize(){
	/*
	meWindowMain *wndMain = pWPSelection->GetWindowProperties()->GetWindowMain();
	meObject *obj = wndMain->GetCurrentObject();
	igdeGameDefinition *gameDef = map->GetGameDefinition();
	igdeGDClass *classDef;
	
	// if classname matches a class use that size instead of the given one
	if( ! obj ) DETHROW( deeInvalidParam );
	classDef = gameDef->GetClassAt( obj->GetClassName() );
	if( classDef ){
		obj->SetSize( classDef->GetSize() );
	}else{
		obj->SetSize( decVector( 0.5f, 0.5f, 0.5f ) );
	}
	*/
}

void meWPSObject::OnGameDefinitionChanged(){
	UpdateClassList();
	UpdateIdentifierLists();
	
	UpdateLight();
	UpdatePropertyKeys();
	UpdateProperties();
	UpdateAttachBehaviors();
	( ( meWPPropertyList& )( igdeWidget&  )pEditProperties ).OnGameDefinitionChanged();
	
	UpdateTexPropertyKeys();
	UpdateTexProperties();
	( ( meWPPropertyList& )( igdeWidget&  )pEditTexProperties ).OnGameDefinitionChanged();
}

void meWPSObject::SlideLightProperty( igdeGDCLight::eProperties property, const char *value, bool scrubbing ){
	meObject * const object = GetActiveObject();
	if( ! object || ! object->GetGDClass() ){
		return;
	}
	
	igdeGDCLight *gdLight = NULL;
	decString gdpPrefix;
	meHelpers::FindFirstLight( *object->GetGDClass(), gdpPrefix, gdLight );
	
	if( ! gdLight || ! gdLight->IsPropertySet( property ) ){
		return;
	}
	
	const decString propertyName( gdpPrefix + gdLight->GetPropertyName( property ) );
	
	try{
		if( pUndoSetProperty ){
			( ( meUObjectSetProperty& )( igdeUndo& )pUndoSetProperty ).SetNewValue( value );
			( ( meUObjectSetProperty& )( igdeUndo& )pUndoSetProperty ).ProgressiveRedo();
			
		}else if( pUndoAddProperty ){
			( ( meUObjectAddProperty& )( igdeUndo& )pUndoAddProperty ).SetValue( value );
			( ( meUObjectAddProperty& )( igdeUndo& )pUndoAddProperty ).ProgressiveRedo();
			
		}else{
			if( object->GetProperties().Has( propertyName ) ){
				pUndoSetProperty.TakeOver( new meUObjectSetProperty( object, propertyName,
					object->GetProperties().GetAt( propertyName ), value ) );
				pUndoSetProperty->Redo();
				
			}else{
				pUndoAddProperty.TakeOver( new meUObjectAddProperty( object, propertyName, value ) );
				pUndoAddProperty->Redo();
			}
		}
		
		if( ! scrubbing ){
			if( pUndoSetProperty ){
				pWorld->GetUndoSystem()->Add( pUndoSetProperty );
				pUndoSetProperty = NULL;
				
			}else if( pUndoAddProperty ){
				pWorld->GetUndoSystem()->Add( pUndoAddProperty );
				pUndoAddProperty = NULL;
			}
		}
		
	}catch( const deException & ){
		pUndoSetProperty = NULL;
		pUndoAddProperty = NULL;
		throw;
	}
}



// Private Functions
//////////////////////

bool meWPSObject::pPropertyValue( const meObject &object, const decString &prefix,
const decString &name, decString &value ) const{
	if( name.IsEmpty() ){
		return false;
	}
	
	const decString propertyName( prefix + name );
	
	const decString *propertyValue;
	if( object.GetProperties().GetAt( propertyName, &propertyValue ) ){
		value = *propertyValue;
		return true;
	}
	
	const igdeGDClass * const gdClass = object.GetGDClass();
	decString defaultValue;
	if( gdClass && gdClass->GetDefaultPropertyValue( propertyName, defaultValue ) ){
		value = defaultValue;
		return true;
	}
	
	return false;
}
