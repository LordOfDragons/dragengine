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

#ifndef _PROPERTYNAMES_H_
#define _PROPERTYNAMES_H_

/**
 * \brief Names for gui theme properties provided by the basic IGDE implementation.
 */
namespace igdeGuiThemePropertyNames{
	/**
	 * \brief Font size relative to application default font size.
	 * 
	 * Name: "font.size"
	 * Type: Float type.
	 * Default: 1.
	 * Target: All text
	 */
	extern const char *fontSize;
	
	/**
	 * \brief Absolute font size in points.
	 * 
	 * Used only if value is larger than 0.
	 * 
	 * Name: "font.size.absolute"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: All text
	 */
	extern const char *fontSizeAbsolute;
	
	
	
	/**
	 * \brief GroupBox title font size relative to application default  font size.
	 * 
	 * Name: "groupBox.font.size"
	 * Type: Float type.
	 * Default: 1.
	 * Target: igdeGroupBox
	 */
	extern const char *groupBoxFontSize;
	
	/**
	 * \brief GroupBox title absolute font size in points.
	 * 
	 * Used only if value is larger than 0.
	 * 
	 * Name: "groupBox.font.size.absolute"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeGroupBox
	 */
	extern const char *groupBoxFontSizeAbsolute;
	
	/**
	 * \brief GroupBox left padding in pixels.
	 * 
	 * Name: "groupBox.padding.left"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeGroupBox
	 */
	extern const char *groupBoxPaddingLeft;
	
	/**
	 * \brief GroupBox right padding in pixels.
	 * 
	 * Name: "groupBox.padding.right"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeGroupBox
	 */
	extern const char *groupBoxPaddingRight;
	
	/**
	 * \brief GroupBox top padding in pixels.
	 * 
	 * Name: "groupBox.padding.top"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeGroupBox
	 */
	extern const char *groupBoxPaddingTop;
	
	/**
	 * \brief GroupBox bottom padding in pixels.
	 * 
	 * Name: "groupBox.padding.bottom"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeGroupBox
	 */
	extern const char *groupBoxPaddingBottom;
	
	
	
	/**
	 * \brief Label font size relative to application default  font size.
	 * 
	 * Name: "label.font.size"
	 * Type: Float type.
	 * Default: 1.
	 * Target: igdeLabel
	 */
	extern const char *labelFontSize;
	
	/**
	 * \brief Label absolute font size in points.
	 * 
	 * Used only if value is larger than 0.
	 * 
	 * Name: "label.font.size.absolute"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeLabel
	 */
	extern const char *labelFontSizeAbsolute;
	
	
	
	/**
	 * \brief TextField font size relative to application default  font size.
	 * 
	 * Name: "textField.font.size"
	 * Type: Float type.
	 * Default: 1.
	 * Target: igdeTextField
	 */
	extern const char *textFieldFontSize;
	
	/**
	 * \brief TextField absolute font size in points.
	 * 
	 * Used only if value is larger than 0.
	 * 
	 * Name: "textField.font.size.absolute"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeTextField
	 */
	extern const char *textFieldFontSizeAbsolute;
	
	/**
	 * \brief TextField left padding in pixels.
	 * 
	 * Name: "textField.padding.left"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeTextField
	 */
	extern const char *textFieldPaddingLeft;
	
	/**
	 * \brief TextField right padding in pixels.
	 * 
	 * Name: "textField.padding.right"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeTextField
	 */
	extern const char *textFieldPaddingRight;
	
	/**
	 * \brief TextField top padding in pixels.
	 * 
	 * Name: "textField.padding.top"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeTextField
	 */
	extern const char *textFieldPaddingTop;
	
	/**
	 * \brief TextField bottom padding in pixels.
	 * 
	 * Name: "textField.padding.bottom"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeTextField
	 */
	extern const char *textFieldPaddingBottom;
	
	
	
	/**
	 * \brief SpinTextField font size relative to application default  font size.
	 * 
	 * Name: "spinTextField.font.size"
	 * Type: Float type.
	 * Default: 1.
	 * Target: igdeSpinTextField
	 */
	extern const char *spinTextFieldFontSize;
	
	/**
	 * \brief SpinTextField absolute font size in points.
	 * 
	 * Used only if value is larger than 0.
	 * 
	 * Name: "spinTextField.font.size.absolute"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeSpinTextField
	 */
	extern const char *spinTextFieldFontSizeAbsolute;
	
