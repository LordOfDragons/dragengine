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

#include "igdeNativeBeosMainWindow.h"
#include "../../igdeMainWindow.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../../environment/igdeEnvironment.h"
#include <dragengine/common/exceptions.h>


igdeNativeBeosMainWindow::igdeNativeBeosMainWindow(igdeMainWindow &owner) :
pOwner(owner){
}

igdeNativeBeosMainWindow::~igdeNativeBeosMainWindow() = default;


igdeNativeBeosMainWindow* igdeNativeBeosMainWindow::CreateNativeWidget(igdeMainWindow &owner){
	return new igdeNativeBeosMainWindow(owner);
}

void igdeNativeBeosMainWindow::PostCreateNativeWidget(){
}

void igdeNativeBeosMainWindow::DestroyNativeWidget(){
	delete this;
}

decColor igdeNativeBeosMainWindow::GetSystemColor(igdeEnvironment::eSystemColors color) const{
	// Return BeOS system colors based on environment color enum
	switch(color){
	case igdeEnvironment::escWindowBackground:
		return decColor(ui_color(B_PANEL_BACKGROUND_COLOR));
		
	case igdeEnvironment::escWindowText:
		return decColor(ui_color(B_PANEL_TEXT_COLOR));
		
	case igdeEnvironment::escButtonBackground:
		return decColor(ui_color(B_BUTTON_BACKGROUND_COLOR));
		
	case igdeEnvironment::escButtonText:
		return decColor(ui_color(B_BUTTON_TEXT_COLOR));
		
	case igdeEnvironment::escFocusBackground:
		return decColor(ui_color(B_KEYBOARD_NAVIGATION_COLOR));
		
	default:
		return decColor(0.5f, 0.5f, 0.5f);
	}
}

void igdeNativeBeosMainWindow::UpdateWindowState(){
	// Sync window state from owner to native representation
}

void igdeNativeBeosMainWindow::UpdateEnabled(){
	// Main window enable/disable would affect all children
}

void igdeNativeBeosMainWindow::UpdatePosition(){
	// Main window position update
}

void igdeNativeBeosMainWindow::UpdateIcon(){
	// Main window icon update
}

void igdeNativeBeosMainWindow::UpdateTitle(){
	// Main window title update
}

void igdeNativeBeosMainWindow::UpdateSize(){
	// Main window size update
}

void igdeNativeBeosMainWindow::RaiseAndActivate(){
	// Raise and activate main window
}

void igdeNativeBeosMainWindow::SetWindowState(){
	// Sync window state from native back to owner
}

void igdeNativeBeosMainWindow::GetAppFontConfig(igdeFont::sConfiguration &config){
	// Set up default BeOS application font configuration
	config.name = "DejaVu Sans";
	config.size = 12;
}

#endif
