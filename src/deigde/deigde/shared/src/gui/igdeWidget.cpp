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

#include "igdeWidget.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "../engine/igdeEngineController.h"
#include "../environment/igdeEnvironment.h"
#include "../gameproject/igdeGameProject.h"

#include <dragengine/common/exceptions.h>



// Class igdeWidget
/////////////////////

// Constructor, destructor
////////////////////////////

igdeWidget::igdeWidget(igdeEnvironment &environment) :
pEnvironment(environment),
pNativeWidget(NULL),
pParent(NULL),
pVisible(true){
}

igdeWidget::~igdeWidget(){
}



// Management
///////////////

void igdeWidget::SetVisible(bool visible){
	if(pVisible == visible){
		return;
	}
	
	pVisible = visible;
	OnVisibleChanged();
}

igdeEngineController &igdeWidget::GetEngineController() const{
	return *pEnvironment.GetEngineController();
}

igdeGameProject *igdeWidget::GetGameProject() const{
	return pEnvironment.GetGameProject();
}

igdeGameDefinition *igdeWidget::GetGameDefinition() const{
	return GetGameProject()->GetGameDefinition();
}

deEngine *igdeWidget::GetEngine() const{
	return GetEngineController().GetEngine();
}

deLogger *igdeWidget::GetLogger() const{
	return pEnvironment.GetLogger();
}



void igdeWidget::SetWidgetGuiTheme(igdeGuiTheme *guitheme){
	if(pGuiTheme == guitheme){
		return;
	}
	
	pGuiTheme = guitheme;
	OnGuiThemeChanged();
}

void igdeWidget::SetWidgetGuiThemeName(const char *guitheme){
	if(pGuiThemeName == guitheme){
		return;
	}
	
	pGuiThemeName = guitheme;
	OnGuiThemeChanged();
}

igdeGuiTheme *igdeWidget::GetGuiTheme() const{
	if(pGuiTheme){
		return pGuiTheme;
		
	}else if(!pGuiThemeName.IsEmpty()){
		return pEnvironment.GetGuiThemeNamed(pGuiThemeName);
		
	}else if(pParent){
		return pParent->GetGuiTheme();
		
	}else{
		return pEnvironment.GetDefaultGuiTheme();
	}
}



void igdeWidget::SetParent(igdeContainer *parent){
	pParent = parent;
}

void igdeWidget::OnResize(){
}



decPoint igdeWidget::WidgetToScreen(const decPoint &position) const{
	return igdeNativeWidget::WidgetToScreen(*this, position);
}

igdeWindow *igdeWidget::GetParentWindow(){
	if(pParent){
		return pParent->GetParentWindow();
		
	}else{
		return NULL;
	}
}



void igdeWidget::OnVisibleChanged(){
	igdeNativeWidget::UpdateVisible(*this);
}

void igdeWidget::OnGuiThemeChanged(){
}

void igdeWidget::SetNativeWidget(void *nativeWidget){
	pNativeWidget = nativeWidget;
}

void igdeWidget::DropNativeWidget(){
	pNativeWidget = NULL;
}
