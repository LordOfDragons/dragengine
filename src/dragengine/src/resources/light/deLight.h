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

#ifndef _DELIGHT_H_
#define _DELIGHT_H_

#include "../deResource.h"
#include "../canvas/deCanvasViewReference.h"
#include "../skin/deSkinReference.h"
#include "../skin/dynamic/deDynamicSkinReference.h"
#include "../../common/collection/decObjectSet.h"
#include "../../common/math/decMath.h"
#include "../../common/shape/decShapeList.h"
#include "../../common/utils/decLayerMask.h"

class deComponent;
class deBaseGraphicLight;
class deLightManager;
class deWorld;
class decShape;


/**
 * \brief Light source.
 *
 * Places a local light source in the world. Light sources have various parameters
 * influencing the light appearance. The color indicates the tint of the light
 * source while the intensities indicate the strength of the light source.
 * 
 * Intensities are measured in an engine specific version of lumen. A value of 1
 * equals to a lit living room. A value of 20 equals to direct sun light at a sunny
 * and clear sky. This is a resclaing of lumen scala found in literature to avoid
 * large values.
 * 
 * The ambient ratio defines the amount of intensity affecting objects in shadow.
 * Using ambient intensity larger than 0 allows to simulate overhead lights in rooms
 * without needing multiple light sources to cancel out pitch black areas. Using
 * fewer light sources is faster than more. The ambient ratio can be set in the
 * range from 0 to 1. Increasing the ambient ratio only affects shadow areas. The
 * light intensity is not altered.
 * 
 * The half intensity distance and the attenuation exponent determine how the light
 * attenuates over distance. This model is more powerful than the typical static
 * linear-quadratic attenuation model and requires also less parameters. The light
 * is attenuated to half the strength reaching the distance set in the half intensity
 * distance. The attenuation exponent indicates the power to which the distance of a
 * fragment to the light source is raised. A value of 2 yields physically correct
 * lighting while larger values produce dramatic and harsher lighting effects like
 * light staying mostly constant over a given distance then falling off sharply.
 * 
 * The light cage is a hint for graphic modules to optimize lighting. Objects
 * outside the cage shape should not be reachable by the light source. If set
 * properly performance can be increased due to the graphic module requiring less
 * objects to be considered for lighting or shadow casting.
 * 
 * By default light sources have a uniform light color attenuated by the light type
 * and other factors. The light color can be modified using an image, video or canvas.
 * The required image type depends on the light type. For spot and projector lights
 * regular flat images are used. For point lights 3D images are required. The texture
 * coordinates used to sample the image are predefined similar to skin textures.
 * 
 * Lights can be set to not cast shadows. This way point lights can be used to
 * simulate ambient lighting.
 * 
 * The layer mask defines which cameras are affected by light. Matches layer masks in
 * deCamera. The shadow layer mask defines which world content does cast shadows.
 * Matches layer masks in deComponent. If empty both layer mask match everything. A
 * typical use for these layer masks is to use two bits. The light layer mask has the
 * first bit set and the shadow layer mask the second. Cameras have the first bit set
 * so the light is used during rendering. All components have the first bit set to be
 * rendered with the same camera and receive lighting. Only those casting shadows have
 * also the second bit set. This allows to modify shadow casting on a per component
 * level without modifying skin texture properties.
 * 
 * It is also possible to add specific components to a list of components to ignore
 * for shadow casting. This is useful for light source models. The light can be
 * placed inside the model where it is physically correctly located without the
 * model itself causing troubles to the light. Only this light ignores this specific
 * component. All other lights still cast shadows using this specific component.
 * 
 * Light shape can be used to create area lights. It is best to use one shape in
 * the list but potentially more can be set. How well graphic module can handle
 * this situation differs.
 * 
 * 
 * 
 * \par Point Light
 * 
 * Point lights emmit light in all directions equally. No additional parameters are
 * used for this light type. Point lights are simple to use but often more expensive
 * than a spot or projector lights especially for shadow casting.
 * 
 * If a light image is used the image data has to be a 3D image. If the image has a
 * depth of 1 it has to be a spherical image (mercartor projection). If the image has
 * a depth of 6 it has to be a cube map type image with each layer representing one
 * cube map face. The order is X+, X-, Y+, Y-, Z+ and Z-. The image is oriented using
 * the light coordinate frame. The light direction equals to the Z+.
 * 
 * 
 * 
 * \par Spot and Projector Light
 * 
 * Spot lights emit light in a cone shape. The cone is defined using the light
 * orientation. The angle parameters define the inner and outer angle of the cone.
 * The attenuation due to the cone is multiplied with the distance attenuation.
 * Inside the inner angle no attenuation from the cone is contributed. Outside the
 * outer angle light is attenuated to 0. For angles between the two cone angles an
 * exponential decay is used with the spot exponent indicating the power to use. With
 * these three parameters a large variety of spot lights can be modeled.
 * 
 * If a light image is used the image data has to be a 2D image. The image is applied
 * as if the light source shines through the image like through a slide. The image
 * boundary lines up with the outer cone angle. Due to the cone shape only a radial
 * area fitting into the image are used.
 * 
 * 
 * 
 * \par Projector Light Source
 * 
 * Projector lights work similar to spot lights but have a pyramidal shape. This allows
 * to have rectangular shaped lights whereas spot lights are always symmetrical. The
 * angle parameters define the width and height of the pyramid.
 */
