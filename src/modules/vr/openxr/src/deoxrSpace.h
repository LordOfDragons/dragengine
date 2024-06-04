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

#ifndef _DEOXRSPACE_H_
#define _DEOXRSPACE_H_

#include "deoxrBasics.h"
#include "deoxrPath.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

class deoxrSession;
class deoxrAction;


/**
 * Oxr space.
 */
class deoxrSpace : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<deoxrSpace> Ref;
	
	
	
private:
	deoxrSession &pSession;
	
	XrSpace pSpace;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create reference space. */
	deoxrSpace( deoxrSession &session, XrReferenceSpaceType referenceType );
	
	/** Create action space. */
	deoxrSpace( deoxrSession &session, const deoxrAction &action,
		const deoxrPath &subactionPath, const decVector &rotation );
	
	/** Create action space. */
	deoxrSpace( deoxrSession &session, const deoxrAction &action );
	
protected:
	/** Clean up space. */
	virtual ~deoxrSpace();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Session. */
	inline deoxrSession &GetSession() const{ return pSession; }
	
	/** Space. */
	inline XrSpace GetSpace() const{ return pSpace; }
	
	/**
	 * Transform relative to another space. If time is not valid or data is not present
	 * \em matrix is not changed.
	 */
	void LocateSpace( const deoxrSpace &space, XrTime time,
		decVector &position, decQuaternion &orientation ) const;
	
	/**
	 * Transform relative to another space with velocities. If time is not valid or data
	 * is not present \em matrix, \em linearVelocity and \em angularVelocity are not changed.
	 */
	void LocateSpace( const deoxrSpace &space, XrTime time, decVector &position,
		decQuaternion &orientation, decVector &linearVelocity, decVector &angularVelocity ) const;
	
	/**
	 * Transform relative to another space with velocities. If time is not valid or data
	 * is not present \em matrix, \em linearVelocity and \em angularVelocity are not changed.
	 */
	void LocateSpaceEye( XrTime time, decVector &position, decQuaternion &orientation,
		decVector &linearVelocity, decVector &angularVelocity ) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