	/**
	 * \brief SpinTextField left padding in pixels.
	 * 
	 * Name: "spinTextField.padding.left"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeSpinTextField
	 */
	extern const char *spinTextFieldPaddingLeft;
	
	/**
	 * \brief SpinTextField right padding in pixels.
	 * 
	 * Name: "spinTextField.padding.right"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeSpinTextField
	 */
	extern const char *spinTextFieldPaddingRight;
	
	/**
	 * \brief SpinTextField top padding in pixels.
	 * 
	 * Name: "spinTextField.padding.top"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeSpinTextField
	 */
	extern const char *spinTextFieldPaddingTop;
	
	/**
	 * \brief SpinTextField bottom padding in pixels.
	 * 
	 * Name: "spinTextField.padding.bottom"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeSpinTextField
	 */
	extern const char *spinTextFieldPaddingBottom;
	
	
	
	/**
	 * \brief ProgressBar font size relative to application default  font size.
	 * 
	 * Name: "progressBar.font.size"
	 * Type: Float type.
	 * Default: 1.
	 * Target: igdeProgressBar
	 */
	extern const char *progressBarFontSize;
	
	/**
	 * \brief ProgressBar absolute font size in points.
	 * 
	 * Used only if value is larger than 0.
	 * 
	 * Name: "progressBar.font.size.absolute"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeProgressBar
	 */
	extern const char *progressBarFontSizeAbsolute;
	
	/**
	 * \brief ProgressBar left padding in pixels.
	 * 
	 * Name: "progressBar.padding.left"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeProgressBar
	 */
	extern const char *progressBarPaddingLeft;
	
	/**
	 * \brief ProgressBar right padding in pixels.
	 * 
	 * Name: "progressBar.padding.right"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeProgressBar
	 */
	extern const char *progressBarPaddingRight;
	
	/**
	 * \brief ProgressBar top padding in pixels.
	 * 
	 * Name: "progressBar.padding.top"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeProgressBar
	 */
	extern const char *progressBarPaddingTop;
	
	/**
	 * \brief ProgressBar bottom padding in pixels.
	 * 
	 * Name: "progressBar.padding.bottom"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeProgressBar
	 */
	extern const char *progressBarPaddingBottom;
	
	
	
	/**
	 * \brief ComboBox font size relative to application default  font size.
	 * 
	 * Name: "comboBox.font.size"
	 * Type: Float type.
	 * Default: 1.
	 * Target: igdeComboBox
	 */
	extern const char *comboBoxFontSize;
	
	/**
	 * \brief ComboBox absolute font size in points.
	 * 
	 * Used only if value is larger than 0.
	 * 
	 * Name: "comboBox.font.size.absolute"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeComboBox
	 */
	extern const char *comboBoxFontSizeAbsolute;
	
	/**
	 * \brief ComboBox left padding in pixels.
	 * 
	 * Name: "comboBox.padding.left"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeComboBox
	 */
	extern const char *comboBoxPaddingLeft;
	
	/**
	 * \brief ComboBox right padding in pixels.
	 * 
	 * Name: "comboBox.padding.right"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeComboBox
	 */
	extern const char *comboBoxPaddingRight;
	
	/**
	 * \brief ComboBox top padding in pixels.
	 * 
	 * Name: "comboBox.padding.top"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeComboBox
	 */
	extern const char *comboBoxPaddingTop;
	
	/**
	 * \brief ComboBox bottom padding in pixels.
	 * 
	 * Name: "comboBox.padding.bottom"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeComboBox
	 */
	extern const char *comboBoxPaddingBottom;
	
	
	
	/**
	 * \brief ListBox font size relative to application default  font size.
	 * 
	 * Name: "listBox.font.size"
	 * Type: Float type.
	 * Default: 1.
	 * Target: igdeListBox
	 */
	extern const char *listBoxFontSize;
	
	/**
	 * \brief ListBox absolute font size in points.
	 * 
	 * Used only if value is larger than 0.
	 * 
	 * Name: "listBox.font.size.absolute"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeListBox
	 */
	extern const char *listBoxFontSizeAbsolute;
	
	/**
	 * \brief ListBox left padding in pixels.
	 * 
	 * Name: "listBox.padding.left"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeListBox
	 */
	extern const char *listBoxPaddingLeft;
	
