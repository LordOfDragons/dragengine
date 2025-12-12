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

#ifndef _IGDEUIHELPER_H_
#define _IGDEUIHELPER_H_

#include <stdint.h>

#include "igdeButton.h"
#include "igdeCheckBox.h"
#include "igdeColorBox.h"
#include "igdeComboBox.h"
#include "igdeComboBoxFilter.h"
#include "igdeContainer.h"
#include "igdeIconListBox.h"
#include "igdeLabel.h"
#include "igdeListBox.h"
#include "igdeProgressBar.h"
#include "igdeScrollBar.h"
#include "igdeSlider.h"
#include "igdeSpacer.h"
#include "igdeSpinTextField.h"
#include "igdeTextArea.h"
#include "igdeTextField.h"
#include "igdeToggleButton.h"
#include "igdeTreeList.h"
#include "igdeWidget.h"
#include "composed/igdeEditDVector.h"
#include "composed/igdeEditDirectory.h"
#include "composed/igdeEditPath.h"
#include "composed/igdeEditPoint.h"
#include "composed/igdeEditPoint3.h"
#include "composed/igdeEditPropertyValue.h"
#include "composed/igdeEditSliderText.h"
#include "composed/igdeEditTags.h"
#include "composed/igdeEditVector.h"
#include "composed/igdeEditVector2.h"
#include "composed/igdeToggleTags.h"
#include "curveedit/igdeViewCurveBezier.h"
#include "event/igdeAction.h"
#include "layout/igdeContainerBorder.h"
#include "layout/igdeContainerScroll.h"
#include "properties/igdeWPCamera.h"
#include "properties/igdeWPSky.h"
#include "properties/igdeWPTriggerTable.h"
#include "properties/igdeWPWObject.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class igdeAction;
class igdeColorBoxListener;
class igdeComboBoxListener;
class igdeContainer;
class igdeEditDVectorListener;
class igdeEditDirectoryListener;
class igdeEditPathListener;
class igdeEditPoint3Listener;
class igdeEditPointListener;
class igdeEditPropertyValueListener;
class igdeEditSliderTextListener;
class igdeEditVector2Listener;
class igdeEditVectorListener;
class igdeEnvironment;
class igdeIcon;
class igdeIconListBoxListener;
class igdeListBoxListener;
class igdeMenuCascade;
class igdeRecentFiles;
class igdeScrollBarListener;
class igdeSliderListener;
class igdeSpinTextFieldListener;
class igdeTextAreaListener;
class igdeTextFieldListener;
class igdeToolBar;
class igdeTreeListListener;
class igdeViewCurveBezierListener;
class igdeWidget;


/**
 * \brief UI Helpers.
 * 
 * Simplifies common tasks.
 */
class DE_DLL_EXPORT igdeUIHelper{
public:
	/** \brief Column header configuration. */
	struct DE_DLL_EXPORT sColumnHeader{
		/** \brief Title. */
		const char *title;
		
		/** \brief Icon. */
		igdeIcon *icon;
		
		/** \brief Size in pixels. */
		int size;
		
		/** \brief Init struct. */
		sColumnHeader(const char *title, igdeIcon *icon, int size);
	};
	
	
	
private:
	igdeEnvironment &pEnvironment;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create UI Helper. */
	igdeUIHelper(igdeEnvironment &environment);
	
	/** \brief Create copy of UI Helper. */
	igdeUIHelper(const igdeUIHelper &helper);
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
	void SidePanel(igdeContainerScroll::Ref &scroll, igdeContainer::Ref &panel,
		bool stretchLast = false, int spacing = 0);
	
	void SidePanel(igdeContainer &parent, igdeContainer::Ref &panel,
		bool stretchLast = false, int spacing = 0);
	
	
	
	/** \brief Create igdeGroupBox with an igdeContainerForm layout. */
	void GroupBox(igdeContainer &parent, igdeContainer::Ref &groupBox,
		const char *title, bool collapsed = false) const;
	
	/** \brief Create non-collapsable igdeGroupBox with an igdeContainerForm layout. */
	void GroupBoxStatic(igdeContainer &parent, igdeContainer::Ref &groupBox, const char *title) const;
	
	/** \brief Create igdeGroupBox with an igdeContainerFlow layout. */
	void GroupBoxFlow(igdeContainer &parent, igdeContainer::Ref &groupBox,
		const char *title, bool stretchLast = false, bool collapsed = false) const;
	
