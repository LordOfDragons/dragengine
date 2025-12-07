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

#ifndef _DEDAICOSTTABLE_H_
#define _DEDAICOSTTABLE_H_

#include <dragengine/common/collection/decIntOrderedSet.h>



/**
 * \brief Cost Table.
 * 
 * Definition of a cost table for a given layer. All navigation elements with the same layer
 * have the same layout of type entries as defined in the cost table. Changes to the cost
 * table are communicated to the individual navigation elements upon the next time the layer
 * is prepared for usage.
 */
class dedaiCostTable{
private:
	decIntOrderedSet pTypes;
	bool pChanged;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create cost table. */
	dedaiCostTable();
	
	/** \brief Clean up cost table. */
	~dedaiCostTable();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Cost table definition changed since the last time. */
	inline bool GetChanged() const{ return pChanged; }
	
	/** \brief Clear changed state. */
	void ClearChanged();
	
	
	
	/** \brief Number of types. */
	int GetTypeCount() const;
	
	/** \brief Type at index. */
	int GetTypeAt(int index) const;
	
	/**
	 * \brief Index of type with type number.
	 * \details Adds type if absent.
	 */
	int GetTypeWith(int typeNumber);
	
	/**
	 * \brief Index of type with type number.
	 * \returns \em defaultValue if type is absent.
	 */
	int GetTypeWith(int typeNumber, int defaultValue) const;
	
	/** \brief Remove all types. */
	void RemoveAllTypes();
	/*@}*/
};

#endif
