/* 
 * Drag[en]gine IGDE Game Definition Editor
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
#include <stdint.h>

#include "gdeWPSOCComponent.h"
#include "gdeWPSOCComponentListener.h"
#include "../gdeWindowProperties.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/component/gdeOCComponent.h"
#include "../../../gamedef/objectClass/component/gdeOCComponentTexture.h"
#include "../../../gamedef/property/gdeProperty.h"
#include "../../../gamedef/skin/gdeSkin.h"
#include "../../../undosys/objectClass/component/gdeUOCAddComponent.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetSkinPath.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetAnimPath.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetPropertyName.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetColRespType.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetOccMeshPath.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetAudioModelPath.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentToggleDoNotScale.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetModelPath.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentTogglePartialHide.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentToggleAttachTarget.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentToggleRenderEnvMap.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentToggleAffectsAudio.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentToggleLightShadowIgnore.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentToggleStatic.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetPlaybackController.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetPosition.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetRotation.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetBoneName.h"
#include "../../../undosys/objectClass/component/gdeUOCRemoveComponent.h"
#include "../../../undosys/objectClass/component/gdeUOCComponentSetRigPath.h"
#include "../../../undosys/objectClass/component/texture/gdeUOCCAddTexture.h"
#include "../../../undosys/objectClass/component/texture/gdeUOCCRemoveTexture.h"
#include "../../../undosys/objectClass/component/texture/gdeUOCCTextureSetColorTint.h"
#include "../../../undosys/objectClass/component/texture/gdeUOCCTextureSetName.h"
#include "../../../undosys/objectClass/component/texture/gdeUOCCTextureSetOffset.h"
#include "../../../undosys/objectClass/component/texture/gdeUOCCTextureSetPathSkin.h"
#include "../../../undosys/objectClass/component/texture/gdeUOCCTextureSetProperties.h"
#include "../../../undosys/objectClass/component/texture/gdeUOCCTextureSetRotation.h"
#include "../../../undosys/objectClass/component/texture/gdeUOCCTextureSetScale.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeIconListBox.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/composed/igdeEditVector2.h>
#include <deigde/gui/composed/igdeEditVector2Listener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeIconListBoxListener.h>
#include <deigde/gui/event/igdeColorBoxListener.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelReference.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelManager.h>



// Actions
////////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	gdeWPSOCComponent &pPanel;
	
public:
	cBaseTextFieldListener( gdeWPSOCComponent &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		gdeOCComponent * const component = pPanel.GetComponent();
		if( ! component ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( *textField, pPanel.GetObjectClass(), component ) );
		if( undo ){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField &textField,
		gdeObjectClass *objectClass, gdeOCComponent *component ) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	gdeWPSOCComponent &pPanel;
	
public:
	cBaseEditVectorListener( gdeWPSOCComponent &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		gdeOCComponent * const component = pPanel.GetComponent();
		if( ! component ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( editVector->GetVector(), pPanel.GetObjectClass(), component ) );
		if( undo ){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( const decVector &vector, gdeObjectClass *objectClass,
		gdeOCComponent *component ) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	gdeWPSOCComponent &pPanel;
	
public:
	cBaseComboBoxListener( gdeWPSOCComponent &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		gdeOCComponent * const component = pPanel.GetComponent();
		if( ! component ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( *comboBox, pPanel.GetObjectClass(), component ) );
		if( undo ){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox,
		gdeObjectClass *objectClass, gdeOCComponent *component ) = 0;
};

class cBaseAction : public igdeAction{
protected:
	gdeWPSOCComponent &pPanel;
	
public:
	cBaseAction( gdeWPSOCComponent &panel, const char *text, const char *description ) :
		igdeAction( text, description ), pPanel( panel ){ }
	
	cBaseAction( gdeWPSOCComponent &panel, const char *text, igdeIcon *icon, const char *description ) :
		igdeAction( text, icon, description ), pPanel( panel ){ }
	
	virtual void OnAction(){
		gdeOCComponent * const component = pPanel.GetComponent();
		if( ! component ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnActionComponent( pPanel.GetObjectClass(), component ) );
		if( undo ){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnActionComponent( gdeObjectClass *objectClass, gdeOCComponent *component ) = 0;
	
	virtual void Update(){
		SetEnabled( pPanel.GetComponent() );
	}
};



class cEditPathModel : public igdeEditPathListener{
	gdeWPSOCComponent &pPanel;
	
public:
	cEditPathModel( gdeWPSOCComponent &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		gdeOCComponent * const component = pPanel.GetComponent();
		if( ! component || component->GetModelPath() == editPath->GetPath() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUOCComponentSetModelPath(
			pPanel.GetObjectClass(), component, editPath->GetPath() ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};

class cEditPathSkin : public igdeEditPathListener{
	gdeWPSOCComponent &pPanel;
	
public:
	cEditPathSkin( gdeWPSOCComponent &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		gdeOCComponent * const component = pPanel.GetComponent();
		if( ! component || component->GetSkinPath() == editPath->GetPath() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUOCComponentSetSkinPath(
			pPanel.GetObjectClass(), component, editPath->GetPath() ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};

class cEditPathRig : public igdeEditPathListener{
	gdeWPSOCComponent &pPanel;
	
public:
	cEditPathRig( gdeWPSOCComponent &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		gdeOCComponent * const component = pPanel.GetComponent();
		if( ! component || component->GetRigPath() == editPath->GetPath() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUOCComponentSetRigPath(
			pPanel.GetObjectClass(), component, editPath->GetPath() ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};

class cEditPathAnimator : public igdeEditPathListener{
	gdeWPSOCComponent &pPanel;
	
public:
	cEditPathAnimator( gdeWPSOCComponent &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		gdeOCComponent * const component = pPanel.GetComponent();
		if( ! component || component->GetAnimatorPath() == editPath->GetPath() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUOCComponentSetAnimPath(
			pPanel.GetObjectClass(), component, editPath->GetPath() ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};

class cEditPathOcclusionMesh : public igdeEditPathListener{
	gdeWPSOCComponent &pPanel;
	
public:
	cEditPathOcclusionMesh( gdeWPSOCComponent &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		gdeOCComponent * const component = pPanel.GetComponent();
		if( ! component || component->GetOcclusionMeshPath() == editPath->GetPath() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUOCComponentSetOccMeshPath(
			pPanel.GetObjectClass(), component, editPath->GetPath() ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};

class cEditPathAudioModel : public igdeEditPathListener{
	gdeWPSOCComponent &pPanel;
	
public:
	cEditPathAudioModel( gdeWPSOCComponent &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		gdeOCComponent * const component = pPanel.GetComponent();
		if( ! component || component->GetAudioModelPath() == editPath->GetPath() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUOCComponentSetAudioModelPath(
			pPanel.GetObjectClass(), component, editPath->GetPath() ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};

class cTextPlaybackController : public cBaseTextFieldListener{
public:
	cTextPlaybackController( gdeWPSOCComponent &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCComponent *component ){
		if( component->GetPlaybackController() == textField.GetText() ){
			return NULL;
		}
		return new gdeUOCComponentSetPlaybackController( objectClass, component, textField.GetText() );
	}
};

class cActionDoNotScale : public cBaseAction{
public:
	cActionDoNotScale( gdeWPSOCComponent &panel ) : cBaseAction( panel, "Do not scale",
		"Component scale does not change with scale of parent object class instance" ){ }
	
	virtual igdeUndo *OnActionComponent( gdeObjectClass *objectClass, gdeOCComponent *component ){
		return new gdeUOCComponentToggleDoNotScale( objectClass, component );
	}
	
	virtual void Update(){ /* empty on purpose! */ }
};

