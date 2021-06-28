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

#ifndef _IGDEUIHELPER_H_
#define _IGDEUIHELPER_H_

#include "../environment/igdeEnvironment.h"

#include <stdint.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class igdeAction;
class igdeButtonReference;
class igdeCheckBoxReference;
class igdeColorBoxListener;
class igdeColorBoxReference;
class igdeComboBoxFilterReference;
class igdeComboBoxListener;
class igdeComboBoxReference;
class igdeContainer;
class igdeContainerBorderReference;
class igdeContainerReference;
class igdeContainerScrollReference;
class igdeEditDirectoryListener;
class igdeEditDirectoryReference;
class igdeEditDVectorListener;
class igdeEditDVectorReference;
class igdeEditPathListener;
class igdeEditPathReference;
class igdeEditPoint3Listener;
class igdeEditPoint3Reference;
class igdeEditPointListener;
class igdeEditPointReference;
class igdeEditPropertyValueListener;
class igdeEditPropertyValueReference;
class igdeEditSliderTextListener;
class igdeEditSliderTextReference;
class igdeEditTagsReference;
class igdeEditVector2Listener;
class igdeEditVector2Reference;
class igdeEditVectorListener;
class igdeEditVectorReference;
class igdeEnvironment;
class igdeIcon;
class igdeIconListBoxListener;
class igdeIconListBoxReference;
class igdeLabelReference;
class igdeListBoxListener;
class igdeListBoxReference;
class igdeMenuCascade;
class igdeProgressBarReference;
class igdeRecentFiles;
class igdeScrollBarListener;
class igdeScrollBarReference;
class igdeSliderListener;
class igdeSliderReference;
class igdeSpacerReference;
class igdeSpinTextFieldListener;
class igdeSpinTextFieldReference;
class igdeTextAreaListener;
class igdeTextAreaReference;
class igdeTextFieldListener;
class igdeTextFieldReference;
class igdeToggleButtonReference;
class igdeToggleTagsReference;
class igdeToolBar;
class igdeTreeListListener;
class igdeTreeListReference;
class igdeViewCurveBezierListener;
class igdeViewCurveBezierReference;
class igdeWidgetReference;
class igdeWPCameraReference;
class igdeWPSkyReference;
class igdeWPTriggerTableReference;
class igdeWPWObjectReference;
class igdeWidget;


/**
 * \brief UI Helpers.
 * 
 * Simplifies common tasks.
 */
class igdeUIHelper{
public:
	/** \brief Column header configuration. */
	struct sColumnHeader{
		/** \brief Title. */
		const char *title;
		
		/** \brief Icon. */
		igdeIcon *icon;
		
		/** \brief Size in pixels. */
		int size;
		
		/** \brief Init struct. */
		sColumnHeader( const char *title, igdeIcon *icon, int size );
	};
	
	
	
private:
	igdeEnvironment &pEnvironment;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create UI Helper. */
	igdeUIHelper( igdeEnvironment &environment );
	
	/** \brief Create copy of UI Helper. */
	igdeUIHelper( const igdeUIHelper &helper );
	/*@}*/
	
	
	
	/** \name Helpers */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	
	
	/**
	 * \brief Create scrollable side panel layout.
	 * 
	 * Creates scrolling container with vertical flow layout as widget. Optionally the
	 * flow layout is stretching the last widget. The flow layout is placed in \em panel.
	 * The flow layout is assigned the theme name igdeGuiThemeNames::properties .
	 */
	void SidePanel( igdeContainerScrollReference &scroll, igdeContainerReference &panel,
		bool stretchLast = false, int spacing = 0 );
	
	void SidePanel( igdeContainer &parent, igdeContainerReference &panel,
		bool stretchLast = false, int spacing = 0 );
	
	
	
	/** \brief Create igdeGroupBox with an igdeContainerForm layout. */
	void GroupBox( igdeContainer &parent, igdeContainerReference &groupBox,
		const char *title, bool collapsed = false ) const;
	
	/** \brief Create non-collapsable igdeGroupBox with an igdeContainerForm layout. */
	void GroupBoxStatic( igdeContainer &parent, igdeContainerReference &groupBox, const char *title ) const;
	
