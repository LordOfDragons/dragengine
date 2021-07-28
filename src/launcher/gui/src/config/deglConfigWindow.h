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

#ifndef _DEGLCONFIGWINDOW_H_
#define _DEGLCONFIGWINDOW_H_


/**
 * Window Configuration.
 */
class deglConfigWindow{
private:
	int pX;
	int pY;
	int pWidth;
	int pHeight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create window configuration. */
	deglConfigWindow();
	
	/** Clean up window configuration. */
	~deglConfigWindow();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** X position. */
	inline int GetX() const{ return pX; }
	
	/** Set x position. */
	void SetX( int x );
	
	/** Y position. */
	inline int GetY() const{ return pY; }
	
	/** Set y position. */
	void SetY( int y );
	
	/** Width. */
	inline int GetWidth() const{ return pWidth; }
	
	/** Set widht. */
	void SetWidth( int width );
	
	/** Height. */
	inline int GetHeight() const{ return pHeight; }
	
	/** Set height. */
	void SetHeight( int height );
	/*@}*/
};

#endif
