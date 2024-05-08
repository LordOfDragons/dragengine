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

#ifndef _DESERVICEEVENT_H_
#define _DESERVICEEVENT_H_

#include "deService.h"
#include "deServiceObject.h"
#include "../dragengine_export.h"
#include "../common/utils/decUniqueID.h"


/**
 * \brief Service event.
 * \version 1.23
 * 
 * Stores a service event to send asynchronous events synchronously to the scripting module.
 */
class DE_DLL_EXPORT deServiceEvent : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deServiceEvent> Ref;
	
	/** \brief Events. */
	enum eEvents{
		/** \brief Request response. */
		eeRequestResponse,
		
		/** \brief Request failed. */
		eeRequestFailed,
		
		/** \brief Event received. */
		eeEventReceived
	};
	
	
	
private:
	eEvents pType;
	deService::Ref pService;
	decUniqueID pId;
	deServiceObject::Ref pData;
	bool pFinished;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create service event. */
	deServiceEvent( eEvents type, const deService::Ref &service, const decUniqueID &id,
		const deServiceObject::Ref &data, bool finished );
	
protected:
	/** \brief Clean up service event object. */
	~deServiceEvent() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Event type. */
	inline eEvents GetType() const{ return pType; }
	
	/** \brief Service. */
	inline const deService::Ref &GetService() const{ return pService; }
	
	/** \brief Identifier. */
	inline const decUniqueID &GetId() const{ return pId; }
	
	/** \brief Data or nullptr. */
	inline const deServiceObject::Ref &GetData() const{ return pData; }
	
	/** \brief Finished state. */
	inline bool GetFinished() const{ return pFinished; }
	/*@}*/
};

#endif
