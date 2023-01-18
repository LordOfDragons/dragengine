/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLSHAPEMANAGER_H_
#define _DEOGLSHAPEMANAGER_H_

class deoglShape;



/**
 * @brief Shape Manager.
 */
class deoglShapeManager{
private:
	deoglShape **pShapes;
	int pShapeCount;
	int pShapeSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new shape. */
	deoglShapeManager();
	/** Cleans up the shape. */
	~deoglShapeManager();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of shapes. */
	inline int GetShapeCount() const{ return pShapeCount; }
	/** Retrieves a shape by index. */
	deoglShape *GetShapeAt( int index ) const;
	/** Adds a shape. */
	void AddShape( deoglShape *shape );
	/** Removes all shapes. */
	void RemoveAllShapes();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
