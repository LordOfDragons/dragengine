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

#include "gdeWPSTIMParticleEmitter.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/particleemitter/gdeParticleEmitter.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItemReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMParticleEmitter::gdeWPSTIMParticleEmitter( gdeWPSTreeModel &tree, gdeParticleEmitter *particleEmitter ) :
gdeWPSTreeItemModel( tree, etParticleEmitter ),
pParticleEmitter( NULL )
{
	if( ! particleEmitter ){
		DETHROW( deeInvalidParam );
	}
	
	SetText( particleEmitter->GetName() );
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ) );
	
	pParticleEmitter = particleEmitter;
	particleEmitter->AddReference();
}

gdeWPSTIMParticleEmitter::~gdeWPSTIMParticleEmitter(){
	if( pParticleEmitter ){
		pParticleEmitter->FreeReference();
	}
}



// Management
///////////////

void gdeWPSTIMParticleEmitter::NameChanged(){
	SetText( pParticleEmitter->GetName() );
	ParentSortItems();
}



int gdeWPSTIMParticleEmitter::Compare( const gdeWPSTreeItemModel &item ) const{
	const gdeWPSTIMParticleEmitter &other = ( const gdeWPSTIMParticleEmitter & )item;
	return pParticleEmitter->GetName().Compare( other.GetParticleEmitter()->GetName() );
}

void gdeWPSTIMParticleEmitter::OnSelected(){
	GetGameDefinition().SetActiveParticleEmitter( pParticleEmitter );
	GetGameDefinition().SetSelectedObjectType( gdeGameDefinition::eotParticleEmitter );
}

void gdeWPSTIMParticleEmitter::OnContextMenu( igdeMenuCascade &contextMenu ){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, windowMain.GetActionParticleEmitterAdd() );
	helper.MenuCommand( contextMenu, windowMain.GetActionParticleEmitterRemove() );
}

void gdeWPSTIMParticleEmitter::SelectBestMatching( const char *string ){
	if( GetParent() ){
		( ( gdeWPSTreeItemModel* )GetParent() )->SelectBestMatching( string );
	}
}
