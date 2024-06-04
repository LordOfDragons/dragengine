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

#ifndef _IGDEGIZMO_H_
#define _IGDEGIZMO_H_


#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/resources/collider/deColliderRig.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/world/deWorld.h>

class igdeEnvironment;
class deModel;
class deRig;
class decCollisionFilter;
class deCollisionInfo;
class deCollider;


/**
 * \brief Interactive 3D gizmo.
 * 
 * Gizmo shape is initialized using the content of a deModel resource. For each material in
 * the model one deDebugDrawerShape is created. The colors to use for each shape can be set
 * up front using a list of named shape colors. Shapes with names matching this list will
 * be adjusted. Hence the list of set shape colors and the list of actual shapes do not have
 * to be the same. This allows to change the gizmo shape later on retaining the set shape
 * colors.
 * 
 * Gizmo collider is assigned a deRig resource which usually has only rig shapes. Shapes are
 * matches using the rig shape properties. Hence assign the rig shapes the name you want to
 * use to identify the actions to carry out when interacting with different parts of the gizmo.
 */
class DE_DLL_EXPORT igdeGizmo : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeGizmo> Ref;
	
	
	
private:
	class cShapeColor : public deObject {
	public:
		typedef deTObjectReference<cShapeColor> Ref;
		
		decString name;
		decColor color;
		int ddshapeIndex;
		
		cShapeColor( const char *aname, const decColor &acolor, int addshapeIndex ) :
		name( aname ), color( acolor ), ddshapeIndex( addshapeIndex ){
		}
	};
	
	
	
	igdeEnvironment &pEnvironment;
	
	deWorld::Ref pWorld;
	deDebugDrawer::Ref pDebugDrawer;
	deColliderRig::Ref pCollider;
	
	decObjectList pShapeColors;
	float pTransparency;
	float pHoverTransparency;
	decColor pHoverColorMultiply;
	
	decStringList pModelTextureNames;
	
	bool pIsEditing;
	bool pIsHovering;
	decString pHoverShapeName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create gizmo. */
	igdeGizmo( igdeEnvironment &environment );
	
	
	
