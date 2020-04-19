/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALCONFIGWINDOW_H_
#define _DEALCONFIGWINDOW_H_



/**
 * \brief Window Configuration.
 */
class dealConfigWindow{
private:
	int pX;
	int pY;
	int pWidth;
	int pHeight;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window configuration. */
	dealConfigWindow();
	
	/** \brief Clean up window configuration. */
	~dealConfigWindow();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief X position. */
	inline int GetX() const{ return pX; }
	
	/** \brief Set x position. */
	void SetX( int x );
	
	/** \brief Y position. */
	inline int GetY() const{ return pY; }
	
	/** \brief Set y position. */
	void SetY( int y );
	
	/** \brief Width. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Set widht. */
	void SetWidth( int width );
	
	/** \brief Height. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Set height. */
	void SetHeight( int height );
	/*@}*/
};

#endif
