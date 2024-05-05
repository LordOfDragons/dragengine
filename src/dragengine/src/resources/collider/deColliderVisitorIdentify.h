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

#ifndef _DECOLLIDERVISITORIDENTIFY_H_
#define _DECOLLIDERVISITORIDENTIFY_H_

#include "deColliderVisitor.h"


/**
 * \brief Identify collider visitor.
 */
class DE_DLL_EXPORT deColliderVisitorIdentify : public deColliderVisitor{
public:
	/** \brief Collider types. */
	enum eColliderTypes{
		/** \brief Unknown type. */
		ectUnknown,
		
		/** \brief Collider volume. */
		ectVolume,
		
		/** \brief Collider rig. */
		ectRig,
		
		/** \brief Collider component. */
		ectComponent
	};
	
	
	
private:
	deCollider *pCollider;
	eColliderTypes pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create identify visitor. */
	deColliderVisitorIdentify();
	
	/** \brief Clean up identify collider visitor. */
	virtual ~deColliderVisitorIdentify();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Collider type. */
	inline eColliderTypes GetType() const{ return pType; }
	
	
	
	/** \brief Visited collider is an unknown collider. */
	inline bool IsUnknown() const{ return pType == ectUnknown; }
	
	/** \brief Visited collider is a volume collider. */
	inline bool IsVolume() const{ return pType == ectVolume; }
	
	/** \brief Visited collider is a rig collider. */
	inline bool IsRigged() const{ return pType == ectRig; }
	
	/** \brief Visited collider is component collider. */
	inline bool IsComponent() const{ return pType == ectComponent; }
	
	
	
	/**
	 * \brief Cast to volume collider.
	 * \throws deeInvalidParam Visited collider is not a volume collider.
	 */
	deColliderVolume &CastToVolume() const;
	
	/**
	 * \brief Cast to rig collider.
	 * \throws deeInvalidParam Visited collider is not a rig collider.
	 */
	deColliderRig &CastToRig() const;
	
	/**
	 * \brief Cast to component collider.
	 * \throws deeInvalidParam Visited collider is not a component collider.
	 */
	deColliderComponent &CastToComponent() const;
	
	
	
	/** \brief Reset visitor. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit collider. */
	virtual void VisitCollider( deCollider &collider );
	
	/** \brief Visit volume collider. */
	virtual void VisitVolume( deColliderVolume &collider );
	
	/** \brief Visit rig collider. */
	virtual void VisitRig( deColliderRig &collider );
	
	/** \brief Visit component collider. */
	virtual void VisitComponent( deColliderComponent &collider );
	/*@}*/
};

#endif
