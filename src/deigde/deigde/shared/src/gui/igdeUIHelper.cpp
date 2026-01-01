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

#include "igdeUIHelper.h"
#include "igdeGroupBox.h"
#include "igdeContainer.h"
#include "igdeTextArea.h"
#include "igdeTextField.h"
#include "igdeProgressBar.h"
#include "igdeSpinTextField.h"
#include "igdeButton.h"
#include "igdeColorBox.h"
#include "igdeComboBox.h"
#include "igdeComboBoxFilter.h"
#include "igdeLabel.h"
#include "igdeCheckBox.h"
#include "igdeToolBar.h"
#include "igdeToolBarSeparator.h"
#include "igdeToggleButton.h"
#include "igdeListBox.h"
#include "igdeIconListBox.h"
#include "igdeScrollBar.h"
#include "igdeSeparator.h"
#include "igdeSlider.h"
#include "igdeTreeList.h"
#include "igdeWidget.h"
#include "igdeSpacer.h"
#include "composed/igdeEditPath.h"
#include "composed/igdeEditPathListener.h"
#include "composed/igdeEditDirectory.h"
#include "composed/igdeEditDirectoryListener.h"
#include "composed/igdeEditPoint.h"
#include "composed/igdeEditPointListener.h"
#include "composed/igdeEditPoint3.h"
#include "composed/igdeEditPoint3Listener.h"
#include "composed/igdeEditSliderText.h"
#include "composed/igdeEditSliderTextListener.h"
#include "composed/igdeEditDVector.h"
#include "composed/igdeEditDVectorListener.h"
#include "composed/igdeEditVector.h"
#include "composed/igdeEditVectorListener.h"
#include "composed/igdeEditVector2.h"
#include "composed/igdeEditVector2Listener.h"
#include "composed/igdeEditTags.h"
#include "composed/igdeEditPropertyValue.h"
#include "composed/igdeEditPropertyValueListener.h"
#include "composed/igdeToggleTags.h"
#include "curveedit/igdeViewCurveBezier.h"
#include "curveedit/igdeViewCurveBezierListener.h"
#include "layout/igdeContainerForm.h"
#include "layout/igdeContainerBorder.h"
#include "layout/igdeContainerBox.h"
#include "layout/igdeContainerFlow.h"
#include "layout/igdeContainerScroll.h"
#include "event/igdeIconListBoxListener.h"
#include "event/igdeTextAreaListener.h"
#include "event/igdeTextFieldListener.h"
#include "event/igdeSpinTextFieldListener.h"
#include "event/igdeColorBoxListener.h"
#include "event/igdeComboBoxListener.h"
#include "event/igdeListBoxListener.h"
#include "event/igdeTreeListListener.h"
#include "event/igdeScrollBarListener.h"
#include "event/igdeSliderListener.h"
#include "menu/igdeMenuCascade.h"
#include "menu/igdeMenuCommand.h"
#include "menu/igdeMenuCheck.h"
#include "menu/igdeMenuOption.h"
#include "menu/igdeMenuSeparator.h"
#include "model/igdeListHeader.h"
#include "properties/igdeWPCamera.h"
#include "properties/igdeWPWObject.h"
#include "properties/igdeWPSky.h"
#include "properties/igdeWPTriggerTable.h"
#include "theme/themeNames.h"
#include "../utils/igdeRecentFiles.h"

#include <dragengine/common/exceptions.h>



// Class igdeUIHelper::EnableBoolGuard
/////////////////////////////////////////

igdeUIHelper::EnableBoolGuard::EnableBoolGuard(bool &flag) : pFlag(flag){
	pFlag = true;
}

igdeUIHelper::EnableBoolGuard::~EnableBoolGuard(){
	pFlag = false;
}



// struct igdeUIHelper::sColumnHeader
///////////////////////////////////////

igdeUIHelper::sColumnHeader::sColumnHeader(const char *ptitle, igdeIcon *picon, int psize) :
title(ptitle),
icon(picon),
size(psize){
}



// Class igdeUIHelper
///////////////////////

// Constructor, destructor
////////////////////////////

igdeUIHelper::igdeUIHelper(igdeEnvironment &environment) :
pEnvironment(environment){
}

igdeUIHelper::igdeUIHelper(const igdeUIHelper &helper) :
pEnvironment(helper.pEnvironment){
}



// Helpers
////////////

void igdeUIHelper::SidePanel(igdeContainerScroll::Ref &scroll, igdeContainer::Ref &panel,
bool stretchLast, int spacing){
	scroll = igdeContainerScroll::Ref::New(pEnvironment, false, true);
	
	panel = igdeContainerFlow::Ref::New(pEnvironment, igdeContainerFlow::eaY,
		stretchLast ? igdeContainerFlow::esLast : igdeContainerFlow::esNone, spacing);
	scroll->AddChild(panel);
	panel->SetWidgetGuiThemeName(igdeGuiThemeNames::properties);
}

void igdeUIHelper::SidePanel(igdeContainer &parent, igdeContainer::Ref &panel,
bool stretchLast, int spacing){
	igdeContainerScroll::Ref scroll;
	SidePanel(scroll, panel, stretchLast, spacing);
	parent.AddChild(scroll);
}



void igdeUIHelper::GroupBox(igdeContainer &parent, igdeContainer::Ref &groupBox,
const char *title, bool collapsed) const{
	igdeGroupBox::Ref frame(igdeGroupBox::Ref::New(pEnvironment, title, collapsed));
	parent.AddChild(frame);
	
	groupBox = igdeContainerForm::Ref::New(pEnvironment);
	frame->AddChild(groupBox);
}

void igdeUIHelper::GroupBoxStatic(igdeContainer &parent,
igdeContainer::Ref &groupBox, const char *title) const{
	igdeGroupBox::Ref frame(igdeGroupBox::Ref::New(pEnvironment, title));
	parent.AddChild(frame);
	
	groupBox = igdeContainerForm::Ref::New(pEnvironment);
	frame->AddChild(groupBox);
}

void igdeUIHelper::GroupBoxFlow(igdeContainer &parent, igdeContainer::Ref &groupBox,
const char *title, bool stretchLast, bool collapsed) const{
	igdeGroupBox::Ref frame(igdeGroupBox::Ref::New(pEnvironment, title, collapsed));
	parent.AddChild(frame);
	
	groupBox = igdeContainerFlow::Ref::New(pEnvironment, igdeContainerFlow::eaY,
		stretchLast ? igdeContainerFlow::esLast : igdeContainerFlow::esNone);
	frame->AddChild(groupBox);
}

void igdeUIHelper::GroupBoxBorder(igdeContainer &parent, igdeContainerBorder::Ref &groupBox,
const char *title, bool collapsed) const{
	igdeGroupBox::Ref frame(igdeGroupBox::Ref::New(pEnvironment, title, collapsed));
	parent.AddChild(frame);
	
	groupBox = igdeContainerBorder::Ref::New(pEnvironment);
	frame->AddChild(groupBox);
}

void igdeUIHelper::GroupBoxStaticFlow(igdeContainer &parent, igdeContainer::Ref &groupBox,
const char *title, bool stretchLast) const{
	igdeGroupBox::Ref frame(igdeGroupBox::Ref::New(pEnvironment, title));
	if(stretchLast){
		frame->SetStretchLast(stretchLast);
	}
	parent.AddChild(frame);
	
	groupBox = igdeContainerFlow::Ref::New(pEnvironment, igdeContainerFlow::eaY,
		stretchLast ? igdeContainerFlow::esLast : igdeContainerFlow::esNone);
	frame->AddChild(groupBox);
}

