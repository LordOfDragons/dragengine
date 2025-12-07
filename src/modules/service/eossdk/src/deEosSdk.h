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

#ifndef _DEEOSSDK_H_
#define _DEEOSSDK_H_

#include <eos_sdk.h>

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/systems/modules/service/deBaseServiceModule.h>


/**
 * EOS SDK Service Module.
 */
class deEosSdk : public deBaseServiceModule{
private:
	bool pSdkInited;
	decPointerList pFrameUpdaters;
	
	
public:
	class cFrameUpdater{
	public:
		cFrameUpdater();
		virtual ~cFrameUpdater();
		virtual void FrameUpdate(float elapsed) = 0;
	};
	
	static deEosSdk *globalModule;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create module. */
	deEosSdk(deLoadableModule &loadableModule);
	
	/** Delete module. */
	~deEosSdk() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Set of supported service names. */
	decStringSet GetSupportedServices() override;
	
	/** Initialize Steam SDK if not initialized yet. */
	void InitSdk(const deServiceObject::Ref &data);
	
	/**
	 * Create service peer.
	 * 
	 * If service name is not supported nullptr is returned.
	 */
	deBaseServiceService *CreateService(deService *service,
		const char *name, const deServiceObject::Ref &data) override;
	
	/** Frame update. */
	void FrameUpdate(float elapsed) override;
	
	/** Add frame updater. */
	void AddFrameUpdater(cFrameUpdater *updater);
	
	/** Remove frame updater. */
	void RemoveFrameUpdater(cFrameUpdater *updater);
	/*@}*/
};

#endif
