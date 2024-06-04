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

#ifndef _DEBASESERVICESERVICE_H_
#define _DEBASESERVICESERVICE_H_

#include "../deBaseModule.h"

class decUniqueID;
class deServiceObject;


/**
 * \brief Base service service.
 *
 * Services provide various platform or community functionality not handled by other modules.
 * Such services are typically queried infrequently and take a certain time time complete.
 * For this reason service request are asynchronous. Data is carried in a generic
 * deServiceObject object. Users have to know the interface of the service. There is no
 * support to query interface structure unless a service desires to provide these using
 * a special service query.
 * 
 * Each service has a unique name across the entire game engine instance. If the name of
 * services of different service modules collide it is undefined which service the user
 * receives.
 */
class DE_DLL_EXPORT deBaseServiceService{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create service peer. */
	deBaseServiceService();
	
	/** \brief Clean up service peer. */
	virtual ~deBaseServiceService();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Start service request.
	 * 
	 * Script module peer is called with the received answer. Depending on the service the
	 * answer can be received continuously or as individual responses or in smaller chunks.
	 * The script module has to use a unique id for each request. The same id is returned
	 * in responses to allow matching them to requests. The id can also be used to cancel
	 * a request at any time.
	 */
	virtual void StartRequest( const decUniqueID &id, const deServiceObject &request ) = 0;
	
	/**
	 * \brief Cancel service request if running.
	 */
	virtual void CancelRequest( const decUniqueID &id ) = 0;
	/*@}*/
};

#endif
