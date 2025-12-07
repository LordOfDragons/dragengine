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

#ifndef _DEOGLPROPFIELD_H_
#define _DEOGLPROPFIELD_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicPropField.h>

class deoglRPropField;
class deoglPropFieldType;

class deGraphicOpenGl;
class dePropField;
class dePropFieldType;



/**
 * Prop field peer.
 */
class deoglPropField : public deBaseGraphicPropField{
private:
	deGraphicOpenGl &pOgl;
	dePropField &pPropField;
	
	deoglRPropField::Ref pRPropField;
	
	decPointerList pTypes;
	
	int pLODLevel;
	int pBestLOD;
	bool pCheckLOD;
	
	bool pDirtyPosition;
	bool pDirtyExtends;
	bool pDirtyTypes;
	bool pDirtyInstances;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new peer. */
	deoglPropField(deGraphicOpenGl &ogl, dePropField &propField);
	
	/** Clean up peer. */
	virtual ~deoglPropField();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl object. */
	inline deGraphicOpenGl &GetOpenGL() const{ return pOgl; }
	
	/** Prop field. */
	inline const dePropField &GetPropField() const{ return pPropField; }
	
	
	
	/** Render prop field. */
	inline const deoglRPropField::Ref &GetRPropField() const{ return pRPropField; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** LOD level. */
	inline int GetLODLevel() const{ return pLODLevel; }
	
	/** Set lod level not doing the updates yet. */
	void SetLODLevel(int level);
	
	/** Test lod level against the given camera point. */
	void TestLODLevel(const decDVector &camera);
	
	/** Update instance counts. */
	void UpdateInstanceCounts();
	
	
	
	/** Update prop field if required. */
	void Update();
	
	
	
	/** Number of types. */
	int GetTypeCount() const;
	
	/** Type at index. */
	deoglPropFieldType &GetTypeAt(int index) const;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Ground changed. */
	virtual void GroundChanged();
	
	
	
	/** Position changed. */
	virtual void PositionChanged();
	
	
	
	/** Type has been added. */
	virtual void TypeAdded(int index, dePropFieldType *type);
	
	/** Type has been removed. */
	virtual void TypeRemoved(int index, dePropFieldType *type);
	
	/** All types have been removed. */
	virtual void AllTypesRemoved();
	
	/** Type changed. */
	virtual void TypeChanged(int index, dePropFieldType *type);
	
	
	
	/** Instances changed. */
	virtual void InstancesChanged(int index, dePropFieldType *type);
	
	/** Instances to Bend States assignments changed. */
	virtual void AssignmentsChanged(int index, dePropFieldType *type);
	
	/** Bend States changed. */
	virtual void BendStatesChanged(int index, dePropFieldType *type);
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
