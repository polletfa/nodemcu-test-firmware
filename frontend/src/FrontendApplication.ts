/*****************************************************
 *
 * NodeMCU Test Firmware - Web interface
 *
 * (c) 2021-2022 Fabien Pollet <polletfa@posteo.de>
 * MIT License (see LICENSE.md file)
 *
 *****************************************************/

import { Layout } from './Layout';
import { APIRequest } from './APIRequest';

// extend Window to add a reference to the FrontendApplication instance
interface CustomWindow extends Window {
    application: FrontendApplication;
}
declare let window: CustomWindow;

/**
 * Main class for the JavaScript logic of the website
 */
export class FrontendApplication {
    readonly layout: Layout;                                    /**< Manage layout */

    /**
     * @param config Configuration provided by the backend
     */
    constructor() {
        window.application = this; // save the main class in the window object to be able to access it globally
        this.layout = new Layout(this);

        this.fillTable();
        setInterval(this.readStatus.bind(this), 1000);
    }

    private readStatus(): void {
        if(APIRequest.getActiveRequests() == 0) {
            new APIRequest(this, "/status", true)
                .onReceive((response) => {
                    for(let i = 0; i <= 8; ++i) {
                        if(response !== undefined) {
                            switch(response[i]) {
                                case 'L':
                                    this.setSwitches(i, true, false);
                                    break;
                                case 'H':
                                    this.setSwitches(i, true, true);
                                    break;
                                case 'l':
                                    this.setSwitches(i, false, false);
                                    break;
                                case 'h':
                                    this.setSwitches(i, false, true);
                                    break;
                            }
                        } else {
                            this.setSwitches(i, undefined, undefined);
                        }
                    }
                })
                .onFinish((success) => {
                    if(!success) {
                        for(let i = 0; i <= 8; ++i) {
                            this.setSwitches(i, undefined, undefined);
                        }
                    }
                    this.layout.show("pins-table", true);
                });
        }
    }

    private setModeSwitch(index: number, mode: boolean|undefined): void {
        const stateEl = document.getElementById('stateD'+index);
        const state = (stateEl instanceof HTMLInputElement) ? stateEl.checked : undefined;
        this.setSwitches(index, mode, state);
    }

    private setStateSwitch(index: number, state: boolean|undefined): void {
        const modeEl = document.getElementById('modeD'+index);
        const mode = (modeEl instanceof HTMLInputElement) ? modeEl.checked : undefined;
        this.setSwitches(index, mode, state);
    }

    private setSwitches(index: number, mode: boolean|undefined, state: boolean|undefined): void {
        const modeEl = document.getElementById('modeD'+index);
        const modeLabelEl = document.getElementById('modeD'+index+'label');
        const stateEl = document.getElementById('stateD'+index);
        const stateLabelEl = document.getElementById('stateD'+index+'label');

        if(modeEl instanceof HTMLInputElement) {
            modeEl.disabled = mode === undefined;
            modeEl.checked = mode === true;
        }
        if(modeLabelEl instanceof HTMLElement) modeLabelEl.innerHTML = mode === undefined ? "Undefined" : mode === true ? "OUTPUT" : "INPUT";

        if(stateEl instanceof HTMLInputElement) {
            stateEl.disabled = state === undefined || mode === undefined || mode === false;
            stateEl.checked = state === true;
        }
        if(stateLabelEl instanceof HTMLElement) stateLabelEl.innerHTML = state === undefined ? "Undefined" : state === true ? "HIGH" : "LOW";
    }

    private fillTable(): void {
        let html = "";
        for(let i = 0; i <= 8; ++i) {
            html += '<tr><td>'
                + 'D' + i
                + '</td><td>'
                + '<div class="form-check form-switch">'
                + '<input class="form-check-input" type="checkbox" id="modeD'+i+'" disabled onchange="window.application.switchMode('+i+');">'
                + '<label class="form-check-label" id="modeD'+i+'label" for="modeD'+i+'">Undefined</label>'
                + '</div>'
                + '</td><td>'
                + '<div class="form-check form-switch">'
                + '<input class="form-check-input" type="checkbox" id="stateD'+i+'" disabled onchange="window.application.switchState('+i+');">'
                + '<label class="form-check-label" id="stateD'+i+'label" for="stateD'+i+'">Undefined</label>'
                + '</div>'
                + '</td></tr>';
        }
        const el = document.getElementById('pins-table-body');
        if(el instanceof HTMLElement) el.innerHTML = html;
    }

    public switchMode(index: number) {
        const modeEl = document.getElementById('modeD'+index);
        if(modeEl instanceof HTMLInputElement) {
            this.setModeSwitch(index, modeEl.checked);
            new APIRequest(this, "/mode/"+index+"/"+(modeEl.checked ? "out" : "in"));
        }
    }

    public switchState(index: number) {
        const stateEl = document.getElementById('stateD'+index);
        if(stateEl instanceof HTMLInputElement) {
            this.setStateSwitch(index, stateEl.checked);
            new APIRequest(this, "/write/"+index+"/"+(stateEl.checked ? "high" : "low"));
        }
    }
}
