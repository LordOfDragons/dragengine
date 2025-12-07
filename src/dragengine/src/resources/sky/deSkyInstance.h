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

#ifndef _DESKYINSTANCE_H_
#define _DESKYINSTANCE_H_

#include "deSky.h"
#include "../deResource.h"
#include "../../common/utils/decLayerMask.h"

class deSkyInstanceManager;
class deSkyController;
class deBaseGraphicSkyInstance;
class deWorld;


/**
 * \brief Sky instance.
 * 
 * Every sky contains a set of layers and controllers indicating how the final sky looks
 * like. While skies define how the sky looks like the actual sky is produced using per
 * instance parameters.
 */
class DE_DLL_EXPORT deSkyInstance : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSkyInstance> Ref;
	
	
private:
	deSky::Ref pSky;
	int pOrder;
	
	deSkyController *pControllers;
	int pControllerCount;
	
	decLayerMask pLayerMask;
	float pPassthroughTransparency;
	
	deBaseGraphicSkyInstance *pPeerGraphic;
	
	deWorld *pParentWorld;
	deSkyInstance *pLLWorldPrev, *pLLWorldNext;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky instance. */
	deSkyInstance(deSkyInstanceManager *manager);
	
protected:
	/**
	 * \brief Clean up the sky instance.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deSkyInstance() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Sky or NULL if not set. */
	inline deSky *GetSky() const{ return pSky; }
	
	/** \brief Set sky or NULL to clear. */
	void SetSky(deSky *sky);
	
	/** \brief Render order with higher order covering lower order. */
	inline int GetOrder() const{ return pOrder; }
	
	/** \brief Set render order with higher order covering lower order. */
	void SetOrder(int order);
	
	
	
	/** \brief Number of controllers. */
	inline int GetControllerCount() const{ return pControllerCount; }
	
	/**
	 * \brief Controller at index.
	 * \throws deeInvalidParam \em index is less than 0 or greater or equal
	 * than GetControllerCount().
	 */
	deSkyController &GetControllerAt(int index) const;
	
	/** \brief Index of named controller or -1 if absent. */
	int IndexOfControllerNamed(const char *name) const;
	
	/** \brief Notify peer controller changed. */
	void NotifyControllerChangedAt(int index);
	
	
	/** \brief Layer mask. */
	const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask(const decLayerMask &layerMask);
	
	/** \brief Passthrough transparency for background rendering. */
	inline float GetPassthroughTransparency() const{ return pPassthroughTransparency; }
	
	/** \brief Set passthrough transparency for background rendering. */
	void SetPassthroughTransparency(float transparency);
	/*@}*/
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic peer or nullptr if not set. */
	inline deBaseGraphicSkyInstance *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic peer or nullptr if not set. */
	void SetPeerGraphic(deBaseGraphicSkyInstance *peer);
	/*@}*/
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or nullptr. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or nullptr. */
	void SetParentWorld(deWorld *world);
	
	/** \brief Previous sky in the parent world linked list. */
	inline deSkyInstance *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next sky in the parent world linked list. */
	void SetLLWorldPrev(deSkyInstance *sky);
	
	/** \brief Next sky in the parent world linked list. */
	inline deSkyInstance *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next sky in the parent world linked list. */
	void SetLLWorldNext(deSkyInstance *sky);
	/*@}*/
};

#endif
