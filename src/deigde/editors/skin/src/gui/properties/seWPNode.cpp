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
#include <stdint.h>

#include "seWPNode.h"
#include "seWPNodeListener.h"
#include "seWindowProperties.h"
#include "../seWindowMain.h"
#include "../../skin/seSkin.h"
#include "../../skin/texture/seTexture.h"
#include "../../skin/property/seProperty.h"
#include "../../skin/property/node/sePropertyNodeImage.h"
#include "../../skin/property/node/sePropertyNodeGroup.h"
#include "../../skin/property/node/sePropertyNodeShape.h"
#include "../../skin/property/node/sePropertyNodeText.h"
#include "../../undosys/property/node/seUPropertyNodeSetBrightness.h"
#include "../../undosys/property/node/seUPropertyNodeSetColorize.h"
#include "../../undosys/property/node/seUPropertyNodeSetContrast.h"
#include "../../undosys/property/node/seUPropertyNodeSetGamma.h"
#include "../../undosys/property/node/seUPropertyNodeSetMask.h"
#include "../../undosys/property/node/seUPropertyNodeSetPosition.h"
#include "../../undosys/property/node/seUPropertyNodeSetRotation.h"
#include "../../undosys/property/node/seUPropertyNodeSetShear.h"
#include "../../undosys/property/node/seUPropertyNodeSetSize.h"
#include "../../undosys/property/node/seUPropertyNodeSetTransparency.h"
#include "../../undosys/property/node/seUPropertyNodeSetCombineMode.h"
#include "../../undosys/property/node/image/seUPropertyNodeImageSetPath.h"
#include "../../undosys/property/node/image/seUPropertyNodeImageSetRepeat.h"
#include "../../undosys/property/node/shape/seUPropertyNodeShapeSetFillColor.h"
#include "../../undosys/property/node/shape/seUPropertyNodeShapeSetLineColor.h"
#include "../../undosys/property/node/shape/seUPropertyNodeShapeSetThickness.h"
#include "../../undosys/property/node/shape/seUPropertyNodeShapeSetType.h"
#include "../../undosys/property/node/text/seUPropertyNodeTextSetColor.h"
#include "../../undosys/property/node/text/seUPropertyNodeTextSetPath.h"
#include "../../undosys/property/node/text/seUPropertyNodeTextSetSize.h"
#include "../../undosys/property/node/text/seUPropertyNodeTextSetText.h"

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
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditPoint.h>
#include <deigde/gui/composed/igdeEditPointListener.h>
#include <deigde/gui/composed/igdeEditPoint3.h>
#include <deigde/gui/composed/igdeEditPoint3Listener.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/composed/igdeEditSliderTextListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionSelectFile.h>
#include <deigde/gui/event/igdeColorBoxListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeTreeListListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/model/igdeTreeItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/image/deImage.h>



// Actions
////////////