void igdeUIHelper::GroupBoxStaticBorder(igdeContainer &parent,
igdeContainerBorder::Ref &groupBox, const char *title, bool stretch) const{
	igdeGroupBox::Ref frame(igdeGroupBox::Ref::New(pEnvironment, title));
	if(stretch){
		frame->SetStretchLast(stretch);
	}
	parent.AddChild(frame);
	
	groupBox = igdeContainerBorder::Ref::New(pEnvironment);
	frame->AddChild(groupBox);
}



void igdeUIHelper::FormLine(igdeContainer &form, const char *label, const char *description,
igdeWidget *widget){
	FormLine(form, label, description, widget, igdeLabel::eaLeft | igdeLabel::eaMiddle);
}

void igdeUIHelper::FormLine(igdeContainer &form, const char *label, const char *description,
igdeWidget *widget, int labelAlignment){
	igdeLabel::Ref wlabel(igdeLabel::Ref::New(pEnvironment, label, description, labelAlignment));
	form.AddChild(wlabel);
	form.AddChild(widget);
}

void igdeUIHelper::FormLine(igdeContainer &form, const char *label, const char *description,
igdeContainer::Ref &line){
	FormLine(form, label, description, line, igdeLabel::eaLeft | igdeLabel::eaMiddle);
}

void igdeUIHelper::FormLine(igdeContainer &form, const char *label, const char *description,
igdeContainer::Ref &line, int labelAlignment){
	igdeLabel::Ref wlabel(igdeLabel::Ref::New(pEnvironment, label, description, labelAlignment));
	form.AddChild(wlabel);
	line = igdeContainerBox::Ref::New(pEnvironment, igdeContainerBox::eaX);
	form.AddChild(line);
}

void igdeUIHelper::FormLineStretchFirst(igdeContainer &form, const char *label,
const char *description, igdeContainer::Ref &line){
	FormLineStretchFirst(form, label, description, line, igdeLabel::eaLeft | igdeLabel::eaMiddle);
}

void igdeUIHelper::FormLineStretchFirst(igdeContainer &form, const char *label,
const char *description, igdeContainer::Ref &line, int labelAlignment){
	igdeLabel::Ref wlabel(igdeLabel::Ref::New(pEnvironment, label, description, labelAlignment));
	form.AddChild(wlabel);
	line = igdeContainerFlow::Ref::New(pEnvironment, igdeContainerFlow::eaX, igdeContainerFlow::esFirst);
	form.AddChild(line);
}

void igdeUIHelper::FormLineDownFlow(igdeContainer &form, const char *label,
const char *description, igdeContainer::Ref &line){
	FormLineDownFlow(form, label, description, line, igdeLabel::eaLeft | igdeLabel::eaTop);
}

void igdeUIHelper::FormLineDownFlow(igdeContainer &form, const char *label,
const char *description, igdeContainer::Ref &line, int labelAlignment){
	igdeLabel::Ref wlabel(igdeLabel::Ref::New(pEnvironment, label, description, labelAlignment));
	form.AddChild(wlabel);
	line = igdeContainerFlow::Ref::New(pEnvironment, igdeContainerFlow::eaY);
	form.AddChild(line);
}



void igdeUIHelper::EditString(igdeContainer &form, const char *label, const char *description,
igdeTextField::Ref &textField, const igdeTextFieldListener::Ref &listener){
	EditString(form, label, description, 15, textField, listener);
}

void igdeUIHelper::EditString(igdeContainer &form, const char *label, const char *description,
int rows, igdeTextField::Ref &textField, const igdeTextFieldListener::Ref &listener){
	EditString(description, rows, textField, listener);
	FormLine(form, label, description, textField);
}

void igdeUIHelper::EditString(igdeContainer &parent, const char *description,
igdeTextField::Ref &textField, const igdeTextFieldListener::Ref &listener){
	EditString(description, textField, listener);
	parent.AddChild(textField);
}

void igdeUIHelper::EditString(igdeContainer &parent, const char *description, int rows,
igdeTextField::Ref &textField, const igdeTextFieldListener::Ref &listener){
	EditString(description, rows, textField, listener);
	parent.AddChild(textField);
}

void igdeUIHelper::EditString(const char *description, igdeTextField::Ref &textField,
const igdeTextFieldListener::Ref &listener){
	EditString(description, 15, textField, listener);
}

void igdeUIHelper::EditString(const char *description, int rows,
igdeTextField::Ref &textField, const igdeTextFieldListener::Ref &listener){
	textField = igdeTextField::Ref::New(pEnvironment, rows, description);
	if(listener){
		textField->AddListener(listener);
	}
}



void igdeUIHelper::EditString(igdeContainer &form, const char *label, const char *description,
igdeTextArea::Ref &textArea, int rows, const igdeTextAreaListener::Ref &listener){
	EditString(form, label, description, textArea, 15, rows, listener);
}

void igdeUIHelper::EditString(igdeContainer &form, const char *label, const char *description,
igdeTextArea::Ref &textArea, int columns, int rows, const igdeTextAreaListener::Ref &listener){
	EditString(description, columns, rows, textArea, listener);
	FormLine(form, label, description, textArea, igdeLabel::eaLeft | igdeLabel::eaTop);
}

void igdeUIHelper::EditString(igdeContainer &parent, const char *description,
igdeTextArea::Ref &textArea, int rows, const igdeTextAreaListener::Ref &listener){
	EditString(parent, description, textArea, 15, rows, listener);
}

void igdeUIHelper::EditString(igdeContainer &parent, const char *description,
igdeTextArea::Ref &textArea, int columns, int rows, const igdeTextAreaListener::Ref &listener){
	EditString(description, columns, rows, textArea, listener);
	parent.AddChild(textArea);
}

void igdeUIHelper::EditString(const char *description, int rows,
igdeTextArea::Ref &textArea, const igdeTextAreaListener::Ref &listener){
	EditString(description, 15, rows, textArea, listener);
}

void igdeUIHelper::EditString(const char *description, int columns, int rows,
igdeTextArea::Ref &textArea, const igdeTextAreaListener::Ref &listener){
	textArea = igdeTextArea::Ref::New(pEnvironment, columns, rows, description);
	if(listener){
		textArea->AddListener(listener);
	}
}



void igdeUIHelper::EditFloat(igdeContainer &form, const char *label, const char *description,
igdeTextField::Ref &textField, const igdeTextFieldListener::Ref &listener){
	EditFloat(form, label, description, 6, 3, textField, listener);
}

void igdeUIHelper::EditFloat(igdeContainer &form, const char *label, const char *description,
int columns, int precision, igdeTextField::Ref &textField, const igdeTextFieldListener::Ref &listener){
	EditFloat(description, columns, precision, textField, listener);
	FormLine(form, label, description, textField);
}

void igdeUIHelper::EditFloat(igdeContainer &parent, const char *description,
igdeTextField::Ref &textField, const igdeTextFieldListener::Ref &listener){
	EditFloat(parent, description, 6, 3, textField, listener);
}

