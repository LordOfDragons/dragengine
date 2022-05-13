/* 
 * Drag[en]gine GUI Launcher
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

// include only once
#ifndef _DEGLGUIBUILDER_H_
#define _DEGLGUIBUILDER_H_

#include "foxtoolkit.h"



/**
 * GUI Builder.
 */
class deglGuiBuilder{
private:
	FXFont *pFont;
	
	int pColonsFloat;
	int pColonsDouble;
	int pColonsTextShort;
	int pColonsComboBox;
	int pRowsComboBox;
	
	int pPaddingGroupBox;
	int pPaddingLabel;
	int pSpacingGroupBox;
	int pPaddingMatrixRows;
	int pPaddingTextHorz;
	int pPaddingTextVert;
	int pPaddingButtonHorz;
	int pPaddingButtonVert;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new gui builder. */
	deglGuiBuilder();
	/** Creates a new gui builder copying the parameters from another. */
	deglGuiBuilder( const deglGuiBuilder &builder );
	/** Cleans up the camera object. */
	~deglGuiBuilder();
	/*@}*/

	/** \name Parameters */
	/*@{*/
	/** Retrieves the font to use. */
	inline FXFont *GetFont() const{ return pFont; }
	/** Sets the font to use. */
	void SetFont( FXFont *font );
	
	/**
	 * Retrieves the number of colons text fields should have that display
	 * a single precision floating point value.
	 */
	inline int GetColonsFloat() const{ return pColonsFloat; }
	/**
	 * Sets the number of colons text fields should have that display
	 * a single precision floating point value.
	 */
	void SetColonsFloat( int colons );
	/**
	 * Retrieves the number of colons text fields should have that display
	 * a double precision floating point value.
	 */
	inline int GetColonsDouble() const{ return pColonsDouble; }
	/**
	 * Sets the number of colons text fields should have that display
	 * a double precision floating point value.
	 */
	void SetColonsDouble( int colons );
	/**
	 * Retrieves the number of colons text fields should have that display
	 * a short text string.
	 */
	inline int GetColonsTextShort() const{ return pColonsTextShort; }
	/**
	 * Sets the number of colons text fields should have that display
	 * a short text string.
	 */
	void SetColonsTextShort( int colons );
	/**
	 * Retrieves the number of colons combo boxes should have.
	 */
	inline int GetColonsComboBox() const{ return pColonsComboBox; }
	/**
	 * Sets the number of colons combo boxes should have.
	 */
	void SetColonsComboBox( int colons );
	/**
	 * Retrieves the number of rows combo boxes should have.
	 */
	inline int GetRowsComboBox() const{ return pRowsComboBox; }
	/**
	 * Sets the number of rows combo boxes should have.
	 */
	void SetRowsComboBox( int colons );
	
	/** Retrieves the padding for group boxes. */
	inline int GetPaddingGroupBox() const{ return pPaddingGroupBox; }
	/** Sets the padding for group boxes. */
	void SetPaddingGroupBox( int padding );
	/** Retrieves the padding for labels. */
	inline int GetPaddingLabel() const{ return pPaddingLabel; }
	/** Sets the padding for labels. */
	void SetPaddingLabel( int padding );
	/** Retrieves the spacing for group boxes. */
	inline int GetSpacingGroupBox() const{ return pSpacingGroupBox; }
	/** Sets the spacing for group boxes. */
	void SetSpacingGroupBox( int spacing );
	/** Retrieves the padding for matrix rows. */
	inline int GetPaddingMatrixRows() const{ return pPaddingMatrixRows; }
	/** Sets the padding for matrix rows. */
	void SetPaddingMatrixRows( int padding );
	/** Retrieves the horizontal padding for text elements. */
	inline int GetPaddingTextHorz() const{ return pPaddingTextHorz; }
	/** Sets the horizontal padding for text elements. */
	void SetPaddingTextHorz( int padding );
	/** Retrieves the vertical padding for text elements. */
	inline int GetPaddingTextVert() const{ return pPaddingTextVert; }
	/** Sets the vertical padding for text elements. */
	void SetPaddingTextVert( int padding );
	/** Retrieves the horizontal padding for buttons. */
	inline int GetPaddingButtonHorz() const{ return pPaddingButtonHorz; }
	/** Sets the horizontal padding for buttons. */
	void SetPaddingButtonHorz( int padding );
	/** Retrieves the vertical padding for buttons. */
	inline int GetPaddingButtonVert() const{ return pPaddingButtonVert; }
	/** Sets the vertical padding for buttons. */
	void SetPaddingButtonVert( int padding );
	/*@}*/
	
