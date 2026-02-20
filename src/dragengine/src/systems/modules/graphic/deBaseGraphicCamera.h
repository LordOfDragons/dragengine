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

#ifndef _DEBASEGRAPHICCAMERA_H_
#define _DEBASEGRAPHICCAMERA_H_

#include "../../../dragengine_export.h"

class deEffect;


/**
 * \brief Graphic Module Camera Peer.
 *
 */
class DE_DLL_EXPORT deBaseGraphicCamera{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new peer. */
	deBaseGraphicCamera();
	
	/** \brief Clean up peer. */
	virtual ~deBaseGraphicCamera();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Camera parameter changed. */
	virtual void ParameterChanged();
	
	/** \brief Adaption parameter changed. */
	virtual void AdaptionChanged();
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	
	/** \brief Effect has been added. */
	virtual void EffectAdded(int index, deEffect *effect);
	
	/** \brief Effect has been removed. */
	virtual void EffectRemoved(int index, deEffect *effect);
	
	/** \brief All effects have been removed. */
	virtual void AllEffectsRemoved();
	
	/** \brief Request graphic module to reset adapted intensity to optimal value. */
	virtual void ResetAdaptedIntensity();
	/*@}*/
	
	
	
	/** \name For use by VR Module only */
	/*@{*/
	/**
	 * \brief Camera assigned to HMD.
	 * \version 1.6
	 * 
	 * Called from main thread.
	 */
	virtual void VRAssignedToHMD();
	
	/**
	 * \brief Camera resigned from HMD.
	 * \version 1.6
	 * 
	 * Called from main thread.
	 */
	virtual void VRResignedFromHMD();
	
	/**
	 * \version 1.6
	 * \deprecated Not used.
	 */
	virtual void VRRenderParametersChanged();
	/*@}*/
};

#endif