class cActionStatic : public cBaseAction{
public:
	cActionStatic( gdeWPSOCComponent &panel ) : cBaseAction( panel, "Static",
		"Component is static (optimization for graphic module)" ){ }
	
	virtual igdeUndo *OnActionComponent( gdeObjectClass *objectClass, gdeOCComponent *component ){
		return new gdeUOCComponentToggleStatic( objectClass, component );
	}
	
	virtual void Update(){ /* empty on purpose! */ }
};

class cActionRenderEnvMap : public cBaseAction{
public:
	cActionRenderEnvMap( gdeWPSOCComponent &panel ) : cBaseAction( panel, "Render Env-Map",
		"Component is included in environment maps" ){ }
	
	virtual igdeUndo *OnActionComponent( gdeObjectClass *objectClass, gdeOCComponent *component ){
		return new gdeUOCComponentToggleRenderEnvMap( objectClass, component );
	}
	
	virtual void Update(){ /* empty on purpose! */ }
};

class cActionAffectsAudio : public cBaseAction{
public:
	cActionAffectsAudio( gdeWPSOCComponent &panel ) : cBaseAction( panel, "Affects audio",
		"Component affects audio" ){ }
	
	virtual igdeUndo *OnActionComponent( gdeObjectClass *objectClass, gdeOCComponent *component ){
		return new gdeUOCComponentToggleAffectsAudio( objectClass, component );
	}
	
	virtual void Update(){ /* empty on purpose! */ }
};

class cActionLightShadowIgnore : public cBaseAction{
public:
	cActionLightShadowIgnore( gdeWPSOCComponent &panel ) : cBaseAction( panel, "Light Shadow Ignore",
		"Component is not casting shadows from lights present in the same object" ){ }
	
	virtual igdeUndo *OnActionComponent( gdeObjectClass *objectClass, gdeOCComponent *component ){
		return new gdeUOCComponentToggleLightShadowIgnore( objectClass, component );
	}
	
	virtual void Update(){ /* empty on purpose! */ }
};

class cActionPartialHide : public cBaseAction{
public:
	cActionPartialHide( gdeWPSOCComponent &panel ) : cBaseAction( panel, "Partial hide",
		"Component is hidden if partial hide tags match" ){ }
	
	virtual igdeUndo *OnActionComponent( gdeObjectClass *objectClass, gdeOCComponent *component ){
		return new gdeUOCComponentTogglePartialHide( objectClass, component );
	}
	
	virtual void Update(){ /* empty on purpose! */ }
};

class cActionAttachTarget : public cBaseAction{
public:
	cActionAttachTarget( gdeWPSOCComponent &panel ) : cBaseAction( panel, "Attach Target",
		"Resourcec can be attached to this component in the editor." ){ }
	
	virtual igdeUndo *OnActionComponent( gdeObjectClass *objectClass, gdeOCComponent *component ){
		return new gdeUOCComponentToggleAttachTarget( objectClass, component );
	}
	
	virtual void Update(){ /* empty on purpose! */ }
};

class cComboCollisionResponseType : public cBaseComboBoxListener{
public:
	cComboCollisionResponseType( gdeWPSOCComponent &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox, gdeObjectClass *objectClass,
	gdeOCComponent *component ){
		const deCollider::eResponseType responseType = ( deCollider::eResponseType )
			( intptr_t )comboBox.GetSelectedItem()->GetData();
		if( component->GetColliderResponseType() == responseType ){
			return NULL;
		}
		return new gdeUOCComponentSetColRespType( objectClass, component, responseType );
	}
};