	/** \brief Create igdeGroupBox with an igdeContainerFlow layout. */
	void GroupBoxFlow( igdeContainer &parent, igdeContainerReference &groupBox,
		const char *title, bool stretchLast = false, bool collapsed = false ) const;
	
	/** \brief Create igdeGroupBox with an igdeContainerBorder layout. */
	void GroupBoxBorder( igdeContainer &parent, igdeContainerBorderReference &groupBox,
		const char *title, bool collapsed = false ) const;
	
	/** \brief Create non-collapsable igdeGroupBox with an igdeContainerFlow layout. */
	void GroupBoxStaticFlow( igdeContainer &parent, igdeContainerReference &groupBox,
		const char *title, bool stretchLast = false ) const;
	
	/** \brief Create non-collapsable igdeGroupBox with an igdeContainerBorder layout. */
	void GroupBoxStaticBorder( igdeContainer &parent, igdeContainerBorderReference &groupBox,
		const char *title, bool stretch = false ) const;
	
	
	
	/** \brief Form line with label and one widget, */
	void FormLine( igdeContainer &form, const char *label, const char *description,
		igdeWidget *widget );
	
	void FormLine( igdeContainer &form, const char *label, const char *description,
		igdeWidget *widget, int labelAlignment );
	
	/**
	 * \brief Form line with label and container for additing multiple widgets.
	 * 
	 * Container is igdeContainerBox equally stretching widgets.
	 */
	void FormLine( igdeContainer &form, const char *label, const char *description,
		igdeContainerReference &line );
	
	void FormLine( igdeContainer &form, const char *label, const char *description,
		igdeContainerReference &line, int labelAlignment );
	
	/**
	 * \brief Form line with label and container for additing multiple widgets.
	 * 
	 * Container is igdeContainerFlow is used instead set to stretch the first widget.
	 */
	void FormLineStretchFirst( igdeContainer &form, const char *label,
		const char *description, igdeContainerReference &line );
	
	void FormLineStretchFirst( igdeContainer &form, const char *label,
		const char *description, igdeContainerReference &line, int labelAlignment );
	
	/**
	 * \brief Form line with label and container flowing downwards.
	 * 
	 * Container is igdeContainerFlow along y axis.
	 */
	void FormLineDownFlow( igdeContainer &form, const char *label,
		const char *description, igdeContainerReference &line );
	
	void FormLineDownFlow( igdeContainer &form, const char *label,
		const char *description, igdeContainerReference &line, int labelAlignment );
	
	
	
