/* 
 * Drag[en]gine IGDE
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

#include "igdeUIHelper.h"
#include "igdeGroupBox.h"
#include "igdeGroupBoxReference.h"
#include "igdeContainer.h"
#include "igdeContainerReference.h"
#include "igdeTextArea.h"
#include "igdeTextAreaReference.h"
#include "igdeTextField.h"
#include "igdeTextFieldReference.h"
#include "igdeProgressBar.h"
#include "igdeProgressBarReference.h"
#include "igdeSpinTextField.h"
#include "igdeSpinTextFieldReference.h"
#include "igdeButton.h"
#include "igdeButtonReference.h"
#include "igdeColorBox.h"
#include "igdeColorBoxReference.h"
#include "igdeComboBox.h"
#include "igdeComboBoxReference.h"
#include "igdeComboBoxFilter.h"
#include "igdeComboBoxFilterReference.h"
#include "igdeLabel.h"
#include "igdeLabelReference.h"
#include "igdeCheckBox.h"
#include "igdeCheckBoxReference.h"
#include "igdeToolBar.h"
#include "igdeToolBarSeparator.h"
#include "igdeToggleButton.h"
#include "igdeToggleButtonReference.h"
#include "igdeListBox.h"
#include "igdeListBoxReference.h"
#include "igdeIconListBox.h"
#include "igdeIconListBoxReference.h"
#include "igdeScrollBar.h"
#include "igdeScrollBarReference.h"
#include "igdeSeparator.h"
#include "igdeSlider.h"
#include "igdeSliderReference.h"
#include "igdeTreeList.h"
#include "igdeTreeListReference.h"
#include "igdeWidget.h"
#include "igdeWidgetReference.h"
#include "igdeSpacer.h"
#include "igdeSpacerReference.h"
#include "composed/igdeEditPath.h"
#include "composed/igdeEditPathReference.h"
#include "composed/igdeEditPathListener.h"
#include "composed/igdeEditDirectory.h"
#include "composed/igdeEditDirectoryReference.h"
#include "composed/igdeEditDirectoryListener.h"
#include "composed/igdeEditPoint.h"
#include "composed/igdeEditPointReference.h"
#include "composed/igdeEditPointListener.h"
#include "composed/igdeEditPoint3.h"
#include "composed/igdeEditPoint3Reference.h"
#include "composed/igdeEditPoint3Listener.h"
#include "composed/igdeEditSliderText.h"
#include "composed/igdeEditSliderTextReference.h"
#include "composed/igdeEditSliderTextListener.h"
#include "composed/igdeEditDVector.h"
#include "composed/igdeEditDVectorReference.h"
#include "composed/igdeEditDVectorListener.h"
#include "composed/igdeEditVector.h"
#include "composed/igdeEditVectorReference.h"
#include "composed/igdeEditVectorListener.h"
#include "composed/igdeEditVector2.h"
#include "composed/igdeEditVector2Reference.h"
#include "composed/igdeEditVector2Listener.h"
#include "composed/igdeEditTags.h"
#include "composed/igdeEditTagsReference.h"
#include "composed/igdeEditPropertyValue.h"
#include "composed/igdeEditPropertyValueListener.h"
#include "composed/igdeEditPropertyValueReference.h"
#include "composed/igdeToggleTags.h"
#include "composed/igdeToggleTagsReference.h"
#include "curveedit/igdeViewCurveBezier.h"
#include "curveedit/igdeViewCurveBezierReference.h"
#include "curveedit/igdeViewCurveBezierListener.h"
#include "layout/igdeContainerForm.h"
#include "layout/igdeContainerBorder.h"
#include "layout/igdeContainerBorderReference.h"
#include "layout/igdeContainerBox.h"
#include "layout/igdeContainerFlow.h"
#include "layout/igdeContainerScroll.h"
#include "layout/igdeContainerScrollReference.h"
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
#include "menu/igdeMenuCascadeReference.h"
#include "menu/igdeMenuCommand.h"
#include "menu/igdeMenuCheck.h"
#include "menu/igdeMenuOption.h"
#include "menu/igdeMenuSeparator.h"
#include "model/igdeListHeader.h"
#include "model/igdeListHeaderReference.h"
#include "properties/igdeWPCamera.h"
#include "properties/igdeWPCameraReference.h"
#include "properties/igdeWPWObject.h"
#include "properties/igdeWPWObjectReference.h"
#include "properties/igdeWPSky.h"
#include "properties/igdeWPSkyReference.h"
#include "properties/igdeWPTriggerTable.h"
#include "properties/igdeWPTriggerTableReference.h"
#include "theme/themeNames.h"
#include "../utils/igdeRecentFiles.h"

#include <dragengine/common/exceptions.h>



// struct igdeUIHelper::sColumnHeader
///////////////////////////////////////

igdeUIHelper::sColumnHeader::sColumnHeader( const char *ptitle, igdeIcon *picon, int psize ) :
title( ptitle ),
icon( picon ),
size( psize ){
}



// Class igdeUIHelper
///////////////////////

// Constructor, destructor
////////////////////////////

igdeUIHelper::igdeUIHelper( igdeEnvironment &environment ) :
pEnvironment( environment ){
}

igdeUIHelper::igdeUIHelper( const igdeUIHelper &helper ) :
pEnvironment( helper.pEnvironment ){
}



// Helpers
////////////

void igdeUIHelper::SidePanel( igdeContainerScrollReference &scroll, igdeContainerReference &panel,
bool stretchLast, int spacing ){
	scroll.TakeOver( new igdeContainerScroll( pEnvironment, false, true ) );
	
	panel.TakeOver( new igdeContainerFlow( pEnvironment, igdeContainerFlow::eaY,
		stretchLast ? igdeContainerFlow::esLast : igdeContainerFlow::esNone, spacing ) );
	scroll->AddChild( panel );
	panel->SetWidgetGuiThemeName( igdeGuiThemeNames::properties );
}

void igdeUIHelper::SidePanel( igdeContainer &parent, igdeContainerReference &panel,
bool stretchLast, int spacing ){
	igdeContainerScrollReference scroll;
	SidePanel( scroll, panel, stretchLast, spacing );
	parent.AddChild( scroll );
}



void igdeUIHelper::GroupBox( igdeContainer &parent, igdeContainerReference &groupBox,
const char *title, bool collapsed ) const{
	igdeContainerReference frame;
	frame.TakeOver( new igdeGroupBox( pEnvironment, title, collapsed ) );
	parent.AddChild( frame );
	
	groupBox.TakeOver( new igdeContainerForm( pEnvironment ) );
	frame->AddChild( groupBox );
}

void igdeUIHelper::GroupBoxStatic( igdeContainer &parent,
igdeContainerReference &groupBox, const char *title ) const{
	igdeContainerReference frame;
	frame.TakeOver( new igdeGroupBox( pEnvironment, title ) );
	parent.AddChild( frame );
	
	groupBox.TakeOver( new igdeContainerForm( pEnvironment ) );
	frame->AddChild( groupBox );
}

void igdeUIHelper::GroupBoxFlow( igdeContainer &parent, igdeContainerReference &groupBox,
const char *title, bool stretchLast, bool collapsed ) const{
	igdeContainerReference frame;
	frame.TakeOver( new igdeGroupBox( pEnvironment, title, collapsed ) );
	parent.AddChild( frame );
	
	groupBox.TakeOver( new igdeContainerFlow( pEnvironment, igdeContainerFlow::eaY,
		stretchLast ? igdeContainerFlow::esLast : igdeContainerFlow::esNone ) );
	frame->AddChild( groupBox );
}

void igdeUIHelper::GroupBoxBorder( igdeContainer &parent, igdeContainerBorderReference &groupBox,
const char *title, bool collapsed ) const{
	igdeContainerReference frame;
	frame.TakeOver( new igdeGroupBox( pEnvironment, title, collapsed ) );
	parent.AddChild( frame );
	
	groupBox.TakeOver( new igdeContainerBorder( pEnvironment ) );
	frame->AddChild( groupBox );
}

void igdeUIHelper::GroupBoxStaticFlow( igdeContainer &parent, igdeContainerReference &groupBox,
const char *title, bool stretchLast ) const{
	igdeGroupBoxReference frame;
	frame.TakeOver( new igdeGroupBox( pEnvironment, title ) );
	if( stretchLast ){
		frame->SetStretchLast( stretchLast );
	}
	parent.AddChild( frame );
	
	groupBox.TakeOver( new igdeContainerFlow( pEnvironment, igdeContainerFlow::eaY,
		stretchLast ? igdeContainerFlow::esLast : igdeContainerFlow::esNone ) );
	frame->AddChild( groupBox );
}

void igdeUIHelper::GroupBoxStaticBorder( igdeContainer &parent,
igdeContainerBorderReference &groupBox, const char *title, bool stretch ) const{
	igdeGroupBoxReference frame;
	frame.TakeOver( new igdeGroupBox( pEnvironment, title ) );
	if( stretch ){
		frame->SetStretchLast( stretch );
	}
	parent.AddChild( frame );
	
	groupBox.TakeOver( new igdeContainerBorder( pEnvironment ) );
	frame->AddChild( groupBox );
}



void igdeUIHelper::FormLine( igdeContainer &form, const char *label, const char *description,
igdeWidget *widget ){
	FormLine( form, label, description, widget, igdeLabel::eaLeft | igdeLabel::eaMiddle );
}

void igdeUIHelper::FormLine( igdeContainer &form, const char *label, const char *description,
igdeWidget *widget, int labelAlignment ){
	igdeLabelReference wlabel;
	wlabel.TakeOver( new igdeLabel( pEnvironment, label, description, labelAlignment ) );
	form.AddChild( wlabel );
	form.AddChild( widget );
}

void igdeUIHelper::FormLine( igdeContainer &form, const char *label, const char *description,
igdeContainerReference &line ){
	FormLine( form, label, description, line, igdeLabel::eaLeft | igdeLabel::eaMiddle );
}

void igdeUIHelper::FormLine( igdeContainer &form, const char *label, const char *description,
igdeContainerReference &line, int labelAlignment ){
	igdeLabelReference wlabel;
	wlabel.TakeOver( new igdeLabel( pEnvironment, label, description, labelAlignment ) );
	form.AddChild( wlabel );
	line.TakeOver( new igdeContainerBox( pEnvironment, igdeContainerBox::eaX ) );
	form.AddChild( line );
}

void igdeUIHelper::FormLineStretchFirst( igdeContainer &form, const char *label,
const char *description, igdeContainerReference &line ){
	FormLineStretchFirst( form, label, description, line, igdeLabel::eaLeft | igdeLabel::eaMiddle );
}

void igdeUIHelper::FormLineStretchFirst( igdeContainer &form, const char *label,
const char *description, igdeContainerReference &line, int labelAlignment ){
	igdeLabelReference wlabel;
	wlabel.TakeOver( new igdeLabel( pEnvironment, label, description, labelAlignment ) );
	form.AddChild( wlabel );
	line.TakeOver( new igdeContainerFlow( pEnvironment, igdeContainerFlow::eaX, igdeContainerFlow::esFirst ) );
	form.AddChild( line );
}

void igdeUIHelper::FormLineDownFlow( igdeContainer &form, const char *label,
const char *description, igdeContainerReference &line ){
	FormLineDownFlow( form, label, description, line, igdeLabel::eaLeft | igdeLabel::eaTop );
}

void igdeUIHelper::FormLineDownFlow( igdeContainer &form, const char *label,
const char *description, igdeContainerReference &line, int labelAlignment ){
	igdeLabelReference wlabel;
	wlabel.TakeOver( new igdeLabel( pEnvironment, label, description, labelAlignment ) );
	form.AddChild( wlabel );
	line.TakeOver( new igdeContainerFlow( pEnvironment, igdeContainerFlow::eaY ) );
	form.AddChild( line );
}



void igdeUIHelper::EditString( igdeContainer &form, const char *label, const char *description,
igdeTextFieldReference &textField, igdeTextFieldListener *listener ){
	EditString( form, label, description, 15, textField, listener );
}

void igdeUIHelper::EditString( igdeContainer &form, const char *label, const char *description,
int rows, igdeTextFieldReference &textField, igdeTextFieldListener *listener ){
	EditString( description, rows, textField, listener );
	FormLine( form, label, description, textField );
}

void igdeUIHelper::EditString( igdeContainer &parent, const char *description,
igdeTextFieldReference &textField, igdeTextFieldListener *listener ){
	EditString( description, textField, listener );
	parent.AddChild( textField );
}

void igdeUIHelper::EditString( const char *description, igdeTextFieldReference &textField,
igdeTextFieldListener *listener ){
	EditString( description, 15, textField, listener );
}

void igdeUIHelper::EditString( const char *description, int rows,
igdeTextFieldReference &textField, igdeTextFieldListener *listener ){
	textField.TakeOver( new igdeTextField( pEnvironment, rows, description ) );
	if( listener ){
		textField->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::EditString( igdeContainer &form, const char *label, const char *description,
igdeTextAreaReference &textArea, int rows, igdeTextAreaListener *listener ){
	EditString( form, label, description, textArea, 15, rows, listener );
}

void igdeUIHelper::EditString( igdeContainer &form, const char *label, const char *description,
igdeTextAreaReference &textArea, int columns, int rows, igdeTextAreaListener *listener ){
	EditString( description, columns, rows, textArea, listener );
	FormLine( form, label, description, textArea, igdeLabel::eaLeft | igdeLabel::eaTop );
}

void igdeUIHelper::EditString( igdeContainer &parent, const char *description,
igdeTextAreaReference &textArea, int rows, igdeTextAreaListener *listener ){
	EditString( parent, description, textArea, 15, rows, listener );
}

void igdeUIHelper::EditString( igdeContainer &parent, const char *description,
igdeTextAreaReference &textArea, int columns, int rows, igdeTextAreaListener *listener ){
	EditString( description, columns, rows, textArea, listener );
	parent.AddChild( textArea );
}

void igdeUIHelper::EditString( const char *description, int rows,
igdeTextAreaReference &textArea, igdeTextAreaListener *listener ){
	EditString( description, 15, rows, textArea, listener );
}

void igdeUIHelper::EditString( const char *description, int columns, int rows,
igdeTextAreaReference &textArea, igdeTextAreaListener *listener ){
	textArea.TakeOver( new igdeTextArea( pEnvironment, columns, rows, description ) );
	if( listener ){
		textArea->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::EditFloat( igdeContainer &form, const char *label, const char *description,
igdeTextFieldReference &textField, igdeTextFieldListener *listener ){
	EditFloat( form, label, description, 6, 3, textField, listener );
}

void igdeUIHelper::EditFloat( igdeContainer &form, const char *label, const char *description,
int columns, int precision, igdeTextFieldReference &textField, igdeTextFieldListener *listener ){
	EditFloat( description, columns, precision, textField, listener );
	FormLine( form, label, description, textField );
}

void igdeUIHelper::EditFloat( igdeContainer &parent, const char *description,
igdeTextFieldReference &textField, igdeTextFieldListener *listener ){
	EditFloat( parent, description, 6, 3, textField, listener );
}

void igdeUIHelper::EditFloat( igdeContainer &parent, const char *description,
int columns, int precision, igdeTextFieldReference &textField, igdeTextFieldListener *listener ){
	EditFloat( description, columns, precision, textField, listener );
	parent.AddChild( textField );
}

void igdeUIHelper::EditFloat( const char *description, igdeTextFieldReference &textField,
igdeTextFieldListener *listener ){
	EditFloat( description, 6, 3, textField, listener );
}

void igdeUIHelper::EditFloat( const char *description, int columns, int precision,
igdeTextFieldReference &textField, igdeTextFieldListener *listener ){
	textField.TakeOver( new igdeTextField( pEnvironment, columns, description ) );
	textField->SetPrecision( precision );
	if( listener ){
		textField->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::EditInteger( igdeContainer &form, const char *label, const char *description,
igdeTextFieldReference &textField, igdeTextFieldListener *listener ){
	EditInteger( form, label, description, 6, textField, listener );
}

void igdeUIHelper::EditInteger( igdeContainer &form, const char *label, const char *description,
int columns, igdeTextFieldReference &textField, igdeTextFieldListener *listener ){
	EditInteger( description, columns, textField, listener );
	FormLine( form, label, description, textField );
}

void igdeUIHelper::EditInteger( igdeContainer &parent, const char *description,
igdeTextFieldReference &textField, igdeTextFieldListener *listener ){
	EditInteger( parent, description, 6, textField, listener );
}

void igdeUIHelper::EditInteger( igdeContainer &parent, const char *description, int columns,
igdeTextFieldReference &textField, igdeTextFieldListener *listener ){
	EditInteger( description, columns, textField, listener );
	parent.AddChild( textField );
}

void igdeUIHelper::EditInteger( const char *description, igdeTextFieldReference &textField,
igdeTextFieldListener *listener ){
	EditInteger( description, 6, textField, listener );
}

void igdeUIHelper::EditInteger( const char *description, int columns,
igdeTextFieldReference &textField, igdeTextFieldListener *listener ){
	textField.TakeOver( new igdeTextField( pEnvironment, columns, description ) );
	if( listener ){
		textField->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::EditSpinInteger( igdeContainer &form, const char *label,
const char *description, int lower, int upper, igdeSpinTextFieldReference &spinTextField,
igdeSpinTextFieldListener *listener ){
	EditSpinInteger( description, lower, upper, spinTextField, listener );
	FormLine( form, label, description, spinTextField );
}

void igdeUIHelper::EditSpinInteger( igdeContainer &parent, const char *description,
int lower, int upper, igdeSpinTextFieldReference &spinTextField, igdeSpinTextFieldListener *listener ){
	spinTextField.TakeOver( new igdeSpinTextField( pEnvironment, lower, upper, 6, description ) );
	if( listener ){
		spinTextField->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
	parent.AddChild( spinTextField );
}

void igdeUIHelper::EditSpinInteger( const char *description, int lower, int upper,
igdeSpinTextFieldReference &spinTextField, igdeSpinTextFieldListener *listener ){
	spinTextField.TakeOver( new igdeSpinTextField( pEnvironment, lower, upper, 6, description ) );
	if( listener ){
		spinTextField->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::ProgressBar( igdeContainer &form, const char *label, const char *description,
int lower, int upper, bool horizontal, igdeProgressBarReference &progressBar ){
	ProgressBar( description, lower, upper, horizontal, progressBar );
	FormLine( form, label, description, progressBar );
}

void igdeUIHelper::ProgressBar( igdeContainer &parent, const char *description,
int lower, int upper, bool horizontal, igdeProgressBarReference &progressBar ){
	ProgressBar( description, lower, upper, horizontal, progressBar );
	parent.AddChild( progressBar );
}

void igdeUIHelper::ProgressBar( const char *description, int lower, int upper,
bool horizontal, igdeProgressBarReference &progressBar ){
	progressBar.TakeOver( new igdeProgressBar( pEnvironment, lower, upper,
		horizontal ? igdeProgressBar::eoHorizontal : igdeProgressBar::eoVertical, description ) );
}



void igdeUIHelper::ComboBox( igdeContainer &form, const char *label, const char *description,
igdeComboBoxReference &comboBox, igdeComboBoxListener *listener ){
	ComboBox( form, label, false, description, comboBox, listener );
}

void igdeUIHelper::ComboBox( igdeContainer &form, const char *label, bool editable,
const char *description, igdeComboBoxReference &comboBox, igdeComboBoxListener *listener ){
	ComboBox( 15, 10, editable, description, comboBox, listener );
	FormLine( form, label, description, comboBox );
}

void igdeUIHelper::ComboBox( igdeContainer &parent, const char *description,
igdeComboBoxReference &comboBox, igdeComboBoxListener *listener ){
	ComboBox( parent, 15, 10, false, description, comboBox, listener );
}

void igdeUIHelper::ComboBox( igdeContainer &parent, bool editable, const char *description,
igdeComboBoxReference &comboBox, igdeComboBoxListener *listener ){
	ComboBox( parent, 15, 10, editable, description, comboBox, listener );
}

void igdeUIHelper::ComboBox( igdeContainer &parent, int columns, int rows,
const char *description, igdeComboBoxReference &comboBox, igdeComboBoxListener *listener ){
	ComboBox( parent, columns, rows, false, description, comboBox, listener );
}

void igdeUIHelper::ComboBox( igdeContainer &parent, int columns, int rows, bool editable,
const char *description, igdeComboBoxReference &comboBox, igdeComboBoxListener *listener ){
	ComboBox( columns, rows, editable, description, comboBox, listener );
	parent.AddChild( comboBox );
}

void igdeUIHelper::ComboBox( int columns, int rows, bool editable, const char* description,
igdeComboBoxReference& comboBox, igdeComboBoxListener* listener ){
	comboBox.TakeOver( new igdeComboBox( pEnvironment, columns, rows, editable, description ) );
	if( listener ){
		comboBox->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::ComboBoxFilter( igdeContainer &form, const char *label,
const char *description, igdeComboBoxFilterReference &comboBox, igdeComboBoxListener *listener ){
	ComboBoxFilter( form, label, false, description, comboBox, listener );
}

void igdeUIHelper::ComboBoxFilter( igdeContainer &form, const char *label, bool editable,
const char *description, igdeComboBoxFilterReference &comboBox, igdeComboBoxListener *listener ){
	ComboBoxFilter( 15, 10, editable, description, comboBox, listener );
	FormLine( form, label, description, comboBox );
}

void igdeUIHelper::ComboBoxFilter( igdeContainer &parent, const char *description,
igdeComboBoxFilterReference &comboBox, igdeComboBoxListener *listener ){
	ComboBoxFilter( parent, 15, 10, false, description, comboBox, listener );
}

void igdeUIHelper::ComboBoxFilter( igdeContainer &parent, bool editable, const char *description,
igdeComboBoxFilterReference &comboBox, igdeComboBoxListener *listener ){
	ComboBoxFilter( parent, 15, 10, editable, description, comboBox, listener );
}

void igdeUIHelper::ComboBoxFilter( igdeContainer &parent, int columns, int rows,
const char *description, igdeComboBoxFilterReference &comboBox, igdeComboBoxListener *listener ){
	ComboBoxFilter( parent, columns, rows, false, description, comboBox, listener );
}

void igdeUIHelper::ComboBoxFilter( igdeContainer &parent, int columns, int rows, bool editable,
const char *description, igdeComboBoxFilterReference &comboBox, igdeComboBoxListener *listener ){
	ComboBoxFilter( columns, rows, editable, description, comboBox, listener );
	parent.AddChild( comboBox );
}

void igdeUIHelper::ComboBoxFilter( int columns, int rows, bool editable, const char *description,
igdeComboBoxFilterReference &comboBox, igdeComboBoxListener *listener ){
	comboBox.TakeOver( new igdeComboBoxFilter( pEnvironment, columns, rows, editable, description ) );
	if( listener ){
		comboBox->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::CheckBox( igdeContainer &form, igdeCheckBoxReference &checkBox,
igdeAction *action, bool takeOverAction ){
	CheckBox( checkBox, action, takeOverAction );
	FormLine( form, "", "", checkBox );
}

void igdeUIHelper::CheckBox( igdeContainer &form, const char *text, const char *description,
igdeCheckBoxReference &checkBox ){
	CheckBox( text, description, checkBox );
	FormLine( form, "", "", checkBox );
}

void igdeUIHelper::CheckBox( igdeCheckBoxReference &checkBox, igdeAction *action, bool takeOverAction ){
	checkBox.TakeOver( new igdeCheckBox( pEnvironment, action ) );
	if( takeOverAction && action ){
		action->FreeReference();
	}
}

void igdeUIHelper::CheckBox( const char *text, const char *description,
igdeCheckBoxReference &checkBox ){
	checkBox.TakeOver( new igdeCheckBox( pEnvironment, text, description, NULL ) );
}

void igdeUIHelper::CheckBoxOnly( igdeContainer &parent, igdeCheckBoxReference &checkBox,
igdeAction *action, bool takeOverAction ){
	checkBox.TakeOver( new igdeCheckBox( pEnvironment, action ) );
	if( takeOverAction && action ){
		action->FreeReference();
	}
	parent.AddChild( checkBox );
}

void igdeUIHelper::CheckBoxOnly( igdeContainer &parent, const char *text, const char *description,
igdeCheckBoxReference &checkBox ){
	checkBox.TakeOver( new igdeCheckBox( pEnvironment, text, description, NULL ) );
	parent.AddChild( checkBox );
}



void igdeUIHelper::Slider( igdeContainer &form, const char *label, const char *description,
float lower, float upper, int precision, float tickSpacing, igdeSliderReference &slider,
igdeSliderListener *listener ){
	Slider( description, lower, upper, precision, tickSpacing, slider, listener );
	FormLine( form, label, description, slider );
}

void igdeUIHelper::Slider( igdeContainer &form, const char *label, const char *description,
int lower, int upper, int tickSpacing, igdeSliderReference &slider, igdeSliderListener *listener ){
	Slider( description, lower, upper, tickSpacing, slider, listener );
	if( listener ){
		slider->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
	FormLine( form, label, description, slider );
}

void igdeUIHelper::Slider( igdeContainer &parent, const char *description,
float lower, float upper, int precision, float tickSpacing, igdeSliderReference &slider,
igdeSliderListener *listener ){
	slider.TakeOver( new igdeSlider( pEnvironment, igdeSlider::eoHorizontal,
		lower, upper, precision, tickSpacing, description ) );
	if( listener ){
		slider->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
	parent.AddChild( slider );
}

void igdeUIHelper::Slider( igdeContainer &parent, const char *description, int lower,
int upper, int tickSpacing, igdeSliderReference &slider, igdeSliderListener *listener ){
	slider.TakeOver( new igdeSlider( pEnvironment, igdeSlider::eoHorizontal,
		( float )lower, ( float )upper, 0, ( float )tickSpacing, description ) );
	if( listener ){
		slider->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
	parent.AddChild( slider );
}

void igdeUIHelper::Slider( const char *description, float lower, float upper, int precision,
float tickSpacing, igdeSliderReference &slider, igdeSliderListener *listener ){
	slider.TakeOver( new igdeSlider( pEnvironment, igdeSlider::eoHorizontal,
		lower, upper, precision, tickSpacing, description ) );
	if( listener ){
		slider->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}

void igdeUIHelper::Slider( const char *description, int lower, int upper, int tickSpacing,
igdeSliderReference &slider, igdeSliderListener *listener ){
	slider.TakeOver( new igdeSlider( pEnvironment, igdeSlider::eoHorizontal,
		( float )lower, ( float )upper, 0, ( float )tickSpacing, description ) );
	if( listener ){
		slider->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::ColorBox( igdeContainer &form, const char *label, const char *description,
igdeColorBoxReference &colorBox, igdeColorBoxListener *listener ){
	ColorBox( description, colorBox, listener );
	FormLine( form, label, description, colorBox );
}

void igdeUIHelper::ColorBox( igdeContainer &parent, const char* description,
igdeColorBoxReference &colorBox, igdeColorBoxListener *listener ){
	ColorBox( description, colorBox, listener );
	parent.AddChild( colorBox );
}

void igdeUIHelper::ColorBox( const char *description, igdeColorBoxReference &colorBox,
igdeColorBoxListener *listener ){
	colorBox.TakeOver( new igdeColorBox( pEnvironment, description ) );
	if( listener ){
		colorBox->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::EditPoint( igdeContainer &form, const char *label, const char *description,
igdeEditPointReference &editPoint, igdeEditPointListener *listener ){
	EditPoint( description, editPoint, listener );
	FormLine( form, label, description, editPoint );
}

void igdeUIHelper::EditPoint( igdeContainer &parent, const char *description,
igdeEditPointReference &editPoint, igdeEditPointListener *listener ){
	EditPoint( description, editPoint, listener );
	parent.AddChild( editPoint );
}

void igdeUIHelper::EditPoint( const char *description, igdeEditPointReference &editPoint,
igdeEditPointListener *listener ){
	editPoint.TakeOver( new igdeEditPoint( *this, 6, description ) );
	if( listener ){
		editPoint->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::EditPoint3( igdeContainer &form, const char *label, const char *description,
igdeEditPoint3Reference &editPoint, igdeEditPoint3Listener *listener ){
	EditPoint3( description, editPoint, listener );
	FormLine( form, label, description, editPoint );
}

void igdeUIHelper::EditPoint3( igdeContainer &parent, const char *description,
igdeEditPoint3Reference &editPoint, igdeEditPoint3Listener *listener ){
	EditPoint3( description, editPoint, listener );
	parent.AddChild( editPoint );
}

void igdeUIHelper::EditPoint3( const char *description, igdeEditPoint3Reference &editPoint,
igdeEditPoint3Listener *listener ){
	editPoint.TakeOver( new igdeEditPoint3( *this, 6, description ) );
	if( listener ){
		editPoint->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::EditPath( igdeContainer &form, const char *label,
const char *description, igdeEnvironment::eFilePatternListTypes resourceType,
igdeEditPathReference &editPath, igdeEditPathListener *listener, bool useGameVFS  ){
	EditPath( description, resourceType, editPath, listener, useGameVFS );
	FormLine( form, label, description, editPath );
}

void igdeUIHelper::EditPath( igdeContainer &form, const char *label, const char *description,
const igdeFilePatternList &customFilePatterns, igdeEditPathReference &editPath,
igdeEditPathListener *listener, bool useGameVFS  ){
	EditPath( description, customFilePatterns, editPath, listener, useGameVFS );
	FormLine( form, label, description, editPath );
}

void igdeUIHelper::EditPath( igdeContainer &parent, const char *description,
igdeEnvironment::eFilePatternListTypes resourceType,
igdeEditPathReference &editPath, igdeEditPathListener *listener, bool useGameVFS  ){
	EditPath( description, resourceType, editPath, listener, useGameVFS );
	parent.AddChild( editPath );
}

void igdeUIHelper::EditPath( igdeContainer &parent, const char *description,
const igdeFilePatternList &customFilePatterns, igdeEditPathReference &editPath,
igdeEditPathListener *listener, bool useGameVFS  ){
	EditPath( description, customFilePatterns, editPath, listener, useGameVFS );
	parent.AddChild( editPath );
}

void igdeUIHelper::EditPath( const char *description, igdeEnvironment::eFilePatternListTypes resourceType,
igdeEditPathReference &editPath, igdeEditPathListener *listener, bool useGameVFS  ){
	editPath.TakeOver( new igdeEditPath( *this, resourceType, description, useGameVFS ) );
	if( listener ){
		editPath->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}

void igdeUIHelper::EditPath( const char *description, const igdeFilePatternList &customFilePatterns,
igdeEditPathReference &editPath, igdeEditPathListener *listener, bool useGameVFS  ){
	editPath.TakeOver( new igdeEditPath( *this, customFilePatterns, description, useGameVFS ) );
	if( listener ){
		editPath->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::EditDirectory( igdeContainer &form, const char *label,
const char *description, igdeEditDirectoryReference &editDirectory,
igdeEditDirectoryListener *listener, bool useGameVFS ){
	EditDirectory( description, editDirectory, listener, useGameVFS );
	FormLine( form, label, description, editDirectory );
}

void igdeUIHelper::EditDirectory( igdeContainer &parent, const char *description,
igdeEditDirectoryReference &editDirectory, igdeEditDirectoryListener *listener, bool useGameVFS ){
	EditDirectory( description, editDirectory, listener, useGameVFS );
	parent.AddChild( editDirectory );
}

void igdeUIHelper::EditDirectory( const char *description, igdeEditDirectoryReference &editDirectory,
igdeEditDirectoryListener *listener, bool useGameVFS ){
	editDirectory.TakeOver( new igdeEditDirectory( *this, description, useGameVFS ) );
	if( listener ){
		editDirectory->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::EditDVector( igdeContainer &form, const char *label, const char *description,
igdeEditDVectorReference &editDVector, igdeEditDVectorListener *listener ){
	EditDVector( form, label, description, 6, 3, editDVector, listener );
}

void igdeUIHelper::EditDVector( igdeContainer &form, const char *label, const char *description,
int columns, int precision, igdeEditDVectorReference &editDVector, igdeEditDVectorListener *listener ){
	EditDVector( description, columns, precision, editDVector, listener );
	FormLine( form, label, description, editDVector );
}

void igdeUIHelper::EditDVector( igdeContainer &parent, const char *description,
igdeEditDVectorReference &editDVector, igdeEditDVectorListener *listener ){
	EditDVector( parent, description, 6, 3, editDVector, listener );
}

void igdeUIHelper::EditDVector( igdeContainer &parent, const char *description, int columns,
int precision, igdeEditDVectorReference &editDVector, igdeEditDVectorListener *listener ){
	EditDVector( description, columns, precision, editDVector, listener );
	parent.AddChild( editDVector );
}

void igdeUIHelper::EditDVector( const char *description, igdeEditDVectorReference &editDVector,
igdeEditDVectorListener *listener ){
	EditDVector( description, 6, 3, editDVector, listener );
}

void igdeUIHelper::EditDVector( const char *description, int columns, int precision,
igdeEditDVectorReference &editDVector, igdeEditDVectorListener *listener ){
	editDVector.TakeOver( new igdeEditDVector( *this, columns, precision, description ) );
	if( listener ){
		editDVector->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::EditVector( igdeContainer &form, const char *label, const char *description,
igdeEditVectorReference &editVector, igdeEditVectorListener *listener ){
	EditVector( form, label, description, 6, 3, editVector, listener );
}

void igdeUIHelper::EditVector( igdeContainer &form, const char *label, const char *description,
int columns, int precision, igdeEditVectorReference &editVector, igdeEditVectorListener *listener ){
	EditVector( description, columns, precision, editVector, listener );
	FormLine( form, label, description, editVector );
}

void igdeUIHelper::EditVector( igdeContainer &parent, const char *description,
igdeEditVectorReference &editVector, igdeEditVectorListener *listener ){
	EditVector( parent, description, 6, 3, editVector, listener );
}

void igdeUIHelper::EditVector( igdeContainer &parent, const char *description, int columns,
int precision, igdeEditVectorReference &editVector, igdeEditVectorListener *listener ){
	EditVector( description, columns, precision, editVector, listener );
	parent.AddChild( editVector );
}

void igdeUIHelper::EditVector( const char *description, igdeEditVectorReference &editVector,
igdeEditVectorListener *listener ){
	EditVector( description, 6, 3, editVector, listener );
}

void igdeUIHelper::EditVector( const char *description, int columns, int precision,
igdeEditVectorReference &editVector, igdeEditVectorListener *listener ){
	editVector.TakeOver( new igdeEditVector( *this, columns, precision, description ) );
	if( listener ){
		editVector->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::EditVector2( igdeContainer &form, const char *label, const char *description,
igdeEditVector2Reference &editVector2, igdeEditVector2Listener *listener ){
	EditVector2( form, label, description, 6, 3, editVector2, listener );
}

void igdeUIHelper::EditVector2( igdeContainer &form, const char *label, const char *description,
int columns, int precision, igdeEditVector2Reference &editVector2, igdeEditVector2Listener *listener ){
	EditVector2( description, columns, precision, editVector2, listener );
	FormLine( form, label, description, editVector2 );
}

void igdeUIHelper::EditVector2( igdeContainer &parent, const char *description,
igdeEditVector2Reference &editVector2, igdeEditVector2Listener *listener ){
	EditVector2( parent, description, 6, 3, editVector2, listener );
}

void igdeUIHelper::EditVector2( igdeContainer &parent, const char *description, int columns,
int precision, igdeEditVector2Reference &editVector2, igdeEditVector2Listener *listener ){
	EditVector2( description, columns, precision, editVector2, listener );
	parent.AddChild( editVector2 );
}

void igdeUIHelper::EditVector2( const char *description, igdeEditVector2Reference &editVector2,
igdeEditVector2Listener *listener ){
	EditVector2( description, 6, 3, editVector2, listener );
}

void igdeUIHelper::EditVector2( const char *description, int columns, int precision,
igdeEditVector2Reference &editVector2, igdeEditVector2Listener *listener ){
	editVector2.TakeOver( new igdeEditVector2( *this, columns, precision, description ) );
	if( listener ){
		editVector2->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::EditSliderText( igdeContainer &form, const char *label, const char *description,
float lower, float upper, int columns, int precision, float tickSpacing,
igdeEditSliderTextReference &slider, igdeEditSliderTextListener *listener ){
	EditSliderText( description, lower, upper, columns, precision, tickSpacing, slider, listener );
	FormLine( form, label, description, slider );
}

void igdeUIHelper::EditSliderText( igdeContainer &form, const char *label,
const char *description, int lower, int upper, int columns, int tickSpacing,
igdeEditSliderTextReference &slider, igdeEditSliderTextListener *listener ){
	EditSliderText( description, lower, upper, columns, tickSpacing, slider, listener );
	FormLine( form, label, description, slider );
}

void igdeUIHelper::EditSliderText( igdeContainer &parent, const char *description,
float lower, float upper, int columns, int precision, float tickSpacing,
igdeEditSliderTextReference &slider, igdeEditSliderTextListener *listener ){
	EditSliderText( description, lower, upper, columns, precision, tickSpacing, slider, listener );
	parent.AddChild( slider );
}

void igdeUIHelper::EditSliderText( igdeContainer &parent, const char *description,
int lower, int upper, int columns, int tickSpacing, igdeEditSliderTextReference &slider,
igdeEditSliderTextListener *listener ){
	EditSliderText( description, lower, upper, columns, tickSpacing, slider, listener );
	parent.AddChild( slider );
}

void igdeUIHelper::EditSliderText( const char *description, float lower, float upper,
int columns, int precision, float tickSpacing, igdeEditSliderTextReference &slider,
igdeEditSliderTextListener *listener ){
	slider.TakeOver( new igdeEditSliderText( *this, lower, upper, columns, precision,
		tickSpacing, description ) );
	if( listener ){
		slider->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}

void igdeUIHelper::EditSliderText( const char *description, int lower, int upper, int columns,
int tickSpacing, igdeEditSliderTextReference &slider, igdeEditSliderTextListener *listener ){
	slider.TakeOver( new igdeEditSliderText( *this, ( float )lower,
		( float )upper, columns, 0, ( float )tickSpacing, description ) );
	if( listener ){
		slider->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::Button( igdeContainer &parent, igdeAction *action, bool takeOverAction ){
	igdeButtonReference button;
	Button( button, action, takeOverAction );
	parent.AddChild( button );
}

void igdeUIHelper::Button( igdeContainer &parent, igdeButtonReference &button,
igdeAction *action, bool takeOverAction ){
	Button( button, action, takeOverAction );
	parent.AddChild( button );
}

void igdeUIHelper::Button( igdeButtonReference &button, igdeAction *action, bool takeOverAction ){
	button.TakeOver( new igdeButton( pEnvironment, action ) );
	if( takeOverAction && action ){
		action->FreeReference();
	}
}



void igdeUIHelper::ToggleButton( igdeContainer &parent, igdeAction *action, bool takeOverAction ){
	igdeToggleButtonReference button;
	ToggleButton( button, action, takeOverAction );
	parent.AddChild( button );
}

void igdeUIHelper::ToggleButton( igdeContainer &parent, igdeToggleButtonReference &button,
igdeAction *action, bool takeOverAction ){
	ToggleButton( button, action, takeOverAction );
	parent.AddChild( button );
}

void igdeUIHelper::ToggleButton( igdeToggleButtonReference &button, igdeAction *action, bool takeOverAction ){
	button.TakeOver( new igdeToggleButton( pEnvironment, action ) );
	if( takeOverAction && action ){
		action->FreeReference();
	}
}



void igdeUIHelper::ListBox( igdeContainer &form, const char *label, int rows,
const char *description, igdeListBoxReference &listBox, igdeListBoxListener *listener ){
	ListBox( rows, description, listBox, listener );
	FormLine( form, label, description, listBox, igdeLabel::eaLeft | igdeLabel::eaTop );
}

void igdeUIHelper::ListBox( igdeContainer &parent, int rows, const char *description,
igdeListBoxReference &listBox, igdeListBoxListener *listener ){
	ListBox( rows, description, listBox, listener );
	parent.AddChild( listBox );
}

void igdeUIHelper::ListBox( int rows, const char *description, igdeListBoxReference &listBox,
igdeListBoxListener *listener ){
	listBox.TakeOver( new igdeListBox( pEnvironment, rows, description ) );
	if( listener ){
		listBox->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::IconListBox( igdeContainer &parent, igdeIconListBoxReference &listBox,
const sColumnHeader *headers, int headerCount, const char *description, igdeIconListBoxListener *listener ){
	IconListBox( headers, headerCount, description, listBox, listener );
	parent.AddChild( listBox );
}


void igdeUIHelper::IconListBox( igdeContainer &parent, igdeIconListBoxReference &listBox,
const decPoint &minimumSize, const sColumnHeader *headers, int headerCount, const char *description,
igdeIconListBoxListener *listener ){
	IconListBox( minimumSize, headers, headerCount, description, listBox, listener );
	parent.AddChild( listBox );
}

static void igdeUIHelperIconListBoxShared( igdeIconListBox &listBox,
const igdeUIHelper::sColumnHeader *headers, int headerCount, igdeIconListBoxListener *listener ){
	igdeListHeaderReference realHeader;
	
	int i;
	for( i=0; i<headerCount; i++ ){
		realHeader.TakeOver( new igdeListHeader( headers[ i ].title,
			headers[ i ].icon, headers[ i ].size ) );
		listBox.AddHeader( realHeader );
	}
	
	if( listener ){
		listBox.AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}

void igdeUIHelper::IconListBox( const sColumnHeader *headers, int headerCount,
const char *description, igdeIconListBoxReference &listBox, igdeIconListBoxListener *listener ){
	if( ! headers || headerCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	listBox.TakeOver( new igdeIconListBox( pEnvironment, description ) );
	igdeUIHelperIconListBoxShared( listBox, headers, headerCount, listener );
}

void igdeUIHelper::IconListBox( const decPoint &minimumSize, const sColumnHeader *headers, int headerCount,
const char *description, igdeIconListBoxReference &listBox, igdeIconListBoxListener *listener ){
	if( ! headers || headerCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	listBox.TakeOver( new igdeIconListBox( pEnvironment, minimumSize, description ) );
	igdeUIHelperIconListBoxShared( listBox, headers, headerCount, listener );
}



void igdeUIHelper::TreeList( igdeContainer &parent, igdeTreeListReference &treeList,
int rows, const char *description, igdeTreeListListener *listener ){
	TreeList( rows, description, treeList, listener );
	parent.AddChild( treeList );
}

void igdeUIHelper::TreeList( int rows, const char *description, igdeTreeListReference &treeList,
igdeTreeListListener *listener ){
	treeList.TakeOver( new igdeTreeList( pEnvironment, rows, description ) );
	if( listener ){
		treeList->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::Label( igdeContainer &parent, const char *text ){
	igdeLabelReference label;
	label.TakeOver( new igdeLabel( pEnvironment, text ) );
	parent.AddChild( label );
}

void igdeUIHelper::Label( igdeContainer &parent, const char *text, const char *description ){
	igdeLabelReference label;
	label.TakeOver( new igdeLabel( pEnvironment, text, description ) );
	parent.AddChild( label );
}

void igdeUIHelper::Label( igdeContainer &parent, const char *text,
const char *description, int alignment ){
	igdeLabelReference label;
	label.TakeOver( new igdeLabel( pEnvironment, text, description, alignment ) );
	parent.AddChild( label );
}

void igdeUIHelper::Label( igdeContainer &parent, igdeLabelReference &label, const char *text ){
	label.TakeOver( new igdeLabel( pEnvironment, text ) );
	parent.AddChild( label );
}

void igdeUIHelper::Label( igdeContainer &parent, igdeLabelReference &label,
const char *text, const char *description, int alignment ){
	label.TakeOver( new igdeLabel( pEnvironment, text, description, alignment ) );
	parent.AddChild( label );
}

void igdeUIHelper::Label( igdeLabelReference &label, const char *text ){
	label.TakeOver( new igdeLabel( pEnvironment, text ) );
}

void igdeUIHelper::Label( igdeLabelReference &label, const char *text,
const char *description, int alignment ){
	label.TakeOver( new igdeLabel( pEnvironment, text, description, alignment ) );
}



void igdeUIHelper::ScrollBar( igdeContainer &parent, bool horizontal, int lower, int upper,
int pageSize, int value, igdeScrollBarReference &scrollBar, igdeScrollBarListener *listener ){
	ScrollBar( horizontal, lower, upper, pageSize, value, scrollBar, listener );
	parent.AddChild( scrollBar );
}

void igdeUIHelper::ScrollBar( bool horizontal, int lower, int upper, int pageSize, int value,
igdeScrollBarReference &scrollBar, igdeScrollBarListener *listener ){
	scrollBar.TakeOver( new igdeScrollBar( pEnvironment,
		horizontal ? igdeScrollBar::eoHorizontal : igdeScrollBar::eoVertical,
		lower, upper, pageSize, value ) );
	if( listener ){
		scrollBar->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::ViewCurveBezier( igdeContainer &parent,
igdeViewCurveBezierReference &viewCurveBezier, igdeViewCurveBezierListener *listener ){
	ViewCurveBezier( viewCurveBezier, listener );
	parent.AddChild( viewCurveBezier );
}

void igdeUIHelper::ViewCurveBezier( igdeViewCurveBezierReference &viewCurveBezier,
igdeViewCurveBezierListener *listener ){
	viewCurveBezier.TakeOver( new igdeViewCurveBezier( pEnvironment ) );
	if( listener ){
		viewCurveBezier->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}



void igdeUIHelper::Separator( igdeContainer &parent, bool horizontal ){
	igdeWidgetReference entry;
	entry.TakeOver( new igdeSeparator( pEnvironment,
		horizontal ? igdeSeparator::eoHorizontal : igdeSeparator::eoVertical ) );
	parent.AddChild( entry );
}



void igdeUIHelper::Spacer( igdeContainer &parent, const decPoint &size ){
	igdeSpacerReference spacer;
	Spacer( parent, size, spacer );
}

void igdeUIHelper::Spacer( igdeContainer &parent, const decPoint &size, igdeSpacerReference &spacer ){
	spacer.TakeOver( new igdeSpacer( pEnvironment, size ) );
	parent.AddChild( spacer );
}



void igdeUIHelper::WPSky( igdeContainer &parent, igdeAction *action,
const char *title, bool stretchLast, bool collapsed, bool takeOverAction ){
	igdeWPSkyReference panel;
	WPSky( parent, panel, action, title, stretchLast, collapsed, takeOverAction );
}

void igdeUIHelper::WPSky( igdeContainer &parent, igdeWPSkyReference &panel,
igdeAction *action, const char *title, bool stretchLast, bool collapsed, bool takeOverAction ){
	igdeContainerReference groupBox;
	GroupBoxFlow( parent, groupBox, title, stretchLast, collapsed );
	WPSky( panel, action, takeOverAction );
	groupBox->AddChild( panel );
}

void igdeUIHelper::WPSky( igdeWPSkyReference &panel, igdeAction *action, bool takeOverAction ){
	panel.TakeOver( new igdeWPSky( pEnvironment, action ) );
	if( takeOverAction && action ){
		action->FreeReference();
	}
}



void igdeUIHelper::WPCamera( igdeContainer &parent, igdeAction *action,
const char *title, bool stretchLast, bool collapsed, bool takeOverAction ){
	igdeWPCameraReference panel;
	WPCamera( parent, panel, action, title, stretchLast, collapsed, takeOverAction );
}

void igdeUIHelper::WPCamera( igdeContainer &parent, igdeWPCameraReference &panel,
igdeAction *action, const char *title, bool stretchLast, bool collapsed, bool takeOverAction ){
	igdeContainerReference groupBox;
	GroupBoxFlow( parent, groupBox, title, stretchLast, collapsed );
	WPCamera( panel, action, takeOverAction );
	groupBox->AddChild( panel );
}

void igdeUIHelper::WPCamera( igdeWPCameraReference &panel, igdeAction *action, bool takeOverAction ){
	panel.TakeOver( new igdeWPCamera( pEnvironment, action ) );
	if( takeOverAction && action ){
		action->FreeReference();
	}
}



void igdeUIHelper::WPWObject( igdeContainer &parent, igdeAction *action,
const char *title, bool stretchLast, bool collapsed, bool takeOverAction ){
	igdeWPWObjectReference panel;
	WPWObject( parent, panel, action, title, stretchLast, collapsed, takeOverAction );
}

void igdeUIHelper::WPWObject( igdeContainer &parent, igdeWPWObjectReference &panel,
igdeAction *action, const char *title, bool stretchLast, bool collapsed, bool takeOverAction ){
	igdeContainerReference groupBox;
	GroupBoxFlow( parent, groupBox, title, stretchLast, collapsed );
	WPWObject( panel, action, takeOverAction );
	groupBox->AddChild( panel );
}

void igdeUIHelper::WPWObject( igdeWPWObjectReference &panel, igdeAction *action, bool takeOverAction ){
	panel.TakeOver( new igdeWPWObject( pEnvironment, action ) );
	if( takeOverAction && action ){
		action->FreeReference();
	}
}



void igdeUIHelper::EditTags( igdeContainer &parent, igdeEditTagsReference &editTags,
igdeAction *action, bool takeOverAction ){
	EditTags( editTags, action, takeOverAction );
	parent.AddChild( editTags );
}

void igdeUIHelper::EditTags( igdeEditTagsReference &editTags, igdeAction *action, bool takeOverAction ){
	editTags.TakeOver( new igdeEditTags( *this, 5, action ) );
	if( takeOverAction && action ){
		action->FreeReference();
	}
}



void igdeUIHelper::ToggleTags( igdeContainer &parent, igdeToggleTagsReference &toggleTags,
igdeAction *action, bool takeOverAction ){
	ToggleTags( toggleTags, action, takeOverAction );
	parent.AddChild( toggleTags );
}

void igdeUIHelper::ToggleTags( igdeToggleTagsReference &toggleTags, igdeAction *action,
bool takeOverAction ){
	toggleTags.TakeOver( new igdeToggleTags( *this, 5, action ) );
	if( takeOverAction && action ){
		action->FreeReference();
	}
}



void igdeUIHelper::EditPropertyValue( igdeEditPropertyValueReference &widget,
igdeEditPropertyValueListener *listener ){
	widget.TakeOver( new igdeEditPropertyValue( *this ) );
	if( listener ){
		widget->AddListener( listener );
		listener->FreeReference(); // we take over the reference
	}
}

void igdeUIHelper::EditPropertyValue( igdeContainer &parent,
igdeEditPropertyValueReference &widget, igdeEditPropertyValueListener *listener ){
	EditPropertyValue( widget, listener );
	parent.AddChild( widget );
}



void igdeUIHelper::WPTriggerTable( igdeContainer &parent, igdeAction *action,
const char *title, bool stretchLast, bool collapsed, bool takeOverAction ){
	igdeWPTriggerTableReference panel;
	WPTriggerTable( parent, panel, action, title, stretchLast, collapsed, takeOverAction );
}

void igdeUIHelper::WPTriggerTable( igdeContainer &parent, igdeWPTriggerTableReference &panel,
igdeAction *action, const char *title, bool stretchLast, bool collapsed, bool takeOverAction ){
	igdeContainerReference groupBox;
	GroupBoxFlow( parent, groupBox, title, stretchLast, collapsed );
	WPTriggerTable( panel, action, takeOverAction );
	groupBox->AddChild( panel );
}

void igdeUIHelper::WPTriggerTable( igdeWPTriggerTableReference &panel, igdeAction *action,
bool takeOverAction ){
	panel.TakeOver( new igdeWPTriggerTable( pEnvironment, action ) );
	if( takeOverAction && action ){
		action->FreeReference();
	}
}



void igdeUIHelper::MenuCommand( igdeMenuCascade &menu, igdeAction *action, bool takeOverAction ){
	igdeWidgetReference entry;
	entry.TakeOver( new igdeMenuCommand( pEnvironment, action ) );
	if( takeOverAction && action ){
		action->FreeReference();
	}
	menu.AddChild( entry );
}

void igdeUIHelper::MenuCheck( igdeMenuCascade &menu, igdeAction *action, bool takeOverAction ){
	igdeWidgetReference entry;
	entry.TakeOver( new igdeMenuCheck( pEnvironment, action ) );
	if( takeOverAction && action ){
		action->FreeReference();
	}
	menu.AddChild( entry );
}

void igdeUIHelper::MenuOption( igdeMenuCascade &menu, igdeAction *action, bool takeOverAction ){
	igdeWidgetReference entry;
	entry.TakeOver( new igdeMenuOption( pEnvironment, action ) );
	if( takeOverAction && action ){
		action->FreeReference();
	}
	menu.AddChild( entry );
}

void igdeUIHelper::MenuSeparator( igdeMenuCascade &menu ){
	igdeWidgetReference entry;
	entry.TakeOver( new igdeMenuSeparator( pEnvironment ) );
	menu.AddChild( entry );
}

void igdeUIHelper::MenuRecentFiles( igdeMenuCascade &menu, igdeRecentFiles &recentFiles, const char *text ){
	igdeMenuCascadeReference subMenu;
	subMenu.TakeOver( new igdeMenuCascade( pEnvironment, text,
		pEnvironment.GetStockIcon( igdeEnvironment::esiOpen ), text ) );
	recentFiles.SetMenu( subMenu );
	menu.AddChild( subMenu );
}



void igdeUIHelper::ToolBarButton( igdeToolBar &toolBar, igdeAction *action, bool takeOverAction ){
	igdeWidgetReference widget;
	widget.TakeOver( new igdeButton( pEnvironment, action, igdeButton::ebsToolBar ) );
	if( takeOverAction && action ){
		action->FreeReference();
	}
	toolBar.AddChild( widget );
}

void igdeUIHelper::ToolBarToggleButton( igdeToolBar &toolBar, igdeAction *action, bool takeOverAction ){
	igdeWidgetReference widget;
	widget.TakeOver( new igdeToggleButton( pEnvironment, action, igdeToggleButton::ebsToolBar ) );
	if( takeOverAction && action ){
		action->FreeReference();
	}
	toolBar.AddChild( widget );
}

void igdeUIHelper::ToolBarSeparator( igdeToolBar &toolBar ){
	igdeWidgetReference widget;
	widget.TakeOver( new igdeToolBarSeparator( pEnvironment ) );
	toolBar.AddChild( widget );
}



decString igdeUIHelper::FormatSize1024( uint64_t size ){
	decString text;
	
	if( size >= 1024000000 ){
		text.Format( "%d.%d GB", ( int )( size / 1024000000 ),
			( int )( size % 1024000000 ) / 102400000 );
		
	}else if( size >= 1024000 ){
		text.Format( "%d.%d MB", ( int )( size / 1024000 ),
			( int )( size % 1024000 ) / 102400 );
		
	}else if( size >= 1024 ){
		text.Format( "%d.%d kB", ( int )( size / 1024 ),
			( ( int )( size % 1024 ) * 10 ) / 1024 );
		
	}else{
		text.Format( "%d B", ( int )size );
	}
	
	return text;
}

decString igdeUIHelper::FormatSize1000( uint64_t size ){
	decString text;
	
	if( size >= 1000000000 ){
		text.Format( "%d.%d GB", ( int )( size / 1000000000 ),
			( int )( size % 1000000000 ) / 100000000 );
		
	}else if( size >= 1000000 ){
		text.Format( "%d.%d MB", ( int )( size / 1000000 ),
			( int )( size % 1000000 ) / 100000 );
		
	}else if( size >= 1000 ){
		text.Format( "%d.%d kB", ( int )( size / 1000 ),
			( int )( size % 1000 ) % 100 );
		
	}else{
		text.Format( "%d B", ( int )size );
	}
	
	return text;
}

decString igdeUIHelper::FormatSizeTousand( uint64_t size ){
	if( size < 1000 ){
		decString text;
		text.Format( "%d", ( int )size );
		return text;
	}
	
	uint64_t scratch = size;
	int digitCount = 0;
	while( scratch > 0 ){
		scratch /= 10;
		digitCount++;
	}
	
	const int separatorCount = ( digitCount - 1 ) / 3;
	
	decString text;
	text.Set( '\'', digitCount + separatorCount );
	
	int i;
	scratch = size;
	char *tp = ( char* )text.GetString() + ( digitCount + separatorCount - 1 );
	for( i=1; i<=digitCount; i++ ){
		*(tp--) = '0' + ( scratch % 10 );
		scratch /= 10;
		if( ( i % 3 ) == 0 ){
			tp--;
		}
	}
	
	return text;
}