	/** \brief Create igdeGroupBox with an igdeContainerBorder layout. */
	void GroupBoxBorder(igdeContainer &parent, igdeContainerBorder::Ref &groupBox,
		const char *title, bool collapsed = false) const;
	
	/** \brief Create non-collapsable igdeGroupBox with an igdeContainerFlow layout. */
	void GroupBoxStaticFlow(igdeContainer &parent, igdeContainer::Ref &groupBox,
		const char *title, bool stretchLast = false) const;
	
	/** \brief Create non-collapsable igdeGroupBox with an igdeContainerBorder layout. */
	void GroupBoxStaticBorder(igdeContainer &parent, igdeContainerBorder::Ref &groupBox,
		const char *title, bool stretch = false) const;
	
	
	
	/** \brief Form line with label and one widget, */
	void FormLine(igdeContainer &form, const char *label, const char *description,
		igdeWidget *widget);
	
	void FormLine(igdeContainer &form, const char *label, const char *description,
		igdeWidget *widget, int labelAlignment);
	
	/**
	 * \brief Form line with label and container for additing multiple widgets.
	 * 
	 * Container is igdeContainerBox equally stretching widgets.
	 */
	void FormLine(igdeContainer &form, const char *label, const char *description,
		igdeContainer::Ref &line);
	
	void FormLine(igdeContainer &form, const char *label, const char *description,
		igdeContainer::Ref &line, int labelAlignment);
	
	/**
	 * \brief Form line with label and container for additing multiple widgets.
	 * 
	 * Container is igdeContainerFlow is used instead set to stretch the first widget.
	 */
	void FormLineStretchFirst(igdeContainer &form, const char *label,
		const char *description, igdeContainer::Ref &line);
	
	void FormLineStretchFirst(igdeContainer &form, const char *label,
		const char *description, igdeContainer::Ref &line, int labelAlignment);
	
	/**
	 * \brief Form line with label and container flowing downwards.
	 * 
	 * Container is igdeContainerFlow along y axis.
	 */
	void FormLineDownFlow(igdeContainer &form, const char *label,
		const char *description, igdeContainer::Ref &line);
	
	void FormLineDownFlow(igdeContainer &form, const char *label,
		const char *description, igdeContainer::Ref &line, int labelAlignment);
	
	
	
