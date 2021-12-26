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

#ifndef _DEEFFECTFILTERKERNEL_H_
#define _DEEFFECTFILTERKERNEL_H_

#include "deEffect.h"


// definitions


/**
 * \brief Effect Filter Kernel Class.
 *
 * Filter kernel effects are post processing effects that manipulate
 * the final image after rendering. A filter kernel of a given size
 * is applied to every pixel of the final image multiplying the
 * neighboring pixels with the values stored in the filter kernel
 * and clamping the result to valid color values.
 */
class DE_DLL_EXPORT deEffectFilterKernel : public deEffect{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deEffectFilterKernel> Ref;
	
	
	
private:
	float *pKernel;
	int pKernelRows;
	int pKernelCols;
	float pScale;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new effect filter kernel object with the given resource manager. */
	deEffectFilterKernel( deEffectManager *manager );
	
protected:
	/**
	 * \brief Clean up world.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deEffectFilterKernel();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Rows of the filter kernel. */
	inline int GetKernelRows() const{ return pKernelRows; }
	
	/** \brief Cols of the filter kernel. */
	inline int GetKernelCols() const{ return pKernelCols; }
	
	/**
	 * \brief Set the size of the filter kernel.
	 * 
	 * The size has to be at least 1 in each direction and has to be odd. The oddness
	 * is required as the middle value in the filter kernel corresponds to the pixel
	 * filtered in the final image.
	 */
	void SetKernelSize( int rows, int cols );
	
	/** \brief Retrieves kernel value at the given location. */
	float GetKernelValueAt( int row, int col ) const;
	
	/** \brief Set kernel value at the given location. */
	void SetKernelValueAt( int row, int col, float value );
	
	/** \brief Scale of kernel. */
	inline float GetScale() const{ return pScale; }
	
	/** \brief Set scale of kernel. */
	void SetScale( float scale );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effect. */
	virtual void Visit( deEffectVisitor &visitor );
	/*@}*/
};

#endif