	/** \name Creation */
	/*@{*/
	/** Creates a scrolling window filling the entire parent container. */
	FXScrollWindow *CreateScrollWindow( FXComposite *container ) const;
	/**
	 * Creates a scrollable container filling the entire parent container.
	 * This creates first a scroll window using CreateScrollWindow and then
	 * a FXVerticalFrame parented to this window.
	 */
	FXVerticalFrame *CreateScrollContainer( FXComposite *container ) const;
	/**
	 * Creates a group box with a vertical frame box inside. This creates
	 * first a group box and then a FXVerticalFrame parent to this box.
	 */
	FXVerticalFrame *CreateGroupBox( FXComposite *container, const char *caption, bool fillY ) const;
	/**
	 * Creates a matrix packer setup for a 2-colon layout of GUI elements.
	 */
	FXMatrix *CreateMatrixPacker( FXComposite *container ) const;
	FXMatrix *CreateMatrixPacker( FXComposite *container, int paddingRow ) const;
	/**
	 * Creates a horizontal packer to layout elements horizontally.
	 */
	FXHorizontalFrame *CreateHFrame( FXComposite *container ) const;
	/**
	 * Creates a horizontal packer to layout elements horizontally.
	 */
	FXHorizontalFrame *CreateHFrame( FXComposite *container, int gap ) const;
	/**
	 * Creates a vertical packer to layout elements vertically.
	 */
	FXVerticalFrame *CreateVFrame( FXComposite *container ) const;
	/**
	 * Creates a vertical packer to layout elements vertically.
	 */
	FXVerticalFrame *CreateVFrame( FXComposite *container, int gap ) const;
	/**
	 * Creates a label suitable for packing into a matrix packer. Can be used
	 * though also elsewhere. Uses the parameter padding on the right side.
	 */
	FXLabel *CreateLabel( FXComposite *container, const char *text, const char *toolTip ) const;
	/**
	 * Creates a label suitable for packing into a matrix packer. Can be used
	 * though also elsewhere. Uses custom padding.
	 */
	FXLabel *CreateLabel( FXComposite *container, const char *text, const char *toolTip, int padding ) const;
	/**
	 * Creates a text area.
	 */
	FXText *CreateTextArea( FXComposite *container, FXObject *target, FXSelector selector,
	const char *toolTip, bool fixedSizeX, bool fixedSizeY ) const;
	/**
	 * Creates a text field of a fixed or variable size. If fixed it stays at
	 * the same size at all times. If variable it grows to the size of the
	 * parent container unless smaller than the fixed size in which case it
	 * stays there.
	 */
	FXTextField *CreateTextField( FXComposite *container, FXObject *target, FXSelector selector,
	const char *toolTip, bool fixedSize ) const;
	/**
	 * Creates a text field of a fixed or variable size. If fixed it stays at
	 * the same size at all times. If variable it grows to the size of the
	 * parent container unless smaller than the fixed size in which case it
	 * stays there.
	 */
	FXTextField *CreateTextField( FXComposite *container, FXObject *target, FXSelector selector,
	const char *toolTip, int colons, bool fixedSize ) const;
	/**
	 * Creates a text field of fixed or variable size for editing a number values.
	 */
	FXTextField *CreateNumberField( FXComposite *container, FXObject *target, FXSelector selector,
	const char *toolTip, int colons, bool fixedSize ) const;
	/**
	 * Creates a push button.
	 */
	FXButton *CreateButton( FXComposite *container, const char *text, FXObject *target, FXSelector selector,
	const char *toolTip ) const;
	/**
	 * Creates a push button.
	 */
	FXButton *CreateButton( FXComposite *container, const char *text, FXIcon *icon, FXObject *target,
	FXSelector selector, const char *toolTip ) const;
	/**
	 * Creates a combo box.
	 */
	FXComboBox *CreateComboBox( FXComposite *container, FXObject *target, FXSelector selector,
	const char *toolTip, bool editable ) const;
	/**
	 * Creates a combo box.
	 */
	FXComboBox *CreateComboBox( FXComposite *container, FXObject *target, FXSelector selector,
	const char *toolTip, bool editable, int colons, int rows, bool fixedSize ) const;
	/**
	 * Creates a check box.
	 */
	FXCheckButton *CreateCheckBox( FXComposite *container, const char *text, FXObject *target, FXSelector selector,
	const char *toolTip ) const;
	/**
	 * Creates a slider.
	 */
	FXSlider *CreateSlider( FXComposite *container, FXObject *target, FXSelector selector, const char *toolTip ) const;
	/**
	 * Creates an icon list.
	 */
	FXIconList *CreateIconList( FXComposite *container, FXObject *target, FXSelector selector ) const;
	
	/**
	 * Sorts list items lexicographically by their name.
	 */
	static FXint SortListItemByName( const FXListItem *item1, const FXListItem *item2 );
	/**
	 * Sorts icon items lexicographically by their name.
	 */
	static FXint SortIconItemByName( const FXIconItem *item1, const FXIconItem *item2 );
	/*@}*/
};

// end of include only once
#endif