	/**
	 * \brief Form line with label and text field.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditString( igdeContainer &form, const char *label, const char *description,
		igdeTextFieldReference &textField, igdeTextFieldListener *listener );
	
	void EditString( igdeContainer &form, const char *label, const char *description,
		int rows, igdeTextFieldReference &textField, igdeTextFieldListener *listener );
	
	/**
	 * \brief Add text field.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditString( igdeContainer &parent, const char *description,
		igdeTextFieldReference &textField, igdeTextFieldListener *listener );
	
	void EditString( const char *description, igdeTextFieldReference &textField,
		igdeTextFieldListener *listener );
	
	void EditString( const char *description, int rows, igdeTextFieldReference &textField,
		igdeTextFieldListener *listener );
	
	
	
	/**
	 * \brief Form line with label and text area.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditString( igdeContainer &form, const char *label, const char *description,
		igdeTextAreaReference &textArea, int rows, igdeTextAreaListener *listener );
	
	void EditString( igdeContainer &form, const char *label, const char *description,
		igdeTextAreaReference &textArea, int columns, int rows, igdeTextAreaListener *listener );
	
	/**
	 * \brief Add text area.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditString( igdeContainer &parent, const char *description,
		igdeTextAreaReference &textArea, int rows, igdeTextAreaListener *listener );
	
	void EditString( igdeContainer &parent, const char *description,
		igdeTextAreaReference &textArea, int columns, int rows, igdeTextAreaListener *listener );
	
	void EditString( const char *description, int rows, igdeTextAreaReference &textArea,
		igdeTextAreaListener *listener );
	
	void EditString( const char *description, int columns, int rows,
		igdeTextAreaReference &textArea, igdeTextAreaListener *listener );
	
	
	
	/**
	 * \brief Form line with label and text field.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditFloat( igdeContainer &form, const char *label, const char *description,
		igdeTextFieldReference &textField, igdeTextFieldListener *listener );
	
	void EditFloat( igdeContainer &form, const char *label, const char *description,
		int columns, int precision, igdeTextFieldReference &textField,
		igdeTextFieldListener *listener );
	
	/**
	 * \brief Add text field.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditFloat( igdeContainer &parent, const char *description,
		igdeTextFieldReference &textField, igdeTextFieldListener *listener );
	
	void EditFloat( igdeContainer &parent, const char *description, int columns, int precision,
		igdeTextFieldReference &textField, igdeTextFieldListener *listener );
	
	void EditFloat( const char *description, igdeTextFieldReference &textField,
		igdeTextFieldListener *listener );
	
	void EditFloat( const char *description, int columns, int precision,
		igdeTextFieldReference &textField, igdeTextFieldListener *listener );
	
	
	
	/**
	 * \brief Form line with label and text field.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditInteger( igdeContainer &form, const char *label, const char *description,
		igdeTextFieldReference &textField, igdeTextFieldListener *listener );
	
	void EditInteger( igdeContainer &form, const char *label, const char *description,
		int columns, igdeTextFieldReference &textField, igdeTextFieldListener *listener );
	
	/**
	 * \brief Add text field.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditInteger( igdeContainer &parent, const char *description,
		igdeTextFieldReference &textField, igdeTextFieldListener *listener );
	
	void EditInteger( igdeContainer &parent, const char *description, int columns,
		igdeTextFieldReference &textField, igdeTextFieldListener *listener );
	
	void EditInteger( const char *description, igdeTextFieldReference &textField,
		igdeTextFieldListener *listener );
	
	void EditInteger( const char *description, int columns, igdeTextFieldReference &textField,
		igdeTextFieldListener *listener );
	
	
	
	/**
	 * \brief Form line with label and spin text field.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditSpinInteger( igdeContainer &form, const char *label, const char *description,
		int lower, int upper, igdeSpinTextFieldReference &spinTextField,
		igdeSpinTextFieldListener *listener );
	
	void EditSpinInteger( igdeContainer &parent, const char *description, int lower, int upper,
		igdeSpinTextFieldReference &spinTextField, igdeSpinTextFieldListener *listener );
	
	void EditSpinInteger( const char *description, int lower, int upper,
		igdeSpinTextFieldReference &spinTextField, igdeSpinTextFieldListener *listener );
	
	
	
	/** \brief Form line with label and progress bar. */
	void ProgressBar( igdeContainer &form, const char *label, const char *description,
		int lower, int upper, bool horizontal, igdeProgressBarReference &progressBar );
	
	void ProgressBar( igdeContainer &parent, const char *description, int lower, int upper,
		bool horizontal, igdeProgressBarReference &progressBar );
	
	void ProgressBar( const char *description, int lower, int upper, bool horizontal,
		igdeProgressBarReference &progressBar );
	
	
	
