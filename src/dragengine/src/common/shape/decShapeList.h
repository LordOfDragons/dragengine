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

#ifndef _DECSHAPELIST_H_
#define _DECSHAPELIST_H_

class decShape;
class decShapeVisitor;


/**
 * \brief List of analytic shapes.
 *
 * Provides support for visiting the shapes in the list.
 */
class DE_DLL_EXPORT decShapeList{
private:
	decShape **pShapes;
	int pCount;
	int pSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shape list. */
	decShapeList();
	
	/** \brief Create copy of shape list. */
	decShapeList( const decShapeList &copy );
	
	/** \brief Clean up shape list. */
	~decShapeList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of shapes. */
	inline int GetCount() const{ return pCount; }
	
	/**
	 * \brief Shape at index.
	 * \throws deeOutOfBoundary \em index is less than 0 or greater than or equal to GetCount().
	 */
	decShape *GetAt( int index ) const;
	
	/** \brief Index of shape or -1 if absent. */
	int IndexOf( decShape *shape ) const;
	
	/** \brief Shape is present. */
	bool Has( decShape *shape ) const;
	
	/**
	 * \brief Set shape at index.
	 * \throws deeOutOfBoundary \em index is less than 0 or greater than or equal to GetCount().
	 */
	void SetAt( int index, decShape *shape );
	
	/**
	 * \brief Add shape.
	 * \throws deeInvalidParam Shape is present.
	 */
	void Add( decShape *shape );
	
	/**
	 * \brief Remove shape.
	 * \throws deeInvalidParam Shape is absent.
	 */
	void Remove( decShape *shape );
	
	/**
	 * \brief Remove shape from index.
	 * \throws deeOutOfBoundary \em index is less than 0 or greater than or equal to GetCount().
	 */
	void RemoveFrom( int index );
	
	/** \brief Remove all shapes. */
	void RemoveAll();
	
	
	
	/** \brief Visit shapes. */
	void Visit( decShapeVisitor &visitor );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list to copy of another list. */
	decShapeList &operator=( const decShapeList &list );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