void igdeUIHelper::EditFloat(igdeContainer &parent, const char *description,
int columns, int precision, igdeTextField::Ref &textField, const igdeTextFieldListener::Ref &listener){
	EditFloat(description, columns, precision, textField, listener);
	parent.AddChild(textField);
}

void igdeUIHelper::EditFloat(const char *description, igdeTextField::Ref &textField,
const igdeTextFieldListener::Ref &listener){
	EditFloat(description, 6, 3, textField, listener);
}

void igdeUIHelper::EditFloat(const char *description, int columns, int precision,
igdeTextField::Ref &textField, const igdeTextFieldListener::Ref &listener){
	textField = igdeTextField::Ref::New(pEnvironment, columns, description);
	textField->SetPrecision(precision);
	if(listener){
		textField->AddListener(listener);
	}
}



void igdeUIHelper::EditInteger(igdeContainer &form, const char *label, const char *description,
igdeTextField::Ref &textField, const igdeTextFieldListener::Ref &listener){
	EditInteger(form, label, description, 6, textField, listener);
}

void igdeUIHelper::EditInteger(igdeContainer &form, const char *label, const char *description,
int columns, igdeTextField::Ref &textField, const igdeTextFieldListener::Ref &listener){
	EditInteger(description, columns, textField, listener);
	FormLine(form, label, description, textField);
}

void igdeUIHelper::EditInteger(igdeContainer &parent, const char *description,
igdeTextField::Ref &textField, const igdeTextFieldListener::Ref &listener){
	EditInteger(parent, description, 6, textField, listener);
}

void igdeUIHelper::EditInteger(igdeContainer &parent, const char *description, int columns,
igdeTextField::Ref &textField, const igdeTextFieldListener::Ref &listener){
	EditInteger(description, columns, textField, listener);
	parent.AddChild(textField);
}

void igdeUIHelper::EditInteger(const char *description, igdeTextField::Ref &textField,
const igdeTextFieldListener::Ref &listener){
	EditInteger(description, 6, textField, listener);
}

void igdeUIHelper::EditInteger(const char *description, int columns,
igdeTextField::Ref &textField, const igdeTextFieldListener::Ref &listener){
	textField = igdeTextField::Ref::New(pEnvironment, columns, description);
	if(listener){
		textField->AddListener(listener);
	}
}



void igdeUIHelper::EditSpinInteger(igdeContainer &form, const char *label,
const char *description, int lower, int upper, igdeSpinTextField::Ref &spinTextField,
const igdeSpinTextFieldListener::Ref &listener){
	EditSpinInteger(description, lower, upper, spinTextField, listener);
	FormLine(form, label, description, spinTextField);
}

void igdeUIHelper::EditSpinInteger(igdeContainer &parent, const char *description,
int lower, int upper, igdeSpinTextField::Ref &spinTextField, const igdeSpinTextFieldListener::Ref &listener){
	spinTextField = igdeSpinTextField::Ref::New(pEnvironment, lower, upper, 6, description);
	if(listener){
		spinTextField->AddListener(listener);
	}
	parent.AddChild(spinTextField);
}

void igdeUIHelper::EditSpinInteger(const char *description, int lower, int upper,
igdeSpinTextField::Ref &spinTextField, const igdeSpinTextFieldListener::Ref &listener){
	spinTextField = igdeSpinTextField::Ref::New(pEnvironment, lower, upper, 6, description);
	if(listener){
		spinTextField->AddListener(listener);
	}
}



void igdeUIHelper::ProgressBar(igdeContainer &form, const char *label, const char *description,
int lower, int upper, bool horizontal, igdeProgressBar::Ref &progressBar){
	ProgressBar(description, lower, upper, horizontal, progressBar);
	FormLine(form, label, description, progressBar);
}

void igdeUIHelper::ProgressBar(igdeContainer &parent, const char *description,
int lower, int upper, bool horizontal, igdeProgressBar::Ref &progressBar){
	ProgressBar(description, lower, upper, horizontal, progressBar);
	parent.AddChild(progressBar);
}

void igdeUIHelper::ProgressBar(const char *description, int lower, int upper,
bool horizontal, igdeProgressBar::Ref &progressBar){
	progressBar = igdeProgressBar::Ref::New(pEnvironment, lower, upper,
		horizontal ? igdeProgressBar::eoHorizontal : igdeProgressBar::eoVertical, description);
}



void igdeUIHelper::ComboBox(igdeContainer &form, const char *label, const char *description,
igdeComboBox::Ref &comboBox, const igdeComboBoxListener::Ref &listener){
	ComboBox(form, label, false, description, comboBox, listener);
}

void igdeUIHelper::ComboBox(igdeContainer &form, const char *label, bool editable,
const char *description, igdeComboBox::Ref &comboBox, const igdeComboBoxListener::Ref &listener){
	ComboBox(15, 10, editable, description, comboBox, listener);
	FormLine(form, label, description, comboBox);
}

void igdeUIHelper::ComboBox(igdeContainer &form, const char *label, int columns, bool editable,
const char *description, igdeComboBox::Ref &comboBox, const igdeComboBoxListener::Ref &listener){
	ComboBox(columns, 10, editable, description, comboBox, listener);
	FormLine(form, label, description, comboBox);
}

void igdeUIHelper::ComboBox(igdeContainer &form, const char *label, int columns, int rows,
bool editable, const char *description, igdeComboBox::Ref &comboBox, const igdeComboBoxListener::Ref &listener){
	ComboBox(columns, rows, editable, description, comboBox, listener);
	FormLine(form, label, description, comboBox);
}

void igdeUIHelper::ComboBox(igdeContainer &parent, const char *description,
igdeComboBox::Ref &comboBox, const igdeComboBoxListener::Ref &listener){
	ComboBox(parent, 15, 10, false, description, comboBox, listener);
}

void igdeUIHelper::ComboBox(igdeContainer &parent, bool editable, const char *description,
igdeComboBox::Ref &comboBox, const igdeComboBoxListener::Ref &listener){
	ComboBox(parent, 15, 10, editable, description, comboBox, listener);
}

void igdeUIHelper::ComboBox(igdeContainer &parent, int columns, int rows,
const char *description, igdeComboBox::Ref &comboBox, const igdeComboBoxListener::Ref &listener){
	ComboBox(parent, columns, rows, false, description, comboBox, listener);
}

void igdeUIHelper::ComboBox(igdeContainer &parent, int columns, int rows, bool editable,
const char *description, igdeComboBox::Ref &comboBox, const igdeComboBoxListener::Ref &listener){
	ComboBox(columns, rows, editable, description, comboBox, listener);
	parent.AddChild(comboBox);
}

void igdeUIHelper::ComboBox(int columns, int rows, bool editable, const char* description,
igdeComboBox::Ref& comboBox, const igdeComboBoxListener::Ref &listener){
	comboBox = igdeComboBox::Ref::New(pEnvironment, columns, rows, editable, description);
	if(listener){
		comboBox->AddListener(listener);
	}
}



void igdeUIHelper::ComboBoxFilter(igdeContainer &form, const char *label,
const char *description, igdeComboBoxFilter::Ref &comboBox, const igdeComboBoxListener::Ref &listener){
	ComboBoxFilter(form, label, false, description, comboBox, listener);
}