	/**
	 * \brief Form line with label and text field.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditString(igdeContainer &form, const char *label, const char *description,
		igdeTextField::Ref &textField, igdeTextFieldListener *listener);
	
	void EditString(igdeContainer &form, const char *label, const char *description,
		int rows, igdeTextField::Ref &textField, igdeTextFieldListener *listener);
	
	/**
	 * \brief Add text field.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditString(igdeContainer &parent, const char *description,
		igdeTextField::Ref &textField, igdeTextFieldListener *listener);
	
	void EditString(igdeContainer &parent, const char *description, int rows,
		igdeTextField::Ref &textField, igdeTextFieldListener *listener);
	
	void EditString(const char *description, igdeTextField::Ref &textField,
		igdeTextFieldListener *listener);
	
	void EditString(const char *description, int rows, igdeTextField::Ref &textField,
		igdeTextFieldListener *listener);
	
	
	
	/**
	 * \brief Form line with label and text area.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditString(igdeContainer &form, const char *label, const char *description,
		igdeTextArea::Ref &textArea, int rows, igdeTextAreaListener *listener);
	
	void EditString(igdeContainer &form, const char *label, const char *description,
		igdeTextArea::Ref &textArea, int columns, int rows, igdeTextAreaListener *listener);
	
	/**
	 * \brief Add text area.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditString(igdeContainer &parent, const char *description,
		igdeTextArea::Ref &textArea, int rows, igdeTextAreaListener *listener);
	
	void EditString(igdeContainer &parent, const char *description,
		igdeTextArea::Ref &textArea, int columns, int rows, igdeTextAreaListener *listener);
	
	void EditString(const char *description, int rows, igdeTextArea::Ref &textArea,
		igdeTextAreaListener *listener);
	
	void EditString(const char *description, int columns, int rows,
		igdeTextArea::Ref &textArea, igdeTextAreaListener *listener);
	
	
	
	/**
	 * \brief Form line with label and text field.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditFloat(igdeContainer &form, const char *label, const char *description,
		igdeTextField::Ref &textField, igdeTextFieldListener *listener);
	
	void EditFloat(igdeContainer &form, const char *label, const char *description,
		int columns, int precision, igdeTextField::Ref &textField,
		igdeTextFieldListener *listener);
	
	/**
	 * \brief Add text field.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditFloat(igdeContainer &parent, const char *description,
		igdeTextField::Ref &textField, igdeTextFieldListener *listener);
	
	void EditFloat(igdeContainer &parent, const char *description, int columns, int precision,
		igdeTextField::Ref &textField, igdeTextFieldListener *listener);
	
	void EditFloat(const char *description, igdeTextField::Ref &textField,
		igdeTextFieldListener *listener);
	
	void EditFloat(const char *description, int columns, int precision,
		igdeTextField::Ref &textField, igdeTextFieldListener *listener);
	
	
	
	/**
	 * \brief Form line with label and text field.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditInteger(igdeContainer &form, const char *label, const char *description,
		igdeTextField::Ref &textField, igdeTextFieldListener *listener);
	
	void EditInteger(igdeContainer &form, const char *label, const char *description,
		int columns, igdeTextField::Ref &textField, igdeTextFieldListener *listener);
	
	/**
	 * \brief Add text field.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditInteger(igdeContainer &parent, const char *description,
		igdeTextField::Ref &textField, igdeTextFieldListener *listener);
	
	void EditInteger(igdeContainer &parent, const char *description, int columns,
		igdeTextField::Ref &textField, igdeTextFieldListener *listener);
	
	void EditInteger(const char *description, igdeTextField::Ref &textField,
		igdeTextFieldListener *listener);
	
	void EditInteger(const char *description, int columns, igdeTextField::Ref &textField,
		igdeTextFieldListener *listener);
	
	
	
	/**
	 * \brief Form line with label and spin text field.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditSpinInteger(igdeContainer &form, const char *label, const char *description,
		int lower, int upper, igdeSpinTextField::Ref &spinTextField,
		igdeSpinTextFieldListener *listener);
	
	void EditSpinInteger(igdeContainer &parent, const char *description, int lower, int upper,
		igdeSpinTextField::Ref &spinTextField, igdeSpinTextFieldListener *listener);
	
	void EditSpinInteger(const char *description, int lower, int upper,
		igdeSpinTextField::Ref &spinTextField, igdeSpinTextFieldListener *listener);
	
	
	
	/** \brief Form line with label and progress bar. */
	void ProgressBar(igdeContainer &form, const char *label, const char *description,
		int lower, int upper, bool horizontal, igdeProgressBar::Ref &progressBar);
	
	void ProgressBar(igdeContainer &parent, const char *description, int lower, int upper,
		bool horizontal, igdeProgressBar::Ref &progressBar);
	
	void ProgressBar(const char *description, int lower, int upper, bool horizontal,
		igdeProgressBar::Ref &progressBar);
	
	
	
