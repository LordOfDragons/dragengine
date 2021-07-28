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

#ifndef _DEGLDPLLISTITEMPARAM_H_
#define _DEGLDPLLISTITEMPARAM_H_

#include "../foxtoolkit.h"

class deglDialogProfileList;



/**
 * Profile List Dialog List Item Parameter.
 */
class deglDPLListItemParam : public FXListItem{
	FXDECLARE( deglDPLListItemParam )
protected:
	deglDPLListItemParam();
	
private:
	FXFont *pFont;
	FXString pParameterName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create list item. */
	deglDPLListItemParam( const FXString &text, const char *parameterName, FXFont *font );
	
	/** Clean up list item. */
	virtual ~deglDPLListItemParam();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parameter name. */
	inline const FXString &GetParameterName() const{ return pParameterName; }
	
	/** Set font. */
	void SetFont( FXFont *font );
	
	/** Draw item. */
	virtual void draw( const FXList *list, FXDC &dc, FXint x, FXint y, FXint w, FXint h ) const;
	/*@}*/
};

#endif