void igdeUIHelper::ComboBoxFilter(igdeContainer &form, const char *label, bool editable,
const char *description, igdeComboBoxFilter::Ref &comboBox, const igdeComboBoxListener::Ref &listener){
	ComboBoxFilter(15, 10, editable, description, comboBox, listener);
	FormLine(form, label, description, comboBox);
}

void igdeUIHelper::ComboBoxFilter(igdeContainer &form, const char *label, int columns, bool editable,
const char *description, igdeComboBoxFilter::Ref &comboBox, const igdeComboBoxListener::Ref &listener){
	ComboBoxFilter(columns, 10, editable, description, comboBox, listener);
	FormLine(form, label, description, comboBox);
}

void igdeUIHelper::ComboBoxFilter(igdeContainer &form, const char *label, int columns, int rows,
bool editable, const char *description, igdeComboBoxFilter::Ref &comboBox,
const igdeComboBoxListener::Ref &listener){
	ComboBoxFilter(columns, rows, editable, description, comboBox, listener);
	FormLine(form, label, description, comboBox);
}

void igdeUIHelper::ComboBoxFilter(igdeContainer &parent, const char *description,
igdeComboBoxFilter::Ref &comboBox, const igdeComboBoxListener::Ref &listener){
	ComboBoxFilter(parent, 15, 10, false, description, comboBox, listener);
}

void igdeUIHelper::ComboBoxFilter(igdeContainer &parent, bool editable, const char *description,
igdeComboBoxFilter::Ref &comboBox, const igdeComboBoxListener::Ref &listener){
	ComboBoxFilter(parent, 15, 10, editable, description, comboBox, listener);
}

void igdeUIHelper::ComboBoxFilter(igdeContainer &parent, int columns, int rows,
const char *description, igdeComboBoxFilter::Ref &comboBox, const igdeComboBoxListener::Ref &listener){
	ComboBoxFilter(parent, columns, rows, false, description, comboBox, listener);
}

void igdeUIHelper::ComboBoxFilter(igdeContainer &parent, int columns, int rows, bool editable,
const char *description, igdeComboBoxFilter::Ref &comboBox, const igdeComboBoxListener::Ref &listener){
	ComboBoxFilter(columns, rows, editable, description, comboBox, listener);
	parent.AddChild(comboBox);
}

void igdeUIHelper::ComboBoxFilter(int columns, int rows, bool editable, const char *description,
igdeComboBoxFilter::Ref &comboBox, const igdeComboBoxListener::Ref &listener){
	comboBox = igdeComboBoxFilter::Ref::New(pEnvironment, columns, rows, editable, description);
	if(listener){
		comboBox->AddListener(listener);
	}
}



void igdeUIHelper::CheckBox(igdeContainer &form, igdeCheckBox::Ref &checkBox,
const igdeAction::Ref &action){
	CheckBox(checkBox, action);
	FormLine(form, "", "", checkBox);
}

void igdeUIHelper::CheckBox(igdeContainer &form, const char *text, const char *description,
igdeCheckBox::Ref &checkBox){
	CheckBox(text, description, checkBox);
	FormLine(form, "", "", checkBox);
}

void igdeUIHelper::CheckBox(igdeCheckBox::Ref &checkBox, const igdeAction::Ref &action){
	checkBox = igdeCheckBox::Ref::New(pEnvironment, action);
}

void igdeUIHelper::CheckBox(const char *text, const char *description,
igdeCheckBox::Ref &checkBox){
	checkBox = igdeCheckBox::Ref::New(pEnvironment, text, description, nullptr);
}

void igdeUIHelper::CheckBoxOnly(igdeContainer &parent, igdeCheckBox::Ref &checkBox,
const igdeAction::Ref &action){
	checkBox = igdeCheckBox::Ref::New(pEnvironment, action);
	parent.AddChild(checkBox);
}

void igdeUIHelper::CheckBoxOnly(igdeContainer &parent, const char *text, const char *description,
igdeCheckBox::Ref &checkBox){
	checkBox = igdeCheckBox::Ref::New(pEnvironment, text, description, nullptr);
	parent.AddChild(checkBox);
}



void igdeUIHelper::Slider(igdeContainer &form, const char *label, const char *description,
float lower, float upper, int precision, float tickSpacing, igdeSlider::Ref &slider,
const igdeSliderListener::Ref &listener){
	Slider(description, lower, upper, precision, tickSpacing, slider, listener);
	FormLine(form, label, description, slider);
}

void igdeUIHelper::Slider(igdeContainer &form, const char *label, const char *description,
int lower, int upper, int tickSpacing, igdeSlider::Ref &slider, const igdeSliderListener::Ref &listener){
	Slider(description, lower, upper, tickSpacing, slider, listener);
	if(listener){
		slider->AddListener(listener);
	}
	FormLine(form, label, description, slider);
}

void igdeUIHelper::Slider(igdeContainer &parent, const char *description,
float lower, float upper, int precision, float tickSpacing, igdeSlider::Ref &slider,
const igdeSliderListener::Ref &listener){
	slider = igdeSlider::Ref::New(pEnvironment, igdeSlider::eoHorizontal,
		lower, upper, precision, tickSpacing, description);
	if(listener){
		slider->AddListener(listener);
	}
	parent.AddChild(slider);
}

void igdeUIHelper::Slider(igdeContainer &parent, const char *description, int lower,
int upper, int tickSpacing, igdeSlider::Ref &slider, const igdeSliderListener::Ref &listener){
	slider = igdeSlider::Ref::New(pEnvironment, igdeSlider::eoHorizontal,
		(float)lower, (float)upper, 0, (float)tickSpacing, description);
	if(listener){
		slider->AddListener(listener);
	}
	parent.AddChild(slider);
}

void igdeUIHelper::Slider(const char *description, float lower, float upper, int precision,
float tickSpacing, igdeSlider::Ref &slider, const igdeSliderListener::Ref &listener){
	slider = igdeSlider::Ref::New(pEnvironment, igdeSlider::eoHorizontal,
		lower, upper, precision, tickSpacing, description);
	if(listener){
		slider->AddListener(listener);
	}
}

void igdeUIHelper::Slider(const char *description, int lower, int upper, int tickSpacing,
igdeSlider::Ref &slider, const igdeSliderListener::Ref &listener){
	slider = igdeSlider::Ref::New(pEnvironment, igdeSlider::eoHorizontal,
		(float)lower, (float)upper, 0, (float)tickSpacing, description);
	if(listener){
		slider->AddListener(listener);
	}
}



void igdeUIHelper::ColorBox(igdeContainer &form, const char *label, const char *description,
igdeColorBox::Ref &colorBox, const igdeColorBoxListener::Ref &listener){
	ColorBox(description, colorBox, listener);
	FormLine(form, label, description, colorBox);
}

void igdeUIHelper::ColorBox(igdeContainer &parent, const char* description,
igdeColorBox::Ref &colorBox, const igdeColorBoxListener::Ref &listener){
	ColorBox(description, colorBox, listener);
	parent.AddChild(colorBox);
}

void igdeUIHelper::ColorBox(const char *description, igdeColorBox::Ref &colorBox,
const igdeColorBoxListener::Ref &listener){
	colorBox = igdeColorBox::Ref::New(pEnvironment, description);
	if(listener){
		colorBox->AddListener(listener);
	}
}