	/**
	 * \brief Form line with label and combo box.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void ComboBox(igdeContainer &form, const char *label, const char *description,
		igdeComboBox::Ref &comboBox, igdeComboBoxListener *listener);
	
	void ComboBox(igdeContainer &form, const char *label, bool editable, const char *description,
		igdeComboBox::Ref &comboBox, igdeComboBoxListener *listener);
	
	void ComboBox(igdeContainer &form, const char *label, int columns, bool editable,
		const char *description, igdeComboBox::Ref &comboBox, igdeComboBoxListener *listener);
	
	void ComboBox(igdeContainer &form, const char *label, int columns, int rows, bool editable,
		const char *description, igdeComboBox::Ref &comboBox, igdeComboBoxListener *listener);
	
	/**
	 * \brief Add combo box.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void ComboBox(igdeContainer &parent, const char *description,
		igdeComboBox::Ref &comboBox, igdeComboBoxListener *listener);
	
	void ComboBox(igdeContainer &parent, bool editable, const char *description,
		igdeComboBox::Ref &comboBox, igdeComboBoxListener *listener);
	
	void ComboBox(igdeContainer &parent, int columns, int rows, const char *description,
		igdeComboBox::Ref &comboBox, igdeComboBoxListener *listener);
	
	void ComboBox(igdeContainer &parent, int columns, int rows, bool editable,
		const char *description, igdeComboBox::Ref &comboBox, igdeComboBoxListener *listener);
	
	void ComboBox(int columns, int rows, bool editable, const char *description,
		igdeComboBox::Ref &comboBox, igdeComboBoxListener *listener);
	
	
	
	/**
	 * \brief Form line with label and combo box filter.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void ComboBoxFilter(igdeContainer &form, const char *label, const char *description,
		igdeComboBoxFilter::Ref &comboBox, igdeComboBoxListener *listener);
	
	void ComboBoxFilter(igdeContainer &form, const char *label, bool editable, const char *description,
		igdeComboBoxFilter::Ref &comboBox, igdeComboBoxListener *listener);
	
	void ComboBoxFilter(igdeContainer &form, const char *label, int columns, bool editable,
		const char *description, igdeComboBoxFilter::Ref &comboBox, igdeComboBoxListener *listener);
	
	void ComboBoxFilter(igdeContainer &form, const char *label, int columns, int rows, bool editable,
		const char *description, igdeComboBoxFilter::Ref &comboBox, igdeComboBoxListener *listener);
	
	/**
	 * \brief Add combo box filter.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void ComboBoxFilter(igdeContainer &parent, const char *description,
		igdeComboBoxFilter::Ref &comboBox, igdeComboBoxListener *listener);
	
	void ComboBoxFilter(igdeContainer &parent, bool editable, const char *description,
		igdeComboBoxFilter::Ref &comboBox, igdeComboBoxListener *listener);
	
	void ComboBoxFilter(igdeContainer &parent, int columns, int rows, const char *description,
		igdeComboBoxFilter::Ref &comboBox, igdeComboBoxListener *listener);
	
	void ComboBoxFilter(igdeContainer &parent, int columns, int rows, bool editable,
		const char *description, igdeComboBoxFilter::Ref &comboBox, igdeComboBoxListener *listener);
	
	void ComboBoxFilter(int columns, int rows, bool editable, const char *description,
		igdeComboBoxFilter::Ref &comboBox, igdeComboBoxListener *listener);
	
	
	
	/**
	 * \brief Form line with empty label check box,
	 */
	void CheckBox(igdeContainer &form, igdeCheckBox::Ref &checkBox,
		igdeAction *action, bool takeOverAction = false);
	
	void CheckBox(igdeContainer &form, const char *text, const char *description,
		igdeCheckBox::Ref &checkBox);
	
	void CheckBox(igdeCheckBox::Ref &checkBox, igdeAction *action, bool takeOverAction = false);
	
	void CheckBox(const char *text, const char *description, igdeCheckBox::Ref &checkBox);
	
	/**
	 * \brief Check box without form line.
	 */
	void CheckBoxOnly(igdeContainer &parent , igdeCheckBox::Ref &checkBox,
		igdeAction *action, bool takeOverAction = false);
	
	void CheckBoxOnly(igdeContainer &parent, const char *text, const char *description,
		igdeCheckBox::Ref &checkBox);
	
	
	