protected:
	/**
	 * \brief Clean up gizmo.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeGizmo() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief World or nullptr. */
	inline const deWorld::Ref &GetWorld() const{ return pWorld; }
	
	/** \brief Set world or nullptr. */
	void SetWorld( deWorld *world );
	
	
	
	/** \brief Get color of named shape. */
	const decColor &GetShapeColor( const char *name ) const;
	
	/** \brief Set color of named shape. */
	void SetShapeColor( const char *name, const decColor &color );
	
	/** \brief Transparency. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/**
	 * \brief Set transparency.
	 * 
	 * Default value is 0.95 .
	 */
	void SetTransparency( float transparency );
	
	/** \brief Hover transparency. */
	inline float GetHoverTransparency() const{ return pHoverTransparency; }
	
	/**
	 * \brief Set hover transparency.
	 * 
	 * Default value is 0.45 .
	 */
	void SetHoverTransparency( float transparency );
	
	/** \brief Hover color multiply factor. */
	inline const decColor &GetHoverColorMultiply() const{ return pHoverColorMultiply; }
	
	/**
	 * \brief Set hover color multiply factor.
	 * 
	 * Default value is white.
	 */
	void SetHoverColorMultiply( const decColor &multiply );
	
	/** \brief Collision filter. */
	const decCollisionFilter &GetCollisionFilter() const;
	
	/** \brief Set collision filter. */
	void SetCollisionFilter( const decCollisionFilter &filter );
	
	
	
	/** \brief Set collider user pointer. */
	void SetColliderUserPointer( void *userPointer );
	
	
	
	/** \brief Gizmo is visible. */
	bool GetVisible() const;
	
	/** \brief Set if gizmo is visible. */
	void SetVisible( bool visible );
	
	/** \brief Position. */
	const decDVector &GetPosition() const;
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Orientation. */
	const decQuaternion &GetOrientation() const;
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Scale. */
	const decVector &GetScale() const;
	
	/** \brief Set scale. */
	void SetScale( const decVector &scale );
	
	/** \brief Set position and orientation. */
	void SetGeometry( const decDVector &position, const decQuaternion &orientation );
	void SetGeometry( const decDMatrix &matrix );
	
	/** \brief Gizmo matrix without scaling. */
	decDMatrix GetMatrix() const;

	
	
	/**
	 * \brief Set gizmo shape from model.
	 * 
	 * Each material is assigned an individual debug drawer shape. Stores the list of
	 * material names with the list index matching the debug drawer shape index.
	 */
	void SetShapeFromModel( const deModel &model );
	
	/** \brief Rig or nullptr. */
	deRig *GetRig() const;
	
	/** \brief Set rig or nullptr. */
	void SetRig( deRig *rig );
	/*@}*/
	
	
	
	/** \name Interaction */
	/*@{*/
	/**
	 * \brief Start hovering.
	 * 
	 * \param[in] rayOrigin Origin of ray hitting gizmo.
	 * \param[in] rayDirection Direction of ray (including length) hitting gizmo.
	 * \param[in] distance Distance from 0 to 1 along rayDirection up to hit point.
	 * \param[in] bone Index of bone hit by ray or -1.
	 * \param[in] shape Index of shape hit by ray or -1.
	 * \param[in] modifiers Modifier keys pressed at the time of event. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	void StartHovering( const decDVector &rayOrigin, const decDVector &rayDirection,
		double distance, int bone, int shape, int modifiers );
	
	/**
	 * \brief Update hovering.
	 * 
	 * Called if ray position or direction changed between StartHovering() and StopHovering().
	 * If StartEditing() is called and returns true while hovering no more UpdateHover() will
	 * be send but instead UpdateEditing().
	 * 
	 * \param[in] rayOrigin Origin of ray hitting gizmo.
	 * \param[in] rayDirection Direction of ray (including length) hitting gizmo.
	 * \param[in] distance Distance from 0 to 1 along rayDirection up to hit point.
	 * \param[in] bone Index of bone hit by ray or -1.
	 * \param[in] shape Index of shape hit by ray or -1.
	 * \param[in] modifiers Modifier keys pressed at the time of event. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	void UpdateHovering( const decDVector &rayOrigin, const decDVector &rayDirection,
		double distance, int bone, int shape, int modifiers );
	
	/** \brief Stop hovering. */
	void StopHovering();
	
	/**
	 * \brief Gizmo is hovering.
	 * 
	 * Returns true between StartHovering() and StopHovering() otherwise false.
	 */
	inline bool IsHovering() const{ return pIsHovering; }
	
	/** \brief Name of hover shape or empty string. */
	inline const decString &GetHoverShapeName() const{ return pHoverShapeName; }
	
	
	
	/**
	 * \brief Start editing.
	 * 
	 * If gizmo started editing true is returned. If editing can not be started false is returned.
	 * 
	 * \param[in] rayOrigin Origin of ray hitting gizmo.
	 * \param[in] rayDirection Direction of ray (including length) hitting gizmo.
	 * \param[in] viewMatrix View oriented matrix.
	 * \param[in] distance Distance from 0 to 1 along rayDirection up to hit point.
	 * \param[in] bone Index of bone hit by ray or -1.
	 * \param[in] shape Index of shape hit by ray or -1.
	 * \param[in] modifiers Modifier keys pressed at the time of event. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	bool StartEditing( const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDMatrix &viewMatrix, double distance, int bone, int shape, int modifiers );
	
	/**
	 * \brief Update editing.
	 * 
	 * Called if ray position or direction changed between StartEditing() returning
	 * true and StopEditing().
	 * 
	 * \param[in] rayOrigin Origin of ray.
	 * \param[in] rayDirection Normalized direction of ray.
	 * \param[in] viewMatrix View oriented matrix.
	 * \param[in] modifiers Modifier keys pressed at the time of event. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	void UpdateEditing( const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDMatrix &viewMatrix, int modifiers );
	
	/**
	 * \brief User wheeled mouse while editing.
	 * 
	 * \param[in] rayOrigin Origin of ray.
	 * \param[in] rayDirection Normalized direction of ray.
	 * \param[in] viewMatrix View oriented matrix.
	 * \param[in] modifiers Modifier keys pressed at the time of event. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	void MouseWheeledEditing( const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDMatrix &viewMatrix, const decPoint &change, int modifiers );
	
	/**
	 * \brief Stop editing.
	 * 
	 * \param[in] cancel Cancel editing if possible.
	 * 
	 * Called if StartEditing() returned true.
	 */
	void StopEditing( bool cancel );
	
	/**
	 * \brief Gizmo is editing.
	 * 
	 * Returns true between StartEditing() returning true and StopEditing() otherwise false.
	 */
	inline bool IsEditing() const{ return pIsEditing; }
	
	
	
	/**
	 * \brief Frame update.
	 * \param[in] elapsed Elapsed time.
	 */
	virtual void OnFrameUpdate( float elapsed );
	/*@}*/
	
	
	