void igdeUIHelper::EditPoint(igdeContainer &form, const char *label, const char *description,
igdeEditPoint::Ref &editPoint, const igdeEditPointListener::Ref &listener){
	EditPoint(description, editPoint, listener);
	FormLine(form, label, description, editPoint);
}

void igdeUIHelper::EditPoint(igdeContainer &parent, const char *description,
igdeEditPoint::Ref &editPoint, const igdeEditPointListener::Ref &listener){
	EditPoint(description, editPoint, listener);
	parent.AddChild(editPoint);
}

void igdeUIHelper::EditPoint(const char *description, igdeEditPoint::Ref &editPoint,
const igdeEditPointListener::Ref &listener){
	editPoint = igdeEditPoint::Ref::New(*this, 6, description);
	if(listener){
		editPoint->AddListener(listener);
	}
}



void igdeUIHelper::EditPoint3(igdeContainer &form, const char *label, const char *description,
igdeEditPoint3::Ref &editPoint, const igdeEditPoint3Listener::Ref &listener){
	EditPoint3(description, editPoint, listener);
	FormLine(form, label, description, editPoint);
}

void igdeUIHelper::EditPoint3(igdeContainer &parent, const char *description,
igdeEditPoint3::Ref &editPoint, const igdeEditPoint3Listener::Ref &listener){
	EditPoint3(description, editPoint, listener);
	parent.AddChild(editPoint);
}

void igdeUIHelper::EditPoint3(const char *description, igdeEditPoint3::Ref &editPoint,
const igdeEditPoint3Listener::Ref &listener){
	editPoint = igdeEditPoint3::Ref::New(*this, 6, description);
	if(listener){
		editPoint->AddListener(listener);
	}
}



void igdeUIHelper::EditPath(igdeContainer &form, const char *label,
const char *description, igdeEnvironment::eFilePatternListTypes resourceType,
igdeEditPath::Ref &editPath, const igdeEditPathListener::Ref &listener, bool useGameVFS){
	EditPath(description, resourceType, editPath, listener, useGameVFS);
	FormLine(form, label, description, editPath);
}

void igdeUIHelper::EditPath(igdeContainer &form, const char *label, const char *description,
const igdeFilePattern::List &customFilePatterns, igdeEditPath::Ref &editPath,
const igdeEditPathListener::Ref &listener, bool useGameVFS){
	EditPath(description, customFilePatterns, editPath, listener, useGameVFS);
	FormLine(form, label, description, editPath);
}

void igdeUIHelper::EditPath(igdeContainer &parent, const char *description,
igdeEnvironment::eFilePatternListTypes resourceType,
igdeEditPath::Ref &editPath, const igdeEditPathListener::Ref &listener, bool useGameVFS){
	EditPath(description, resourceType, editPath, listener, useGameVFS);
	parent.AddChild(editPath);
}

void igdeUIHelper::EditPath(igdeContainer &parent, const char *description,
const igdeFilePattern::List &customFilePatterns, igdeEditPath::Ref &editPath,
const igdeEditPathListener::Ref &listener, bool useGameVFS){
	EditPath(description, customFilePatterns, editPath, listener, useGameVFS);
	parent.AddChild(editPath);
}

void igdeUIHelper::EditPath(const char *description, igdeEnvironment::eFilePatternListTypes resourceType,
igdeEditPath::Ref &editPath, const igdeEditPathListener::Ref &listener, bool useGameVFS){
	editPath = igdeEditPath::Ref::New(*this, resourceType, description, useGameVFS);
	if(listener){
		editPath->AddListener(listener);
	}
}

void igdeUIHelper::EditPath(const char *description, const igdeFilePattern::List &customFilePatterns,
igdeEditPath::Ref &editPath, const igdeEditPathListener::Ref &listener, bool useGameVFS){
	editPath = igdeEditPath::Ref::New(*this, customFilePatterns, description, useGameVFS);
	if(listener){
		editPath->AddListener(listener);
	}
}



void igdeUIHelper::EditDirectory(igdeContainer &form, const char *label,
const char *description, igdeEditDirectory::Ref &editDirectory,
const igdeEditDirectoryListener::Ref &listener, bool useGameVFS){
	EditDirectory(description, editDirectory, listener, useGameVFS);
	FormLine(form, label, description, editDirectory);
}

void igdeUIHelper::EditDirectory(igdeContainer &parent, const char *description,
igdeEditDirectory::Ref &editDirectory, const igdeEditDirectoryListener::Ref &listener, bool useGameVFS){
	EditDirectory(description, editDirectory, listener, useGameVFS);
	parent.AddChild(editDirectory);
}

void igdeUIHelper::EditDirectory(const char *description, igdeEditDirectory::Ref &editDirectory,
const igdeEditDirectoryListener::Ref &listener, bool useGameVFS){
	editDirectory = igdeEditDirectory::Ref::New(*this, description, useGameVFS);
	if(listener){
		editDirectory->AddListener(listener);
	}
}



void igdeUIHelper::EditDVector(igdeContainer &form, const char *label, const char *description,
igdeEditDVector::Ref &editDVector, const igdeEditDVectorListener::Ref &listener){
	EditDVector(form, label, description, 6, 3, editDVector, listener);
}

void igdeUIHelper::EditDVector(igdeContainer &form, const char *label, const char *description,
int columns, int precision, igdeEditDVector::Ref &editDVector, const igdeEditDVectorListener::Ref &listener){
	EditDVector(description, columns, precision, editDVector, listener);
	FormLine(form, label, description, editDVector);
}

void igdeUIHelper::EditDVector(igdeContainer &parent, const char *description,
igdeEditDVector::Ref &editDVector, const igdeEditDVectorListener::Ref &listener){
	EditDVector(parent, description, 6, 3, editDVector, listener);
}

void igdeUIHelper::EditDVector(igdeContainer &parent, const char *description, int columns,
int precision, igdeEditDVector::Ref &editDVector, const igdeEditDVectorListener::Ref &listener){
	EditDVector(description, columns, precision, editDVector, listener);
	parent.AddChild(editDVector);
}

void igdeUIHelper::EditDVector(const char *description, igdeEditDVector::Ref &editDVector,
const igdeEditDVectorListener::Ref &listener){
	EditDVector(description, 6, 3, editDVector, listener);
}

void igdeUIHelper::EditDVector(const char *description, int columns, int precision,
igdeEditDVector::Ref &editDVector, const igdeEditDVectorListener::Ref &listener){
	editDVector = igdeEditDVector::Ref::New(*this, columns, precision, description);
	if(listener){
		editDVector->AddListener(listener);
	}
}



void igdeUIHelper::EditVector(igdeContainer &form, const char *label, const char *description,
igdeEditVector::Ref &editVector, const igdeEditVectorListener::Ref &listener){
	EditVector(form, label, description, 6, 3, editVector, listener);
}

void igdeUIHelper::EditVector(igdeContainer &form, const char *label, const char *description,
int columns, int precision, igdeEditVector::Ref &editVector, const igdeEditVectorListener::Ref &listener){
	EditVector(description, columns, precision, editVector, listener);
	FormLine(form, label, description, editVector);
}

void igdeUIHelper::EditVector(igdeContainer &parent, const char *description,
igdeEditVector::Ref &editVector, const igdeEditVectorListener::Ref &listener){
	EditVector(parent, description, 6, 3, editVector, listener);
}

