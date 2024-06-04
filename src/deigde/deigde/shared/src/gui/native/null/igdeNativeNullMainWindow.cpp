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

#ifdef IGDE_TOOLKIT_NULL

#include "igdeNativeNullApplication.h"
#include "igdeNativeNullMainWindow.h"
#include "../../igdeApplication.h"
#include "../../igdeMainWindow.h"
#include "../../../engine/igdeEngineController.h"


// Class igdeNativeNullMainWindow
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeNullMainWindow::igdeNativeNullMainWindow( igdeMainWindow &owner ) :
pOwner( owner )
{
	owner.GetEngineController().InitEngine();
	owner.StartEngine();
}

igdeNativeNullMainWindow::~igdeNativeNullMainWindow(){
	pOwner.GetEngineController().UnparentMainRenderWindow();
	( ( igdeNativeNullApplication* )igdeApplication::app().GetNativeApplication() )->Quit();
}

igdeNativeNullMainWindow *igdeNativeNullMainWindow::CreateNativeWidget( igdeMainWindow &owner ){
	return new igdeNativeNullMainWindow( owner );
}

void igdeNativeNullMainWindow::PostCreateNativeWidget(){
}

void igdeNativeNullMainWindow::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

decColor igdeNativeNullMainWindow::GetSystemColor( igdeEnvironment::eSystemColors color ) const{
	switch( color ){
	case igdeEnvironment::escWindowForeground:
	case igdeEnvironment::escWidgetForeground:
	case igdeEnvironment::escWidgetSelectedBackground:
		return decColor( 0.0f, 0.0f, 0.0f );
		
	case igdeEnvironment::escWidgetBackground:
		return decColor( 0.65f, 0.65f, 0.65f );
		
	case igdeEnvironment::escWidgetHighlight:
		return decColor( 0.9f, 0.9f, 0.9f );
		
	case igdeEnvironment::escWidgetShadow:
		return decColor( 0.2f, 0.2f, 0.2f );
		
	case igdeEnvironment::escWindowBackground:
	case igdeEnvironment::escWidgetSelectedForeground:
	default:
		return decColor( 1.0f, 1.0f, 1.0f );
	}
}

void igdeNativeNullMainWindow::UpdateWindowState(){
}

void igdeNativeNullMainWindow::UpdateEnabled(){
}

void igdeNativeNullMainWindow::UpdatePosition(){
}

void igdeNativeNullMainWindow::UpdateIcon(){
}

void igdeNativeNullMainWindow::UpdateTitle(){
}

void igdeNativeNullMainWindow::UpdateSize(){
}

void igdeNativeNullMainWindow::SetWindowState(){
}

void igdeNativeNullMainWindow::GetAppFontConfig( igdeFont::sConfiguration &config ){
	config.name = "Courier";
	config.size = 12.0;
	config.bold = false;
	config.italic = false;
	config.underline = false;
	config.strikeThrough = false;
}

#endif