	/**
	 * \brief Form line with label and combo box.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void ComboBox( igdeContainer &form, const char *label, const char *description,
		igdeComboBoxReference &comboBox, igdeComboBoxListener *listener );
	
	void ComboBox( igdeContainer &form, const char *label, bool editable, const char *description,
		igdeComboBoxReference &comboBox, igdeComboBoxListener *listener );
	
	/**
	 * \brief Add combo box.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void ComboBox( igdeContainer &parent, const char *description,
		igdeComboBoxReference &comboBox, igdeComboBoxListener *listener );
	
	void ComboBox( igdeContainer &parent, bool editable, const char *description,
		igdeComboBoxReference &comboBox, igdeComboBoxListener *listener );
	
	void ComboBox( igdeContainer &parent, int columns, int rows, const char *description,
		igdeComboBoxReference &comboBox, igdeComboBoxListener *listener );
	
	void ComboBox( igdeContainer &parent, int columns, int rows, bool editable,
		const char *description, igdeComboBoxReference &comboBox, igdeComboBoxListener *listener );
	
	void ComboBox( int columns, int rows, bool editable, const char *description,
		igdeComboBoxReference &comboBox, igdeComboBoxListener *listener );
	
	
	
	/**
	 * \brief Form line with label and combo box filter.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void ComboBoxFilter( igdeContainer &form, const char *label, const char *description,
		igdeComboBoxFilterReference &comboBox, igdeComboBoxListener *listener );
	
	void ComboBoxFilter( igdeContainer &form, const char *label, bool editable, const char *description,
		igdeComboBoxFilterReference &comboBox, igdeComboBoxListener *listener );
	
	/**
	 * \brief Add combo box filter.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void ComboBoxFilter( igdeContainer &parent, const char *description,
		igdeComboBoxFilterReference &comboBox, igdeComboBoxListener *listener );
	
	void ComboBoxFilter( igdeContainer &parent, bool editable, const char *description,
		igdeComboBoxFilterReference &comboBox, igdeComboBoxListener *listener );
	
	void ComboBoxFilter( igdeContainer &parent, int columns, int rows, const char *description,
		igdeComboBoxFilterReference &comboBox, igdeComboBoxListener *listener );
	
	void ComboBoxFilter( igdeContainer &parent, int columns, int rows, bool editable,
		const char *description, igdeComboBoxFilterReference &comboBox, igdeComboBoxListener *listener );
	
	void ComboBoxFilter( int columns, int rows, bool editable, const char *description,
		igdeComboBoxFilterReference &comboBox, igdeComboBoxListener *listener );
	
	
	
	/**
	 * \brief Form line with empty label check box,
	 */
	void CheckBox( igdeContainer &form, igdeCheckBoxReference &checkBox,
		igdeAction *action, bool takeOverAction = false );
	
	void CheckBox( igdeContainer &form, const char *text, const char *description,
		igdeCheckBoxReference &checkBox );
	
	void CheckBox( igdeCheckBoxReference &checkBox, igdeAction *action, bool takeOverAction = false );
	
	void CheckBox( const char *text, const char *description, igdeCheckBoxReference &checkBox );
	
	/**
	 * \brief Check box without form line.
	 */
	void CheckBoxOnly( igdeContainer &parent , igdeCheckBoxReference &checkBox,
		igdeAction *action, bool takeOverAction = false );
	
	void CheckBoxOnly( igdeContainer &parent, const char *text, const char *description,
		igdeCheckBoxReference &checkBox );
	
	
	