void igdeUIHelper::EditVector(igdeContainer &parent, const char *description, int columns,
int precision, igdeEditVector::Ref &editVector, const igdeEditVectorListener::Ref &listener){
	EditVector(description, columns, precision, editVector, listener);
	parent.AddChild(editVector);
}

void igdeUIHelper::EditVector(const char *description, igdeEditVector::Ref &editVector,
const igdeEditVectorListener::Ref &listener){
	EditVector(description, 6, 3, editVector, listener);
}

void igdeUIHelper::EditVector(const char *description, int columns, int precision,
igdeEditVector::Ref &editVector, const igdeEditVectorListener::Ref &listener){
	editVector = igdeEditVector::Ref::New(*this, columns, precision, description);
	if(listener){
		editVector->AddListener(listener);
	}
}



void igdeUIHelper::EditVector2(igdeContainer &form, const char *label, const char *description,
igdeEditVector2::Ref &editVector2, const igdeEditVector2Listener::Ref &listener){
	EditVector2(form, label, description, 6, 3, editVector2, listener);
}

void igdeUIHelper::EditVector2(igdeContainer &form, const char *label, const char *description,
int columns, int precision, igdeEditVector2::Ref &editVector2, const igdeEditVector2Listener::Ref &listener){
	EditVector2(description, columns, precision, editVector2, listener);
	FormLine(form, label, description, editVector2);
}

void igdeUIHelper::EditVector2(igdeContainer &parent, const char *description,
igdeEditVector2::Ref &editVector2, const igdeEditVector2Listener::Ref &listener){
	EditVector2(parent, description, 6, 3, editVector2, listener);
}

void igdeUIHelper::EditVector2(igdeContainer &parent, const char *description, int columns,
int precision, igdeEditVector2::Ref &editVector2, const igdeEditVector2Listener::Ref &listener){
	EditVector2(description, columns, precision, editVector2, listener);
	parent.AddChild(editVector2);
}

void igdeUIHelper::EditVector2(const char *description, igdeEditVector2::Ref &editVector2,
const igdeEditVector2Listener::Ref &listener){
	EditVector2(description, 6, 3, editVector2, listener);
}

void igdeUIHelper::EditVector2(const char *description, int columns, int precision,
igdeEditVector2::Ref &editVector2, const igdeEditVector2Listener::Ref &listener){
	editVector2 = igdeEditVector2::Ref::New(*this, columns, precision, description);
	if(listener){
		editVector2->AddListener(listener);
	}
}



void igdeUIHelper::EditSliderText(igdeContainer &form, const char *label, const char *description,
float lower, float upper, int columns, int precision, float tickSpacing,
igdeEditSliderText::Ref &slider, const igdeEditSliderTextListener::Ref &listener){
	EditSliderText(description, lower, upper, columns, precision, tickSpacing, slider, listener);
	FormLine(form, label, description, slider);
}

void igdeUIHelper::EditSliderText(igdeContainer &form, const char *label,
const char *description, int lower, int upper, int columns, int tickSpacing,
igdeEditSliderText::Ref &slider, const igdeEditSliderTextListener::Ref &listener){
	EditSliderText(description, lower, upper, columns, tickSpacing, slider, listener);
	FormLine(form, label, description, slider);
}

void igdeUIHelper::EditSliderText(igdeContainer &parent, const char *description,
float lower, float upper, int columns, int precision, float tickSpacing,
igdeEditSliderText::Ref &slider, const igdeEditSliderTextListener::Ref &listener){
	EditSliderText(description, lower, upper, columns, precision, tickSpacing, slider, listener);
	parent.AddChild(slider);
}

void igdeUIHelper::EditSliderText(igdeContainer &parent, const char *description,
int lower, int upper, int columns, int tickSpacing, igdeEditSliderText::Ref &slider,
const igdeEditSliderTextListener::Ref &listener){
	EditSliderText(description, lower, upper, columns, tickSpacing, slider, listener);
	parent.AddChild(slider);
}

void igdeUIHelper::EditSliderText(const char *description, float lower, float upper,
int columns, int precision, float tickSpacing, igdeEditSliderText::Ref &slider,
const igdeEditSliderTextListener::Ref &listener){
	slider = igdeEditSliderText::Ref::New(*this, lower, upper, columns, precision,
		tickSpacing, description);
	if(listener){
		slider->AddListener(listener);
	}
}

void igdeUIHelper::EditSliderText(const char *description, int lower, int upper, int columns,
int tickSpacing, igdeEditSliderText::Ref &slider, const igdeEditSliderTextListener::Ref &listener){
	slider = igdeEditSliderText::Ref::New(*this, (float)lower,
		(float)upper, columns, 0, (float)tickSpacing, description);
	if(listener){
		slider->AddListener(listener);
	}
}



void igdeUIHelper::Button(igdeContainer &parent, const igdeAction::Ref &action){
	igdeButton::Ref button;
	Button(button, action);
	parent.AddChild(button);
}

void igdeUIHelper::Button(igdeContainer &parent, igdeButton::Ref &button,
const igdeAction::Ref &action){
	button = igdeButton::Ref::New(pEnvironment, action);
	parent.AddChild(button);
}

void igdeUIHelper::Button(igdeButton::Ref &button, const igdeAction::Ref &action){
	button = igdeButton::Ref::New(pEnvironment, action);
}



void igdeUIHelper::ToggleButton(igdeContainer &parent, const igdeAction::Ref &action){
	igdeToggleButton::Ref button;
	ToggleButton(button, action);
	parent.AddChild(button);
}

void igdeUIHelper::ToggleButton(igdeContainer &parent, igdeToggleButton::Ref &button,
const igdeAction::Ref &action){
	ToggleButton(button, action);
	parent.AddChild(button);
}

void igdeUIHelper::ToggleButton(igdeToggleButton::Ref &button, const igdeAction::Ref &action){
	button = igdeToggleButton::Ref::New(pEnvironment, action);
}



void igdeUIHelper::ListBox(igdeContainer &form, const char *label, int rows,
const char *description, igdeListBox::Ref &listBox, const igdeListBoxListener::Ref &listener){
	ListBox(rows, description, listBox, listener);
	FormLine(form, label, description, listBox, igdeLabel::eaLeft | igdeLabel::eaTop);
}

void igdeUIHelper::ListBox(igdeContainer &parent, int rows, const char *description,
igdeListBox::Ref &listBox, const igdeListBoxListener::Ref &listener){
	ListBox(rows, description, listBox, listener);
	parent.AddChild(listBox);
}

void igdeUIHelper::ListBox(int rows, const char *description, igdeListBox::Ref &listBox,
const igdeListBoxListener::Ref &listener){
	listBox = igdeListBox::Ref::New(pEnvironment, rows, description);
	if(listener){
		listBox->AddListener(listener);
	}
}



void igdeUIHelper::IconListBox(igdeContainer &parent, igdeIconListBox::Ref &listBox,
const sColumnHeader *headers, int headerCount, const char *description, const igdeIconListBoxListener::Ref &listener){
	IconListBox(headers, headerCount, description, listBox, listener);
	parent.AddChild(listBox);
}


