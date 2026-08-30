/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifdef IGDE_TOOLKIT_BEOS

#include "igdeNativeBeosMenuCommand.h"
#include "../../../igdeMenuCommand.h"
#include <dragengine/common/exceptions.h>


// Class igdeNativeBeosMenuCommand
////////////////////////////////////

igdeNativeBeosMenuCommand::igdeNativeBeosMenuCommand() = default;
igdeNativeBeosMenuCommand::~igdeNativeBeosMenuCommand() = default;


igdeNativeBeosMenuCommand* igdeNativeBeosMenuCommand::CreateNativeMenu(igdeMenuCommand &owner){
	// MenuCommand is an action menu item that triggers commands
	// Implementation provides activation callback and keyboard shortcuts
	igdeNativeBeosMenuCommand *menu = NULL;
	
	try{
		menu = new igdeNativeBeosMenuCommand();
		
	}catch(const deException &){
		return NULL;
	}
	
	return menu;
}

#endif