namespace{

class cBaseAction : public igdeAction{
protected:
	seWPNode &pPanel;
	
public:
	cBaseAction( seWPNode &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		sePropertyNode * const node = pPanel.GetNode();
		if( ! skin || ! texture || ! property || ! node ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnAction( skin, texture, property, node ) );
		if( undo ){
			skin->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( seSkin *skin, seTexture *texture, seProperty *property, sePropertyNode *node ) = 0;
	
	virtual void Update(){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		sePropertyNode * const node = pPanel.GetNode();
		if( skin && texture && property && node ){
			Update( *skin, *texture, *property, *node );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const seSkin &, const seTexture &, const seProperty &, sePropertyNode & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	seWPNode &pPanel;
	
public:
	cBaseTextFieldListener( seWPNode &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		sePropertyNode * const node = pPanel.GetNode();
		if( ! skin || ! texture || ! property || ! node ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( *textField, skin, texture, property, node ) );
		if( undo ){
			skin->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, seSkin *skin,
		seTexture *texture, seProperty *property, sePropertyNode *node ) = 0;
};

class cBaseEditPathListener : public igdeEditPathListener{
protected:
	seWPNode &pPanel;
	
public:
	cBaseEditPathListener( seWPNode &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		sePropertyNode * const node = pPanel.GetNode();
		if( ! skin || ! texture || ! property || ! node ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( *editPath, skin, texture, property, node ) );
		if( undo ){
			skin->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeEditPath &editPath, seSkin *skin,
		seTexture *texture, seProperty *property, sePropertyNode *node ) = 0;
};

class cBaseColorBoxListener : public igdeColorBoxListener{
protected:
	seWPNode &pPanel;
	
public:
	cBaseColorBoxListener( seWPNode &panel ) : pPanel( panel ){ }
	
	virtual void OnColorChanged( igdeColorBox *colorBox ){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		sePropertyNode * const node = pPanel.GetNode();
		if( ! skin || ! texture || ! property || ! node ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( *colorBox, skin, texture, property, node ) );
		if( undo ){
			skin->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeColorBox &colorBox, seSkin *skin,
		seTexture *texture, seProperty *property, sePropertyNode *node ) = 0;
};

class cBaseEditPoint3Listener : public igdeEditPoint3Listener{
protected:
	seWPNode &pPanel;
	
public:
	cBaseEditPoint3Listener( seWPNode &panel ) : pPanel( panel ){ }
	
	virtual void OnPoint3Changed( igdeEditPoint3 *editPoint3 ){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		sePropertyNode * const node = pPanel.GetNode();
		if( ! skin || ! texture || ! property || ! node ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( *editPoint3, skin, texture, property, node ) );
		if( undo ){
			skin->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeEditPoint3 &editPoint3, seSkin *skin,
		seTexture *texture, seProperty *property, sePropertyNode *node ) = 0;
};

class cBaseEditPointListener : public igdeEditPointListener{
protected:
	seWPNode &pPanel;
	
public:
	cBaseEditPointListener( seWPNode &panel ) : pPanel( panel ){ }
	
	virtual void OnPointChanged( igdeEditPoint *editPoint ){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		sePropertyNode * const node = pPanel.GetNode();
		if( ! skin || ! texture || ! property || ! node ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( *editPoint, skin, texture, property, node ) );
		if( undo ){
			skin->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeEditPoint &editPoint, seSkin *skin,
		seTexture *texture, seProperty *property, sePropertyNode *node ) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	seWPNode &pPanel;
	
public:
	cBaseComboBoxListener( seWPNode &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		sePropertyNode * const node = pPanel.GetNode();
		if( ! skin || ! texture || ! property || ! node ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( *comboBox, skin, texture, property, node ) );
		if( undo ){
			skin->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox, seSkin *skin,
		seTexture *texture, seProperty *property, sePropertyNode *node ) = 0;
};


class cTreeOutliner : public igdeTreeListListener{
protected:
	seWPNode &pPanel;
	bool &pPreventUpdate;
	
public:
	cTreeOutliner( seWPNode &panel, bool &preventUpdate ) :
	pPanel( panel ), pPreventUpdate( preventUpdate ){ }
	
	virtual void OnSelectionChanged( igdeTreeList *treeList ){
		if( pPreventUpdate ){
			return;
		}
		
		seProperty * const property = pPanel.GetProperty();
		if( ! property ){
			return;
		}
		
		igdeTreeItem * const item = treeList->GetSelection();
		sePropertyNode *node = NULL;
		if( item ){
			node = ( sePropertyNode* )item->GetData();
			if( ! node ){
				return; // data not assigned yet. skip
			}
		}
		
		if( node ){
			sePropertyNode *findNode = node;
			
			while( findNode ){
				if( findNode->GetMaskParent() ){
					node = findNode->GetMaskParent();
					findNode = node;
					
				}else{
					findNode = findNode->GetParent();
				}
			}
			
			if( node->GetActive() ){
				return; // otherwise we get into troubles
			}
			
		}else if( ! property->GetNodeSelection().HasActive() ){
			return; // otherwise we get into troubles
		}
		
		pPreventUpdate = true;
		try{
			property->GetNodeSelection().RemoveAll();
			if( node ){
				property->SetActiveNodeGroup( node->GetParent() );
				property->GetNodeSelection().Add( node );
			}
			
			pPreventUpdate = false;
			
		}catch( const deException & ){
			pPreventUpdate = false;
			throw;
		}
	}
};


class cEditPosition : public cBaseEditPoint3Listener{
public:
	cEditPosition( seWPNode &panel ) : cBaseEditPoint3Listener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditPoint3 &editPoint3, seSkin*, seTexture*,
	seProperty*, sePropertyNode *node ){
		return node->GetPosition() != editPoint3.GetPoint3()
			? new seUPropertyNodeSetPosition( node, editPoint3.GetPoint3() ) : NULL;
	}
};

class cEditSize : public cBaseEditPoint3Listener{
public:
	cEditSize( seWPNode &panel ) : cBaseEditPoint3Listener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditPoint3 &editPoint3, seSkin*, seTexture*,
	seProperty*, sePropertyNode *node ){
		return node->GetSize() != editPoint3.GetPoint3()
			? new seUPropertyNodeSetSize( node, editPoint3.GetPoint3() ) : NULL;
	}
};

class cTextRotation : public cBaseTextFieldListener{
public:
	cTextRotation( seWPNode &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, seSkin*, seTexture*,
	seProperty*, sePropertyNode *node ){
		const float value = textField.GetFloat();
		return fabsf( node->GetRotation() - value ) > FLOAT_SAFE_EPSILON
			? new seUPropertyNodeSetRotation( node, value ) : NULL;
	}
};

class cTextShearing : public cBaseTextFieldListener{
public:
	cTextShearing( seWPNode &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, seSkin*, seTexture*,
	seProperty*, sePropertyNode *node ){
		const float value = textField.GetFloat();
		return fabsf( node->GetShearing() - value ) > FLOAT_SAFE_EPSILON
			? new seUPropertyNodeSetShear( node, value ) : NULL;
	}
};

class cTextBrightness : public cBaseTextFieldListener{
public:
	cTextBrightness( seWPNode &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, seSkin*, seTexture*,
	seProperty*, sePropertyNode *node ){
		const float value = textField.GetFloat();
		return fabsf( node->GetBrightness() - value ) > FLOAT_SAFE_EPSILON
			? new seUPropertyNodeSetBrightness( node, value ) : NULL;
	}
};

class cTextContrast : public cBaseTextFieldListener{
public:
	cTextContrast( seWPNode &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, seSkin*, seTexture*,
	seProperty*, sePropertyNode *node ){
		const float value = textField.GetFloat();
		return fabsf( node->GetContrast() - value ) > FLOAT_SAFE_EPSILON
			? new seUPropertyNodeSetContrast( node, value ) : NULL;
	}
};

class cTextGamma : public cBaseTextFieldListener{
public:
	cTextGamma( seWPNode &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, seSkin*, seTexture*,
	seProperty*, sePropertyNode *node ){
		const float value = textField.GetFloat();
		return fabsf( node->GetGamma() - value ) > FLOAT_SAFE_EPSILON
			? new seUPropertyNodeSetGamma( node, value ) : NULL;
	}
};

class cColorColorize : public cBaseColorBoxListener{
public:
	cColorColorize( seWPNode &panel ) : cBaseColorBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeColorBox &colorBox, seSkin*, seTexture*,
	seProperty*, sePropertyNode *node ){
		return ! node->GetColorize().IsEqualTo( colorBox.GetColor() )
			? new seUPropertyNodeSetColorize( node, colorBox.GetColor() ) : NULL;
	}
};

class cSliderTransparency : public igdeEditSliderTextListener{
	seWPNode &pPanel;
	igdeUndoReference pUndo;
	
public:
	cSliderTransparency( seWPNode &panel ) : pPanel( panel ){ }
	
	virtual void OnSliderTextValueChanged( igdeEditSliderText *sliderText ){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		sePropertyNode * const node = pPanel.GetNode();
		if( ! skin || ! texture || ! property || ! node ){
			return;
		}
		
		if( pUndo ){
			( ( seUPropertyNodeSetTransparency& )( igdeUndo& )pUndo ).SetNewValue( sliderText->GetValue() );
			pUndo->Redo();
			
		}else{
			pUndo.TakeOver( new seUPropertyNodeSetTransparency( node, sliderText->GetValue() ) );
			skin->GetUndoSystem()->Add( pUndo );
		}
		pUndo = NULL;
	}
	
	virtual void OnSliderTextValueChanging( igdeEditSliderText *sliderText ){
		seSkin * const skin = pPanel.GetSkin();
		seTexture * const texture = pPanel.GetTexture();
		seProperty * const property = pPanel.GetProperty();
		sePropertyNode * const node = pPanel.GetNode();
		if( ! skin || ! texture || ! property || ! node ){
			return;
		}
		
		if( pUndo ){
			( ( seUPropertyNodeSetTransparency& )( igdeUndo& )pUndo ).SetNewValue( sliderText->GetValue() );
			pUndo->Redo();
			
		}else{
			pUndo.TakeOver( new seUPropertyNodeSetTransparency( node, sliderText->GetValue() ) );
			skin->GetUndoSystem()->Add( pUndo );
		}
	}
};

class cComboCombineMode : public cBaseComboBoxListener{
public:
	cComboCombineMode( seWPNode &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox, seSkin*, seTexture*,
	seProperty*, sePropertyNode *node ){
		if( ! comboBox.GetSelectedItem() ){
			return NULL;
		}
		deSkinPropertyNode::eCombineModes value = ( deSkinPropertyNode::eCombineModes )
			( intptr_t )comboBox.GetSelectedItem()->GetData();
		
		return node->GetCombineMode() != value ? new seUPropertyNodeSetCombineMode( node, value ) : NULL;
	}
};


class cPathImage : public cBaseEditPathListener{
public:
	cPathImage( seWPNode &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditPath &editPath, seSkin*, seTexture*,
	seProperty*, sePropertyNode *node ){
		return node->GetNodeType() == sePropertyNode::entImage
			&& ( ( sePropertyNodeImage* )node )->GetPath() != editPath.GetPath()
			? new seUPropertyNodeImageSetPath( ( sePropertyNodeImage* )node, editPath.GetPath() ) : NULL;
	}
};

class cEditImageRepeat : public cBaseEditPointListener{
public:
	cEditImageRepeat( seWPNode &panel ) : cBaseEditPointListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditPoint &editPoint, seSkin*, seTexture*,
	seProperty*, sePropertyNode *node ){
		return node->GetNodeType() == sePropertyNode::entImage
			&& ( ( sePropertyNodeImage* )node )->GetRepeat() != editPoint.GetPoint()
			? new seUPropertyNodeImageSetRepeat( ( sePropertyNodeImage* )node, editPoint.GetPoint() ) : NULL;
	}
};


class cComboShapeType : public cBaseComboBoxListener{
public:
	cComboShapeType( seWPNode &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox, seSkin*, seTexture*,
	seProperty*, sePropertyNode *node ){
		if( ! comboBox.GetSelectedItem() || node->GetNodeType() != sePropertyNode::entShape ){
			return NULL;
		}
		deSkinPropertyNodeShape::eShapeTypes value = ( deSkinPropertyNodeShape::eShapeTypes )
			( intptr_t )comboBox.GetSelectedItem()->GetData();
		
		return ( ( sePropertyNodeShape* )node )->GetShapeType() != value
			? new seUPropertyNodeShapeSetType( ( sePropertyNodeShape* )node, value ) : NULL;
	}
};

class cColorShapeLineColor : public cBaseColorBoxListener{
public:
	cColorShapeLineColor( seWPNode &panel ) : cBaseColorBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeColorBox &colorBox, seSkin*, seTexture*,
	seProperty*, sePropertyNode *node ){
		return node->GetNodeType() == sePropertyNode::entShape
			&& ! ( ( sePropertyNodeShape* )node )->GetLineColor().IsEqualTo( colorBox.GetColor() )
			? new seUPropertyNodeShapeSetLineColor( ( sePropertyNodeShape* )node, colorBox.GetColor() ) : NULL;
	}
};

class cColorShapeFillColor : public cBaseColorBoxListener{
public:
	cColorShapeFillColor( seWPNode &panel ) : cBaseColorBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeColorBox &colorBox, seSkin*, seTexture*,
	seProperty*, sePropertyNode *node ){
		return node->GetNodeType() == sePropertyNode::entShape
			&& ! ( ( sePropertyNodeShape* )node )->GetFillColor().IsEqualTo( colorBox.GetColor() )
			? new seUPropertyNodeShapeSetFillColor( ( sePropertyNodeShape* )node, colorBox.GetColor() ) : NULL;
	}
};

class cColorShapeThickness : public cBaseTextFieldListener{
public:
	cColorShapeThickness( seWPNode &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, seSkin*, seTexture*,
	seProperty*, sePropertyNode *node ){
		const float value = textField.GetFloat();
		return node->GetNodeType() == sePropertyNode::entShape
			&& fabsf( ( ( sePropertyNodeShape* )node )->GetThickness() - value ) > FLOAT_SAFE_EPSILON
			? new seUPropertyNodeShapeSetThickness( ( sePropertyNodeShape* )node, value ) : NULL;
	}
};


class cEditPathFont : public cBaseEditPathListener{
public:
	cEditPathFont( seWPNode &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditPath &editPath, seSkin*, seTexture*,
	seProperty*, sePropertyNode *node ){
		return node->GetNodeType() == sePropertyNode::entText
			&& ( ( sePropertyNodeText* )node )->GetPath() != editPath.GetPath()
			? new seUPropertyNodeTextSetPath( ( sePropertyNodeText* )node, editPath.GetPath() ) : NULL;
	}
};

class cTextFontSize : public cBaseTextFieldListener{
public:
	cTextFontSize( seWPNode &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, seSkin*, seTexture*,
	seProperty*, sePropertyNode *node ){
		const float value = textField.GetFloat();
		return node->GetNodeType() == sePropertyNode::entText
			&& fabsf( ( ( sePropertyNodeText* )node )->GetFontSize() - value ) > FLOAT_SAFE_EPSILON
			? new seUPropertyNodeTextSetSize( ( sePropertyNodeText* )node, value ) : NULL;
	}
};

class cTextText : public igdeTextFieldListener{
	seWPNode &pPanel;
	igdeUndoReference pUndo;
public:
	cTextText( seWPNode &panel ) : pPanel( panel ){ }
	
	sePropertyNodeText *GetNodeText() const{
		return pPanel.GetNode() && pPanel.GetNode()->GetNodeType() == sePropertyNode::entText
			? ( sePropertyNodeText* )pPanel.GetNode() : NULL;
	}
	
	virtual void OnTextChanged( igdeTextField *textField ){
		if( GetNodeText() ){
			UpdateChanged( textField->GetText(), GetNodeText() );
			pUndo = NULL;
		}
	}
	
	virtual void OnTextChanging( igdeTextField *textField ){
		if( GetNodeText() ){
			UpdateChanged( textField->GetText(), GetNodeText() );
		}
	}
	
	virtual void UpdateChanged( const decString &text, sePropertyNodeText *node ){
		if( node->GetText() == text ){
			return;
		}
		
		if( pUndo ){
			( ( seUPropertyNodeTextSetText& )( igdeUndo& )pUndo ).SetNewValue( text );
			pUndo->Redo();
			
		}else{
			pUndo.TakeOver( new seUPropertyNodeTextSetText( node, text ) );
			pPanel.GetSkin()->GetUndoSystem()->Add( pUndo );
		}
	}
};

class cColorText : public cBaseColorBoxListener{
public:
	cColorText( seWPNode &panel ) : cBaseColorBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeColorBox &colorBox, seSkin*, seTexture*,
	seProperty*, sePropertyNode *node ){
		return node->GetNodeType() == sePropertyNode::entText
			&& ! ( ( sePropertyNodeText* )node )->GetColor().IsEqualTo( colorBox.GetColor() )
			? new seUPropertyNodeTextSetColor( ( sePropertyNodeText* )node, colorBox.GetColor() ) : NULL;
	}
};

}



// Class seWPNode
///////////////////

// Constructor, destructor
////////////////////////////

seWPNode::seWPNode( seWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pSkin( NULL ),
pPreventUpdate( false )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainerReference content, panel, groupBox, form, formLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = new seWPNodeListener( *this );
	
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	
	// outliner
	helper.TreeList( content, pTreeOutline, 10, "Outliner", new cTreeOutliner( *this, pPreventUpdate ) );
	
	helper.EditString( content, "", pLabSelection, NULL );
	pLabSelection->SetEditable( false );
	
	
	// node
	helper.GroupBox( content, groupBox, "Node:" );
	
	helper.EditPoint3( groupBox, "Position:", "Position of node center",
		pEditPosition, new cEditPosition( *this ) );
	helper.EditPoint3( groupBox, "Size:", "Size of node center", pEditSize, new cEditSize( *this ) );
	helper.EditFloat( groupBox, "Rotation:", "Counter clock-wise rotation of node",
		pEditRotation, new cTextRotation( *this ) );
	helper.EditFloat( groupBox, "Shear:", "Shearing of node in degrees along X direction",
		pEditShear, new cTextShearing( *this ) );
	helper.EditFloat( groupBox, "Brightness:", "Brightness", pEditBrightness, new cTextBrightness( *this ) );
	helper.EditFloat( groupBox, "Contrast:", "Contrast", pEditContrast, new cTextContrast( *this ) );
	helper.EditFloat( groupBox, "Gamma:", "Gamma", pEditGamma, new cTextGamma( *this ) );
	helper.ColorBox( groupBox, "Colorize:", "Colorize", pClrColorize, new cColorColorize( *this ) );
	helper.EditSliderText( groupBox, "Transparency:", "Transparency of node",
		0.0f, 1.0f, 6, 3, 0.1f, pSldTransparency, new cSliderTransparency( *this ) );
	
	helper.ComboBox( groupBox, "Combine Mode:", "Combine mode", pCBCombineMode, new cComboCombineMode( *this ) );
	pCBCombineMode->AddItem( "Blend", NULL, ( void* )( intptr_t )deSkinPropertyNode::ecmBlend );
	pCBCombineMode->AddItem( "Overlay", NULL, ( void* )( intptr_t )deSkinPropertyNode::ecmOverlay );
	
	helper.EditString( content, "", pLabMask, NULL );
	pLabMask->SetEditable( false );
	
	
	// type specific
	pSwitcher.TakeOver( new igdeSwitcher( env ) );
	content->AddChild( pSwitcher );
	
	
	// empty
	helper.Label( pSwitcher, "", "" );
	
	
	// image node
	panel.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	pSwitcher->AddChild( panel );
	helper.GroupBox( panel, groupBox, "Image:" );
	
	helper.EditPath( groupBox, "Image:", "Image resource to display",
		igdeEnvironment::efpltImage, pImageEditImage, new cPathImage( *this ) );
	helper.EditString( groupBox, "", "Image information", pImageLabImageInfo, NULL );
	pImageLabImageInfo->SetEditable( false );
	
	helper.EditPoint( groupBox, "Repeat:", "Repetition of image inside node size",
		pImageEditRepeat, new cEditImageRepeat( *this ) );
	
	
	// shape node
	panel.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	pSwitcher->AddChild( panel );
	helper.GroupBox( panel, groupBox, "Shape:" );
	
	helper.ComboBox( groupBox, "Type:", "Shape type", pShapeCBType, new cComboShapeType( *this ) );
	pShapeCBType->AddItem( "Rectangle", NULL, ( void* )( intptr_t )deSkinPropertyNodeShape::estRectangle );
	pShapeCBType->AddItem( "Ellipse", NULL, ( void* )( intptr_t )deSkinPropertyNodeShape::estEllipse );
	
	helper.ColorBox( groupBox, "Line Color:", "Line color", pShapeClrLine, new cColorShapeLineColor( *this ) );
	helper.ColorBox( groupBox, "Fill Color:", "Fill color", pShapeClrFill, new cColorShapeFillColor( *this ) );
	helper.EditFloat( groupBox, "Thickness:", "Thickness of line in pixels",
		pShapeEditThickness, new cColorShapeThickness( *this ) );
	
	
	// text node
	panel.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	pSwitcher->AddChild( panel );
	helper.GroupBox( panel, groupBox, "Text:" );
	
	helper.EditPath( groupBox, "Font:", "Font resource to use for text",
		igdeEnvironment::efpltFont, pTextEditFont, new cEditPathFont( *this ) );
	helper.EditString( groupBox, "", "Font information", pTextLabFontInfo, NULL );
	pTextLabFontInfo->SetEditable( false );
	
	helper.EditFloat( groupBox, "Font size:", "Font size in pixels (equals height of text line)",
		pTextEditFontSize, new cTextFontSize( *this ) );
	helper.EditString( groupBox, "Text:", "Text", pTextEditText, new cTextText( *this ) );
	helper.ColorBox( groupBox, "Color:", "Text color", pTextClrColor, new cColorText( *this ) );
	
	
	// group node
	panel.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	pSwitcher->AddChild( panel );
	helper.GroupBox( panel, groupBox, "Group:" );
}

seWPNode::~seWPNode(){
	SetSkin( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void seWPNode::SetSkin( seSkin *skin ){
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
	
	UpdateNode();
	ShowNodePanel();
	UpdateOutline();
}

seTexture *seWPNode::GetTexture() const{
	return pSkin ? pSkin->GetActiveTexture() : NULL;
}

seProperty *seWPNode::GetProperty() const{
	seTexture * const texture = GetTexture();
	return texture ? texture->GetActiveProperty() : NULL;
}

sePropertyNode *seWPNode::GetNode() const{
	seProperty * const property = GetProperty();
	return property ? property->GetNodeSelection().GetActive() : NULL;
}

void seWPNode::ShowNodePanel(){
	sePropertyNode * const node = GetNode();
	if( ! node ){
		pSwitcher->SetCurrent( 0 ); // empty
		return;
	}
	
	switch( node->GetNodeType() ){
	case sePropertyNode::entImage:
		pSwitcher->SetCurrent( 1 );
		break;
		
	case sePropertyNode::entShape:
		pSwitcher->SetCurrent( 2 );
		break;
		
	case sePropertyNode::entText:
		pSwitcher->SetCurrent( 3 );
		break;
		
	case sePropertyNode::entGroup:
		pSwitcher->SetCurrent( 4 );
		break;
		
	default:
		pSwitcher->SetCurrent( 0 ); // empty
	}
}



void seWPNode::UpdateNode(){
	const seProperty * const property = GetProperty();
	const sePropertyNode * const node = GetNode();
	
	if( property && property->GetValueType() == seProperty::evtConstructed ){
		decString text;
		text.Format( "Selected Nodes: %d", property->GetNodeSelection().GetSelected().GetCount() );
		pLabSelection->SetText( text );
		
	}else{
		pLabSelection->ClearText();
	}
	
	if( node ){
		pEditPosition->SetPoint3( node->GetPosition() );
		pEditSize->SetPoint3( node->GetSize() );
		pEditRotation->SetFloat( node->GetRotation() );
		pEditShear->SetFloat( node->GetShearing() );
		pEditBrightness->SetFloat( node->GetBrightness() );
		pEditContrast->SetFloat( node->GetContrast() );
		pEditGamma->SetFloat( node->GetGamma() );
		pClrColorize->SetColor( node->GetColorize() );
		pSldTransparency->SetValue( node->GetTransparency() );
		pCBCombineMode->SetSelectionWithData( ( void* )( intptr_t )node->GetCombineMode() );
		
		if( node->GetMask() ){
			switch( node->GetMask()->GetNodeType() ){
			case sePropertyNode::entGroup:
				pLabMask->SetText( "Maske: Group" );
				break;
				
			case sePropertyNode::entImage:
				pLabMask->SetText( "Maske: Image" );
				break;
				
			case sePropertyNode::entShape:
				pLabMask->SetText( "Maske: Shape" );
				break;
				
			case sePropertyNode::entText:
				pLabMask->SetText( "Maske: Text" );
				break;
			}
			
		}else{
			pLabMask->SetText( "No Mask" );
		}
		
		switch( node->GetNodeType() ){
		case sePropertyNode::entImage:{
			const sePropertyNodeImage &nodeImage = *( ( sePropertyNodeImage* )node );
			
			pImageEditImage->SetPath( nodeImage.GetPath() );
			
			if( nodeImage.GetImage() ){
				const deImage &image = *nodeImage.GetImage();
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
				pImageLabImageInfo->SetText( text );
				pImageLabImageInfo->SetDescription( text );
				
			}else{
				pImageLabImageInfo->ClearText();
				pImageLabImageInfo->SetDescription( "" );
			}
			
			pImageEditRepeat->SetPoint( nodeImage.GetRepeat() );
			}break;
			
		case sePropertyNode::entShape:{
			const sePropertyNodeShape &nodeShape = *( ( sePropertyNodeShape* )node );
			pShapeCBType->SetSelectionWithData( ( void* )( intptr_t )nodeShape.GetShapeType() );
			pShapeClrFill->SetColor( nodeShape.GetFillColor() );
			pShapeClrLine->SetColor( nodeShape.GetLineColor() );
			pShapeEditThickness->SetFloat( nodeShape.GetThickness() );
			}break;
			
		case sePropertyNode::entText:{
			const sePropertyNodeText &nodeText = *( ( sePropertyNodeText* )node );
			pTextEditFont->SetPath( nodeText.GetPath() );
			pTextEditFontSize->SetFloat( nodeText.GetFontSize() );
			
			if( nodeText.GetFont() ){
				const deFont &font = *nodeText.GetFont();
				const char *type;
				decString text;
				
				if( font.GetIsColorFont() ){
					type = "Color";
					
				}else{
					type ="Grayscale";
				}
				
				text.Format( "%s, Line height %d", type, font.GetLineHeight() );
				pTextLabFontInfo->SetText( text );
				pTextLabFontInfo->SetDescription( text );
				
			}else{
				pTextLabFontInfo->ClearText();
				pTextLabFontInfo->SetDescription( "" );
			}
			
			pTextEditText->SetText( nodeText.GetText() );
			pTextClrColor->SetColor( nodeText.GetColor() );
			}break;
			
		default:
			break;
		}
		
	}else{
		pEditPosition->SetPoint3( decPoint3() );
		pEditSize->SetPoint3( decPoint3() );
		pEditRotation->ClearText();
		pEditShear->ClearText();
		pEditBrightness->ClearText();
		pEditContrast->ClearText();
		pEditGamma->ClearText();
		pClrColorize->SetColor( decColor( 1.0f, 1.0f, 1.0f ) );
		pSldTransparency->SetValue( 1.0f );
		pLabMask->ClearText();
	}
	
	const bool enabled = node;
	pEditPosition->SetEnabled( enabled );
	pEditSize->SetEnabled( enabled );
	pEditRotation->SetEnabled( enabled );
	pEditShear->SetEnabled( enabled );
	pEditBrightness->SetEnabled( enabled );
	pEditContrast->SetEnabled( enabled );
	pEditGamma->SetEnabled( enabled );
	pClrColorize->SetEnabled( enabled );
	pSldTransparency->SetEnabled( enabled );
	pCBCombineMode->SetEnabled( enabled );
}

void seWPNode::UpdateOutline(){
	if( pPreventUpdate ){
		// this happens if the user clicks in the tree list to change the selection.
		// this changes the the node selection which in turn triggers events calling
		// again this method. without this check a dead-loop can happen.
		return;
	}
	
	pPreventUpdate = true;
	
	try{
		const seProperty * const property = GetProperty();
		if( ! property ){
			pTreeOutline->RemoveAllItems();
			pPreventUpdate = false;
			return;
		}
		
		const sePropertyNodeGroup &group = *property->GetNodeGroup();
		const int count = group.GetNodeCount();
		igdeTreeItem *item = pTreeOutline->GetFirstChild();
		int i;
		
		for( i=count-1; i>=0; i-- ){
			if( ! item ){
				item = pTreeOutline->AppendItem( NULL, "" );
			}
			UpdateOutline( item, group.GetNodeAt( i ), "" );
			item = item->GetNext();
		}
		
		while( item ){
			igdeTreeItem * const removeItem = item;
			item = item->GetNext();
			pTreeOutline->RemoveItem( removeItem );
		}
		
		pPreventUpdate = false;
		
	}catch( const deException & ){
		pPreventUpdate = false;
		throw;
	}
}

void seWPNode::OutlinerSelectActive(){
	pTreeOutline->SetSelectionWithData( GetNode() );
}



// Private Functions
//////////////////////

void seWPNode::UpdateOutline( igdeTreeItem *item, sePropertyNode *node, const decString &prefix ){
	igdeTreeItem *childItem = item->GetFirstChild();
	
	item->SetData( node );
	
	switch( node->GetNodeType() ){
	case sePropertyNode::entGroup:{
		sePropertyNodeGroup &group = ( sePropertyNodeGroup& )*node;
		const int count = group.GetNodeCount();
		int i;
		
		item->SetText( prefix + "Group" );
		
		for( i=count-1; i>=0; i-- ){
			if( ! childItem ){
				childItem = pTreeOutline->AppendItem( item, "" );
			}
			UpdateOutline( childItem, group.GetNodeAt( i ), "" );
			childItem = childItem->GetNext();
		}
		}break;
		
	case sePropertyNode::entImage:{
		decString path( ( ( sePropertyNodeImage* )node )->GetPath() );
		if( path.GetLength() > 40 ){
			path = decString( "..." ) + path.GetRight( 40 );
		}
		item->SetText( prefix + "Image: " + path );
		}break;
		
	case sePropertyNode::entShape:
		switch( ( ( sePropertyNodeShape* )node )->GetShapeType() ){
		case deSkinPropertyNodeShape::estRectangle:
			item->SetText( prefix + "Shape: Rectangle" );
			break;
			
		case deSkinPropertyNodeShape::estEllipse:
			item->SetText( prefix + "Shape: Ellipse" );
			break;
			
		default:
			item->SetText( prefix + "Shape" );
		}
		break;
		
	case sePropertyNode::entText:{
		decString text( ( ( sePropertyNodeText* )node )->GetText() );
		if( text.GetLength() > 40 ){
			text = text.GetLeft( 40 ) + "...";
		}
		item->SetText( prefix + "Text: " + text );
		}break;
	}
	
	pTreeOutline->ItemChanged( item );
	
	if( node->GetMask() ){
		if( ! childItem ){
			childItem = pTreeOutline->AppendItem( item, "" );
		}
		UpdateOutline( childItem, node->GetMask(), "Mask: ");
		childItem = childItem->GetNext();
	}
	
	while( childItem ){
		igdeTreeItem * const removeItem = childItem;
		childItem = childItem->GetNext();
		pTreeOutline->RemoveItem( removeItem );
	}
}
