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

#ifndef _MEPATHFINDTESTTYPE_H_
#define _MEPATHFINDTESTTYPE_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>



/**
 * @brief Path Find Test Type.
 */
class mePathFindTestType : public deObject{
private:
	decString pName;
	int pTypeNumber;
	float pFixCost;
	float pCostPerMeter;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<mePathFindTestType> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new path find test type. */
	mePathFindTestType(int typeNumber);
	/** Cleans up the path find test type. */
	virtual ~mePathFindTestType();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the type number. */
	inline int GetTypeNumber() const{return pTypeNumber;}
	/** Sets the type number. */
	void SetTypeNumber(int typeNumber);
	/** Retrieves the name of the type. */
	inline const decString &GetName() const{return pName;}
	/** Sets the name of the type. */
	void SetName(const char *name);
	/** Retrieves the fix cost. */
	inline float GetFixCost() const{return pFixCost;}
	/** Sets the fix cost. */
	void SetFixCost(float fixCost);
	/** Retrieves the cost per meter. */
	inline float GetCostPerMeter() const{return pCostPerMeter;}
	/** Sets the cost per meter. */
	void SetCostPerMeter(float costPerMeter);
	/*@}*/
};

#endif
