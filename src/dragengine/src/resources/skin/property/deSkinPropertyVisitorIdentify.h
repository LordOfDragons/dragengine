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

#ifndef _DESKINPROPERTYVISITORIDENTIFY_H_
#define _DESKINPROPERTYVISITORIDENTIFY_H_

#include "deSkinPropertyVisitor.h"


/**
 * \brief Skin Property Identify Visitor.
 * Skin property visitor used to identify the type of a visited skin property. After
 * visiting a skin property the visitor can be asked what kind of skin property has
 * been visited. Provides also save casting. If the cast is not valid an exception
 * is raised.
 */
class DE_DLL_EXPORT deSkinPropertyVisitorIdentify : public deSkinPropertyVisitor{
public:
	/** \brief Property types. */
	enum ePropertyTypes{
		/** \brief Single value property. */
		eptValue,
		
		/** \brief Uniform color property. */
		eptColor,
		
		/** \brief Image property. */
		eptImage,
		
		/** \brief Video property. */
		eptVideo,
		
		/** \brief Constructed property. */
		eptConstructed,
		
		/** \brief Mapped. */
		eptMapped,
		
		/** \brief Unknown property. */
		eptUnknown = -1
	};
	
	
	
private:
	deSkinProperty *pProperty;
	ePropertyTypes pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animator property visitor identify object. */
	deSkinPropertyVisitorIdentify();
	
	/** \brief Clean up animator property visitor identify. */
	virtual ~deSkinPropertyVisitorIdentify();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Property type. */
	inline ePropertyTypes GetType() const{return pType;}
	
	/** \brief Determines if this is an unknown property. */
	inline bool IsUnknown() const{return pType == eptUnknown;}
	
	/** \brief Determines if this is a single value property. */
	inline bool IsValue() const{return pType == eptValue;}
	
	/** \brief Determines if this is a color property. */
	inline bool IsColor() const{return pType == eptColor;}
	
	/** \brief Determines if this is an image property. */
	inline bool IsImage() const{return pType == eptImage;}
	
	/** \brief Determines if this is a video property. */
	inline bool IsVideo() const{return pType == eptVideo;}
	
	/** \brief Determines if this is a constructed property. */
	inline bool IsConstructed() const{return pType == eptConstructed;}
	
	/** \brief Determines if this is a mapped property. */
	inline bool IsMapped() const{return pType == eptMapped;}
	
	/** \brief Cast to a single value property. */
	deSkinPropertyValue &CastToValue() const;
	
	/** \brief Cast to a uniform color property. */
	deSkinPropertyColor &CastToColor() const;
	
	/** \brief Cast to an image property. */
	deSkinPropertyImage &CastToImage() const;
	
	/** \brief Cast to a video property. */
	deSkinPropertyVideo &CastToVideo() const;
	
	/** \brief Cast to a constructed property. */
	deSkinPropertyConstructed &CastToConstructed() const;
	
	/** \brief Cast to a mapped property. */
	deSkinPropertyMapped &CastToMapped() const;
	
	/** \brief Resets the visitor. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit property. */
	virtual void VisitProperty(deSkinProperty &property);
	
	/** \brief Visit single value property. */
	virtual void VisitValue(deSkinPropertyValue &property);
	
	/** \brief Visit uniform color property. */
	virtual void VisitColor(deSkinPropertyColor &property);
	
	/** \brief Visit image property. */
	virtual void VisitImage(deSkinPropertyImage &property);
	
	/** \brief Visit video property. */
	virtual void VisitVideo(deSkinPropertyVideo &property);
	
	/** \brief Visit constructed property. */
	virtual void VisitConstructed(deSkinPropertyConstructed &property);
	
	/** \brief Visit mapped property. */
	virtual void VisitMapped(deSkinPropertyMapped &property);
	/*@}*/
};

#endif