	/**
	 * \brief ListBox right padding in pixels.
	 * 
	 * Name: "listBox.padding.right"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeListBox
	 */
	extern const char *listBoxPaddingRight;
	
	/**
	 * \brief ListBox top padding in pixels.
	 * 
	 * Name: "listBox.padding.top"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeListBox
	 */
	extern const char *listBoxPaddingTop;
	
	/**
	 * \brief ListBox bottom padding in pixels.
	 * 
	 * Name: "listBox.padding.bottom"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeListBox
	 */
	extern const char *listBoxPaddingBottom;
	
	
	
	/**
	 * \brief IconListBox font size relative to application default  font size.
	 * 
	 * Name: "iconListBox.font.size"
	 * Type: Float type.
	 * Default: 1.
	 * Target: igdeIconListBox
	 */
	extern const char *iconListBoxFontSize;
	
	/**
	 * \brief IconListBox absolute font size in points.
	 * 
	 * Used only if value is larger than 0.
	 * 
	 * Name: "iconListBox.font.size.absolute"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeIconListBox
	 */
	extern const char *iconListBoxFontSizeAbsolute;
	
	/**
	 * \brief IconListBox left padding in pixels.
	 * 
	 * Name: "iconListBox.padding.left"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeIconListBox
	 */
	extern const char *iconListBoxPaddingLeft;
	
	/**
	 * \brief IconListBox right padding in pixels.
	 * 
	 * Name: "iconListBox.padding.right"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeIconListBox
	 */
	extern const char *iconListBoxPaddingRight;
	
	/**
	 * \brief IconListBox top padding in pixels.
	 * 
	 * Name: "iconListBox.padding.top"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeIconListBox
	 */
	extern const char *iconListBoxPaddingTop;
	
	/**
	 * \brief IconListBox bottom padding in pixels.
	 * 
	 * Name: "iconListBox.padding.bottom"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeIconListBox
	 */
	extern const char *iconListBoxPaddingBottom;
	
	
	
	/**
	 * \brief TreeList font size relative to application default  font size.
	 * 
	 * Name: "treeList.font.size"
	 * Type: Float type.
	 * Default: 1.
	 * Target: igdeTreeList
	 */
	extern const char *treeListFontSize;
	
	/**
	 * \brief TreeList absolute font size in points.
	 * 
	 * Used only if value is larger than 0.
	 * 
	 * Name: "treeList.font.size.absolute"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeTreeList
	 */
	extern const char *treeListFontSizeAbsolute;
	
	/**
	 * \brief TreeList left padding in pixels.
	 * 
	 * Name: "treeList.padding.left"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeTreeList
	 */
	extern const char *treeListPaddingLeft;
	
	/**
	 * \brief TreeList right padding in pixels.
	 * 
	 * Name: "treeList.padding.right"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeTreeList
	 */
	extern const char *treeListPaddingRight;
	
	/**
	 * \brief TreeList top padding in pixels.
	 * 
	 * Name: "treeList.padding.top"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeTreeList
	 */
	extern const char *treeListPaddingTop;
	
	/**
	 * \brief TreeList bottom padding in pixels.
	 * 
	 * Name: "treeList.padding.bottom"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeTreeList
	 */
	extern const char *treeListPaddingBottom;
	
	
	
	/**
	 * \brief Button font size relative to application default  font size.
	 * 
	 * Name: "button.font.size"
	 * Type: Float type.
	 * Default: 1.
	 * Target: igdeButton
	 */
	extern const char *buttonFontSize;
	
	/**
	 * \brief Button absolute font size in points.
	 * 
	 * Used only if value is larger than 0.
	 * 
	 * Name: "button.font.size.absolute"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeButton
	 */
	extern const char *buttonFontSizeAbsolute;
	
	/**
	 * \brief Button left padding in pixels.
	 * 
	 * Name: "button.padding.left"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeButton
	 */
	extern const char *buttonPaddingLeft;
	
	/**
	 * \brief Button right padding in pixels.
	 * 
	 * Name: "button.padding.right"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeButton
	 */
	extern const char *buttonPaddingRight;
	
	/**
	 * \brief Button top padding in pixels.
	 * 
	 * Name: "button.padding.top"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeButton
	 */
	extern const char *buttonPaddingTop;
	
