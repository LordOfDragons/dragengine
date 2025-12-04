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

#include "gdeWPSTIMOCBillboard.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/billboard/gdeOCBillboard.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItem::Ref.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMOCBillboard::gdeWPSTIMOCBillboard( gdeWPSTreeModel &tree, gdeObjectClass *objectClass,
	gdeOCBillboard *billboard, int index ) :
gdeWPSTIMOCSubObject( tree, etObjectClassBillboard, objectClass, index ),
pBillboard( NULL )
{
	if( ! billboard ){
		DETHROW( deeInvalidParam );
	}
	
	decString text;
	text.Format( "Billboard #%d", index + 1 );
	SetText( text );
	
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ) );
	
	pBillboard = billboard;
	billboard->AddReference();
}

gdeWPSTIMOCBillboard::~gdeWPSTIMOCBillboard(){
	if( pBillboard ){
		pBillboard->FreeReference();
	}
}



// Management
///////////////

void gdeWPSTIMOCBillboard::Validate(){
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon(
		IsValid() ? igdeEnvironment::esiNew : igdeEnvironment::esiWarning ) );
	ItemChanged();
}

bool gdeWPSTIMOCBillboard::IsValid() const{
	deVirtualFileSystem &vfs = *GetWindowMain().GetEnvironment().GetFileSystemGame();
	
	if( ! pBillboard->GetSkinPath().IsEmpty()
	&& ! vfs.ExistsFile( decPath::CreatePathUnix( pBillboard->GetSkinPath() ) ) ){
		return false;
	}
	
	return true;
}



void gdeWPSTIMOCBillboard::OnAddedToTree(){
	Validate();
}

void gdeWPSTIMOCBillboard::OnSelected(){
	GetGameDefinition().SetActiveObjectClass( GetObjectClass() );
	GetGameDefinition().SetActiveOCBillboard( pBillboard );
	GetGameDefinition().SetSelectedObjectType( gdeGameDefinition::eotOCBillboard );
}

void gdeWPSTIMOCBillboard::OnContextMenu( igdeMenuCascade &contextMenu ){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, windowMain.GetActionOCBillboardAdd() );
	helper.MenuCommand( contextMenu, windowMain.GetActionOCBillboardRemove() );
	helper.MenuSeparator( contextMenu );
	
	helper.MenuCommand( contextMenu, windowMain.GetActionOCBillboardCopy() );
	helper.MenuCommand( contextMenu, windowMain.GetActionOCBillboardCut() );
	helper.MenuCommand( contextMenu, windowMain.GetActionOCBillboardPaste() );
}
