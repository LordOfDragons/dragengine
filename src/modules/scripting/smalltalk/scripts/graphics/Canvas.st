" Smalltalk Class DECanvas
|
| Copyright (C) 2015, Plüss Roland ( roland@rptd.ch )
|
| This program is free software; you can redistribute it and/or 
| modify it under the terms of the GNU General Public License 
| as published by the Free Software Foundation; either 
| version 2 of the License, or (at your option) any later 
| version.
| 
| This program is distributed in the hope that it will be useful,
| but WITHOUT ANY WARRANTY; without even the implied warranty of
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
| GNU General Public License for more details.
| 
| You should have received a copy of the GNU General Public License
| along with this program; if not, write to the Free Software
| Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA."

Object subclass: DECanvas [
	<comment: 'I am a canvas for rendering 2D content.
		Canvas are the basic elements of 2D rendering. At the top stands a deCanvasView
		representing a view on a set of child canvas. A canvas has a position and size
		relative to the parent canvas view coordinate system. The center of the canvas
		is located at the position affecting a size dimensioned area around the center.
		An additional transformation matrix allows to apply transformation effects to
		the canvas and its coordinate system. The canvas coordinate system maps
		(-size.x/2,-size.y/2) to the top-left of the canvas and (size.x/2,size.y/2) to
		the lower-right. This coordinate system is then eventually transformed by the
		transformation matrix. Units are measured as floating points values in pixels
		unless transformed. Canvas can be set invisible hiding the canvas and all content
		inside. The order of the canvas defines the render order with canvas of higher
		order rendered above those with smaller number. This uncouples the render
		ordering from the actual ordering of the canvas objects in a deCanvasView. If
		two canvas have the same order the render order is undefined.'>
	<shape: #byte>
	
	DECanvas class [
		| scripting |
		
		new [
			self error: #shouldNotImplement
		]
		
		new: aSymbol [
			<category: 'initialization'>
			"Create new canvas with type. Supported types are:
			 #image, #paint, #renderTarget, #renderWorld, #text, #videoPlayer or #view"
			<cCall: 'DECanvas.new' returning: #smalltalk args: #(#self #smalltalk)>
		]
	]
	
	initialize [
		<category: 'initialization'>
		"Registers the object for finalization."
		self addToBeFinalized
	]
	
	finalize [
		<category: 'finalization'>
		"Release engine resources."
		<cCall: 'DECanvas.finalize' returning: #void args: #(#self)>
	]
	
	
	
	type [
		<category: 'accessors'>
		"Type of canvas. Returns #image, #paint, #renderTarget, #renderWorld, #text,
         #videoPlayer or #view"
		<cCall: 'DECanvas.getType' returning: #smalltalk args: #(#self)>
	]
	
	position [
		<category: 'accessors'>
		"Position in parent canvas view units."
		<cCall: 'DECanvas.getPosition' returning: #smalltalk args: #(#self)>
	]
	
	position: aDEPoint [
		<category: 'accessors'>
		"Set position in parent canvas view units."
		<cCall: 'DECanvas.setPosition' returning: #void args: #(#self #smalltalk)>
	]
	
	size [
		<category: 'accessors'>
		"Size in parent canvas view units."
		<cCall: 'DECanvas.getSize' returning: #smalltalk args: #(#self)>
	]
	
	size: aDEPoint [
		<category: 'accessors'>
		"Set size in parent canvas view units."
		<cCall: 'DECanvas.setSize' returning: #void args: #(#self #smalltalk)>
	]
	
	transform [
		<category: 'accessors'>
		"Transformation matrix affecting the canvas and all its content."
		<cCall: 'DECanvas.getTransform' returning: #smalltalk args: #(#self)>
	]
	
	transform: aDETexMatrix2 [
		<category: 'accessors'>
		"Set transformation matrix affecting the canvas and all its content."
		<cCall: 'DECanvas.setTransform' returning: #void args: #(#self #smalltalk)>
	]
	
	visible [
		<category: 'accessors'>
		"Canvas is visible."
		<cCall: 'DECanvas.getVisible' returning: #boolean args: #(#self)>
	]
	
	visible: aBoolean [
		<category: 'accessors'>
		"Set if canvas is visible."
		<cCall: 'DECanvas.setVisible' returning: #void args: #(#self #boolean)>
	]
	
	order [
		<category: 'accessors'>
		"Render order. Larger renders ontop of smaller."
		<cCall: 'DECanvas.getOrder' returning: #double args: #(#self)>
	]
	
	order: aDouble [
		<category: 'accessors'>
		"Set render order. Larger renders ontop of smaller."
		<cCall: 'DECanvas.setOrder' returning: #void args: #(#self #double)>
	]
	
	transparency [
		<category: 'accessors'>
		"Transparency in the range from 0 to 1 where 1 is opaque and 0 fully transparent."
		<cCall: 'DECanvas.getTransparency' returning: #double args: #(#self)>
	]
	
	transparency: aDouble [
		<category: 'accessors'>
		"Set transparency in the range from 0 to 1 where 1 is opaque and 0 fully transparent."
		<cCall: 'DECanvas.setTransparency' returning: #void args: #(#self #double)>
	]
	
	
	
	image [
		<category: 'accessors'>
		"Image or nil if not set."
		<cCall: 'DECanvas.getImage' returning: #smalltalk args: #(#self)>
	]
	
	image: aDEImage [
		<category: 'accessors'>
		"Set image or nil to unset."
		<cCall: 'DECanvas.getType' returning: #void args: #(#self #smalltalk)>
	]
	
	repeatX [
		<category: 'accessors'>
		"Repeat count along X axis."
		<cCall: 'DECanvas.getRepeatX' returning: #int args: #(#self)>
	]
	
	repeatX: anInteger [
		<category: 'accessors'>
		"Set repeat count along X axis."
		<cCall: 'DECanvas.setRepeatX' returning: #void args: #(#self #int)>
	]
	
	repeatY [
		<category: 'accessors'>
		"Repeat count along Y axis."
		<cCall: 'DECanvas.getRepeatY' returning: #int args: #(#self)>
	]
	
	repeatY: anInteger [
		<category: 'accessors'>
		"Set repeat count along Y axis."
		<cCall: 'DECanvas.setRepeatY' returning: #void args: #(#self #int)>
	]
	
	
	
	shapeType [
		<category: 'accessors'>
		"Shape type. Returns #points, #lines, #rectangle, #ellipse, #pie or #polygon."
		<cCall: 'DECanvas.getShapeType' returning: #smalltalk args: #(#self)>
	]
	
	shapeType: aSymbol [
		<category: 'accessors'>
		"Set shape type. Supported types are: #points, #lines, #rectangle, #ellipse, #pie or #polygon"
		<cCall: 'DECanvas.setShapeType' returning: #void args: #(#self #smalltalk)>
	]
	
	lineColor [
		<category: 'accessors'>
		"Line color."
		<cCall: 'DECanvas.getLineColor' returning: #smalltalk args: #(#self)>
	]
	
	lineColor: aDEColor [
		<category: 'accessors'>
		"Set line color."
		<cCall: 'DECanvas.setLineColor' returning: #void args: #(#self #smalltalk)>
	]
	
	fillColor [
		<category: 'accessors'>
		"Fill color."
		<cCall: 'DECanvas.getFillColor' returning: #smalltalk args: #(#self)>
	]
	
	fillColor: aDEColor [
		<category: 'accessors'>
		"Set fill color."
		<cCall: 'DECanvas.setFillColor' returning: #void args: #(#self #smalltalk)>
	]
	
	thickness [
		<category: 'accessors'>
		"Line thickness in units."
		<cCall: 'DECanvas.getThickness' returning: #double args: #(#self)>
	]
	
	thickness: aDouble [
		<category: 'accessors'>
		"Line thickness in units."
		<cCall: 'DECanvas.setThickness' returning: #void args: #(#self #double)>
	]
	
	pointCount [
		<category: 'accessors'>
		"Number of points."
		<cCall: 'DECanvas.getPointCount' returning: #int args: #(#self)>
	]
	
	pointAt: anInteger [
		<category: 'accessors'>
		"Point at position."
		<cCall: 'DECanvas.getPointAt' returning: #smalltalk args: #(#self)>
	]
	
	addPoint: aDEPoint [
		<category: 'accessors'>
		"Add a point."
		<cCall: 'DECanvas.addPoint' returning: #void args: #(#self #smalltalk)>
	]
	
	removeAllPoints [
		<category: 'accessors'>
		"Remove all points."
		<cCall: 'DECanvas.removeAllPoints' returning: #void args: #(#self)>
	]
	
	
	
	renderTarget [
		<category: 'accessors'>
		"Render target or nil if not set."
		<cCall: 'DECanvas.getRenderTarget' returning: #smalltalk args: #(#self)>
	]
	
	renderTarget: aDERenderTarget [
		<category: 'accessors'>
		"Set render target or nil to unset."
		<cCall: 'DECanvas.setRenderTarget' returning: #void args: #(#self #smalltalk)>
	]
	
	
	
	camera [
		<category: 'accessors'>
		"Camera or nil if not set."
		<cCall: 'DECanvas.getCamera' returning: #smalltalk args: #(#self)>
	]
	
	camera: aDECamera [
		<category: 'accessors'>
		"Set camera or nil to unset."
		<cCall: 'DECanvas.setCamera' returning: #void args: #(#self #smalltalk)>
	]
	
	
	
	font [
		<category: 'accessors'>
		"Font or nil if not set."
		<cCall: 'DECanvas.getFont' returning: #smalltalk args: #(#self)>
	]
	
	font: aDEFont [
		<category: 'accessors'>
		"Set font or nil to unset."
		<cCall: 'DECanvas.setFont' returning: #void args: #(#self #smalltalk)>
	]
	
	text [
		<category: 'accessors'>
		"Text."
		<cCall: 'DECanvas.getText' returning: #string args: #(#self)>
	]
	
	text: aString [
		<category: 'accessors'>
		"Set text."
		<cCall: 'DECanvas.setText' returning: #void args: #(#self #string)>
	]
	
	color [
		<category: 'accessors'>
		"Text color."
		<cCall: 'DECanvas.getColor' returning: #smalltalk args: #(#self)>
	]
	
	color: aDEColor [
		<category: 'accessors'>
		"Set text color."
		<cCall: 'DECanvas.setColor' returning: #void args: #(#self #smalltalk)>
	]
	
	
	
	videoPlayer [
		<category: 'accessors'>
		"Video player or nil if not set."
		<cCall: 'DECanvas.getVideoPlayer' returning: #smalltalk args: #(#self)>
	]
	
	videoPlayer: aDEVideoPlayer [
		<category: 'accessors'>
		"Set video player or nil to unset."
		<cCall: 'DECanvas.setVideoPlayer' returning: #void args: #(#self #smalltalk)>
	]
	
	
	
	canvasCount: aDECanvas [
		<category: 'accessors'>
		"Number of child canvas."
		<cCall: 'DECanvas.getCanvasCount' returning: #int args: #(#self)>
	]
	
	addCanvas: aDECanvas [
		<category: 'accessors'>
		"Add child canvas."
		<cCall: 'DECanvas.addCanvas' returning: #void args: #(#self #smalltalk)>
	]
	
	removeCanvas: aDECanvas [
		<category: 'accessors'>
		"Remove child canvas."
		<cCall: 'DECanvas.removeCanvas' returning: #void args: #(#self #smalltalk)>
	]
	
	removeAllCanvas [
		<category: 'accessors'>
		"Remove all child canvas."
		<cCall: 'DECanvas.removeAllCanvas' returning: #void args: #(#self)>
	]
]
