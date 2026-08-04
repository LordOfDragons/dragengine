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

#ifndef _IGDEMETAPROPERTYDVECTORUNDO_H_
#define _IGDEMETAPROPERTYDVECTORUNDO_H_

#include "../igdeMetaPropertyDVector.h"
#include "../../igdeMetaContext.h"
#include "../../../undo/igdeUndo.h"


/**
 * \brief DVector meta property undo action.
 */
class DE_DLL_EXPORT igdeMetaPropertyDVectorUndo : public igdeUndo{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<igdeMetaPropertyDVectorUndo>;
	
	
private:
	const igdeMetaPropertyDVector::Ref pProperty;
	const igdeMetaContext::Ref pContext;
	decDVector pOldValue, pNewValue;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	igdeMetaPropertyDVectorUndo(igdeMetaPropertyDVector &property,
		const igdeMetaContext::Ref &context, const decDVector &newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr);
	
protected:
	/** \brief Clean up undo action. */
	~igdeMetaPropertyDVectorUndo() override = default;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Property. */
	inline const igdeMetaPropertyDVector::Ref &GetProperty() const{ return pProperty; }
	
	/** \brief Context. */
	inline const igdeMetaContext::Ref &GetContext() const{ return pContext; }
	
	/** \brief Old value. */
	inline const decDVector &GetOldValue() const{ return pOldValue; }
	
	/** \brief New value. */
	inline const decDVector &GetNewValue() const{ return pNewValue; }
	
	/** \brief Set new value. */
	void SetNewValue(const decDVector &newValue);
	
	
	/** \brief Undo. */
	void Undo() override;
	
	/** \brief Redo. */
	void Redo() override;
	
	/** \brief Progressive redo action. */
	void ProgressiveRedo();
	/*@}*/
};

#endif