class DE_DLL_EXPORT deLight : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deLight> Ref;
	
	
	
public:
	/** \brief Light type. */
	enum eLightTypes{
		/** \brief Point light. */
		eltPoint,
		
		/** \brief Spot light. */
		eltSpot,
		
		/** \brief Projector light. */
		eltProjector
	};
	
	/** \brief Movement hints. */
	enum eMovementHints{
		/** \brief Light remains static for the entire lifetime. */
		emhStationary,
		
		/** \brief Light remains mostly static jittering in a small area. */
		emhJittering,
		
		/** \brief Light moves around freely. */
		emhDynamic
	};
	
	/** \brief Parameter hints. */
	enum eParameterHints{
		/** \brief Light parameters never change. */
		ephStatic,
		
		/** \brief Light parameters never change except activation. */
		ephActivation,
		
		/** \brief Geometric light parameters never change but others can. */
		ephFlicker,
		
		/** \brief All light parameters can change at will any time. */
		ephDynamic
	};
	
	
	
private:
	eLightTypes pType;
	decColor pColor;
	float pIntensity;
	float pRange;
	float pHalfIntensityDistance;
	float pAmbientRatio;
	
	decDVector pPosition;
	decQuaternion pOrientation;
	float pSpotAngle;
	float pSpotRatio;
	float pSpotSmoothness;
	float pSpotExponent;
	decShapeList pShape;
	
	deSkinReference pLightSkin;
	deCanvasViewReference pLightCanvas;
	deDynamicSkinReference pDynamicSkin;
	decTexMatrix2 pTransform;
	
	bool pActivated;
	bool pCastShadows;
	
	int pHintLightImportance;
	int pHintShadowImportance;
	eMovementHints pHintMovement;
	eParameterHints pHintParameter;
	decShapeList pCage;
	
	decLayerMask pLayerMask;
	decLayerMask pLayerMaskShadow;
	decObjectSet pShadowIgnoreComponents;
	
	deBaseGraphicLight *pPeerGraphic;
	
	deWorld *pParentWorld;
	deLight *pLLWorldPrev;
	deLight *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create light source. */
	deLight( deLightManager *manager );
	
