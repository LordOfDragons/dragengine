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

#include "gdeMASkyAdd.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/sky/gdeSky.h"
#include "../../../undosys/sky/gdeUAddSky.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>



// Class gdeMASkyAdd
//////////////////////

// Constructor
////////////////

gdeMASkyAdd::gdeMASkyAdd(gdeWindowMain &windowMain) :
gdeBaseAction(windowMain, "Add Sky...",
	windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
	"Add sky")
{
}



// Management
///////////////

igdeUndo *gdeMASkyAdd::OnAction(gdeGameDefinition &gameDefinition){
	igdeEnvironment &environment = pWindowMain.GetEnvironment();
	decString filename;
	//dialog.SetFilename( ... last sky? what directory? );
	
	if(!igdeCommonDialogs::GetFileOpen(&pWindowMain,
	"Select sky", *gameDefinition.GetPreviewVFS(),
	*environment.GetOpenFilePatternList( igdeEnvironment::efpltSky ), filename ) ){
		return NULL;
	}
	
	if(gameDefinition.GetSkies().HasWithPath(filename)){
		igdeCommonDialogs::Information(&pWindowMain, "Add Sky", "Sky with path exists already.");
		return NULL;
	}
	
	decString filetitle(decPath::CreatePathUnix(filename).GetLastComponent());
	const int delimiter = filetitle.FindReverse('.');
	if(delimiter != -1){
		filetitle = filetitle.GetLeft(delimiter);
	}
	
	const gdeSky::Ref sky(gdeSky::Ref::NewWith(filename, filetitle));
	return new gdeUAddSky(&gameDefinition, sky);
}
