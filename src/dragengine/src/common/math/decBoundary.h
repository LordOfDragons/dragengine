/* 
 * Drag[en]gine Game Engine
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


/**
 * \brief 4 Component Boundary for rectangular areas.
 */
class DE_DLL_EXPORT decBoundary{
public:
	/** \brief Top Left corner X coordinate. */
	int x1;
	
	/** \brief Top Left corner Y coordinate. */
	int y1;
	
	/** \brief Bottom Right corner X coordinate. */
	int x2;
	
	/** \brief Bottom Right corner Y coordinate. */
	int y2;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new boundary object initialized with all components to 0. */
	inline decBoundary(){
		x1 = 0;
		y1 = 0;
		x2 = 0;
		y2 = 0;
	}
	
	/** \brief Create new boundary object with the given values. */
	inline decBoundary( int nx1, int ny1, int nx2, int ny2 ){
		x1 = nx1;
		y1 = ny1;
		x2 = nx2;
		y2 = ny2;
	}
	
	/** \brief Create new boundary object with the values of another boundary object. */
	inline decBoundary( const decBoundary &b ){
		x1 = b.x1;
		y1 = b.y1;
		x2 = b.x2;
		y2 = b.y2;
	}
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Rearranges the coordinates so that x1/y1 are the top left corner and x2/y2 the bottom right corner. */
	void OrientateRect();
	
	/**
	 * \brief Rearrange coordinates so that x1/y1 are the top left corner and x2/y2 the bottom right corner.
	 * 
	 * This version is designed to oriente a line. The function will though most probably be
	 * replaced with something else. You should thus not use it if possible.
	 */
	void OrientateLine();
	
	/** \brief Returns the point formed by adding the given point to the top left corner point. */
	inline decPoint MapPoint( const decPoint &pt ) const{ return decPoint( x1 + pt.x, y1 + pt.y ); }
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set components of this boundary object to the values of another one. */
	decBoundary &operator=( const decBoundary &b );
	/*@}*/
};