void igdeUIHelper::IconListBox(igdeContainer &parent, igdeIconListBox::Ref &listBox,
const decPoint &minimumSize, const sColumnHeader *headers, int headerCount, const char *description,
const igdeIconListBoxListener::Ref &listener){
	IconListBox(minimumSize, headers, headerCount, description, listBox, listener);
	parent.AddChild(listBox);
}

static void igdeUIHelperIconListBoxShared(igdeIconListBox &listBox,
const igdeUIHelper::sColumnHeader *headers, int headerCount, const igdeIconListBoxListener::Ref &listener){
	igdeListHeader::Ref realHeader;
	
	int i;
	for(i=0; i<headerCount; i++){
		realHeader = igdeListHeader::Ref::New(headers[i].title,
			headers[i].icon, headers[i].size);
		listBox.AddHeader(realHeader);
	}
	
	if(listener){
		listBox.AddListener(listener);
	}
}

void igdeUIHelper::IconListBox(const sColumnHeader *headers, int headerCount,
const char *description, igdeIconListBox::Ref &listBox, const igdeIconListBoxListener::Ref &listener){
	if(!headers || headerCount < 1){
		DETHROW(deeInvalidParam);
	}
	
	listBox = igdeIconListBox::Ref::New(pEnvironment, description);
	igdeUIHelperIconListBoxShared(listBox, headers, headerCount, listener);
}

void igdeUIHelper::IconListBox(const decPoint &minimumSize, const sColumnHeader *headers, int headerCount,
const char *description, igdeIconListBox::Ref &listBox, const igdeIconListBoxListener::Ref &listener){
	if(!headers || headerCount < 1){
		DETHROW(deeInvalidParam);
	}
	
	listBox = igdeIconListBox::Ref::New(pEnvironment, minimumSize, description);
	igdeUIHelperIconListBoxShared(listBox, headers, headerCount, listener);
}



void igdeUIHelper::TreeList(igdeContainer &parent, igdeTreeList::Ref &treeList,
int rows, const char *description, const igdeTreeListListener::Ref &listener){
	TreeList(rows, description, treeList, listener);
	parent.AddChild(treeList);
}

void igdeUIHelper::TreeList(int rows, const char *description, igdeTreeList::Ref &treeList,
const igdeTreeListListener::Ref &listener){
	treeList = igdeTreeList::Ref::New(pEnvironment, rows, description);
	if(listener){
		treeList->AddListener(listener);
	}
}



void igdeUIHelper::Label(igdeContainer &parent, const char *text){
	parent.AddChild(igdeLabel::Ref::New(pEnvironment, text));
}

void igdeUIHelper::Label(igdeContainer &parent, const char *text, const char *description){
	parent.AddChild(igdeLabel::Ref::New(pEnvironment, text, description));
}

void igdeUIHelper::Label(igdeContainer &parent, const char *text,
const char *description, int alignment){
	parent.AddChild(igdeLabel::Ref::New(pEnvironment, text, description, alignment));
}

void igdeUIHelper::Label(igdeContainer &parent, igdeLabel::Ref &label, const char *text){
	label = igdeLabel::Ref::New(pEnvironment, text);
	parent.AddChild(label);
}

void igdeUIHelper::Label(igdeContainer &parent, igdeLabel::Ref &label,
const char *text, const char *description, int alignment){
	label = igdeLabel::Ref::New(pEnvironment, text, description, alignment);
	parent.AddChild(label);
}

void igdeUIHelper::Label(igdeLabel::Ref &label, const char *text){
	label = igdeLabel::Ref::New(pEnvironment, text);
}

void igdeUIHelper::Label(igdeLabel::Ref &label, const char *text,
const char *description, int alignment){
	label = igdeLabel::Ref::New(pEnvironment, text, description, alignment);
}



void igdeUIHelper::ScrollBar(igdeContainer &parent, bool horizontal, int lower, int upper,
int pageSize, int value, igdeScrollBar::Ref &scrollBar, const igdeScrollBarListener::Ref &listener){
	ScrollBar(horizontal, lower, upper, pageSize, value, scrollBar, listener);
	parent.AddChild(scrollBar);
}

void igdeUIHelper::ScrollBar(bool horizontal, int lower, int upper, int pageSize, int value,
igdeScrollBar::Ref &scrollBar, const igdeScrollBarListener::Ref &listener){
	scrollBar = igdeScrollBar::Ref::New(pEnvironment,
		horizontal ? igdeScrollBar::eoHorizontal : igdeScrollBar::eoVertical,
		lower, upper, pageSize, value);
	if(listener){
		scrollBar->AddListener(listener);
	}
}



void igdeUIHelper::ViewCurveBezier(igdeContainer &parent,
igdeViewCurveBezier::Ref &viewCurveBezier, const igdeViewCurveBezierListener::Ref &listener){
	ViewCurveBezier(viewCurveBezier, listener);
	parent.AddChild(viewCurveBezier);
}

void igdeUIHelper::ViewCurveBezier(igdeViewCurveBezier::Ref &viewCurveBezier,
const igdeViewCurveBezierListener::Ref &listener){
	viewCurveBezier = igdeViewCurveBezier::Ref::New(pEnvironment);
	if(listener){
		viewCurveBezier->AddListener(listener);
	}
}



void igdeUIHelper::Separator(igdeContainer &parent, bool horizontal){
	parent.AddChild(igdeSeparator::Ref::New(pEnvironment,
		horizontal ? igdeSeparator::eoHorizontal : igdeSeparator::eoVertical));
}



void igdeUIHelper::Spacer(igdeContainer &parent, const decPoint &size){
	igdeSpacer::Ref spacer;
	Spacer(parent, size, spacer);
}

void igdeUIHelper::Spacer(igdeContainer &parent, const decPoint &size, igdeSpacer::Ref &spacer){
	spacer = igdeSpacer::Ref::New(pEnvironment, size);
	parent.AddChild(spacer);
}



void igdeUIHelper::WPSky(igdeContainer &parent, const igdeAction::Ref &action,
const char *title, bool stretchLast, bool collapsed){
	igdeWPSky::Ref panel;
	WPSky(parent, panel, action, title, stretchLast, collapsed);
}

void igdeUIHelper::WPSky(igdeContainer &parent, igdeWPSky::Ref &panel,
const igdeAction::Ref &action, const char *title, bool stretchLast, bool collapsed){
	igdeContainer::Ref groupBox;
	GroupBoxFlow(parent, groupBox, title, stretchLast, collapsed);
	WPSky(panel, action);
	groupBox->AddChild(panel);
}

void igdeUIHelper::WPSky(igdeWPSky::Ref &panel, const igdeAction::Ref &action){
	panel = igdeWPSky::Ref::New(pEnvironment, action);
}



void igdeUIHelper::WPCamera(igdeContainer &parent, const igdeAction::Ref &action,
const char *title, bool stretchLast, bool collapsed){
	igdeWPCamera::Ref panel;
	WPCamera(parent, panel, action, title, stretchLast, collapsed);
}

void igdeUIHelper::WPCamera(igdeContainer &parent, igdeWPCamera::Ref &panel,
const igdeAction::Ref &action, const char *title, bool stretchLast, bool collapsed){
	igdeContainer::Ref groupBox;
	GroupBoxFlow(parent, groupBox, title, stretchLast, collapsed);
	WPCamera(panel, action);
	groupBox->AddChild(panel);
}

