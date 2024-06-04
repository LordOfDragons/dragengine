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

#ifndef _DENETWORKVALUEVISITOR_H_
#define _DENETWORKVALUEVISITOR_H_

#include "../../../dragengine_export.h"

class deNetworkValue;
class deNetworkValueInteger;
class deNetworkValueFloat;
class deNetworkValueString;
class deNetworkValueData;
class deNetworkValuePoint2;
class deNetworkValuePoint3;
class deNetworkValueVector2;
class deNetworkValueVector3;
class deNetworkValueQuaternion;


/**
 * \brief Network Value Visitor Class.
 */
class DE_DLL_EXPORT deNetworkValueVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network value visitor object. */
	deNetworkValueVisitor();
	
	/** \brief Clean up network value visitor. */
	virtual ~deNetworkValueVisitor();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit network value. */
	virtual void VisitValue( deNetworkValue *value );
	
	/** \brief Visit integer network value. */
	virtual void VisitInteger( deNetworkValueInteger *value );
	
	/** \brief Visit float network value. */
	virtual void VisitFloat( deNetworkValueFloat *value );
	
	/** \brief Visit string network value. */
	virtual void VisitString( deNetworkValueString *value );
	
	/** \brief Visit data network value. */
	virtual void VisitData( deNetworkValueData *value );
	
	/** \brief Visit point-2 network value. */
	virtual void VisitPoint2( deNetworkValuePoint2 *value );
	
	/** \brief Visit point-3 network value. */
	virtual void VisitPoint3( deNetworkValuePoint3 *value );
	
	/** \brief Visit vector-2 network value. */
	virtual void VisitVector2( deNetworkValueVector2 *value );
	
	/** \brief Visit vector-3 network value. */
	virtual void VisitVector3( deNetworkValueVector3 *value );
	
	/** \brief Visit quaternion network value. */
	virtual void VisitQuaternion( deNetworkValueQuaternion *value );
	/*@}*/
};

#endif
