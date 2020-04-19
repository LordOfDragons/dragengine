/* 
 * Drag[en]gine IGDE World Editor
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

#if 0

// include only once
#ifndef _MEWTFILTERKERNEL_H_
#define _MEWTFILTERKERNEL_H_

// includes



/**
 * @brief Filter Kernel Template.
 * Filter Kernel Template for the world editor.
 */
class meWTFilterKernel{
private:
	char *pName;
	float *pKernel;
	int pKernelRows;
	int pKernelCols;
	float pScale;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new effect filter kernel template object. */
	meWTFilterKernel( const char *name, int rows, int cols, float scale );
	/** Cleans up the world. */
	~meWTFilterKernel();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the name of the template. */
	inline const char *GetName() const{ return ( const char * )pName; }
	/** Retrieves the rows of the filter kernel. */
	inline int GetKernelRows() const{ return pKernelRows; }
	/** Retrieves the cols of the filter kernel. */
	inline int GetKernelCols() const{ return pKernelCols; }
	/** Retrieves kernel value at the given location. */
	float GetKernelValueAt( int row, int col ) const;
	/** Sets the kernel value at the given location. */
	void SetKernelValueAt( int row, int col, float value );
	
	/** \brief Kernel scale. */
	inline float GetScale() const{ return pScale; }
	
	/** \brief Set kernel scale. */
	void SetScale( float scale );
	/*@}*/
};

// end of include only once
#endif

#endif