void igdeUIHelper::WPCamera(igdeWPCamera::Ref &panel, const igdeAction::Ref &action){
	panel = igdeWPCamera::Ref::New(pEnvironment, action);
}



void igdeUIHelper::WPWObject(igdeContainer &parent, const igdeAction::Ref &action,
const char *title, bool stretchLast, bool collapsed){
	igdeWPWObject::Ref panel;
	WPWObject(parent, panel, action, title, stretchLast, collapsed);
}

void igdeUIHelper::WPWObject(igdeContainer &parent, igdeWPWObject::Ref &panel,
const igdeAction::Ref &action, const char *title, bool stretchLast, bool collapsed){
	igdeContainer::Ref groupBox;
	GroupBoxFlow(parent, groupBox, title, stretchLast, collapsed);
	WPWObject(panel, action);
	groupBox->AddChild(panel);
}

void igdeUIHelper::WPWObject(igdeWPWObject::Ref &panel, const igdeAction::Ref &action){
	panel = igdeWPWObject::Ref::New(pEnvironment, action);
}



void igdeUIHelper::EditTags(igdeContainer &parent, igdeEditTags::Ref &editTags,
const igdeAction::Ref &action){
	EditTags(editTags, action);
	parent.AddChild(editTags);
}

void igdeUIHelper::EditTags(igdeEditTags::Ref &editTags, const igdeAction::Ref &action){
	editTags = igdeEditTags::Ref::New(*this, 5, action);
}



void igdeUIHelper::ToggleTags(igdeContainer &parent, igdeToggleTags::Ref &toggleTags,
const igdeAction::Ref &action){
	ToggleTags(toggleTags, action);
	parent.AddChild(toggleTags);
}

void igdeUIHelper::ToggleTags(igdeToggleTags::Ref &toggleTags, const igdeAction::Ref &action){
	toggleTags = igdeToggleTags::Ref::New(*this, 5, action);
}



void igdeUIHelper::EditPropertyValue(igdeEditPropertyValue::Ref &widget,
const igdeEditPropertyValueListener::Ref &listener){
	widget = igdeEditPropertyValue::Ref::New(*this);
	if(listener){
		widget->AddListener(listener);
	}
}

void igdeUIHelper::EditPropertyValue(igdeContainer &parent,
igdeEditPropertyValue::Ref &widget, const igdeEditPropertyValueListener::Ref &listener){
	EditPropertyValue(widget, listener);
	parent.AddChild(widget);
}



void igdeUIHelper::WPTriggerTable(igdeContainer &parent, const igdeAction::Ref &action,
const char *title, bool stretchLast, bool collapsed){
	igdeWPTriggerTable::Ref panel;
	WPTriggerTable(parent, panel, action, title, stretchLast, collapsed);
}

void igdeUIHelper::WPTriggerTable(igdeContainer &parent, igdeWPTriggerTable::Ref &panel,
const igdeAction::Ref &action, const char *title, bool stretchLast, bool collapsed){
	igdeContainer::Ref groupBox;
	GroupBoxFlow(parent, groupBox, title, stretchLast, collapsed);
	WPTriggerTable(panel, action);
	groupBox->AddChild(panel);
}

void igdeUIHelper::WPTriggerTable(igdeWPTriggerTable::Ref &panel, const igdeAction::Ref &action){
	panel = igdeWPTriggerTable::Ref::New(pEnvironment, action);
}



void igdeUIHelper::MenuCommand(igdeMenuCascade &menu, const igdeAction::Ref &action){
	menu.AddChild(igdeMenuCommand::Ref::New(pEnvironment, action));
}

void igdeUIHelper::MenuCheck(igdeMenuCascade &menu, const igdeAction::Ref &action){
	menu.AddChild(igdeMenuCheck::Ref::New(pEnvironment, action));
}

void igdeUIHelper::MenuOption(igdeMenuCascade &menu, const igdeAction::Ref &action){
	menu.AddChild(igdeMenuOption::Ref::New(pEnvironment, action));
}

void igdeUIHelper::MenuSeparator(igdeMenuCascade &menu){
	menu.AddChild(igdeMenuSeparator::Ref::New(pEnvironment));
}

void igdeUIHelper::MenuRecentFiles(igdeMenuCascade &menu, igdeRecentFiles &recentFiles, const char *text){
	const igdeMenuCascade::Ref subMenu(igdeMenuCascade::Ref::New(pEnvironment,
		text, pEnvironment.GetStockIcon(igdeEnvironment::esiOpen), text));
	recentFiles.SetMenu(subMenu);
	menu.AddChild(subMenu);
}



void igdeUIHelper::ToolBarButton(igdeToolBar &toolBar, const igdeAction::Ref &action){
	igdeButton::Ref widget(igdeButton::Ref::New(pEnvironment, action, igdeButton::ebsToolBar));
	toolBar.AddChild(widget);
}

void igdeUIHelper::ToolBarToggleButton(igdeToolBar &toolBar, const igdeAction::Ref &action){
	igdeToggleButton::Ref widget(igdeToggleButton::Ref::New(
		pEnvironment, action, igdeToggleButton::ebsToolBar));
	toolBar.AddChild(widget);
}

void igdeUIHelper::ToolBarSeparator(igdeToolBar &toolBar){
	toolBar.AddChild(igdeToolBarSeparator::Ref::New(pEnvironment));
}



decString igdeUIHelper::FormatSize1024(uint64_t size){
	decString text;
	
	if(size >= 1024000000){
		text.Format("%d.%d GB", (int)(size / 1024000000),
			(int)(size % 1024000000) / 102400000);
		
	}else if(size >= 1024000){
		text.Format("%d.%d MB", (int)(size / 1024000),
			(int)(size % 1024000) / 102400);
		
	}else if(size >= 1024){
		text.Format("%d.%d kB", (int)(size / 1024),
			((int)(size % 1024) * 10) / 1024);
		
	}else{
		text.Format("%d B", (int)size);
	}
	
	return text;
}

decString igdeUIHelper::FormatSize1000(uint64_t size){
	decString text;
	
	if(size >= 1000000000){
		text.Format("%d.%d GB", (int)(size / 1000000000),
			(int)(size % 1000000000) / 100000000);
		
	}else if(size >= 1000000){
		text.Format("%d.%d MB", (int)(size / 1000000),
			(int)(size % 1000000) / 100000);
		
	}else if(size >= 1000){
		text.Format("%d.%d kB", (int)(size / 1000),
			(int)(size % 1000) % 100);
		
	}else{
		text.Format("%d B", (int)size);
	}
	
	return text;
}

decString igdeUIHelper::FormatSizeTousand(uint64_t size){
	if(size < 1000){
		decString text;
		text.Format("%d", (int)size);
		return text;
	}
	
	uint64_t scratch = size;
	int digitCount = 0;
	while(scratch > 0){
		scratch /= 10;
		digitCount++;
	}
	
	const int separatorCount = (digitCount - 1) / 3;
	
	decString text;
	text.Set('\'', digitCount + separatorCount);
	
	int i;
	scratch = size;
	char *tp = text.GetMutableString() + (digitCount + separatorCount - 1);
	for(i=1; i<=digitCount; i++){
		*(tp--) = '0' + ( scratch % 10 );
		scratch /= 10;
		if((i % 3) == 0){
			tp--;
		}
	}
	
	return text;
}
