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

#ifndef _DEBILLBOARD_H_
#define _DEBILLBOARD_H_

#include "../deResource.h"
#include "../skin/deSkin.h"
#include "../skin/dynamic/deDynamicSkin.h"
#include "../../common/math/decMath.h"
#include "../../common/utils/decLayerMask.h"

class deBillboardManager;
class deBaseGraphicBillboard;
class deWorld;


/**
 * \brief Billboard Resource.
 * 
 * Billboards are rectangular objects that are different from
 * components in that they are flat and oriented relative to the
 * camera. Billboards can be locked in which case they are oriented
 * along an axis or free in which case they are oriented around
 * their position. Furthermore billboards are either spherical in
 * which case they are oriented relative to the camera position or
 * planar in which case they are oriented relative to the camera
 * near plane. The image of the billboard is either a skin or a
 * dynamic skin and has a 2 dimensional size as well as a 2
 * dimensional offset relative to the position and orientation axis.
 * The y component of the size and offset moves along the orientation
 * axis and therefore is aligned along the up direction in the camera
 * view. The x component moves perpendicular to the orientation axis
 * relative to the camera view and therefore is aligned along the
 * right direction in the camera view. By default billboards are axis
 * oriented with a world-up axis and are spherical.
 */
class DE_DLL_EXPORT deBillboard : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deBillboard> Ref;
	
	
private:
	decDVector pPosition;
	decVector pAxis;
	decVector2 pSize;
	decVector2 pOffset;
	deSkin::Ref pSkin;
	deDynamicSkin::Ref pDynamicSkin;
	bool pLocked;
	bool pSpherical;
	bool pSizeFixedToScreen;
	bool pVisible;
	decLayerMask pLayerMask;
	
	deBaseGraphicBillboard *pPeerGraphic;
	
	deWorld *pParentWorld;
	deBillboard *pLLWorldPrev;
	deBillboard *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new billboard. */
	deBillboard(deBillboardManager *manager);
	
protected:
	/**
	 * \brief Cleas up billboard.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deBillboard();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decDVector &GetPosition() const{return pPosition;}
	
	/** \brief Set position. */
	void SetPosition(const decDVector &position);
	
	/** \brief Axis. */
	inline const decVector &GetAxis() const{return pAxis;}
	
	/** \brief Set axis. */
	void SetAxis(const decVector &axis);
	
	/** \brief Size. */
	inline const decVector2 &GetSize() const{return pSize;}
	
	/** \brief Set size. */
	void SetSize(const decVector2 &size);
	
	/** \brief Offset. */
	inline const decVector2 &GetOffset() const{return pOffset;}
	
	/** \brief Set offset. */
	void SetOffset(const decVector2 &offset);
	
	/** \brief Skin or NULL if not assigned. */
	inline deSkin *GetSkin() const{return pSkin;}
	
	/** \brief Set skin. */
	void SetSkin(deSkin *skin);
	
	/** \brief Dynamic skin or NULL if not assigned. */
	inline deDynamicSkin *GetDynamicSkin() const{return pDynamicSkin;}
	
	/** \brief Set dynamic skin. */
	void SetDynamicSkin(deDynamicSkin *dynamicSkin);
	
	/** \brief Billboard is locked. */
	inline bool GetLocked() const{return pLocked;}
	
	/** \brief Set billboard is locked. */
	void SetLocked(bool locked);
	
	/** \brief Billboard is spherical. */
	inline bool GetSpherical() const{return pSpherical;}
	
	/** \brief Set if billboard is spherical. */
	void SetSpherical(bool spherical);
	
	/** \brief Billboard size is fixed to the screen size. */
	inline bool GetSizeFixedToScreen() const{return pSizeFixedToScreen;}
	
	/** \brief Set if billboard size is fixed to the screen size. */
	void SetSizeFixedToScreen(bool sizeFixedToScreen);
	
	/** \brief Billboard is visible. */
	inline bool GetVisible() const{return pVisible;}
	
	/** \brief Set if billboard is visible. */
	void SetVisible(bool visible);
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{return pLayerMask;}
	
	/** \brief Set layer mask. */
	void SetLayerMask(const decLayerMask &layerMask);
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer or NULL if not set. */
	inline deBaseGraphicBillboard *GetPeerGraphic() const{return pPeerGraphic;}
	
	/** \brief Set graphic system peer or NULL if not set. */
	void SetPeerGraphic(deBaseGraphicBillboard *peer);
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{return pParentWorld;}
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld(deWorld *world);
	
	/** \brief Previous billboard in the parent world linked list. */
	inline deBillboard *GetLLWorldPrev() const{return pLLWorldPrev;}
	
	/** \brief Set next billboard in the parent world linked list. */
	void SetLLWorldPrev(deBillboard *billboard);
	
	/** \brief Next billboard in the parent world linked list. */
	inline deBillboard *GetLLWorldNext() const{return pLLWorldNext;}
	
	/** \brief Set next billboard in the parent world linked list. */
	void SetLLWorldNext(deBillboard *billboard);
	/*@}*/
};

#endif
