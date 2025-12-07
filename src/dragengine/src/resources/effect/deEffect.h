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

#ifndef _DEEFFECT_H_
#define _DEEFFECT_H_

#include "../../common/math/decMath.h"
#include "../deResource.h"


class deEffectFilterKernel;
class deEffectOverlayImage;
class deEffectColorMatrix;
class deEffectVisitor;
class deEffectManager;
class deBaseGraphicEffect;


/**
 * \brief Effect Class.
 *
 * Effects describe alterations of a world before it is rendered.
 * Such effects can alter the final image ( called post processing )
 * or alter the state of some objects. Various effects can be
 * combined to achieve all sorts of complex effects.
 */
class DE_DLL_EXPORT deEffect : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deEffect> Ref;
	
	
private:
	bool pEnabled;
	deBaseGraphicEffect *pPeerGraphic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new world object with the given resource manager. */
	deEffect(deEffectManager *manager);
	
protected:
	/**
	 * \brief Clean up world.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deEffect() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Effect is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if effect is enabled. */
	void SetEnabled(bool enabled);
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer object. */
	inline deBaseGraphicEffect *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer object. */
	void SetPeerGraphic(deBaseGraphicEffect *peer);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effect. */
	virtual void Visit(deEffectVisitor &visitor);
	/*@}*/
	
	
	
private:
	void pNotifyEnabledChanged();
};

#endif
