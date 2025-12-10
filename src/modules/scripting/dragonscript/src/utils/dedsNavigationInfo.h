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

#ifndef _DEDSNAVIGATIONINFO_H_
#define _DEDSNAVIGATIONINFO_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

class deScriptingDragonScript;

class dsValue;
class dsRealObject;



/**
 * \brief Navigation information.
 */
class dedsNavigationInfo : public deObject{
private:
	deScriptingDragonScript &pDS;
	
	decDVector pPosition;
	int pCostType;
	
	int pPathIndex;
	float pPathFactor;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<dedsNavigationInfo> Ref;
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new navigation information. */
	explicit dedsNavigationInfo(deScriptingDragonScript &ds);
	
	/** \brief Create copy of a navigation information. */
	dedsNavigationInfo(const dedsNavigationInfo &info);
	
	/** \brief Clean up the navigation information. */
	virtual ~dedsNavigationInfo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition(const decDVector &position);
	
	/** \brief Cost type at position if position is not \em null. */
	inline int GetCostType() const{ return pCostType; }
	
	/** \brief Set cost type at position. */
	void SetCostType(int costType);
	
	
	
	/**
	 * \brief Index along navigation path.
	 * \details -1 indicates navigation start position towards first path point.
	 */
	inline int GetPathIndex() const{ return pPathIndex; }
	
	/**
	 * \brief Set index along navigation path.
	 * \details -1 indicates navigation start position towards first path point.
	 * \throws dueEInvalidParam \em index is less than -1.
	 */
	void SetPathIndex(int index);
	
	/** \brief Factor along path between getPathIndex() and getPathIndex()+1 from 0 to 1. */
	inline float GetPathFactor() const{ return pPathFactor; }
	
	/** \brief Set factor along path between getPathIndex() and getPathIndex()+1 from 0 to 1. */
	void SetPathFactor(float factor);
	/*@}*/
};

#endif
