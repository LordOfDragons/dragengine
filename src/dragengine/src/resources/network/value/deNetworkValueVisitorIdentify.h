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

#ifndef _DENETWORKSTATEVISITORIDENTIFY_H_
#define _DENETWORKSTATEVISITORIDENTIFY_H_

#include "deNetworkValueVisitor.h"


/**
 * \brief Network Value Visitor Identify.
 *
 * Network Value visitor providing value identification. After visiting a
 * collidder the visitor can be asked what kind of value has been visited.
 * Useful to identify values and to carry out actions only for certain
 * value types. Furthermore save casting is provided. If the cast is not
 * valid an exception is raised.
 */
class DE_DLL_EXPORT deNetworkValueVisitorIdentify : public deNetworkValueVisitor{
public:
	enum eNetworkValues{
		envtInteger,
		envtFloat,
		envtString,
		envtData,
		envtPoint2,
		envtPoint3,
		envtVector2,
		envtVector3,
		envtQuaternion,
		envtUnknown = -1,
	};
	
	
	
private:
	deNetworkValue *pValue;
	eNetworkValues pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create value visitor identify object. */
	deNetworkValueVisitorIdentify();
	
	/** \brief Clean up value visitor identify. */
	~deNetworkValueVisitorIdentify() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Vvalue type. */
	inline eNetworkValues GetType() const{ return pType; }
	
	/** \brief Is unknown value. */
	inline bool IsUnknown() const{ return pType == envtUnknown; }
	
	/** \brief Is integer value. */
	inline bool IsInteger() const{ return pType == envtInteger; }
	
	/** \brief Is float value. */
	inline bool IsFloat() const{ return pType == envtFloat; }
	
	/** \brief Is string value. */
	inline bool IsString() const{ return pType == envtString; }
	
	/** \brief Is data value. */
	inline bool IsData() const{ return pType == envtData; }
	
	/** \brief Is point-2 value. */
	inline bool IsPoint2() const{ return pType == envtPoint2; }
	
	/** \brief Is point-3 value. */
	inline bool IsPoint3() const{ return pType == envtPoint3; }
	
	/** \brief Is vector-2 value. */
	inline bool IsVector2() const{ return pType == envtVector2; }
	
	/** \brief Is vector-3 value. */
	inline bool IsVector3() const{ return pType == envtVector3; }
	
	/** \brief Is quaternion value. */
	inline bool IsQuaternion() const{ return pType == envtQuaternion; }
	
	/** \brief Cast to integer network value. */
	deNetworkValueInteger *CastToInteger() const;
	
	/** \brief Cast to float network value. */
	deNetworkValueFloat *CastToFloat() const;
	
	/** \brief Cast to string network value. */
	deNetworkValueString *CastToString() const;
	
	/** \brief Cast to data network value. */
	deNetworkValueData *CastToData() const;
	
	/** \brief Cast to point-2 network value. */
	deNetworkValuePoint2 *CastToPoint2() const;
	
	/** \brief Cast to point-3 network value. */
	deNetworkValuePoint3 *CastToPoint3() const;
	
	/** \brief Cast to vector-2 network value. */
	deNetworkValueVector2 *CastToVector2() const;
	
	/** \brief Cast to vector-3 network value. */
	deNetworkValueVector3 *CastToVector3() const;
	
	/** \brief Cast to quaternion network value. */
	deNetworkValueQuaternion *CastToQuaternion() const;
	
	/** \brief Reset visitor. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit network value. */
	void VisitValue(deNetworkValue *value) override;
	
	/** \brief Visit integer network value. */
	void VisitInteger(deNetworkValueInteger *value) override;
	
	/** \brief Visit float network value. */
	void VisitFloat(deNetworkValueFloat *value) override;
	
	/** \brief Visit string network value. */
	void VisitString(deNetworkValueString *value) override;
	
	/** \brief Visit data network value. */
	void VisitData(deNetworkValueData *value) override;
	
	/** \brief Visit point-2 network value. */
	void VisitPoint2(deNetworkValuePoint2 *value) override;
	
	/** \brief Visit point-3 network value. */
	void VisitPoint3(deNetworkValuePoint3 *value) override;
	
	/** \brief Visit vector-2 network value. */
	void VisitVector2(deNetworkValueVector2 *value) override;
	
	/** \brief Visit vector-3 network value. */
	void VisitVector3(deNetworkValueVector3 *value) override;
	
	/** \brief Visit quaternion network value. */
	void VisitQuaternion(deNetworkValueQuaternion *value) override;
	/*@}*/
};

#endif
