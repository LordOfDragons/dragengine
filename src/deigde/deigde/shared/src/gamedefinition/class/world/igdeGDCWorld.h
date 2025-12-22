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

#ifndef _IGDEGDCWORLD_H_
#define _IGDEGDCWORLD_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/resources/collider/deCollider.h>


/**
 * \brief Game definition world.
 * 
 * Contains information about a world used to display the element inside the editor.
 */
class DE_DLL_EXPORT igdeGDCWorld : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeGDCWorld> Ref;
	
	/** \brief Type holding list of objects. */
	typedef decTObjectOrderedSet<igdeGDCWorld> List;
	
	
public:
	/** \brief Properties. */
	enum eProperties{
		epPath,
		epPosition,
		epRotation
	};
	static const int PropertyNameCount = epRotation + 1;
	
	
private:
	decString pPath;
	decDVector pPosition;
	decQuaternion pOrientation;
	
	decString pPropertyNames[PropertyNameCount];
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game definition world. */
	igdeGDCWorld();
	
	/** \brief Create copy of a game definition world. */
	igdeGDCWorld(const igdeGDCWorld &world);
	
	/** \brief Clean up game definition world. */
	~igdeGDCWorld() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Path. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set path. */
	void SetPath(const char *path);
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition(const decDVector &position);
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation(const decQuaternion &orientation);
	
	/** \brief Determines if the name of a property is set or not (empty string). */
	bool IsPropertySet(eProperties property) const;
	
	/** \brief Name of the given property. */
	const decString &GetPropertyName(eProperties property) const;
	
	/** \brief Set name of the given property. */
	void SetPropertyName(eProperties property, const char *name);
	
	/** \brief Determines if one or more properties use a name. */
	bool HasPropertyWithName(const char *name) const;
	/*@}*/
};

#endif
