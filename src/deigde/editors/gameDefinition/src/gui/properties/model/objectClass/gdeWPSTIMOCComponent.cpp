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

#include "gdeWPSTIMOCComponent.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/component/gdeOCComponent.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItemReference.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMOCComponent::gdeWPSTIMOCComponent( gdeWPSTreeModel &tree, gdeObjectClass *objectClass,
	gdeOCComponent *component, int index ) :
gdeWPSTIMOCSubObject( tree, etObjectClassComponent, objectClass, index ),
pComponent( NULL )
{
	if( ! component ){
		DETHROW( deeInvalidParam );
	}
	
	decString text;
	text.Format( "Component #%d", index + 1 );
	SetText( text );
	
	pComponent = component;
	component->AddReference();
}

gdeWPSTIMOCComponent::~gdeWPSTIMOCComponent(){
	if( pComponent ){
		pComponent->FreeReference();
	}
}



// Management
///////////////

void gdeWPSTIMOCComponent::Validate(){
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon(
		IsValid() ? igdeEnvironment::esiNew : igdeEnvironment::esiWarning ) );
	ItemChanged();
}

bool gdeWPSTIMOCComponent::IsValid() const{
	deVirtualFileSystem &vfs = *GetWindowMain().GetEnvironment().GetFileSystemGame();
	
	if( ! pComponent->GetModelPath().IsEmpty()
	&& ! vfs.ExistsFile( decPath::CreatePathUnix( pComponent->GetModelPath() ) ) ){
		return false;
	}
	if( ! pComponent->GetSkinPath().IsEmpty()
	&& ! vfs.ExistsFile( decPath::CreatePathUnix( pComponent->GetSkinPath() ) ) ){
		return false;
	}
	if( ! pComponent->GetRigPath().IsEmpty()
	&& ! vfs.ExistsFile( decPath::CreatePathUnix( pComponent->GetRigPath() ) ) ){
		return false;
	}
	if( ! pComponent->GetAnimatorPath().IsEmpty()
	&& ! vfs.ExistsFile( decPath::CreatePathUnix( pComponent->GetAnimatorPath() ) ) ){
		return false;
	}
	if( ! pComponent->GetAnimationPath().IsEmpty()
	&& ! vfs.ExistsFile( decPath::CreatePathUnix( pComponent->GetAnimationPath() ) ) ){
		return false;
	}
	if( ! pComponent->GetOcclusionMeshPath().IsEmpty()
	&& ! vfs.ExistsFile( decPath::CreatePathUnix( pComponent->GetOcclusionMeshPath() ) ) ){
		return false;
	}
	if( ! pComponent->GetAudioModelPath().IsEmpty()
	&& ! vfs.ExistsFile( decPath::CreatePathUnix( pComponent->GetAudioModelPath() ) ) ){
		return false;
	}
	
	return true;
}

void gdeWPSTIMOCComponent::ValidateObjectClassName(){
	Validate();
}



void gdeWPSTIMOCComponent::OnAddedToTree(){
	Validate();
}

void gdeWPSTIMOCComponent::OnSelected(){
	GetGameDefinition().SetActiveObjectClass( GetObjectClass() );
	GetGameDefinition().SetActiveOCComponent( pComponent );
	GetGameDefinition().SetSelectedObjectType( gdeGameDefinition::eotOCComponent );
}

void gdeWPSTIMOCComponent::OnContextMenu( igdeMenuCascade &contextMenu ){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, windowMain.GetActionOCComponentAdd() );
	helper.MenuCommand( contextMenu, windowMain.GetActionOCComponentRemove() );
	helper.MenuSeparator( contextMenu );
	
	helper.MenuCommand( contextMenu, windowMain.GetActionOCComponentCopy() );
	helper.MenuCommand( contextMenu, windowMain.GetActionOCComponentCut() );
	helper.MenuCommand( contextMenu, windowMain.GetActionOCComponentPaste() );
}
