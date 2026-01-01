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

#include "gdeMASkinAdd.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/skin/gdeSkin.h"
#include "../../../undosys/skin/gdeUAddSkin.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>



// Class gdeMASkinAdd
///////////////////////

// Constructor
////////////////

gdeMASkinAdd::gdeMASkinAdd(gdeWindowMain &windowMain) :
gdeBaseAction(windowMain, "Add Skin...",
	windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
	"Add skin")
{
}



// Management
///////////////

igdeUndo::Ref gdeMASkinAdd::OnAction(gdeGameDefinition &gameDefinition){
	igdeEnvironment &environment = pWindowMain.GetEnvironment();
	decString filename;
	//dialog.SetFilename( ... last skin? what directory? );
	
	if(!igdeCommonDialogs::GetFileOpen(&pWindowMain,
	"Select skin material", *gameDefinition.GetPreviewVFS(),
	*environment.GetOpenFilePatternList( igdeEnvironment::efpltSkin ), filename ) ){
		return {};
	}
	
	if(gameDefinition.GetSkins().HasMatching([&](const gdeSkin &skin){
		return skin.GetPath() == filename;
	})){
		igdeCommonDialogs::Information(&pWindowMain, "Add Skin", "Skin with path exists already.");
		return {};
	}
	
	decString filetitle(decPath::CreatePathUnix(filename).GetLastComponent());
	const int delimiter = filetitle.FindReverse('.');
	if(delimiter != -1){
		filetitle = filetitle.GetLeft(delimiter);
	}
	
	const gdeSkin::Ref skin(gdeSkin::Ref::New(filename, filetitle));
	return gdeUAddSkin::Ref::New(&gameDefinition, skin);
}
