/**
 * Canvas panel.
 */
export class PanelCanvas {
	/**
	 * Create canvas panel.
	 * @param {String} panelId Panel identifier
	 */
	constructor (panelId) {
		this._panelId = panelId
		this._panel = document.getElementById(panelId)
		this._panelParent = this._panel.parentElement
	}
	
	/**
	 * Panel ID.
	 */
	panelId() {
		return this._panelId
	}
	
	/**
	 * Panel width.
	 */
	panelWidth() {
		return this._panel.width
	}
	
	/**
	 * Panel height.
	 */
	panelHeight() {
		return this._panel.height
	}
	
	/**
	 * Resize canvas to fit parent size.
	 */
	resizeCanvas() {
		// different parent tags have different size parameters
		//
		// - body: scrollHeight, offsetHeight
		// - html: clientHeight, scrollHeight, offsetHeight
		// - canvas: width, height
		// - ??: innerWidth, innerHeight
		
		this._panel.width = this._panelParent.clientWidth;
		this._panel.height = this._panelParent.clientHeight;
	}
	
	/**
	 * Fullscreen mode enabled.
	 */
	isFullScreen() {
		return document.fullscreenElement === this._panelParent
	}
	
	/**
	 * Enable full screen mode.
	 */
	enableFullScreen() {
		if (this._panelParent.requestFullscreen) {
			this._panelParent.requestFullscreen()
		}
	}
	
	/**
	 * Disable full screen mode.
	 */
	disableFullScreen() {
		if (document.exitFullscreen) {
			document.exitFullscreen()
		}
	}
	
	/**
	 * Toggle full screen mode.
	 */
	toggleFullScreen() {
		if (this.isFullScreen()) {
			this.disableFullScreen()
		} else {
			this.enableFullScreen()
		}
	}
}