	/**
	 * \brief Form line with label and slider.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void Slider( igdeContainer &form, const char *label, const char *description,
		float lower, float upper, int precision, float tickSpacing,
		igdeSliderReference &slider, igdeSliderListener *listener );
	
	void Slider( igdeContainer &form, const char *label, const char *description,
		int lower, int upper, int tickSpacing, igdeSliderReference &slider,
		igdeSliderListener *listener );
	
	void Slider( igdeContainer &parent, const char *description,
		float lower, float upper, int precision, float tickSpacing,
		igdeSliderReference &slider, igdeSliderListener *listener );
	
	void Slider( igdeContainer &parent, const char *description,
		int lower, int upper, int tickSpacing, igdeSliderReference &slider,
		igdeSliderListener *listener );
	
	void Slider( const char *description, float lower, float upper, int precision,
		float tickSpacing, igdeSliderReference &slider, igdeSliderListener *listener );
	
	void Slider( const char *description, int lower, int upper, int tickSpacing,
		igdeSliderReference &slider, igdeSliderListener *listener );
	
	
	
	/**
	 * \brief Form line with label and color box.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void ColorBox( igdeContainer &form, const char *label, const char *description,
		igdeColorBoxReference &colorBox, igdeColorBoxListener *listener );
	
	/**
	 * \brief Add color box.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void ColorBox( igdeContainer &parent, const char *description,
		igdeColorBoxReference &colorBox, igdeColorBoxListener *listener );
	
	void ColorBox( const char *description, igdeColorBoxReference &colorBox,
		igdeColorBoxListener *listener );
	
	
	
	/**
	 * \brief Form line with label and edit point.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditPoint( igdeContainer &form, const char *label, const char *description,
		igdeEditPointReference &editPoint, igdeEditPointListener *listener );
	
	/**
	 * \brief Add edit point.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditPoint( igdeContainer &parent, const char *description,
		igdeEditPointReference &editPoint, igdeEditPointListener *listener );
	
	void EditPoint( const char *description, igdeEditPointReference &editPoint,
		igdeEditPointListener *listener );
	
	
	
	/**
	 * \brief Form line with label and edit point.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditPoint3( igdeContainer &form, const char *label, const char *description,
		igdeEditPoint3Reference &editPoint, igdeEditPoint3Listener *listener );
	
	/**
	 * \brief Add edit point.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditPoint3( igdeContainer &parent, const char *description,
		igdeEditPoint3Reference &editPoint, igdeEditPoint3Listener *listener );
	
	void EditPoint3( const char *description, igdeEditPoint3Reference &editPoint,
		igdeEditPoint3Listener *listener );
	
	
	
	/**
	 * \brief Form line with label and edit path.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditPath( igdeContainer &form, const char *label, const char *description,
		igdeEnvironment::eFilePatternListTypes resourceType,
		igdeEditPathReference &editPath, igdeEditPathListener *listener,
		bool useGameVFS = true );
	
	void EditPath( igdeContainer &form, const char *label, const char *description,
		const igdeFilePatternList &customFilePatterns, igdeEditPathReference &editPath,
		igdeEditPathListener *listener, bool useGameVFS = true );
	
	/**
	 * \brief Add edit path.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditPath( igdeContainer &parent, const char *description,
		igdeEnvironment::eFilePatternListTypes resourceType,
		igdeEditPathReference &editPath, igdeEditPathListener *listener,
		bool useGameVFS = true );
	
	void EditPath( igdeContainer &parent, const char *description,
		const igdeFilePatternList &customFilePatterns, igdeEditPathReference &editPath,
		igdeEditPathListener *listener, bool useGameVFS = true );
	
	void EditPath( const char *description, igdeEnvironment::eFilePatternListTypes resourceType,
		igdeEditPathReference &editPath, igdeEditPathListener *listener,
		bool useGameVFS = true );
	
	void EditPath( const char *description, const igdeFilePatternList &customFilePatterns,
		igdeEditPathReference &editPath, igdeEditPathListener *listener,
		bool useGameVFS = true );
	
	
	
	/**
	 * \brief Form line with label and edit directory.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditDirectory( igdeContainer &form, const char *label, const char *description,
		igdeEditDirectoryReference &editDirectory, igdeEditDirectoryListener *listener,
		bool useGameVFS = true );
	
	/**
	 * \brief Add edit directory.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditDirectory( igdeContainer &parent, const char *description,
		igdeEditDirectoryReference &editDirectory, igdeEditDirectoryListener *listener,
		bool useGameVFS = true );
	
	void EditDirectory( const char *description, igdeEditDirectoryReference &editDirectory,
		igdeEditDirectoryListener *listener, bool useGameVFS = true );
	
	
	
	/**
	 * \brief Form line with label and edit vector.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditDVector( igdeContainer &form, const char *label, const char *description,
		igdeEditDVectorReference &editDVector, igdeEditDVectorListener *listener );
	
	void EditDVector( igdeContainer &form, const char *label, const char *description,
		int columns, int precision, igdeEditDVectorReference &editDVector,
		igdeEditDVectorListener *listener );
	
	/**
	 * \brief Add edit vector.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditDVector( igdeContainer &parent, const char *description,
		igdeEditDVectorReference &editDVector, igdeEditDVectorListener *listener );
	
	void EditDVector( igdeContainer &parent, const char *description, int columns, int precision,
		igdeEditDVectorReference &editDVector, igdeEditDVectorListener *listener );
	
	void EditDVector( const char *description, igdeEditDVectorReference &editDVector,
		igdeEditDVectorListener *listener );
	
	void EditDVector( const char *description, int columns, int precision,
		igdeEditDVectorReference &editDVector, igdeEditDVectorListener *listener );
	
	
	
	/**
	 * \brief Form line with label and edit vector.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditVector( igdeContainer &form, const char *label, const char *description,
		igdeEditVectorReference &editVector, igdeEditVectorListener *listener );
	
	void EditVector( igdeContainer &form, const char *label, const char *description,
		int columns, int precision, igdeEditVectorReference &editVector,
		igdeEditVectorListener *listener );
	
	/**
	 * \brief Add edit vector.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditVector( igdeContainer &parent, const char *description,
		igdeEditVectorReference &editVector, igdeEditVectorListener *listener );
	
	void EditVector( igdeContainer &parent, const char *description, int columns, int precision,
		igdeEditVectorReference &editVector, igdeEditVectorListener *listener );
	
	void EditVector( const char *description, igdeEditVectorReference &editVector,
		igdeEditVectorListener *listener );
	
	void EditVector( const char *description, int columns, int precision,
		igdeEditVectorReference &editVector, igdeEditVectorListener *listener );
	
	
	
	/**
	 * \brief Form line with label and edit vector2.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditVector2( igdeContainer &form, const char *label, const char *description,
		igdeEditVector2Reference &editVector2, igdeEditVector2Listener *listener );
	
	void EditVector2( igdeContainer &form, const char *label, const char *description,
		int columns, int precision, igdeEditVector2Reference &editVector2,
		igdeEditVector2Listener *listener );
	
	/**
	 * \brief Add edit vector2.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditVector2( igdeContainer &parent, const char *description,
		igdeEditVector2Reference &editVector2, igdeEditVector2Listener *listener );
	
	void EditVector2( igdeContainer &parent, const char *description, int columns, int precision,
		igdeEditVector2Reference &editVector2, igdeEditVector2Listener *listener );
	
	void EditVector2( const char *description, igdeEditVector2Reference &editVector2,
		igdeEditVector2Listener *listener );
	
	void EditVector2( const char *description, int columns, int precision,
		igdeEditVector2Reference &editVector2, igdeEditVector2Listener *listener );
	
	
	
	/**
	 * \brief Form line with label and slider/text.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditSliderText( igdeContainer &form, const char *label, const char *description,
		float lower, float upper, int columns, int precision, float tickSpacing,
		igdeEditSliderTextReference &slider, igdeEditSliderTextListener *listener );
	
	void EditSliderText( igdeContainer &form, const char *label, const char *description,
		int lower, int upper, int columns, int tickSpacing, igdeEditSliderTextReference &slider,
		igdeEditSliderTextListener *listener );
	
	/**
	 * \brief Add slider/text.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditSliderText( igdeContainer &parent, const char *description,
		float lower, float upper, int columns, int precision, float tickSpacing,
		igdeEditSliderTextReference &slider, igdeEditSliderTextListener *listener );
	
	void EditSliderText( igdeContainer &parent, const char *description, int lower,
		int upper, int columns, int tickSpacing, igdeEditSliderTextReference &slider,
		igdeEditSliderTextListener *listener );
	
	void EditSliderText( const char *description, float lower, float upper, int columns,
		int precision, float tickSpacing, igdeEditSliderTextReference &slider,
		igdeEditSliderTextListener *listener );
	
	void EditSliderText( const char *description, int lower, int upper, int columns,
		int tickSpacing, igdeEditSliderTextReference &slider, igdeEditSliderTextListener *listener );
	
	
	
	/**
	 * \brief Add button.
	 */
	void Button( igdeContainer &parent, igdeAction *action, bool takeOverAction = false );
	