	/**
	 * \brief Button bottom padding in pixels.
	 * 
	 * Name: "button.padding.bottom"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeButton
	 */
	extern const char *buttonPaddingBottom;
	
	
	
	/**
	 * \brief CheckBox font size relative to application default  font size.
	 * 
	 * Name: "checkBox.font.size"
	 * Type: Float type.
	 * Default: 1.
	 * Target: igdeCheckBox
	 */
	extern const char *checkBoxFontSize;
	
	/**
	 * \brief CheckBox absolute font size in points.
	 * 
	 * Used only if value is larger than 0.
	 * 
	 * Name: "checkBox.font.size.absolute"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeCheckBox
	 */
	extern const char *checkBoxFontSizeAbsolute;
	
	/**
	 * \brief CheckBox left padding in pixels.
	 * 
	 * Name: "checkBox.padding.left"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeCheckBox
	 */
	extern const char *checkBoxPaddingLeft;
	
	/**
	 * \brief CheckBox right padding in pixels.
	 * 
	 * Name: "checkBox.padding.right"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeCheckBox
	 */
	extern const char *checkBoxPaddingRight;
	
	/**
	 * \brief CheckBox top padding in pixels.
	 * 
	 * Name: "checkBox.padding.top"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeCheckBox
	 */
	extern const char *checkBoxPaddingTop;
	
	/**
	 * \brief CheckBox bottom padding in pixels.
	 * 
	 * Name: "checkBox.padding.bottom"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeCheckBox
	 */
	extern const char *checkBoxPaddingBottom;
	
	
	
	/**
	 * \brief TabBook font size relative to application default  font size.
	 * 
	 * Name: "tabBook.font.size"
	 * Type: Float type.
	 * Default: 1.
	 * Target: igdeTabBook
	 */
	extern const char *tabBookFontSize;
	
	/**
	 * \brief TabBook absolute font size in points.
	 * 
	 * Used only if value is larger than 0.
	 * 
	 * Name: "tabBook.font.size.absolute"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeTabBook
	 */
	extern const char *tabBookFontSizeAbsolute;
	
	/**
	 * \brief TabBook left padding in pixels.
	 * 
	 * Name: "tabBook.padding.left"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeTabBook
	 */
	extern const char *tabBookPaddingLeft;
	
	/**
	 * \brief TabBook right padding in pixels.
	 * 
	 * Name: "tabBook.padding.right"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeTabBook
	 */
	extern const char *tabBookPaddingRight;
	
	/**
	 * \brief TabBook top padding in pixels.
	 * 
	 * Name: "tabBook.padding.top"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeTabBook
	 */
	extern const char *tabBookPaddingTop;
	
	/**
	 * \brief TabBook bottom padding in pixels.
	 * 
	 * Name: "tabBook.padding.bottom"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeTabBook
	 */
	extern const char *tabBookPaddingBottom;
	
	
	
	/**
	 * \brief Dialog left padding in pixels.
	 * 
	 * Name: "dialog.padding.left"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeDialog
	 */
	extern const char *dialogPaddingLeft;
	
	/**
	 * \brief Dialog right padding in pixels.
	 * 
	 * Name: "dialog.padding.right"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeDialog
	 */
	extern const char *dialogPaddingRight;
	
	/**
	 * \brief Dialog top padding in pixels.
	 * 
	 * Name: "dialog.padding.top"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeDialog
	 */
	extern const char *dialogPaddingTop;
	
	/**
	 * \brief Dialog bottom padding in pixels.
	 * 
	 * Name: "dialog.padding.bottom"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeDialog
	 */
	extern const char *dialogPaddingBottom;
	
	/**
	 * \brief Dialog padding between content and button line in pixels.
	 * 
	 * Name: "dialog.padding.content"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeDialog
	 */
	extern const char *dialogPaddingContent;
	
	/**
	 * \brief Dialog padding between buttons in button line in pixels.
	 * 
	 * Name: "dialog.padding.buttons"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeDialog
	 */
	extern const char *dialogPaddingButtons;
	
	
	
	/**
	 * \brief Window left padding in pixels.
	 * 
	 * Name: "window.padding.left"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeWindow
	 */
	extern const char *windowPaddingLeft;
	
	/**
	 * \brief Window right padding in pixels.
	 * 
	 * Name: "window.padding.right"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeWindow
	 */
	extern const char *windowPaddingRight;
	
