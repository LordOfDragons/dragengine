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

#ifndef _DEBASESERVICEMODULE_H_
#define _DEBASESERVICEMODULE_H_

#include "../deBaseModule.h"
#include "../../../common/string/decStringSet.h"

class deBaseServiceService;
class deService;


/**
 * \brief Base service module providing platform or community services.
 * \version 1.23
 */
class DE_DLL_EXPORT deBaseServiceModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseServiceModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBaseServiceModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set of supported service names. */
	virtual decStringSet GetSupportedServices() = 0;
	
	/**
	 * \brief Create service peer.
	 * 
	 * If service name is not supported nullptr is returned.
	 */
	virtual deBaseServiceService *CreateService( deService *service, const char *name ) = 0;
	/*@}*/
};

#endif
