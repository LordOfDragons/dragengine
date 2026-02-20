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

#ifndef _SELOADSAVESKIN_H_
#define _SELOADSAVESKIN_H_

#include "../skin/property/node/sePropertyNodeGroup.h"

#include <deigde/engine/textureProperties/igdeTextureProperty.h>

#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

class seSkin;
class sePropertyNode;
class deBaseSkinModule;
class deSkinPropertyNode;
class deSkinPropertyNodeGroup;
class decBaseFileReader;
class decBaseFileWriter;


/**
 * \brief Load/Saves skins using an engine module.
 */
class seLoadSaveSkin : public deObject{
public:
	typedef deTObjectReference<seLoadSaveSkin> Ref;
	typedef decTObjectOrderedSet<seLoadSaveSkin> List;
	
private:
	deBaseSkinModule *pModule;
	
	decString pName;
	decString pPattern;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new load/save object. */
	explicit seLoadSaveSkin(deBaseSkinModule *module);
	
protected:
	/** \brief Clean up load/save object. */
	~seLoadSaveSkin();
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Set pattern. */
	void SetPattern(const char *pattern);
	
	
	
	/** \brief Load skin. */
	void LoadSkin(seSkin *skin, decBaseFileReader *file, const igdeTextureProperty::List &knownPropertyList);
	
	/** \brief Load propery node group. */
	sePropertyNodeGroup::Ref LoadPropertyNodeGroup(seSkin &skin, const deSkinPropertyNodeGroup &engNodeGroup);
	
	/** \brief Load propery node. */
	sePropertyNode::Ref LoadPropertyNode(seSkin &skin, deSkinPropertyNode &engNode);
	
	/** \brief Load common propery node parameters. */
	void LoadPropertyNodeCommon(seSkin &skin, sePropertyNode &node, const deSkinPropertyNode &engNode);
	
	/** \brief Save skin. */
	void SaveSkin(seSkin *skin, decBaseFileWriter *file);
	/*@}*/
};

#endif
