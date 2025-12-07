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

#ifndef _CEFACEPOSE_H_
#define _CEFACEPOSE_H_

#include "../../utils/ceControllerValueList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class ceConversation;



/**
 * \brief Face Pose.
 */
class ceFacePose : public deObject{
private:
	ceConversation *pConversation;
	
	decString pName;
	ceControllerValueList pControllers;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceFacePose> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new face pose. */
	ceFacePose(const char *name = "Face Pose");
	/** Cleans up the face pose. */
	virtual ~ceFacePose();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the conversation or NULL if not set. */
	inline ceConversation *GetConversation() const{return pConversation;}
	/** Sets the conversation or NULL if not set. */
	void SetConversation(ceConversation *conversation);
	
	/** Retrieves the name. */
	inline const decString &GetName() const{return pName;}
	/** Sets the name. */
	void SetName(const char *name);
	/** Retrieves the controller value list. */
	inline ceControllerValueList &GetControllerList(){return pControllers;}
	inline const ceControllerValueList &GetControllerList() const{return pControllers;}
	/** Notifies all that the controller value list changed. */
	void NotifyControllersChanged();
	/*@}*/
};

#endif
