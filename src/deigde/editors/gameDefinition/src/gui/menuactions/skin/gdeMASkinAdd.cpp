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

#include "gdeMASkinAdd.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/skin/gdeSkin.h"
#include "../../../undosys/skin/gdeUAddSkin.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>



// Class gdeMASkinAdd
///////////////////////

// Constructor
////////////////

gdeMASkinAdd::gdeMASkinAdd( gdeWindowMain &windowMain ) :
gdeBaseAction( windowMain, "Add Skin...",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ),
	"Add skin" )
{
}



// Management
///////////////

igdeUndo *gdeMASkinAdd::OnAction( gdeGameDefinition &gameDefinition ){
	igdeEnvironment &environment = pWindowMain.GetEnvironment();
	decString filename;
	//dialog.SetFilename( ... last skin? what directory? );
	
	if( ! igdeCommonDialogs::GetFileOpen( &pWindowMain,
	"Select skin material", *gameDefinition.GetPreviewVFS(),
	*environment.GetOpenFilePatternList( igdeEnvironment::efpltSkin ), filename ) ){
		return NULL;
	}
	
	if( gameDefinition.GetSkins().HasWithPath( filename ) ){
		igdeCommonDialogs::Information( &pWindowMain, "Add Skin", "Skin with path exists already." );
		return NULL;
	}
	
	decString filetitle( decPath::CreatePathUnix( filename ).GetLastComponent() );
	const int delimiter = filetitle.FindReverse( '.' );
	if( delimiter != -1 ){
		filetitle = filetitle.GetLeft( delimiter );
	}
	
	deObjectReference skin;
	skin.TakeOver( new gdeSkin( filename, filetitle ) );
	return new gdeUAddSkin( &gameDefinition, ( gdeSkin* )( deObject* )skin );
}