	/**
	 * \brief Form line with label and slider.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void Slider(igdeContainer &form, const char *label, const char *description,
		float lower, float upper, int precision, float tickSpacing,
		igdeSlider::Ref &slider, igdeSliderListener *listener);
	
	void Slider(igdeContainer &form, const char *label, const char *description,
		int lower, int upper, int tickSpacing, igdeSlider::Ref &slider,
		igdeSliderListener *listener);
	
	void Slider(igdeContainer &parent, const char *description,
		float lower, float upper, int precision, float tickSpacing,
		igdeSlider::Ref &slider, igdeSliderListener *listener);
	
	void Slider(igdeContainer &parent, const char *description,
		int lower, int upper, int tickSpacing, igdeSlider::Ref &slider,
		igdeSliderListener *listener);
	
	void Slider(const char *description, float lower, float upper, int precision,
		float tickSpacing, igdeSlider::Ref &slider, igdeSliderListener *listener);
	
	void Slider(const char *description, int lower, int upper, int tickSpacing,
		igdeSlider::Ref &slider, igdeSliderListener *listener);
	
	
	
	/**
	 * \brief Form line with label and color box.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void ColorBox(igdeContainer &form, const char *label, const char *description,
		igdeColorBox::Ref &colorBox, igdeColorBoxListener *listener);
	
	/**
	 * \brief Add color box.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void ColorBox(igdeContainer &parent, const char *description,
		igdeColorBox::Ref &colorBox, igdeColorBoxListener *listener);
	
	void ColorBox(const char *description, igdeColorBox::Ref &colorBox,
		igdeColorBoxListener *listener);
	
	
	
	/**
	 * \brief Form line with label and edit point.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditPoint(igdeContainer &form, const char *label, const char *description,
		igdeEditPoint::Ref &editPoint, igdeEditPointListener *listener);
	
	/**
	 * \brief Add edit point.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditPoint(igdeContainer &parent, const char *description,
		igdeEditPoint::Ref &editPoint, igdeEditPointListener *listener);
	
	void EditPoint(const char *description, igdeEditPoint::Ref &editPoint,
		igdeEditPointListener *listener);
	
	
	
	/**
	 * \brief Form line with label and edit point.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditPoint3(igdeContainer &form, const char *label, const char *description,
		igdeEditPoint3::Ref &editPoint, igdeEditPoint3Listener *listener);
	
	/**
	 * \brief Add edit point.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditPoint3(igdeContainer &parent, const char *description,
		igdeEditPoint3::Ref &editPoint, igdeEditPoint3Listener *listener);
	
	void EditPoint3(const char *description, igdeEditPoint3::Ref &editPoint,
		igdeEditPoint3Listener *listener);
	
	
	
	/**
	 * \brief Form line with label and edit path.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditPath(igdeContainer &form, const char *label, const char *description,
		igdeEnvironment::eFilePatternListTypes resourceType,
		igdeEditPath::Ref &editPath, igdeEditPathListener *listener,
		bool useGameVFS = true);
	
	void EditPath(igdeContainer &form, const char *label, const char *description,
		const igdeFilePatternList &customFilePatterns, igdeEditPath::Ref &editPath,
		igdeEditPathListener *listener, bool useGameVFS = true);
	
	/**
	 * \brief Add edit path.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditPath(igdeContainer &parent, const char *description,
		igdeEnvironment::eFilePatternListTypes resourceType,
		igdeEditPath::Ref &editPath, igdeEditPathListener *listener,
		bool useGameVFS = true);
	
	void EditPath(igdeContainer &parent, const char *description,
		const igdeFilePatternList &customFilePatterns, igdeEditPath::Ref &editPath,
		igdeEditPathListener *listener, bool useGameVFS = true);
	
	void EditPath(const char *description, igdeEnvironment::eFilePatternListTypes resourceType,
		igdeEditPath::Ref &editPath, igdeEditPathListener *listener,
		bool useGameVFS = true);
	
	void EditPath(const char *description, const igdeFilePatternList &customFilePatterns,
		igdeEditPath::Ref &editPath, igdeEditPathListener *listener,
		bool useGameVFS = true);
	
	
	
	/**
	 * \brief Form line with label and edit directory.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditDirectory(igdeContainer &form, const char *label, const char *description,
		igdeEditDirectory::Ref &editDirectory, igdeEditDirectoryListener *listener,
		bool useGameVFS = true);
	
	/**
	 * \brief Add edit directory.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditDirectory(igdeContainer &parent, const char *description,
		igdeEditDirectory::Ref &editDirectory, igdeEditDirectoryListener *listener,
		bool useGameVFS = true);
	
	void EditDirectory(const char *description, igdeEditDirectory::Ref &editDirectory,
		igdeEditDirectoryListener *listener, bool useGameVFS = true);
	
	
	
	/**
	 * \brief Form line with label and edit vector.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditDVector(igdeContainer &form, const char *label, const char *description,
		igdeEditDVector::Ref &editDVector, igdeEditDVectorListener *listener);
	
	void EditDVector(igdeContainer &form, const char *label, const char *description,
		int columns, int precision, igdeEditDVector::Ref &editDVector,
		igdeEditDVectorListener *listener);
	
	/**
	 * \brief Add edit vector.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditDVector(igdeContainer &parent, const char *description,
		igdeEditDVector::Ref &editDVector, igdeEditDVectorListener *listener);
	
	void EditDVector(igdeContainer &parent, const char *description, int columns, int precision,
		igdeEditDVector::Ref &editDVector, igdeEditDVectorListener *listener);
	
	void EditDVector(const char *description, igdeEditDVector::Ref &editDVector,
		igdeEditDVectorListener *listener);
	
	void EditDVector(const char *description, int columns, int precision,
		igdeEditDVector::Ref &editDVector, igdeEditDVectorListener *listener);
	
	
	
	/**
	 * \brief Form line with label and edit vector.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditVector(igdeContainer &form, const char *label, const char *description,
		igdeEditVector::Ref &editVector, igdeEditVectorListener *listener);
	
	void EditVector(igdeContainer &form, const char *label, const char *description,
		int columns, int precision, igdeEditVector::Ref &editVector,
		igdeEditVectorListener *listener);
	
	/**
	 * \brief Add edit vector.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditVector(igdeContainer &parent, const char *description,
		igdeEditVector::Ref &editVector, igdeEditVectorListener *listener);
	
	void EditVector(igdeContainer &parent, const char *description, int columns, int precision,
		igdeEditVector::Ref &editVector, igdeEditVectorListener *listener);
	
	void EditVector(const char *description, igdeEditVector::Ref &editVector,
		igdeEditVectorListener *listener);
	
	void EditVector(const char *description, int columns, int precision,
		igdeEditVector::Ref &editVector, igdeEditVectorListener *listener);
	
	
	
	/**
	 * \brief Form line with label and edit vector2.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditVector2(igdeContainer &form, const char *label, const char *description,
		igdeEditVector2::Ref &editVector2, igdeEditVector2Listener *listener);
	
	void EditVector2(igdeContainer &form, const char *label, const char *description,
		int columns, int precision, igdeEditVector2::Ref &editVector2,
		igdeEditVector2Listener *listener);
	
	/**
	 * \brief Add edit vector2.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditVector2(igdeContainer &parent, const char *description,
		igdeEditVector2::Ref &editVector2, igdeEditVector2Listener *listener);
	
	void EditVector2(igdeContainer &parent, const char *description, int columns, int precision,
		igdeEditVector2::Ref &editVector2, igdeEditVector2Listener *listener);
	
	void EditVector2(const char *description, igdeEditVector2::Ref &editVector2,
		igdeEditVector2Listener *listener);
	
	void EditVector2(const char *description, int columns, int precision,
		igdeEditVector2::Ref &editVector2, igdeEditVector2Listener *listener);
	
	
	
	/**
	 * \brief Form line with label and slider/text.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditSliderText(igdeContainer &form, const char *label, const char *description,
		float lower, float upper, int columns, int precision, float tickSpacing,
		igdeEditSliderText::Ref &slider, igdeEditSliderTextListener *listener);
	
	void EditSliderText(igdeContainer &form, const char *label, const char *description,
		int lower, int upper, int columns, int tickSpacing, igdeEditSliderText::Ref &slider,
		igdeEditSliderTextListener *listener);
	
	/**
	 * \brief Add slider/text.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void EditSliderText(igdeContainer &parent, const char *description,
		float lower, float upper, int columns, int precision, float tickSpacing,
		igdeEditSliderText::Ref &slider, igdeEditSliderTextListener *listener);
	
	void EditSliderText(igdeContainer &parent, const char *description, int lower,
		int upper, int columns, int tickSpacing, igdeEditSliderText::Ref &slider,
		igdeEditSliderTextListener *listener);
	
	void EditSliderText(const char *description, float lower, float upper, int columns,
		int precision, float tickSpacing, igdeEditSliderText::Ref &slider,
		igdeEditSliderTextListener *listener);
	
	void EditSliderText(const char *description, int lower, int upper, int columns,
		int tickSpacing, igdeEditSliderText::Ref &slider, igdeEditSliderTextListener *listener);
	
	
	
	/**
	 * \brief Add button.
	 */
	void Button(igdeContainer &parent, igdeAction *action, bool takeOverAction);
	
