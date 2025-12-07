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
	
	
	/** Valid data. */
	enum class eValidData{
		space = 1 << 0,
		position = 1 << 1,
		orientation = 1 << 2,
		linearVelocity = 1 << 3,
		angularVelocity = 1 << 4
	};
	
	
private:
	deoxrSession &pSession;
	
	XrSpace pSpace;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create reference space. */
	deoxrSpace(deoxrSession &session, XrReferenceSpaceType referenceType);
	
	/** Create reference space. */
	deoxrSpace(deoxrSession &session, XrReferenceSpaceType referenceType, const XrPosef &pose);
	
	/** Create reference space. */
	deoxrSpace(deoxrSession &session, XrReferenceSpaceType referenceType, const decMatrix &pose);
	
	/** Create action space. */
	deoxrSpace(deoxrSession &session, const deoxrAction &action,
		const deoxrPath &subactionPath, const decVector &rotation);
	
	/** Create action space. */
	deoxrSpace(deoxrSession &session, const deoxrAction &action);
	
	/** Create extension space. */
	deoxrSpace(deoxrSession &session, XrSpace space);
	
protected:
	/** Clean up space. */
	virtual ~deoxrSpace();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Session. */
	inline deoxrSession &GetSession() const{return pSession;}
	
	/** Space. */
	inline XrSpace GetSpace() const{return pSpace;}
	
	/**
	 * Transform relative to another space. If time is not valid or data is not present
	 * \em matrix is not changed.
	 * 
	 * \returns Valid data flags as combination of eValidData.
	 */
	int LocateSpace(const deoxrSpace &space, XrTime time,
		decVector &position, decQuaternion &orientation) const;
	
	/**
	 * Transform relative to another space with velocities. If time is not valid or data
	 * is not present \em matrix, \em linearVelocity and \em angularVelocity are not changed.
	 * 
	 * \returns Valid data flags as combination of eValidData.
	 */
	int LocateSpace(const deoxrSpace &space, XrTime time, decVector &position,
		decQuaternion &orientation, decVector &linearVelocity, decVector &angularVelocity) const;
	
	/**
	 * Transform relative to another space with velocities. If time is not valid or data
	 * is not present \em matrix, \em linearVelocity and \em angularVelocity are not changed.
	 * 
	 * \returns Valid data flags as combination of eValidData.
	 */
	int LocateSpaceEye(XrTime time, decVector &position, decQuaternion &orientation,
		decVector &linearVelocity, decVector &angularVelocity) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

