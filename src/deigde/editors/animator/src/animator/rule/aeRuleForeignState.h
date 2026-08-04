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

#ifndef _AERULEFOREIGNSTATE_H_
#define _AERULEFOREIGNSTATE_H_

#include "aeRule.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertySet.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>

#include <dragengine/resources/animator/rule/deAnimatorRuleForeignState.h>

/**
 * Foreign state rule.
 */
class aeRuleForeignState : public aeRule{
public:
	using Ref = deTObjectReference<aeRuleForeignState>;
	
	using MetaContext = igdeMetaContextTypeInherit<aeRuleForeignState, aeRule>;
	static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, aeRuleForeignState *rule);
	
	template<typename T>
	using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
	
public:
	igdeMetaPropertyStringStorage::Storage mpForeignBone;
	igdeMetaPropertyStringStorage::Storage mpForeignVertexPositionSet;
	igdeMetaPropertyFloatStorage::Storage mpScalePosition;
	igdeMetaPropertyFloatStorage::Storage mpScaleOrientation;
	igdeMetaPropertyFloatStorage::Storage mpScaleSize;
	igdeMetaPropertyFloatStorage::Storage mpScaleVertexPositionSet;
	igdeMetaPropertyBooleanStorage::Storage mpEnablePosition;
	igdeMetaPropertyBooleanStorage::Storage mpEnableOrientation;
	igdeMetaPropertyBooleanStorage::Storage mpEnableSize;
	igdeMetaPropertyBooleanStorage::Storage mpEnableVertexPositionSet;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleForeignState::eCoordinateFrames>::Storage mpSourceCoordinateFrame;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleForeignState::eCoordinateFrames>::Storage mpDestCoordinateFrame;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetPosition;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetOrientation;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetSize;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetVertexPositionSet;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleForeignState() = delete;
	
	/** Create a new foreign state rule. */
	explicit aeRuleForeignState(aeWindowMain &windowMain, const char *name);
	/** Create a copy of a foreign state rule. */
	aeRuleForeignState(const aeRuleForeignState &copy);
	/** Clean up the foreign state rule. */
protected:
	~aeRuleForeignState() override;
private:
	aeRuleForeignState(aeWindowMain &windowMain, const char *name, const MetaContext::Ref &metaContext);
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Creates an engine animator rule. */
	deAnimatorRule::Ref CreateEngineRule() override;
	/** Update targets. */
	void UpdateTargets() override;
	/** Retrieve the number of targets using a given link. */
	int CountLinkUsage(aeLink *link) const override;
	
	/** Create a copy of this rule. */
	aeRule::Ref CreateCopy() const override;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	aeRuleForeignState &operator=(const aeRuleForeignState &copy) = delete;
	/*@}*/
};

#endif
