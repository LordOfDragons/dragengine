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
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new effect filter kernel template object. */
	meWTFilterKernel( const char *name, int rows, int cols, float scale );
	/** Cleans up the world. */
	~meWTFilterKernel();
	/*@}*/
	
	/** \name Management */
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