class cEditPosition : public cBaseEditVectorListener {
public:
	cEditPosition( gdeWPSOCComponent &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( const decVector &vector, gdeObjectClass *objectClass,
	gdeOCComponent *component ){
		if( component->GetPosition().IsEqualTo( vector ) ){
			return NULL;
		}
		return new gdeUOCComponentSetPosition( objectClass, component, vector );
	}
};

class cEditRotation : public cBaseEditVectorListener {
public:
	cEditRotation( gdeWPSOCComponent &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( const decVector &vector, gdeObjectClass *objectClass,
	gdeOCComponent *component ){
		if( component->GetRotation().IsEqualTo( vector ) ){
			return NULL;
		}
		return new gdeUOCComponentSetRotation( objectClass, component, vector );
	}
};

class cTextBoneName : public cBaseTextFieldListener{
public:
	cTextBoneName( gdeWPSOCComponent &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCComponent *component ){
		if( component->GetBoneName() == textField.GetText() ){
			return NULL;
		}
		return new gdeUOCComponentSetBoneName( objectClass, component, textField.GetText() );
	}
};


class cComboPropertyNames : public igdeComboBoxListener{
	gdeWPSOCComponent &pPanel;
	
public:
	cComboPropertyNames( gdeWPSOCComponent &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox* ){
		if( pPanel.GetComponent() ){
			pPanel.UpdatePropertyName();
		}
	}
};

class cComboPropertyNameTarget : public igdeComboBoxListener{
	gdeWPSOCComponent &pPanel;
	
public:
	cComboPropertyNameTarget( gdeWPSOCComponent &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		gdeOCComponent * const component = pPanel.GetComponent();
		if( ! component ){
			return;
		}
		
		const gdeOCComponent::eProperties propertyName = pPanel.GetPropertyName();
		if( component->GetPropertyName( propertyName ) == comboBox->GetText() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUOCComponentSetPropertyName(
			pPanel.GetObjectClass(), component, propertyName, comboBox->GetText() ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};


// textures

class cBaseComboBoxTextureListener : public cBaseComboBoxListener{
public:
	cBaseComboBoxTextureListener( gdeWPSOCComponent &panel ) : cBaseComboBoxListener( panel ){ }
	
	igdeUndo *OnChanged( igdeComboBox &comboBox, gdeObjectClass *objectClass, gdeOCComponent *component ){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		return texture ? OnChangedTexture( comboBox, objectClass, component, texture ) : NULL;
	}
	
	virtual igdeUndo *OnChangedTexture( igdeComboBox &comboBox, gdeObjectClass *objectClass,
		gdeOCComponent *component, gdeOCComponentTexture *texture ) = 0;
};

class cBaseActionTexture : public cBaseAction{
public:
	cBaseActionTexture( gdeWPSOCComponent &panel, const char *text, const char *description ) :
		cBaseAction( panel, text, description ){ }
	
	cBaseActionTexture( gdeWPSOCComponent &panel, const char *text, igdeIcon *icon, const char *description ) :
		cBaseAction( panel, text, icon, description ){ }
	
	virtual igdeUndo *OnActionComponent( gdeObjectClass *objectClass, gdeOCComponent *component ){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		return texture ? OnActionTexture( objectClass, component, texture ) : NULL;
	}
	
	virtual igdeUndo *OnActionTexture( gdeObjectClass *objectClass,
		gdeOCComponent *component, gdeOCComponentTexture *texture ) = 0;
	
	virtual void Update(){
		SetEnabled( pPanel.GetTexture() );
	}
};

class cBaseTextFieldTextureListener : public cBaseTextFieldListener{
public:
	cBaseTextFieldTextureListener( gdeWPSOCComponent &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeObjectClass *objectClass, gdeOCComponent *component ){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		return texture ? OnChangedTexture( textField, objectClass, component, texture ) : NULL;
	}
	
	virtual igdeUndo *OnChangedTexture( igdeTextField &textField, gdeObjectClass *objectClass,
		gdeOCComponent *component, gdeOCComponentTexture *texture ) = 0;
};

class cBaseEditVector2TextureListener : public igdeEditVector2Listener{
protected:
	gdeWPSOCComponent &pPanel;
	
public:
	cBaseEditVector2TextureListener( gdeWPSOCComponent &panel ) : pPanel( panel ){ }
	
	virtual void OnVector2Changed( igdeEditVector2 *editVector2 ){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		if( ! texture ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChangedTexture( editVector2->GetVector2(),
			pPanel.GetObjectClass(), pPanel.GetComponent(), texture ) );
		if( undo ){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChangedTexture( const decVector2 &vector, gdeObjectClass *objectClass,
		gdeOCComponent *component, gdeOCComponentTexture *texture ) = 0;
};


class cComboTextures : public cBaseComboBoxListener{
public:
	cComboTextures( gdeWPSOCComponent &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox, gdeObjectClass *objectClass,
	gdeOCComponent *component ){
		const igdeListItem * const selection = comboBox.GetSelectedItem();
		component->SetActiveTexture( selection ? ( gdeOCComponentTexture* )selection->GetData() : NULL );
		pPanel.GetGameDefinition()->NotifyOCComponentActiveTextureChanged( objectClass, component );
		return NULL;
	}
};

class cActionTextureAdd : public cBaseAction{
	const decString pTextureName;
	
public:
	cActionTextureAdd( gdeWPSOCComponent &panel ) : cBaseAction( panel, "Add...",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add texture" ){ }
	
	cActionTextureAdd( gdeWPSOCComponent &panel, const decString &textureName ) :
		cBaseAction( panel, textureName, NULL, "Add texture" ), pTextureName( textureName ){ }
	
	virtual igdeUndo *OnActionComponent( gdeObjectClass *objectClass, gdeOCComponent *component ){
		decString name( pTextureName );
		
		if( name.IsEmpty() ){
			name = "Texture";
			
			while( true ){
				if( ! igdeCommonDialogs::GetString( &pPanel, "Add Texture", "Name:", name ) ){
					return NULL;
				}
				
				if( component->GetTextures().HasNamed( name ) ){
					igdeCommonDialogs::Error( &pPanel, "Add Texture", "A texture with this name exists already." );
					
				}else{
					break;
				}
			}
		}
		
		deObjectReference texture;
		texture.TakeOver( new gdeOCComponentTexture( name ) );
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUOCCAddTexture( objectClass, component, ( gdeOCComponentTexture* )( deObject* )texture ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
		
		component->SetActiveTexture( ( gdeOCComponentTexture* )( deObject* )texture );
		pPanel.GetGameDefinition()->NotifyOCComponentActiveTextureChanged( objectClass, component );
		return NULL;
	}
	
	virtual void Update(){
		const gdeOCComponent * const component = pPanel.GetComponent();
		SetEnabled( component && ( pTextureName.IsEmpty() || ! component->GetTextures().HasNamed( pTextureName ) ) );
	}
};

class cActionTextureRemove : public cBaseActionTexture{
public:
	cActionTextureRemove( gdeWPSOCComponent &panel ) : cBaseActionTexture( panel, "Remove",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove selected texture" ){ }
	
	virtual igdeUndo *OnActionTexture( gdeObjectClass *objectClass, gdeOCComponent *component,
	gdeOCComponentTexture *texture ){
		return new gdeUOCCRemoveTexture( objectClass, component, texture );
	}
};

class cActionTexturesMenu : public igdeActionContextMenu{
	gdeWPSOCComponent &pPanel;
	
public:
	cActionTexturesMenu( gdeWPSOCComponent &panel ) : igdeActionContextMenu( "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ),
		"Textures menu" ), pPanel( panel ){ }
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu ){
		pPanel.PrepareEngineModelPath();
		
		const decObjectList &actionsAddFromModel = pPanel.GetActionsTextureAddFromModel();
		igdeEnvironment &env = contextMenu.GetEnvironment();
		igdeUIHelper &helper = env.GetUIHelper();
		
		helper.MenuCommand( contextMenu, pPanel.GetActionTextureAdd() );
		
		if( actionsAddFromModel.GetCount() > 0 ){
			igdeMenuCascadeReference subMenu;
			subMenu.TakeOver( new igdeMenuCascade( env, "Add Texture From Model",
				env.GetStockIcon( igdeEnvironment::esiPlus ), "Add Texture From Model" ) );
			
			const int count = actionsAddFromModel.GetCount();
			int i;
			for( i=0; i<count; i++ ){
				helper.MenuCommand( subMenu, ( igdeAction* )( deObject* )actionsAddFromModel.GetAt( i ) );
			}
			
			contextMenu.AddChild( subMenu );
		}
		
		helper.MenuCommand( contextMenu, pPanel.GetActionTextureRemove() );
	}
};

class cTextTextureEditName : public cBaseTextFieldTextureListener{
public:
	cTextTextureEditName( gdeWPSOCComponent &panel ) : cBaseTextFieldTextureListener( panel ){ }
	
	virtual igdeUndo *OnChangedTexture( igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCComponent *component, gdeOCComponentTexture *texture ){
		if( texture->GetName() == textField.GetText() ){
			return NULL;
		}
		
		if( component->GetTextures().HasNamed( textField.GetText() ) ){
			igdeCommonDialogs::Information( &pPanel, "Rename texture", "A texture with this name exists already." );
			textField.SetText( texture->GetName() );
			return NULL;
		}
		
		return new gdeUOCCTextureSetName( objectClass, component, texture, textField.GetText() );
	}
};

class cEditTextureEditPathSkin : public igdeEditPathListener{
	gdeWPSOCComponent &pPanel;
	
public:
	cEditTextureEditPathSkin( gdeWPSOCComponent &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		if( ! texture || texture->GetPathSkin() == editPath->GetPath() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUOCCTextureSetPathSkin( pPanel.GetObjectClass(),
			pPanel.GetComponent(), texture, editPath->GetPath() ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};

class cEditTextureEditOffset : public cBaseEditVector2TextureListener{
public:
	cEditTextureEditOffset( gdeWPSOCComponent &panel ) : cBaseEditVector2TextureListener( panel ){ }
	
	virtual igdeUndo *OnChangedTexture( const decVector2 &vector, gdeObjectClass *objectClass,
	gdeOCComponent *component, gdeOCComponentTexture *texture ){
		if( texture->GetOffset().IsEqualTo( vector ) ){
			return NULL;
		}
		return new gdeUOCCTextureSetOffset( objectClass, component, texture, vector );
	}
};

class cTextTextureEditRotation : public cBaseTextFieldTextureListener{
public:
	cTextTextureEditRotation( gdeWPSOCComponent &panel ) : cBaseTextFieldTextureListener( panel ){ }
	
	virtual igdeUndo *OnChangedTexture( igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCComponent *component, gdeOCComponentTexture *texture ){
		const float value = textField.GetFloat();
		if( fabsf( texture->GetRotation() - value ) < FLOAT_SAFE_EPSILON ){
			return NULL;
		}
		return new gdeUOCCTextureSetRotation( objectClass, component, texture, value );
	}
};

class cEditTextureEditScale : public cBaseEditVector2TextureListener{
public:
	cEditTextureEditScale( gdeWPSOCComponent &panel ) : cBaseEditVector2TextureListener( panel ){ }
	
	virtual igdeUndo *OnChangedTexture( const decVector2 &vector, gdeObjectClass *objectClass,
	gdeOCComponent *component, gdeOCComponentTexture *texture ){
		if( texture->GetScale().IsEqualTo( vector ) ){
			return NULL;
		}
		return new gdeUOCCTextureSetScale( objectClass, component, texture, vector );
	}
};

class cColorTextureTint : public igdeColorBoxListener{
	gdeWPSOCComponent &pPanel;
	
public:
	cColorTextureTint( gdeWPSOCComponent &panel ) : pPanel( panel ){ }
	
	virtual void OnColorChanged( igdeColorBox *colorBox ){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		if( ! texture || texture->GetColorTint().IsEqualTo( colorBox->GetColor() ) ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUOCCTextureSetColorTint( pPanel.GetObjectClass(),
			pPanel.GetComponent(), texture, colorBox->GetColor() ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};


class cActionPropertyValueSet : public cBaseActionTexture {
public:
	cActionPropertyValueSet( gdeWPSOCComponent &panel ) : cBaseActionTexture( panel, "Set",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Set property value" ){ }
	
	virtual igdeUndo *OnActionTexture( gdeObjectClass *objectClass, gdeOCComponent *component,
	gdeOCComponentTexture *texture ){
		const decString &key = pPanel.GetTexturePropertyKey();
		if( ! texture || key.IsEmpty() ){
			return NULL;
		}
		
		const gdeObjectClass *propertyOwner = NULL;
		const gdeProperty *property = NULL;
		decString value;
		
		if( objectClass->DeepGetNamedProperty( key, propertyOwner, property ) ){
			value = property->GetDefaultValue();
		}
		
		if( ! igdeCommonDialogs::GetString( &pPanel, "Set Texture Property Value", "Value:", value ) ){
			return NULL;
		}
		
		decStringDictionary values( texture->GetProperties() );
		values.SetAt( key, value );
		return new gdeUOCCTextureSetProperties( objectClass, component, texture, values );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetTexture() && ! pPanel.GetTexturePropertyKey().IsEmpty() );
	}
};

class cActionPropertyValueRemove : public cBaseActionTexture {
public:
	cActionPropertyValueRemove( gdeWPSOCComponent &panel ) : cBaseActionTexture( panel, "Remove",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove property value" ){ }
	
	virtual igdeUndo *OnActionTexture( gdeObjectClass *objectClass, gdeOCComponent *component,
	gdeOCComponentTexture *texture ){
		const char * const key = pPanel.GetTexturePropertyValue();
		if( ! key || ! texture->GetProperties().Has( key ) ){
			return NULL;
		}
		
		decStringDictionary values( texture->GetProperties() );
		values.Remove( key );
		return new gdeUOCCTextureSetProperties( objectClass, component, texture, values );
	}
	
	virtual void Update(){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		const char * const key = pPanel.GetTexturePropertyValue();
		SetEnabled( texture && key && texture->GetProperties().Has( key ) );
	}
};

class cActionPropertyValueClear : public cBaseActionTexture {
public:
	cActionPropertyValueClear( gdeWPSOCComponent &panel ) : cBaseActionTexture( panel, "Clear",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove all property values" ){ }
	
	virtual igdeUndo *OnActionTexture( gdeObjectClass *objectClass, gdeOCComponent *component,
	gdeOCComponentTexture *texture ){
		if( texture->GetProperties().GetCount() == 0 ){
			return NULL;
		}
		return new gdeUOCCTextureSetProperties( objectClass, component, texture, decStringDictionary() );
	}
	
	virtual void Update(){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		SetEnabled( texture && texture->GetProperties().GetCount() > 0 );
	}
};

class cListTexturePropertyValues : public igdeIconListBoxListener {
	gdeWPSOCComponent &pPanel;
	
public:
	cListTexturePropertyValues( gdeWPSOCComponent &panel ) : pPanel( panel ){ }
	
	virtual void OnDoubleClickItem( igdeIconListBox *listBox, int index ){
		gdeOCComponentTexture * const texture = pPanel.GetTexture();
		if( ! texture ){
			return;
		}
		
		const decString &key = listBox->GetItemAt( index )->GetText();
		if( ! texture->GetProperties().Has( key ) ){
			return;
		}
		
		decString value( texture->GetProperties().GetAt( key ) );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Edit Texture Property Value", "Value:", value ) ){
			return;
		}
		
		decStringDictionary values( texture->GetProperties() );
		values.SetAt( key, value );
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUOCCTextureSetProperties( pPanel.GetObjectClass(),
			pPanel.GetComponent(), texture, values ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
	
	virtual void AddContextMenuEntries( igdeIconListBox*, igdeMenuCascade &menu ){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand( menu, pPanel.GetActionTexturePropertyValueSet() );
		helper.MenuCommand( menu, pPanel.GetActionTexturePropertyValueRemove() );
		helper.MenuCommand( menu, pPanel.GetActionTexturePropertyValueClear() );
	}
};

}



// Class gdeWPSOCComponent
////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCComponent::gdeWPSOCComponent( gdeWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pGameDefinition( NULL ),
pDirtyEngModelTexNames( true )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference content, groupBox, frameLine;
	
	pListener = new gdeWPSOCComponentListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	// actions
	pActionTexturesMenu.TakeOver( new cActionTexturesMenu( *this ) );
	pActionTextureAdd.TakeOver( new cActionTextureAdd( *this ) );
	pActionTextureRemove.TakeOver( new cActionTextureRemove( *this ) );
	pActionTexturePropertyValueClear.TakeOver( new cActionPropertyValueClear( *this ) );
	pActionTexturePropertyValueRemove.TakeOver( new cActionPropertyValueRemove( *this ) );
	pActionTexturePropertyValueSet.TakeOver( new cActionPropertyValueSet( *this ) );
	
	
	// component
	helper.GroupBox( content, groupBox, "Object Class Component:" );
	
	helper.EditPath( groupBox, "Model:", "Path to model file to use",
		igdeEnvironment::efpltModel, pEditPathModel, new cEditPathModel( *this ) );
	helper.EditPath( groupBox, "Skin:", "Path to skin file to use",
		igdeEnvironment::efpltSkin, pEditPathSkin, new cEditPathSkin( *this ) );
	helper.EditPath( groupBox, "Rig:", "Path to rig file to use",
		igdeEnvironment::efpltRig, pEditPathRig, new cEditPathRig( *this ) );
	helper.EditPath( groupBox, "Animator:", "Path to animator file to use",
		igdeEnvironment::efpltAnimator, pEditPathAnimator, new cEditPathAnimator( *this ) );
	helper.EditPath( groupBox, "Occlusion Mesh:", "Path to occlusion mesh file to use",
		igdeEnvironment::efpltOcclusionMesh, pEditPathOcclusionMesh, new cEditPathOcclusionMesh( *this ) );
	helper.EditPath( groupBox, "Audio Model:", "Path to audio model file to use",
		igdeEnvironment::efpltModel, pEditPathAudioModel, new cEditPathAudioModel( *this ) );
	helper.EditString( groupBox, "Playback Controller:", "Name of animator playback controller",
		pEditPlaybackController, new cTextPlaybackController( *this ) );
	
	helper.ComboBox( groupBox, "Collision:", "Collision response type",
		pCBCollisionResponseType, new cComboCollisionResponseType( *this ) );
	pCBCollisionResponseType->AddItem( "Static", NULL, ( void* )( intptr_t )deCollider::ertStatic );
	pCBCollisionResponseType->AddItem( "Kinematic", NULL, ( void* )( intptr_t )deCollider::ertKinematic );
	pCBCollisionResponseType->AddItem( "Dynamic", NULL, ( void* )( intptr_t )deCollider::ertDynamic );
	
	helper.EditVector( groupBox, "Position:", "Position relative to object class",
		pEditPosition, new cEditPosition( *this ) );
	helper.EditVector( groupBox, "Rotation:", "Rotation in degrees relative to object class", 4, 1,
		pEditRotation, new cEditRotation( *this ) );
	helper.EditString( groupBox, "Bone:", "Bone name or empty string if not used",
		pEditBoneName, new cTextBoneName( *this ) );
	
	helper.CheckBox( groupBox, pChkDoNotScale, new cActionDoNotScale( *this ), true );
	helper.CheckBox( groupBox, pChkPartialHide, new cActionPartialHide( *this ), true );
	helper.CheckBox( groupBox, pChkAttachTarget, new cActionAttachTarget( *this ), true );
	helper.CheckBox( groupBox, pChkRenderEnvMap, new cActionRenderEnvMap( *this ), true );
	helper.CheckBox( groupBox, pChkAffectsAudio, new cActionAffectsAudio( *this ), true );
	helper.CheckBox( groupBox, pChkLightShadowIgnore, new cActionLightShadowIgnore( *this ), true );
	helper.CheckBox( groupBox, pChkStatic, new cActionStatic( *this ), true );
	
	
	// property targets
	helper.GroupBox( content, groupBox, "Properties:" );
	helper.ComboBox( groupBox, "Property:", "Property to set target for",
		pCBPropertyNames, new cComboPropertyNames( *this ) );
	pCBPropertyNames->AddItem( "Model", NULL, ( void* )( intptr_t )gdeOCComponent::epModel );
	pCBPropertyNames->AddItem( "Skin", NULL, ( void* )( intptr_t )gdeOCComponent::epSkin );
	pCBPropertyNames->AddItem( "Rig", NULL, ( void* )( intptr_t )gdeOCComponent::epRig );
	pCBPropertyNames->AddItem( "Animator", NULL, ( void* )( intptr_t )gdeOCComponent::epAnimator );
	pCBPropertyNames->AddItem( "Playback controller", NULL, ( void* )( intptr_t )gdeOCComponent::epPlaybackController );
	pCBPropertyNames->AddItem( "Occlusion mesh", NULL, ( void* )( intptr_t )gdeOCComponent::epOcclusionMesh );
	pCBPropertyNames->AddItem( "Audio model", NULL, ( void* )( intptr_t )gdeOCComponent::epAudioModel );
	pCBPropertyNames->AddItem( "Render env-map", NULL, ( void* )( intptr_t )gdeOCComponent::epRenderEnvMap );
	pCBPropertyNames->AddItem( "Affects audio", NULL, ( void* )( intptr_t )gdeOCComponent::epAffectsAudio );
	pCBPropertyNames->AddItem( "Light Shadow Ignore", NULL, ( void* )( intptr_t )gdeOCComponent::epLightShadowIgnore );
	pCBPropertyNames->AddItem( "Attach position", NULL,  ( void* )( intptr_t )gdeOCComponent::epAttachPosition );
	pCBPropertyNames->AddItem( "Attach rotation", NULL, ( void* )( intptr_t )gdeOCComponent::epAttachRotation );
	
	helper.ComboBoxFilter( groupBox, "Target:", true, "Object class property to target",
		pCBPropertyNameTarget, new cComboPropertyNameTarget( *this ) );
	pCBPropertyNameTarget->SetEditable( true );
	pCBPropertyNameTarget->SetDefaultSorter();
	pCBPropertyNameTarget->SetFilterCaseInsentive( true );
	
	
	// textures
	helper.GroupBox( content, groupBox, "Textures:" );
	
	helper.FormLineStretchFirst( groupBox, "Texture:", "Texture to edit", frameLine );
	helper.ComboBox( frameLine, "Texture to edit", pCBTextures, new cComboTextures( *this ) );
	pCBTextures->SetDefaultSorter();
	helper.Button( frameLine, pBtnTextures, pActionTexturesMenu );
	pActionTexturesMenu->SetWidget( pBtnTextures );
	
	helper.EditString( groupBox, "Name:", "Name of texture", pTextureEditName, new cTextTextureEditName( *this ) );
	helper.EditPath( groupBox, "Skin:", "Path to skin file to use",
		igdeEnvironment::efpltSkin, pTextureEditPathSkin, new cEditTextureEditPathSkin( *this ) );
	helper.EditVector2( groupBox, "Offset:", "Texture coordinate offset",
		pTextureEditOffset, new cEditTextureEditOffset( *this ) );
	helper.EditFloat( groupBox, "Rotation:",
		"Texture coordinate rotation around texture center in degrees",
		pTextureEditRotation, new cTextTextureEditRotation( *this ) );
	helper.EditVector2( groupBox, "Scale:", "Texture coordinate scaling",
		pTextureEditScale, new cEditTextureEditScale( *this ) );
	helper.ColorBox( groupBox, "Tint:", "Texture tint color",
		pTextureClrTint, new cColorTextureTint( *this ) );
	
	
	// texture property values
	helper.GroupBoxFlow( content, groupBox, "Texture Properties:", false, true );
	
	frameLine.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst ) );
	helper.ComboBox( frameLine, "Property value to set", pTextureCBPropertyKeys, NULL );
	pTextureCBPropertyKeys->SetDefaultSorter();
	helper.Button( frameLine, pTextureBtnPropertyValueSet, pActionTexturePropertyValueSet );
	groupBox->AddChild( frameLine );
	
	const igdeUIHelper::sColumnHeader headersPropertyValues[ 2 ] = {
		igdeUIHelper::sColumnHeader( "Key", NULL, 150 ),
		igdeUIHelper::sColumnHeader( "Value", NULL, 200 )
	};
	helper.IconListBox( groupBox, pTextureListProperties, decPoint( 100, 120 ),
		headersPropertyValues, 2, "Property values", new cListTexturePropertyValues( *this ) );
	pTextureListProperties->SetDefaultSorter();
}

gdeWPSOCComponent::~gdeWPSOCComponent(){
	SetGameDefinition( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void gdeWPSOCComponent::SetGameDefinition( gdeGameDefinition *gameDefinition ){
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
	
	UpdateComponent();
	UpdateTextureList();
	UpdatePropertyList();
	UpdateTexturePropertyList();
}



gdeObjectClass *gdeWPSOCComponent::GetObjectClass() const{
	return pGameDefinition ? pGameDefinition->GetActiveObjectClass() : NULL;
}

gdeOCComponent *gdeWPSOCComponent::GetComponent() const{
	const gdeObjectClass * const objectClass = GetObjectClass();
	return objectClass ? pGameDefinition->GetActiveOCComponent() : NULL;
}

gdeOCComponentTexture *gdeWPSOCComponent::GetTexture() const{
	gdeOCComponent * const component = GetComponent();
	return component ? component->GetActiveTexture() : NULL;
}

const decString &gdeWPSOCComponent::GetTextureProperty() const{
	const gdeOCComponentTexture * const texture = GetTexture();
	static decString emptyString;
	return texture ? texture->GetActiveProperty() : emptyString;
}

const gdeOCComponent::eProperties gdeWPSOCComponent::GetPropertyName() const{
	return ( gdeOCComponent::eProperties )( intptr_t )pCBPropertyNames->GetSelectedItem()->GetData();
}

const decString &gdeWPSOCComponent::GetTexturePropertyKey() const{
	return pTextureCBPropertyKeys->GetText();
}

const char *gdeWPSOCComponent::GetTexturePropertyValue() const{
	const igdeListItem * const selection = pTextureListProperties->GetSelectedItem();
	return selection ? selection->GetText().GetString() : NULL;
}



void gdeWPSOCComponent::UpdatePropertyList(){
	const gdeObjectClass *objectClass = GetObjectClass();
	int i;
	
	decStringSet properties;
	if( objectClass ){
		objectClass->AddPropertyNamesTo( properties, true );
	}
	
	const decString selection( pCBPropertyNameTarget->GetText() );
	pCBPropertyNameTarget->RemoveAllItems();
	
	const int count = properties.GetCount();
	for( i=0; i<count; i++ ){
		pCBPropertyNameTarget->AddItem( properties.GetAt( i ) );
	}
	
	pCBPropertyNameTarget->SortItems();
	pCBPropertyNameTarget->SetText( selection );
}

void gdeWPSOCComponent::UpdateTexturePropertyList(){
	const gdeObjectClass *objectClass = GetObjectClass();
	int i;
	
	decStringSet properties;
	if( objectClass ){
		objectClass->AddTexturePropertyNamesTo( properties, true );
	}
	
	const decString selection( pTextureCBPropertyKeys->GetText() );
	pTextureCBPropertyKeys->RemoveAllItems();
	
	const int count = properties.GetCount();
	for( i=0; i<count; i++ ){
		pTextureCBPropertyKeys->AddItem( properties.GetAt( i ).GetString() );
	}
	
	pTextureCBPropertyKeys->SortItems();
	pTextureCBPropertyKeys->SetText( selection );
}



void gdeWPSOCComponent::UpdateComponent(){
	const gdeOCComponent * const component = GetComponent();
	
	if( component ){
		pEditPathModel->SetPath( component->GetModelPath() );
		pEditPathSkin->SetPath( component->GetSkinPath() );
		pEditPathRig->SetPath( component->GetRigPath() );
		pEditPathAnimator->SetPath( component->GetAnimatorPath() );
		pEditPathOcclusionMesh->SetPath( component->GetOcclusionMeshPath() );
		pEditPathAudioModel->SetPath( component->GetAudioModelPath() );
		pEditPlaybackController->SetText( component->GetPlaybackController() );
		pChkDoNotScale->SetChecked( component->GetDoNotScale() );
		pChkStatic->SetChecked( component->GetStatic() );
		pChkRenderEnvMap->SetChecked( component->GetRenderEnvMap() );
		pChkAffectsAudio->SetChecked( component->GetAffectsAudio() );
		pChkLightShadowIgnore->SetChecked( component->GetLightShadowIgnore() );
		pChkPartialHide->SetChecked( component->GetPartialHide() );
		pChkAttachTarget->SetChecked( component->GetAttachTarget() );
		pCBCollisionResponseType->SetSelectionWithData(
			( void* )( intptr_t )component->GetColliderResponseType() );
		pEditPosition->SetVector( component->GetPosition() );
		pEditRotation->SetVector( component->GetRotation() );
		pEditBoneName->SetText( component->GetBoneName() );
		
		SetEngineModelPath( component->GetModelPath() );
		
	}else{
		pEditPathModel->ClearPath();
		pEditPathSkin->ClearPath();
		pEditPathRig->ClearPath();
		pEditPathAnimator->ClearPath();
		pEditPathOcclusionMesh->ClearPath();
		pEditPathAudioModel->ClearPath();
		pEditPlaybackController->ClearText();
		pChkDoNotScale->SetChecked( false );
		pChkStatic->SetChecked( false );
		pChkRenderEnvMap->SetChecked( false );
		pChkAffectsAudio->SetChecked( false );
		pChkLightShadowIgnore->SetChecked( false );
		pChkPartialHide->SetChecked( false );
		pChkAttachTarget->SetChecked( false );
		pCBCollisionResponseType->SetSelectionWithData( ( void* )( intptr_t )deCollider::ertStatic );
		pEditPosition->SetVector( decVector() );
		pEditRotation->SetVector( decVector() );
		pEditBoneName->ClearText();
		
		SetEngineModelPath( "" );
	}
	
	const bool enabled = component;
	pEditPathModel->SetEnabled( enabled );
	pEditPathSkin->SetEnabled( enabled );
	pEditPathRig->SetEnabled( enabled );
	pEditPathAnimator->SetEnabled( enabled );
	pEditPathOcclusionMesh->SetEnabled( enabled );
	pEditPathAudioModel->SetEnabled( enabled );
	pEditPlaybackController->SetEnabled( enabled );
	pChkDoNotScale->SetEnabled( enabled );
	pChkStatic->SetEnabled( enabled );
	pChkRenderEnvMap->SetEnabled( enabled );
	pChkAffectsAudio->SetEnabled( enabled );
	pChkLightShadowIgnore->SetEnabled( enabled );
	pChkPartialHide->SetEnabled( enabled );
	pChkAttachTarget->SetEnabled( enabled );
	pCBCollisionResponseType->SetEnabled( enabled );
	pEditPosition->SetEnabled( enabled );
	pEditRotation->SetEnabled( enabled );
	pEditBoneName->SetEnabled( enabled );
	
	UpdatePropertyName();
}

void gdeWPSOCComponent::UpdatePropertyName(){
	const gdeOCComponent * const component = GetComponent();
	
	if( component ){
		pCBPropertyNameTarget->SetText( component->GetPropertyName( GetPropertyName() ) );
		
	}else{
		pCBPropertyNameTarget->ClearText();
	}
	
	pCBPropertyNameTarget->SetEnabled( component );
}

void gdeWPSOCComponent::UpdateTextureList(){
	const gdeOCComponent * const component = GetComponent();
	
	pCBTextures->RemoveAllItems();
	
	if( component ){
		const gdeOCComponentTextureList &textures = component->GetTextures();
		const int count = textures.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			gdeOCComponentTexture * const texture = textures.GetAt( i );
			pCBTextures->AddItem( texture->GetName(), NULL, texture );
		}
	}
	
	pCBTextures->SortItems();
	
	SelectActiveTexture();
	UpdateTexture();
}

void gdeWPSOCComponent::SelectActiveTexture(){
	pCBTextures->SetSelectionWithData( GetTexture() );
}

void gdeWPSOCComponent::UpdateTexture(){
	const gdeOCComponentTexture * const texture = GetTexture();
	
	if( texture ){
		pTextureEditName->SetText( texture->GetName() );
		pTextureEditPathSkin->SetPath( texture->GetPathSkin() );
		pTextureEditOffset->SetVector2( texture->GetOffset() );
		pTextureEditRotation->SetFloat( texture->GetRotation() );
		pTextureEditScale->SetVector2( texture->GetScale() );
		pTextureClrTint->SetColor( texture->GetColorTint() );
		
	}else{
		pTextureEditName->ClearText();
		pTextureEditPathSkin->ClearPath();
		pTextureEditOffset->SetVector2(decVector2() );
		pTextureEditRotation->ClearText();
		pTextureEditScale->SetVector2(decVector2() );
		pTextureClrTint->SetColor( decColor( 1.0f, 1.0f, 1.0f ) );
	}
	
	const bool enabled = texture;
	
	pTextureEditName->SetEnabled( enabled );
	pTextureEditPathSkin->SetEnabled( enabled );
	pTextureEditOffset->SetEnabled( enabled );
	pTextureEditRotation->SetEnabled( enabled );
	pTextureEditScale->SetEnabled( enabled );
	pTextureClrTint->SetEnabled( enabled );
	
	UpdateTextureProperties();
}

void gdeWPSOCComponent::UpdateTextureProperties(){
	const gdeOCComponentTexture * const texture = GetTexture();
	
	pTextureListProperties->RemoveAllItems();
	
	if( texture ){
		const decStringList keys( texture->GetProperties().GetKeys() );
		const decStringList values( texture->GetProperties().GetValues() );
		const int count = keys.GetCount();
		decStringList details;
		int i;
		
		for( i=0; i<count; i++ ){
			details.RemoveAll();
			details.Add( values.GetAt( i ) );
			pTextureListProperties->AddItem( keys.GetAt( i ), details );
		}
		
		pTextureListProperties->SortItems();
	}
}

void gdeWPSOCComponent::SelectActiveTextureProperty(){
	pTextureListProperties->SetSelection( pTextureListProperties->IndexOfItem( GetTextureProperty() ) );
	pTextureListProperties->EnsureSelectedItemVisible();
}

void gdeWPSOCComponent::SetEngineModelPath( const char *path ){
	if( pEngModelPath == path ){
		return;
	}
	
	pEngModelTexNames.RemoveAll();
	pEngModelPath = path;
	pDirtyEngModelTexNames = true;
}

void gdeWPSOCComponent::PrepareEngineModelPath(){
	if( ! pDirtyEngModelTexNames ){
		return;
	}
	
	pDirtyEngModelTexNames = false;
	pActionsTextureAddFromModel.RemoveAll();
	
	if( pEngModelPath.IsEmpty() ){
		return;
	}
	
	deModelReference model;
	try{
		model.TakeOver( GetEngine()->GetModelManager()->LoadModel( pEngModelPath, "/" ) );
		
		const int count = model->GetTextureCount();
		int i;
		for( i=0; i<count; i++ ){
			pEngModelTexNames.Add( model->GetTextureAt( i )->GetName() );
		}
		
		pEngModelTexNames.SortAscending();
		
		deObjectReference action;
		for( i=0; i<count; i++ ){
			action.TakeOver( new cActionTextureAdd( *this, pEngModelTexNames.GetAt( i ) ) );
			pActionsTextureAddFromModel.Add( action );
		}
		
	}catch( const deException & ){
		// do nothing. this is best effort only
	}
}