	void Button(igdeContainer &parent, igdeButton::Ref &button, igdeAction *action,
		bool takeOverAction);
	
	void Button(igdeButton::Ref &button, igdeAction *action, bool takeOverAction);
	
	void Button(igdeContainer &parent, const igdeAction::Ref &action);
	void Button(igdeContainer &parent, igdeButton::Ref &button, const igdeAction::Ref &action);
	void Button(igdeButton::Ref &button, const igdeAction::Ref &action);
	
	
	
	/**
	 * \brief Add toggle button.
	 */
	void ToggleButton(igdeContainer &parent, igdeAction *action, bool takeOverAction = false);
	
	void ToggleButton(igdeContainer &parent, igdeToggleButton::Ref &button,
		igdeAction *action, bool takeOverAction = false);
	
	void ToggleButton(igdeToggleButton::Ref &button, igdeAction *action, bool takeOverAction = false);
	
	
	
	/**
	 * \brief Form line with label and list box without headers.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void ListBox(igdeContainer &form, const char *label, int rows, const char *description,
		igdeListBox::Ref &listBox, igdeListBoxListener *listener);
	
	/** \brief Add List Box with no header. */
	void ListBox(igdeContainer &parent, int rows, const char *description,
		igdeListBox::Ref &listBox, igdeListBoxListener *listener);
	
	void ListBox(int rows, const char *description, igdeListBox::Ref &listBox,
		igdeListBoxListener *listener);
	
	
	
