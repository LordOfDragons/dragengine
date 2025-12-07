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
#include "../../../resources/service/deServiceObject.h"

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
	deBaseServiceModule(deLoadableModule &loadableModule);
	
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
	virtual deBaseServiceService *CreateService(deService *service,
		const char *name, const deServiceObject::Ref &data) = 0;
	
	/**
	 * \brief Frame update.
	 */
	virtual void FrameUpdate(float elapsed) = 0;
	
	/**
	 * \brief Make module add stage specific VFS containers.
	 * \version 1.23
	 * 
	 * If module requires dynamic modification of containers it should add
	 * add a deVFSRedirect container redirecting to a module owned deVirtualFileSystem.
	 * This way the containers can be modified while the game is running.
	 * 
	 * Stage is defined using a string and depends on the hosting application.
	 * These types are known (but more can be added):
	 * - patches: Game and patch containers are present. Allows modules to add
	 *   containers patching the game. A typical use for this are dynamically
	 *   downloaded patches as used in auto updating online games.
	 * - mods: After 'patches'. Allowes to add containers providing game modifications
	 * - overlay: After 'mods' and before adding the user overlay directory. Allows
	 *   modules to add containers overriding all containers up to this point in time.
	 *   The user overlay directory is always aded after this stage. Hence user
	 *   overlay content is always used first before any other content.
	 */
	virtual void AddVFSContainers(deVirtualFileSystem &vfs, const char *stage);
	/*@}*/
};

#endif
