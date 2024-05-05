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

#ifndef _STCLASSVECTOR2_H_
#define _STCLASSVECTOR2_H_

#include "../../stCommon.h"

#include <dragengine/common/math/decMath.h>

class ScriptingSmalltalk;



/**
 * \brief Script class immutable vector2.
 */
class stClassVector2{
private:
	ScriptingSmalltalk &pST;
	OOP pOOPClass;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	stClassVector2( ScriptingSmalltalk &st );
	
	/** \brief Clean up class. */
	virtual ~stClassVector2();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline ScriptingSmalltalk &GetST() const{ return pST; }
	
	
	
	/** \brief Set up links. */
	void SetUpLinks();
	
	/** \brief Vector2 from object. */
	const decVector2 &OOPToVector2( OOP object ) const;
	
	/** \brief Push vector2. */
	OOP Vector2ToOOP( const decVector2 &vector );
	/*@}*/
	
	
	
	/** \name cCall Methods */
	/*@{*/
	/** \brief Create zero vector2 object. */
	static OOP ccNew( OOP self );
	
	/** \brief Create vector2 object. */
	static OOP ccNewXY( OOP self, double x, double y );
	
	
	
	/** \brief X component. */
	static double ccX( OOP self );
	
	/** \brief Y component. */
	static double ccY( OOP self );
	
	
	
	/** \brief Hash code. */
	static int ccHash( OOP self );
	
	/** \brief String representation. */
	static OOP ccAsString( OOP self );
	/*@}*/
};

#endif