	void Button( igdeContainer &parent, igdeButtonReference &button, igdeAction *action,
		bool takeOverAction = false );
	
	void Button( igdeButtonReference &button, igdeAction *action, bool takeOverAction = false );
	
	
	
	/**
	 * \brief Add toggle button.
	 */
	void ToggleButton( igdeContainer &parent, igdeAction *action, bool takeOverAction = false );
	
	void ToggleButton( igdeContainer &parent, igdeToggleButtonReference &button,
		igdeAction *action, bool takeOverAction = false );
	
	void ToggleButton( igdeToggleButtonReference &button, igdeAction *action, bool takeOverAction = false );
	
	
	
	/**
	 * \brief Form line with label and list box without headers.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void ListBox( igdeContainer &form, const char *label, int rows, const char *description,
		igdeListBoxReference &listBox, igdeListBoxListener *listener );
	
	/** \brief Add List Box with no header. */
	void ListBox( igdeContainer &parent, int rows, const char *description,
		igdeListBoxReference &listBox, igdeListBoxListener *listener );
	
	void ListBox( int rows, const char *description, igdeListBoxReference &listBox,
		igdeListBoxListener *listener );
	
	
	
	/** \brief Add Icon List Box with. */
	void IconListBox( igdeContainer &parent, igdeIconListBoxReference &listBox,
		const sColumnHeader *headers, int headerCount, const char *description,
		igdeIconListBoxListener *listener );
	