	/** \brief Add Icon List Box with. */
	void IconListBox(igdeContainer &parent, igdeIconListBox::Ref &listBox,
		const sColumnHeader *headers, int headerCount, const char *description,
		igdeIconListBoxListener *listener);
	
	void IconListBox(igdeContainer &parent, igdeIconListBox::Ref &listBox,
		const decPoint &minimumSize, const sColumnHeader *headers, int headerCount,
		const char *description, igdeIconListBoxListener *listener);
	
	void IconListBox(const sColumnHeader *headers, int headerCount, const char *description,
		igdeIconListBox::Ref &listBox, igdeIconListBoxListener *listener);
	
	void IconListBox(const decPoint &minimumSize, const sColumnHeader *headers, int headerCount,
		const char *description, igdeIconListBox::Ref &listBox, igdeIconListBoxListener *listener);
	
	
	
	/** \brief Add Tree List with no header. */
	void TreeList(igdeContainer &parent, igdeTreeList::Ref &treeList,
		int rows, const char *description, igdeTreeListListener *listener);
	
	void TreeList(int rows, const char *description, igdeTreeList::Ref &treeList,
		igdeTreeListListener *listener);
	
	
	
	/** \brief Add label. */
	void Label(igdeContainer &parent, const char *text);
	
	void Label(igdeContainer &parent, const char *text, const char *description);
	
	void Label(igdeContainer &parent, const char *text, const char *description, int alignment);
	
	/** \brief Add label. */
	void Label(igdeContainer &parent, igdeLabel::Ref &label, const char *text);
	
	void Label(igdeContainer &parent, igdeLabel::Ref &label, const char *text,
		const char *description, int alignment);
	
	void Label(igdeLabel::Ref &label, const char *text);
	
	void Label(igdeLabel::Ref &label, const char *text, const char *description, int alignment);
	
	
	
	/**
	 * \brief Add scroll bar.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void ScrollBar(igdeContainer &parent, bool horizontal, int lower, int upper, int pageSize,
		int value, igdeScrollBar::Ref &scrollBar, igdeScrollBarListener *listener);
	
	void ScrollBar(bool horizontal, int lower, int upper, int pageSize,
		int value, igdeScrollBar::Ref &scrollBar, igdeScrollBarListener *listener);
	
	
	
	/**
	 * \brief Add view curve bezier.
	 * 
	 * \warning Listener reference is taken over. Only use with new created listeners.
	 */
	void ViewCurveBezier(igdeContainer &parent, igdeViewCurveBezier::Ref &viewCurveBezier,
		igdeViewCurveBezierListener *listener);
	
	void ViewCurveBezier(igdeViewCurveBezier::Ref &viewCurveBezier,
		igdeViewCurveBezierListener *listener);
	
	
	
	/** \brief Add horizontal separator. */
	void Separator(igdeContainer &parent, bool horizontal = true);
	
	
	
	/** \brief Add fixed size spacer. */
	void Spacer(igdeContainer &parent, const decPoint &size);
	
	void Spacer(igdeContainer &parent, const decPoint &size, igdeSpacer::Ref &spacer);
	
	
	/**
	 * \brief Add sky wrapper properties panel.
	 */
	void WPSky(igdeContainer &parent, igdeAction *action, const char *title,
		bool stretchLast = false, bool collapsed = false, bool takeOverAction = false);
	
	void WPSky(igdeContainer &parent, igdeWPSky::Ref &panel, igdeAction *action,
		const char *title, bool stretchLast = false, bool collapsed = false,
		bool takeOverAction = false);
	
	void WPSky(igdeWPSky::Ref &panel, igdeAction *action, bool takeOverAction = false);
	
	
	
	/**
	 * \brief Add camera wrapper properties panel.
	 */
	void WPCamera(igdeContainer &parent, igdeAction *action, const char *title,
		bool stretchLast = false, bool collapsed = false, bool takeOverAction = false);
	
	void WPCamera(igdeContainer &parent, igdeWPCamera::Ref &panel, igdeAction *action,
		const char *title, bool stretchLast = false, bool collapsed = false,
		bool takeOverAction = false);
	
	void WPCamera(igdeWPCamera::Ref &panel, igdeAction *action, bool takeOverAction = false);
	
	
	
	/**
	 * \brief Add object wrapper properties panel.
	 */
	void WPWObject(igdeContainer &parent, igdeAction *action, const char *title,
		bool stretchLast = false, bool collapsed = false, bool takeOverAction = false);
	