	/**
	 * \brief Window top padding in pixels.
	 * 
	 * Name: "window.padding.top"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeWindow
	 */
	extern const char *windowPaddingTop;
	
	/**
	 * \brief Window bottom padding in pixels.
	 * 
	 * Name: "window.padding.bottom"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeWindow
	 */
	extern const char *windowPaddingBottom;
	
	
	
	/**
	 * \brief ColorBox height in pixels.
	 * 
	 * Name: "colorBox.height"
	 * Type: Integer type.
	 * Default: 15.
	 * Target: igdeColorBox
	 */
	extern const char *colorBoxHeight;
	
	
	
	/**
	 * \brief ViewCurveBezier font size relative to application default  font size.
	 * 
	 * Name: "viewCurveBezier.font.size"
	 * Type: Float type.
	 * Default: 1.
	 * Target: igdeViewCurveBezier
	 */
	extern const char *viewCurveBezierFontSize;
	
	/**
	 * \brief ViewCurveBezier absolute font size in points.
	 * 
	 * Used only if value is larger than 0.
	 * 
	 * Name: "viewCurveBezier.font.size.absolute"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeViewCurveBezier
	 */
	extern const char *viewCurveBezierFontSizeAbsolute;
	
	
	
	/**
	 * \brief NodeView Slot font size relative to application default  font size.
	 * 
	 * Name: "nodeViewSlot.font.size"
	 * Type: Float type.
	 * Default: 1.
	 * Target: igdeNodeView Slot
	 */
	extern const char *nodeViewSlotFontSize;
	
	/**
	 * \brief NodeView Slot absolute font size in points.
	 * 
	 * Used only if value is larger than 0.
	 * 
	 * Name: "nodeViewSlot.font.size.absolute"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeNodeView Slot
	 */
	extern const char *nodeViewSlotFontSizeAbsolute;
	
	/**
	 * \brief NodeView Slot left padding in pixels.
	 * 
	 * Name: "nodeViewSlot.padding.left"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeNodeView Slot
	 */
	extern const char *nodeViewSlotPaddingLeft;
	
	/**
	 * \brief NodeView Slot right padding in pixels.
	 * 
	 * Name: "nodeViewSlot.padding.right"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeNodeView Slot
	 */
	extern const char *nodeViewSlotPaddingRight;
	
	/**
	 * \brief NodeView Slot top padding in pixels.
	 * 
	 * Name: "nodeViewSlot.padding.top"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeNodeView Slot
	 */
	extern const char *nodeViewSlotPaddingTop;
	
	/**
	 * \brief NodeView Slot bottom padding in pixels.
	 * 
	 * Name: "nodeViewSlot.padding.bottom"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeNodeView Slot
	 */
	extern const char *nodeViewSlotPaddingBottom;
	
	
	
	/**
	 * \brief NodeView Node font size relative to application default  font size.
	 * 
	 * Name: "nodeViewNode.font.size"
	 * Type: Float type.
	 * Default: 1.
	 * Target: igdeNodeView Node
	 */
	extern const char *nodeViewNodeFontSize;
	
	/**
	 * \brief NodeView Node absolute font size in points.
	 * 
	 * Used only if value is larger than 0.
	 * 
	 * Name: "nodeViewNode.font.size.absolute"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeNodeView Node
	 */
	extern const char *nodeViewNodeFontSizeAbsolute;
	
	/**
	 * \brief NodeView Node left padding in pixels.
	 * 
	 * Name: "nodeViewNode.padding.left"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeNodeView Node
	 */
	extern const char *nodeViewNodePaddingLeft;
	
	/**
	 * \brief NodeView Node right padding in pixels.
	 * 
	 * Name: "nodeViewNode.padding.right"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeNodeView Node
	 */
	extern const char *nodeViewNodePaddingRight;
	
	/**
	 * \brief NodeView Node top padding in pixels.
	 * 
	 * Name: "nodeViewNode.padding.top"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeNodeView Node
	 */
	extern const char *nodeViewNodePaddingTop;
	
	/**
	 * \brief NodeView Node bottom padding in pixels.
	 * 
	 * Name: "nodeViewNode.padding.bottom"
	 * Type: Integer type.
	 * Default: 0.
	 * Target: igdeNodeView Node
	 */
	extern const char *nodeViewNodePaddingBottom;
};

#endif
