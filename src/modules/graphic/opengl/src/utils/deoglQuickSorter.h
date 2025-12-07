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

#ifndef _DEOGLQUICKSORTER_H_
#define _DEOGLQUICKSORTER_H_



/**
 * Quick Sorter.
 */
class deoglQuickSorter{
private:
	void **pElements;
	float *pScores;
	int pCount;
	int pSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new quick sorter. */
	deoglQuickSorter();
	/** Cleans up the quick sorter. */
	~deoglQuickSorter();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of elements. */
	inline int GetElementCount() const{ return pCount; }
	/** Retrieves the list of elements. */
	inline void **GetElements() const{ return pElements; }
	/** Retrieves an element by index. */
	void *GetElementAt(int index) const;
	/** Removes all elements. */
	void RemoveAllElements();
	/** Adds an element. */
	void AddElement(void *element, float score);
	/** Sort elements in ascending order. */
	void SortAscending();
	/** Sort elements in descending order. */
	void SortDescending();
	
	/** Test the implementation. Throws an exception if the implementation fails. */
	static void TestImplementation();
	/*@}*/
	
private:
	void pSortAscendingStep(int left, int right);
	void pSortDescendingStep(int left, int right);
};

#endif
