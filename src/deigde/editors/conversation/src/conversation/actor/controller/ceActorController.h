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

#ifndef _CEACTORCONTROLLER_H_
#define _CEACTORCONTROLLER_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Controller in a conversation actor animator instance.
 */
class ceActorController : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceActorController> Ref;
	
	
	
public:
	/** \brief Update types. */
	enum eUpdateTypes{
		/** \brief Set controller to a fixed value. */
		eutConstant,
		
		/** \brief Update controller using the elapsed time. */
		eutElapsedTime,
		
		/** \brief Set controller to the head turning left and right value. */
		eutHeadLeftRight,
		
		/** \brief Set controller to the head looking up and down value. */
		eutHeadUpDown,
		
		/** \brief Set controller to the eyes looking left and right value. */
		eutEyesLeftRight,
		
		/** \brief Set controller to the eyes looking up and down value. */
		eutEyesUpDown
	};
	
	
	
private:
	decString pName;
	eUpdateTypes pUpdateType;
	float pValue;
	decVector pVector;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create actor controller. */
	ceActorController();
	
	/** \brief Create copy of controller. */
	ceActorController( const ceActorController &controller );
	
protected:
	/** \brief Clean up actor controller. */
	virtual ~ceActorController();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Update type. */
	inline eUpdateTypes GetUpdateType() const{ return pUpdateType; }
	
	/** \brief Set update type. */
	void SetUpdateType( eUpdateTypes type );
	
	/** \brief Constant value. */
	inline float GetValue() const{ return pValue; }
	
	/** \brief Set constant value. */
	void SetValue( float value );
	
	/** \brief Constant vector. */
	inline const decVector &GetVector() const{ return pVector; }
	
	/** \brief Set constant vector. */
	void SetVector( const decVector &vector );
	/*@}*/
};

#endif