protected:
	/**
	 * \brief Start hovering.
	 * 
	 * \param[in] rayOrigin Origin of ray hitting gizmo.
	 * \param[in] rayDirection Direction of ray (including length) hitting gizmo.
	 * \param[in] hitPoint Hit point on gizmo shape.
	 * \param[in] modifiers Modifier keys pressed at the time of event. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	virtual void OnStartHovering( const decDVector &rayOrigin,
		const decDVector &rayDirection, const decDVector &hitPoint, int modifiers );
	
	/**
	 * \brief Update hovering.
	 * 
	 * Called if ray position or direction changed between StartHovering() and StopHovering().
	 * If StartEditing() is called and returns true while hovering no more UpdateHover() will
	 * be send but instead UpdateEditing().
	 * 
	 * \param[in] rayOrigin Origin of ray hitting gizmo.
	 * \param[in] rayDirection Direction of ray (including length) hitting gizmo.
	 * \param[in] hitPoint Hit point on gizmo shape.
	 * \param[in] modifiers Modifier keys pressed at the time of event. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	virtual void OnUpdateHovering( const decDVector &rayOrigin,
		const decDVector &rayDirection, const decDVector &hitPoint, int modifiers );
	
	/** \brief Stop hovering. */
	virtual void OnStopHovering();
	
	
	
	/**
	 * \brief Start editing.
	 * 
	 * Called by StartEditing() if a named gizmo collider shape is hit. Subclass has to
	 * return true to start editing or false otherwise.
	 * 
	 * \param[in] rayOrigin Origin of ray hitting gizmo.
	 * \param[in] rayDirection Direction of ray (including length) hitting gizmo.
	 * \param[in] viewMatrix View oriented matrix.
	 * \param[in] hitPoint Hit point on gizmo shape.
	 * \param[in] shapeName Name of hit gizmo shape
	 * \param[in] modifiers Modifier keys pressed at the time of event. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	virtual bool OnStartEditing( const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDMatrix &viewMatrix, const decDVector &hitPoint, const decString &shapeName,
		int modifiers ) = 0;
	
	/**
	 * \brief Update editing.
	 * 
	 * Called between StartEditing() and StopEditing() on frame update.
	 * 
	 * \param[in] rayOrigin Origin of ray.
	 * \param[in] rayDirection Direction of ray (including length).
	 * \param[in] viewMatrix View oriented matrix.
	 * \param[in] modifiers Modifier keys pressed at the time of event. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	virtual void OnUpdateEditing( const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDMatrix &viewMatrix, int modifiers );
	
	/**
	 * \brief User wheeled mouse while editing.
	 * 
	 * \param[in] rayOrigin Origin of ray.
	 * \param[in] rayDirection Direction of ray (including length).
	 * \param[in] viewMatrix View oriented matrix.
	 * \param[in] modifiers Modifier keys pressed at the time of event. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	virtual void OnMouseWheeledEditing( const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDMatrix &viewMatrix, const decPoint &change, int modifiers );
	
	/**
	 * \brief Stop editing.
	 * 
	 * \param[in] cancel Cancel editing if possible.
	 * 
	 * Called by StopEditing() after a prior to an OnStartEditing() call returning true.
	 */
	virtual void OnStopEditing( bool cancel );
	
	
	
	/** \brief Added to world. */
	virtual void OnAddToWorld();
	
	/** \brief Removed from world. */
	virtual void OnRemoveFromWorld();
	
	
	
private:
	void pApplyShapeColors();
	cShapeColor *pNamedShapeColor( const char *name ) const;
	const decString &pCollisionShapeName( int bone, int shape ) const;
};

#endif