	void WPWObject(igdeContainer &parent, igdeWPWObject::Ref &panel, igdeAction *action,
		const char *title, bool stretchLast = false, bool collapsed = false,
		bool takeOverAction = false);
	
	void WPWObject(igdeWPWObject::Ref &panel, igdeAction *action, bool takeOverAction = false);
	
	
	
	/** \brief Add edit tags panel. */
	void EditTags(igdeEditTags::Ref &editTags, igdeAction *action, bool takeOverAction = false);
	
	void EditTags(igdeContainer &parent, igdeEditTags::Ref &editTags,
		igdeAction *action, bool takeOverAction = false);
	
	
	
	/** \brief Add toggle tags panel. */
	void ToggleTags(igdeContainer &parent, igdeToggleTags::Ref &toggleTags,
		igdeAction *action, bool takeOverAction = false);
	
	void ToggleTags(igdeToggleTags::Ref &toggleTags, igdeAction *action, bool takeOverAction = false);
	
	
	
	/** \brief Add edit property value widget. */
	void EditPropertyValue(igdeContainer &parent, igdeEditPropertyValue::Ref &widget,
		igdeEditPropertyValueListener *listener);
	
	void EditPropertyValue(igdeEditPropertyValue::Ref &widget, igdeEditPropertyValueListener *listener);
	
	
	
	/**
	 * \brief Add trigger table properties panel.
	 */
	void WPTriggerTable(igdeContainer &parent, igdeAction *action, const char *title,
		bool stretchLast = false, bool collapsed = false, bool takeOverAction = false);
	
	void WPTriggerTable(igdeContainer &parent, igdeWPTriggerTable::Ref &panel,
		igdeAction *action, const char *title, bool stretchLast = false, bool collapsed = false,
		bool takeOverAction = false);
	
	void WPTriggerTable(igdeWPTriggerTable::Ref &panel, igdeAction *action, bool takeOverAction = false);
	
	
	
	/** \brief Add menu command. */
	void MenuCommand(igdeMenuCascade &menu, igdeAction *action, bool takeOverAction = false);
	void MenuCommand(igdeMenuCascade &menu, const igdeAction::Ref &action);
	
	/** \brief Add menu check. */
	void MenuCheck(igdeMenuCascade &menu, igdeAction *action, bool takeOverAction = false);
	void MenuCheck(igdeMenuCascade &menu, const igdeAction::Ref &action);
	
	/** \brief Add menu option. */
	void MenuOption(igdeMenuCascade &menu, igdeAction *action, bool takeOverAction = false);
	void MenuOption(igdeMenuCascade &menu, const igdeAction::Ref &action);
	
	/** \brief Add menu separator. */
	void MenuSeparator(igdeMenuCascade &menu);
	
	/** \brief Add menu recent files sub menu. */
	void MenuRecentFiles(igdeMenuCascade &menu, igdeRecentFiles &recentFiles,
		const char *text = "Open Recent File");
	
	
	
	/** \brief Add toolbar button. */
	void ToolBarButton(igdeToolBar &toolBar, igdeAction *action, bool takeOverAction = false);
	
	/** \brief Add toolbar toggle button. */
	void ToolBarToggleButton(igdeToolBar &toolBar, igdeAction *action, bool takeOverAction = false);
	
	/** \brief Add toolbar separator. */
	void ToolBarSeparator(igdeToolBar &toolBar);
	
	
	
	/**
	 * \brief Format file size on 1024 basis with 1 digit precision.
	 * 
	 * Formats text for human readable use. The output is of one of these forms:
	 * - "X B": less than 1024 bytes
	 * - "X.Y kB": less than 1024 kilo-bytes
	 * - "X.Y MB": less than 1024 mega-bytes
	 * - "X.Y GB": otherwise
	 */
	static decString FormatSize1024(uint64_t size);
	
	/**
	 * \brief Format file size on 1000 basis with 1 digit precision.
	 * 
	 * Formats text for human readable use. The output is of one of these forms:
	 * - "X B": less than 1000 bytes
	 * - "X.Y kB": less than 1000 kilo-bytes
	 * - "X.Y MB": less than 1000 mega-bytes
	 * - "X.Y GB": otherwise
	 */
	static decString FormatSize1000(uint64_t size);
	
	/**
	 * \brief Format file size using tousand separators.
	 * 
	 * Formats text for human readable use in the form "X'YYY'ZZZ".
	 */
	static decString FormatSizeTousand(uint64_t size);
	/*@}*/
};

#endif