	void IconListBox( igdeContainer &parent, igdeIconListBoxReference &listBox,
		const decPoint &minimumSize, const sColumnHeader *headers, int headerCount,
		const char *description, igdeIconListBoxListener *listener );
	
	void IconListBox( const sColumnHeader *headers, int headerCount, const char *description,
		igdeIconListBoxReference &listBox, igdeIconListBoxListener *listener );
	
	void IconListBox( const decPoint &minimumSize, const sColumnHeader *headers, int headerCount,
		const char *description, igdeIconListBoxReference &listBox, igdeIconListBoxListener *listener );
	
	
	
	/** \brief Add Tree List with no header. */
	void TreeList( igdeContainer &parent, igdeTreeListReference &treeList,
		int rows, const char *description, igdeTreeListListener *listener );
	
	void TreeList( int rows, const char *description, igdeTreeListReference &treeList,
		igdeTreeListListener *listener );
	
	
	
	/** \brief Add label. */
	void Label( igdeContainer &parent, const char *text );
	
	void Label( igdeContainer &parent, const char *text, const char *description );
	
	void Label( igdeContainer &parent, const char *text, const char *description, int alignment );
	
	/** \brief Add label. */
	void Label( igdeContainer &parent, igdeLabelReference &label, const char *text );
	
	void Label( igdeContainer &parent, igdeLabelReference &label, const char *text,
		const char *description, int alignment );
	
	void Label( igdeLabelReference &label, const char *text );
	
	void Label( igdeLabelReference &label, const char *text, const char *description, int alignment );
	
	
	
	/**
	 * \brief Add scroll bar.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void ScrollBar( igdeContainer &parent, bool horizontal, int lower, int upper, int pageSize,
		int value, igdeScrollBarReference &scrollBar, igdeScrollBarListener *listener );
	
	void ScrollBar( bool horizontal, int lower, int upper, int pageSize,
		int value, igdeScrollBarReference &scrollBar, igdeScrollBarListener *listener );
	
	
	
	/**
	 * \brief Add view curve bezier.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void ViewCurveBezier( igdeContainer &parent, igdeViewCurveBezierReference &viewCurveBezier,
		igdeViewCurveBezierListener *listener );
	
	void ViewCurveBezier( igdeViewCurveBezierReference &viewCurveBezier,
		igdeViewCurveBezierListener *listener );
	
	
	
	/** \brief Add horizontal separator. */
	void Separator( igdeContainer &parent, bool horizontal = true );
	
	
	
	/** \brief Add fixed size spacer. */
	void Spacer( igdeContainer &parent, const decPoint &size );
	
	void Spacer( igdeContainer &parent, const decPoint &size, igdeSpacerReference &spacer );
	
	
	/**
	 * \brief Add sky wrapper properties panel.
	 */
	void WPSky( igdeContainer &parent, igdeAction *action, const char *title,
		bool stretchLast = false, bool collapsed = false, bool takeOverAction = false );
	
	void WPSky( igdeContainer &parent, igdeWPSkyReference &panel, igdeAction *action,
		const char *title, bool stretchLast = false, bool collapsed = false,
		bool takeOverAction = false );
	
	void WPSky( igdeWPSkyReference &panel, igdeAction *action, bool takeOverAction = false );
	
	
	
	/**
	 * \brief Add camera wrapper properties panel.
	 */
	void WPCamera( igdeContainer &parent, igdeAction *action, const char *title,
		bool stretchLast = false, bool collapsed = false, bool takeOverAction = false );
	
	void WPCamera( igdeContainer &parent, igdeWPCameraReference &panel, igdeAction *action,
		const char *title, bool stretchLast = false, bool collapsed = false,
		bool takeOverAction = false );
	
	void WPCamera( igdeWPCameraReference &panel, igdeAction *action, bool takeOverAction = false );
	
	
	
	/**
	 * \brief Add object wrapper properties panel.
	 */
	void WPWObject( igdeContainer &parent, igdeAction *action, const char *title,
		bool stretchLast = false, bool collapsed = false, bool takeOverAction = false );
	
