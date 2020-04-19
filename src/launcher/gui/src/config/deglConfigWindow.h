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
 * @brief Window Configuration.
 */
class deglConfigWindow{
private:
	int pX;
	int pY;
	int pWidth;
	int pHeight;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new window configuration. */
	deglConfigWindow();
	/** Cleans up the window configuration. */
	~deglConfigWindow();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the x position. */
	inline int GetX() const{ return pX; }
	/** Sets the x position. */
	void SetX( int x );
	/** Retrieves the y position. */
	inline int GetY() const{ return pY; }
	/** Sets the y position. */
	void SetY( int y );
	/** Retrieves the width. */
	inline int GetWidth() const{ return pWidth; }
	/** Sets the widht. */
	void SetWidth( int width );
	/** Retrieves the height. */
	inline int GetHeight() const{ return pHeight; }
	/** Sets the height. */
	void SetHeight( int height );
	/*@}*/
};

#endif // _DEGLGAME_H_
