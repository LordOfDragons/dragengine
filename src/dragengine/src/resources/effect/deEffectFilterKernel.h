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
	deEffectFilterKernel(deEffectManager *manager);
	
protected:
	/**
	 * \brief Clean up world.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deEffectFilterKernel() override;
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
	void SetKernelSize(int rows, int cols);
	
	/** \brief Retrieves kernel value at the given location. */
	float GetKernelValueAt(int row, int col) const;
	
	/** \brief Set kernel value at the given location. */
	void SetKernelValueAt(int row, int col, float value);
	
	/** \brief Scale of kernel. */
	inline float GetScale() const{ return pScale; }
	
	/** \brief Set scale of kernel. */
	void SetScale(float scale);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effect. */
	void Visit(deEffectVisitor &visitor) override;
	/*@}*/
};

#endif