protected:
	/**
	 * \brief Clean up light source.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deLight();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Light type. */
	inline eLightTypes GetType() const{ return pType; }
	
	/** \brief Set light type. */
	void SetType( eLightTypes type );
	
	/** \brief Light color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set light color. */
	void SetColor( const decColor &color );
	
	/** \brief Intensity. */
	inline float GetIntensity() const{ return pIntensity; }
	
	/** \brief Set intensity. */
	void SetIntensity( float intensity );
	
	/** \brief Range in meters. */
	inline float GetRange() const{ return pRange; }
	
	/** \brief Set light range in meters. */
	void SetRange( float range );
	
	/** \brief Distance in meters at which the intensity is halved. */
	inline float GetHalfIntensityDistance() const{ return pHalfIntensityDistance; }
	
	/** \brief Set distance in meters at which the intensity is halved. */
	void SetHalfIntensityDistance( float distance );
	
	/** \brief Ratio of ambient light in relation to intensity. */
	inline float GetAmbientRatio() const{ return pAmbientRatio; }
	
	/** \brief Set ratio of ambient light in relation to intensity. */
	void SetAmbientRatio( float ratio );
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Outer spot Angle. */
	inline float GetSpotAngle() const{ return pSpotAngle; }
	
	/** \brief Set outer spot angle. */
	void SetSpotAngle( float angle );
	
	/** \brief Spot ratio as height divided by width. */
	inline float GetSpotRatio() const{ return pSpotRatio; }
	
	/** \brief Set spot ratio as height divided by width. */
	void SetSpotRatio( float ratio );
	
	/** \brief Spot smoothness. */
	inline float GetSpotSmoothness() const{ return pSpotSmoothness; }
	
	/** \brief Set spot smoothness. */
	void SetSpotSmoothness( float smoothness );
	
	/** \brief Spot exponent. */
	inline float GetSpotExponent() const{ return pSpotExponent; }
	
	/** \brief Set spot exponent. */
	void SetSpotExponent( float exponent );
	
	/** \brief Light shape. */
	inline const decShapeList &GetShape() const{ return pShape; }
	
	/** \brief Set light shape. */
	void SetShape( const decShapeList &shape );
	
	
	
	/** \brief Light skin or NULL if not used. */
	inline deSkin *GetLightSkin() const{ return pLightSkin; }
	
	/** \brief Set light skin or NULL if not used. */
	void SetLightSkin( deSkin *skin );
	
	/** \brief Light canvas view or NULL if not used. */
	inline deCanvasView *GetLightCanvas() const{ return pLightCanvas; }
	
	/** \brief Set light canvas view or NULL if not used. */
	void SetLightCanvas( deCanvasView *canvas );
	
	/** \brief Light dynamic skin or NULL if not used. */
	inline deDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** \brief Set light dynamic skin or NULL if not used. */
	void SetDynamicSkin( deDynamicSkin *dynamicSkin );
	
	/** \brief Light texture coordinate transformation matrix. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** \brief Set light texture coordinate transformation matrix. */
	void SetTransform( const decTexMatrix2 &matrix );
	
	
	
	/** \brief Hint light importance. */
	inline int GetHintLightImportance() const{ return ( int )pHintLightImportance; }
	
	/** \brief Set hint light importance. */
	void SetHintLightImportance( int importance );
	
	/** \brief Hint shadow importance. */
	inline int GetHintShadowImportance() const{ return ( int )pHintShadowImportance; }
	
	/** \brief Set hint shadow importance. */
	void SetHintShadowImportance( int importance );
	
	/** \brief Movement hint. */
	inline eMovementHints GetHintMovement() const{ return pHintMovement; }
	
	/** \brief Set movement hint. */
	void SetHintMovement( eMovementHints hint );
	
	/** \brief Parameter hint. */
	inline eParameterHints GetHintParameter() const{ return pHintParameter; }
	
	/** \brief Set parameter hint. */
	void SetHintParameter( eParameterHints hint );
	
	/** \brief Light cage. */
	inline const decShapeList &GetCage() const{ return pCage; }
	
	/** \brief Set light cage. */
	void SetCage( const decShapeList &cage );
	
	
	
	/** \brief Layer mask affecting lighting. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask affecting lighting. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** \brief Layer mask affecting shadow casting. */
	inline const decLayerMask &GetLayerMaskShadow() const{ return pLayerMaskShadow; }
	
	/** \brief Set layer mask affecting shadow casting. */
	void SetLayerMaskShadow( const decLayerMask &layerMask );
	
	
	
	/** \brief Light is activated. */
	inline bool GetActivated() const{ return pActivated; }
	
	/** \brief Set if light is activated. */
	void SetActivated( bool activated );
	
	/** \brief Light casts shadows. */
	inline bool GetCastShadows() const{ return pCastShadows; }
	
	/** \brief Set if light casts shadows. */
	void SetCastShadows( bool castShadows );
	/*@}*/
	
	
	
	/** \name Shadow ignore components */
	/*@{*/
	/** \brief Number of components to ignore for shadow casting. */
	int GetShadowIgnoreComponentCount() const;
	
	/**
	 * \brief Component to ignore for shadow casting at index.
	 * \throws deeInvalidParam \em index is less than 0.
	 * \throws deeInvalidParam \em index is greater or equal than GetIgnoreComponentCount()-1.
	 */
	deComponent *GetShadowIgnoreComponentAt( int index ) const;
	
	/** \brief Component to ignore for shadow casting is present. */
	bool HasShadowIgnoreComponent( deComponent *component ) const;
	
	/**
	 * \brief Add component to ignore for shadow casting.
	 * \throws deeInvalidParam \em component is present.
	 */
	void AddShadowIgnoreComponent( deComponent *component );
	
	/**
	 * \brief Remove component to ignore for shadow casting.
	 * \throws deeInvalidParam \em component is absent.
	 */
	void RemoveShadowIgnoreComponent( deComponent *component );
	
	/** \brief Remove all components to ignore for shadow casting. */
	void RemoveAllShadowIgnoreComponents();
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer. */
	inline deBaseGraphicLight *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer. */
	void SetPeerGraphic( deBaseGraphicLight *peer );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( deWorld *world );
	
	/** \brief Previous light in the parent world linked list. */
	inline deLight *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next light in the parent world linked list. */
	void SetLLWorldPrev( deLight *light );
	
	/** \brief Next light in the parent world linked list. */
	inline deLight *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next light in the parent world linked list. */
	void SetLLWorldNext( deLight *light );
	/*@}*/
};

#endif
