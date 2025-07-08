import { PanelLogs } from './panelLogs.mjs'
import { PanelSettings } from './panelSettings.mjs';
import { PanelCanvas } from './panelCanvas.mjs';

/**
 * Launcher class.
 */
class WebLauncher {
	/**
	 * Create web launcher
	 */
	constructor() {
		this.panelCanvas = new PanelCanvas('panel-canvas')
		
		this.panelSettings = new PanelSettings('panel-settings')
		this.panelSettings.showPanel()
		
		this.panelLogs = new PanelLogs('panel-logs')
		//this.panelLogs.showPanel()
		
		this._launcher = undefined
		
		window.onLogMessage = this.panelLogs.addLogs.bind(this.panelLogs)
	}
	
	/**
	 * Web page finished loading.
	 */
	onPageLoaded() {
		this.panelCanvas.resizeCanvas()
	}
	
	/**
	 * Web page resized.
	 */
	onPageResize() {
		this.panelCanvas.resizeCanvas()
	}
	
	/**
	 * Loading wasm finished.
	 */
	onWasmReady() {
		this.panelLogs.addLogs(PanelLogs.Severity.Info, 'Launcher', "WASM initialized")
		
		this._launcher = new Module.dewlLauncher()
		try {
			this._launcher.AddArgument("this_is_a_test")
			this._launcher.AddArgument("something_else")
			
			var args_in = this._launcher.GetArgumentList()
			var args = new Array(args_in.size()).fill(0).map((_,i) => args_in.get(i))
			this.panelLogs.addLogs(PanelLogs.Severity.Info, 'Launcher', "Arguments: " + args)
			
			this._launcher.Init()
			this._launcher.Run()
			
		} finally {
			this._launcher.CleanUp()
			this._launcher.delete()
			this._launcher = undefined
		}
		this.panelLogs.addLogs(PanelLogs.Severity.Info, 'Launcher', 'Done')
	}
}

window.webLauncher = new WebLauncher()
