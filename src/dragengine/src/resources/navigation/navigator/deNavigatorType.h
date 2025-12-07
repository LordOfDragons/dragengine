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

#ifndef _DENAVIGATORTYPE_H_
#define _DENAVIGATORTYPE_H_

#include "../../../dragengine_export.h"


/**
 * \brief Navigator Type.
 * Defines the costs to apply during path finding for all navigation spaces with a given type number.
 */
class DE_DLL_EXPORT deNavigatorType{
private:
	int pType;
	float pFixCost;
	float pCostPerMeter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new navigator type. */
	deNavigatorType();
	
	/** \brief Clean up navigator type. */
	~deNavigatorType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Type number. */
	inline int GetType() const{return pType;}
	
	/** \brief Set type. */
	void SetType(int type);
	
	/** \brief Fix cost. */
	inline float GetFixCost() const{return pFixCost;}
	
	/** \brief Set fix cost. */
	void SetFixCost(float cost);
	
	/** \brief Retrieves cost per meter. */
	inline float GetCostPerMeter() const{return pCostPerMeter;}
	
	/** \brief Set cost per meter. */
	void SetCostPerMeter(float costPerMeter);
	/*@}*/
};

#endif
