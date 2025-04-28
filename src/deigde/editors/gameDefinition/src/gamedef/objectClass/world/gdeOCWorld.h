/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _GDEOCWORLD_H_
#define _GDEOCWORLD_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/world/deWorld.h>


/**
 * \brief Object class world.
 */
class gdeOCWorld : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<gdeOCWorld> Ref;
	
	/** \brief Properties. */
	enum eProperties{
		/** \brief Path to world file. */
		epPath,
		
		/** \brief Position. */
		epPosition,
		
		/** \brief Rotation. */
		epRotation
	};
	
	static const int PropertyCount = epRotation + 1;
	
	
private:
	decString pPath;
	decVector pPosition, pRotation;
	decString pPropertyNames[PropertyCount];
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create world. */
	gdeOCWorld();
	
	/** \brief Create copy of world. */
	gdeOCWorld(const gdeOCWorld &world);
	
protected:
	/** \brief Clean up world. */
	~gdeOCWorld() override = default;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Path to use. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set path to use. */
	void SetPath(const char *path);
	
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition(const decVector &position);
	
	/** \brief Rotation. */
	inline const decVector &GetRotation() const{ return pRotation; }
	
	/** \brief Set orientation. */
	void SetRotation(const decVector &orientation);
	
	
	/** \brief Property name is set. */
	bool IsPropertySet(eProperties property) const;
	
	/** \brief Name of property. */
	const decString &GetPropertyName(eProperties property) const;
	
	/** \brief Set name of property. */
	void SetPropertyName(eProperties property, const char *name);
	
	/** \brief One or more properties use a name. */
	bool HasPropertyWithName(const char *name) const;
	/*@}*/
};

#endif
