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

#ifndef _DEEFFECTVISITORIDENTIFY_H_
#define _DEEFFECTVISITORIDENTIFY_H_

#include "deEffectVisitor.h"


/**
 * \brief Animator Rule Visitor Identify.
 *
 * Animation effect visitor providing effect identification. After visiting a
 * effect the visitor can be asked what kind of effect has been visited. Useful
 * to identify effects and to carry out actions only for certain effect types.
 * Furthermore save casting is provided. If the cast is not valid an
 * exception is raised.
 */
class DE_DLL_EXPORT deEffectVisitorIdentify : public deEffectVisitor{
public:
	enum eEffects{
		eetUnknown,
		eetFilterKernel,
		eetOverlayImage,
		eetColorMatrix,
		eetDistortImage
	};
	
	
	
private:
	deEffect *pEffect;
	eEffects pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animator effect visitor identify object. */
	deEffectVisitorIdentify();
	
	/** \brief Clean up animator effect visitor identify. */
	virtual ~deEffectVisitorIdentify();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Effect type. */
	inline eEffects GetType() const{return pType;}
	
	/** \brief Determines if this is an unknown effect. */
	inline bool IsUnknown() const{return pType == eetUnknown;}
	
	/** \brief Determines if this is a filter kernel effect. */
	inline bool IsFilterKernel() const{return pType == eetFilterKernel;}
	
	/** \brief Determines if this is a overlay image effect. */
	inline bool IsOverlayImage() const{return pType == eetOverlayImage;}
	
	/** \brief Determines if this is a color matrix effect. */
	inline bool IsColorMatrix() const{return pType == eetColorMatrix;}
	
	/** \brief Determines if this is a pixel matrix effect. */
	inline bool IsDistortImage() const{return pType == eetDistortImage;}
	
	/** \brief Cast to a filter kernel effect. */
	deEffectFilterKernel &CastToFilterKernel() const;
	
	/** \brief Cast to a overlay image effect. */
	deEffectOverlayImage &CastToOverlayImage() const;
	
	/** \brief Cast to a color matrix effect. */
	deEffectColorMatrix &CastToColorMatrix() const;
	
	/** \brief Cast to a distort image effect. */
	deEffectDistortImage &CastToDistortImage() const;
	
	/** \brief Resets the visitor. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effect. */
	virtual void VisitEffect(deEffect &effect);
	
	/** \brief Visit effect filter kernel. */
	virtual void VisitFilterKernel(deEffectFilterKernel &effect);
	
	/** \brief Visit overlay image effect. */
	virtual void VisitOverlayImage(deEffectOverlayImage &effect);
	
	/** \brief Visit color matrix effect. */
	virtual void VisitColorMatrix(deEffectColorMatrix &effect);
	
	/** \brief Visit pixel matrix effect. */
	virtual void VisitDistortImage(deEffectDistortImage &effect);
	/*@}*/
};

#endif
