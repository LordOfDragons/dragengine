/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDEMETAPROPERTYPOINTUNDO_H_
#define _IGDEMETAPROPERTYPOINTUNDO_H_

#include "../igdeMetaPropertyPoint.h"
#include "../../igdeMetaContext.h"
#include "../../../undo/igdeUndo.h"


/**
 * \brief Point meta property undo action.
 */
class DE_DLL_EXPORT igdeMetaPropertyPointUndo : public igdeUndo{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<igdeMetaPropertyPointUndo>;
	
	
private:
	const igdeMetaPropertyPoint::Ref pProperty;
	const igdeMetaContext::Ref pContext;
	decPoint pOldValue, pNewValue;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	igdeMetaPropertyPointUndo(igdeMetaPropertyPoint &property,
		const igdeMetaContext::Ref &context, const decPoint &newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr);
	
protected:
	/** \brief Clean up undo action. */
	~igdeMetaPropertyPointUndo() override = default;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Property. */
	inline const igdeMetaPropertyPoint::Ref &GetProperty() const{ return pProperty; }
	
	/** \brief Context. */
	inline const igdeMetaContext::Ref &GetContext() const{ return pContext; }
	
	/** \brief Old value. */
	inline const decPoint &GetOldValue() const{ return pOldValue; }
	
	/** \brief New value. */
	inline const decPoint &GetNewValue() const{ return pNewValue; }
	
	/** \brief Set new value. */
	void SetNewValue(const decPoint &newValue);
	
	
	/** \brief Undo. */
	void Undo() override;
	
	/** \brief Redo. */
	void Redo() override;
	
	/** \brief Progressive redo action. */
	void ProgressiveRedo();
	/*@}*/
};

#endif