	void WPWObject( igdeContainer &parent, igdeWPWObjectReference &panel, igdeAction *action,
		const char *title, bool stretchLast = false, bool collapsed = false,
		bool takeOverAction = false );
	
	void WPWObject( igdeWPWObjectReference &panel, igdeAction *action, bool takeOverAction = false );
	
	
	
	/** \brief Add edit tags panel. */
	void EditTags( igdeEditTagsReference &editTags, igdeAction *action, bool takeOverAction = false );
	
	void EditTags( igdeContainer &parent, igdeEditTagsReference &editTags,
		igdeAction *action, bool takeOverAction = false );
	
	
	
	/** \brief Add toggle tags panel. */
	void ToggleTags( igdeContainer &parent, igdeToggleTagsReference &toggleTags,
		igdeAction *action, bool takeOverAction = false );
	
	void ToggleTags( igdeToggleTagsReference &toggleTags, igdeAction *action, bool takeOverAction = false );
	
	
	
	/** \brief Add edit property value widget. */
	void EditPropertyValue( igdeContainer &parent, igdeEditPropertyValueReference &widget,
		igdeEditPropertyValueListener *listener );
	
	void EditPropertyValue( igdeEditPropertyValueReference &widget, igdeEditPropertyValueListener *listener );
	
	
	
	/**
	 * \brief Add trigger table properties panel.
	 */
	void WPTriggerTable( igdeContainer &parent, igdeAction *action, const char *title,
		bool stretchLast = false, bool collapsed = false, bool takeOverAction = false );
	
	void WPTriggerTable( igdeContainer &parent, igdeWPTriggerTableReference &panel,
		igdeAction *action, const char *title, bool stretchLast = false, bool collapsed = false,
		bool takeOverAction = false );
	
	void WPTriggerTable( igdeWPTriggerTableReference &panel, igdeAction *action, bool takeOverAction = false );
	
	
	
	/** \brief Add menu command. */
	void MenuCommand( igdeMenuCascade &menu, igdeAction *action, bool takeOverAction = false );
	
	/** \brief Add menu check. */
	void MenuCheck( igdeMenuCascade &menu, igdeAction *action, bool takeOverAction = false );
	
	/** \brief Add menu option. */
	void MenuOption( igdeMenuCascade &menu, igdeAction *action, bool takeOverAction = false );
	
	/** \brief Add menu separator. */
	void MenuSeparator( igdeMenuCascade &menu );
	
	/** \brief Add menu recent files sub menu. */
	void MenuRecentFiles( igdeMenuCascade &menu, igdeRecentFiles &recentFiles,
		const char *text = "Open Recent File" );
	
	
	
	/** \brief Add toolbar button. */
	void ToolBarButton( igdeToolBar &toolBar, igdeAction *action, bool takeOverAction = false );
	
	/** \brief Add toolbar toggle button. */
	void ToolBarToggleButton( igdeToolBar &toolBar, igdeAction *action, bool takeOverAction = false );
	
	/** \brief Add toolbar separator. */
	void ToolBarSeparator( igdeToolBar &toolBar );
	
	
	
	/**
	 * \brief Format file size on 1024 basis with 1 digit precision.
	 * 
	 * Formats text for human readable use. The output is of one of these forms:
	 * - "X B": less than 1024 bytes
	 * - "X.Y kB": less than 1024 kilo-bytes
	 * - "X.Y MB": less than 1024 mega-bytes
	 * - "X.Y GB": otherwise
	 */
	static decString FormatSize1024( uint64_t size );
	
	/**
	 * \brief Format file size on 1000 basis with 1 digit precision.
	 * 
	 * Formats text for human readable use. The output is of one of these forms:
	 * - "X B": less than 1000 bytes
	 * - "X.Y kB": less than 1000 kilo-bytes
	 * - "X.Y MB": less than 1000 mega-bytes
	 * - "X.Y GB": otherwise
	 */
	static decString FormatSize1000( uint64_t size );
	
	/**
	 * \brief Format file size using tousand separators.
	 * 
	 * Formats text for human readable use in the form "X'YYY'ZZZ".
	 */
	static decString FormatSizeTousand( uint64_t size );
	/*@}*/
};

#endif
