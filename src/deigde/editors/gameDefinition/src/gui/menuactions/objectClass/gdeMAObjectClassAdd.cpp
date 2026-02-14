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

#include "gdeMAObjectClassAdd.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../undosys/objectClass/gdeUAddObjectClass.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAObjectClassAdd
//////////////////////////////

// Constructor
////////////////

gdeMAObjectClassAdd::gdeMAObjectClassAdd(gdeWindowMain &windowMain) :
gdeBaseAction(windowMain, "@GameDefinition.Menu.ObjectClassAdd",
	windowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
	"@GameDefinition.Menu.ObjectClassAdd.ToolTip")
{
}



// Management
///////////////

igdeUndo::Ref gdeMAObjectClassAdd::OnAction(gdeGameDefinition &gameDefinition){
	const gdeObjectClass::List &list = gameDefinition.GetObjectClasses();
	decString name(pWindowMain.Translate("GameDefinition.Default.ObjectClass").ToUTF8());
	
	while(igdeCommonDialogs::GetString(pWindowMain, "@GameDefinition.Dialog.ObjectClassAdd.Title", "@GameDefinition.Dialog.ObjectClassAdd.Name", name)){
		if(list.HasNamed(name)){
			igdeCommonDialogs::Error(pWindowMain, "@GameDefinition.Dialog.ObjectClassAdd.Title", "@GameDefinition.Dialog.ObjectClassAdd.ErrorExists");
			continue;
		}
		
		const gdeObjectClass::Ref objectClass(gdeObjectClass::Ref::New(name));
		return gdeUAddObjectClass::Ref::New(&gameDefinition, objectClass);
	}
	
	return {};
}
