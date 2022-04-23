/*****************************************************
 *
 * NodeMCU Test Firmware - Web interface
 *
 * (c) 2022 Fabien Pollet <polletfa@posteo.de>
 * MIT License (see LICENSE.md file)
 *
 *****************************************************/

import { FrontendApplication } from './FrontendApplication';

/**
 * API request
 */
export class APIRequest {
    protected static nRequests = 0;

    protected ui: FrontendApplication;
    
    protected onReceiveHandler: ((resp:string)=>void)|undefined = undefined;
    protected onFinishHandler: ((success:boolean)=>void)|undefined = undefined;

    protected silent: boolean;
    
    constructor(ui: FrontendApplication, request: string, silent = false) {
        this.ui = ui;
        this.silent = silent;
        if(!silent) {
            this.ui.layout.show("error-banner", false);
            this.ui.layout.show("loading-spinner", true);
        }
        new Promise<void>(() => {
            const httpRequest = new XMLHttpRequest();
            if(!silent) APIRequest.nRequests ++;
            httpRequest.open("GET", request, true);
            httpRequest.responseType = "arraybuffer";
            httpRequest.send();

            
            httpRequest.addEventListener("error", () => {
                this.fail("Unable to contact the controller", "XMLHttpRequest error.");
            });
            httpRequest.addEventListener("readystatechange", () => {
                if (httpRequest.readyState === httpRequest.DONE) {
                    if(httpRequest.status === 200) {
                        this.handleResponse(httpRequest.response);
                    } else {
                        this.fail("The controller responded with an error code", "Response code: "+httpRequest.status);
                    }            
                }
            });
        });
    }

    public static getActiveRequests(): number { return APIRequest.nRequests; }
    
    public onReceive(handler: (resp:string)=>void): APIRequest {
        this.onReceiveHandler = handler;
        return this;
    }

    public onFinish(handler: (success:boolean)=>void): APIRequest {
        this.onFinishHandler = handler;
        return this;
    }

    protected finalize(success: boolean): void {
        if(!this.silent) APIRequest.nRequests = APIRequest.nRequests > 0 ? APIRequest.nRequests -1 : 0;
        this.ui.layout.show('loading-spinner', APIRequest.nRequests > 0);
        if(this.onFinishHandler) this.onFinishHandler(success);
    }

    protected fail(errorMessage: string, extendedMessage: string): void {
        this.ui.layout.showError(errorMessage, extendedMessage);
        this.finalize(false);
    }

    protected handleResponse(response: Buffer): void {
        this.ui.layout.show('error-banner', false);
        if(this.onReceiveHandler) {
            const respStr = new TextDecoder().decode(response);
            this.onReceiveHandler(respStr);
        }
        this.finalize(true);
    }
}
