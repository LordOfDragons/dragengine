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

#ifndef _DECSHAPELIST_H_
#define _DECSHAPELIST_H_

#include "../../dragengine_export.h"

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
