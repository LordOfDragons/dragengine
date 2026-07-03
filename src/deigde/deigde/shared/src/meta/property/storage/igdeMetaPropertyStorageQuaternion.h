/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDEMETAPROPERTYSTORAGEQUATERNION_H_
#define _IGDEMETAPROPERTYSTORAGEQUATERNION_H_

#include "igdeMetaPropertyStorage.h"

#include <dragengine/common/math/decMath.h>


/**
 * \brief Quaternion meta property storage using IsEqualsTo.
 * 
 * P the meta property type. The expected value type of P has to be decQuaternion.
 */
template<typename P>
class igdeMetaPropertyStorageQuaternion : public igdeMetaPropertyStorage<P>{
private:
	decVector pEulerAngles = {};
	decQuaternion pQuaternion = {};
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create quaternion meta property storage. */
	igdeMetaPropertyStorageQuaternion(P &property, const deTObjectReference<igdeMetaContext> &context) :
	igdeMetaPropertyStorage<P>(property, context),
	pEulerAngles(property.GetDefaultValue()),
	pQuaternion(decQuaternion::CreateFromEuler(pEulerAngles * DEG2RAD)){
	}
	
	/** \brief Create quaternion meta property storage with initial value. */
	igdeMetaPropertyStorageQuaternion(P &property, const deTObjectReference<igdeMetaContext> &context, const decQuaternion &initialValue) :
	igdeMetaPropertyStorage<P>(property, context),
	pEulerAngles(initialValue.GetEulerAngles() * RAD2DEG),
	pQuaternion(initialValue){
	}
	
	/** \brief Create quaternion meta property storage with initial value from Euler angles. */
	igdeMetaPropertyStorageQuaternion(P &property, const deTObjectReference<igdeMetaContext> &context, const decVector &initialValue) :
	igdeMetaPropertyStorage<P>(property, context),
	pEulerAngles(initialValue),
	pQuaternion(decQuaternion::CreateFromEuler(initialValue * DEG2RAD)){
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Get value. */
	inline const decQuaternion &GetQuaternion() const{ return pQuaternion; }
	
	/** \brief Get value as Euler angles. */
	inline const decVector &GetEulerAngles() const{ return pEulerAngles; }
	inline decVector GetEulerAnglesRadians() const{ return pEulerAngles * DEG2RAD; }
	
	/** \brief Set value. */
	void SetQuaternion(const decQuaternion &value, bool notify = true){
		if(pQuaternion.IsEqualTo(value)){
			return;
		}
		
		pEulerAngles = value.GetEulerAngles() * RAD2DEG;
		pQuaternion = value;
		
		this->onValueChanged();
		if(notify){
			this->Property().NotifyValueChanged(this->Context());
		}
	}
	
	/** \brief Set value from Euler angles. */
	void SetEulerAngles(const decVector &value, bool notify = true){
		if(pEulerAngles.IsEqualTo(value)){
			return;
		}
		
		pEulerAngles = value;
		pQuaternion.SetFromEuler(value * DEG2RAD);
		
		this->onValueChanged();
		if(notify){
			this->Property().NotifyValueChanged(this->Context());
		}
	}
	
	void SetEulerAnglesRadians(const decVector &value, bool notify = true){
		SetEulerAngles(value * RAD2DEG, notify);
	}
	
	/** \brief Set value. */
	void SetValue(const igdeMetaPropertyStorageQuaternion<P> &value, bool notify = true){
		SetQuaternion(value.GetQuaternion(), notify);
	}
	/*@}*/
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Implicit conversion operator. */
	operator const decQuaternion&() const{
		return pQuaternion;
	}
	
	/** \brief Implicit conversion operator to Euler angles. */
	operator const decVector&() const{
		return pEulerAngles;
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageQuaternion<P> &operator=(const decQuaternion &value){
		SetQuaternion(value);
		return *this;
	}
	
	/** \brief Assignment operator from Euler angles. */
	igdeMetaPropertyStorageQuaternion<P> &operator=(const decVector &value){
		SetEulerAngles(value);
		return *this;
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageQuaternion<P> &operator=(const igdeMetaPropertyStorageQuaternion<P> &other){
		SetValue(other);
		return *this;
	}
	
	/** \brief Value is equal. */
	bool operator==(const decQuaternion &value) const{
		return pQuaternion.IsEqualTo(value);
	}
	
	/** \brief Value is equal from Euler angles. */
	bool operator==(const decVector &value) const{
		return pEulerAngles.IsEqualTo(value);
	}
	/*@}*/
};

#endif
