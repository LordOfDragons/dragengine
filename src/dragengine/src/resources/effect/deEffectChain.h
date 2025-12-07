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

#ifndef _DEEFFECTCHAIN_H_
#define _DEEFFECTCHAIN_H_

#include "../../common/math/decMath.h"


// definitions

class deEffect;
class deEffectVisitor;


/**
 * \brief Effect Chain Class.
 *
 * An effect chain holds a list of effects that are execute in the order
 * they have been added to the effect chain. The effect chain is a
 * convenience class that works with effects of various types so you do
 * not have to implement this every time. As this object is not reference
 * counted in any way the user class is responsible to free the effect
 * chain itself once the user class is destroyed.
 */
class DE_DLL_EXPORT deEffectChain{
private:
	deEffect **pEffects;
	int pEffectCount, pEffectSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new effect chain. */
	deEffectChain();
	
	/** \brief Clean up world. */
	~deEffectChain();
	/*@}*/
	
	
	
	/** \name Effect management */
	/*@{*/
	/** \brief Number of effects in the chain. */
	inline int GetEffectCount() const{ return pEffectCount; }
	
	/** \brief Effect at the given index. */
	deEffect *GetEffectAt(int index) const;
	
	/** \brief Given effect is already part of the chain. */
	bool HasEffect(deEffect *effect) const;
	
	/** \brief Position of the given effect in the chain or -1 if not part of it. */
	int IndexOfEffect(deEffect *effect) const;
	
	/**
	 * \brief Add effect to the chain.
	 * 
	 * Throws an exception if already in the chain.
	 */
	void AddEffect(deEffect *effect);
	
	/**
	 * \brief Insert effect at the given position to the chain.
	 * 
	 * Throws an exception if already already part of it. The new effect is inserted
	 * at the given position moving all effects after it up one position.
	 */
	void InsertEffect(deEffect *effect, int position);
	
	/** \brief Removes the given effect from the chain. */
	void RemoveEffect(deEffect *effect);
	
	/** \brief Removes all effects from the chain. */
	void RemoveAllEffects();
	
	/**
	 * \brief Move given effect to a new position.
	 * 
	 * The effect is placed at the new position moving all effects after it up one position.
	 */
	void MoveEffect(deEffect *effect, int newPosition);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effects. */
	void VisitEffects(deEffectVisitor &visitor);
	/*@}*/
};

#endif
