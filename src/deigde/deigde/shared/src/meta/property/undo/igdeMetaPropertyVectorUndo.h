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

#ifndef _IGDEMETAPROPERTYVectorUNDO_H_
#define _IGDEMETAPROPERTYVectorUNDO_H_

#include "../igdeMetaPropertyVector.h"
#include "../../igdeMetaContext.h"
#include "../../../undo/igdeUndo.h"


/**
 * \brief Vector meta property undo action.
 */
class DE_DLL_EXPORT igdeMetaPropertyVectorUndo : public igdeUndo{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<igdeMetaPropertyVectorUndo>;
	
	
private:
	const igdeMetaPropertyVector::Ref pProperty;
	const igdeMetaContext::Ref pContext;
	decVector pOldValue, pNewValue;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	igdeMetaPropertyVectorUndo(igdeMetaPropertyVector &property,
		const igdeMetaContext::Ref &context, const decVector &newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr);
	
protected:
	/** \brief Clean up undo action. */
	~igdeMetaPropertyVectorUndo() override = default;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Property. */
	inline const igdeMetaPropertyVector::Ref &GetProperty() const{ return pProperty; }
	
	/** \brief Context. */
	inline const igdeMetaContext::Ref &GetContext() const{ return pContext; }
	
	/** \brief Old value. */
	inline const decVector &GetOldValue() const{ return pOldValue; }
	
	/** \brief New value. */
	inline const decVector &GetNewValue() const{ return pNewValue; }
	
	/** \brief Set new value. */
	void SetNewValue(const decVector &newValue);
	
	
	/** \brief Undo. */
	void Undo() override;
	
	/** \brief Redo. */
	void Redo() override;
	
	/** \brief Progressive redo action. */
	void ProgressiveRedo();
	/*@}*/
};

#endif
