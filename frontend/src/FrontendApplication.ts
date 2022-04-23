/*****************************************************
 *
 * NodeMCU Test Firmware - Web interface
 *
 * (c) 2022 Fabien Pollet <polletfa@posteo.de>
 * MIT License (see LICENSE.md file)
 *
 *****************************************************/

import { Modal } from "bootstrap";

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

    private settings: string[] = [];
    
    /**
     * @param config Configuration provided by the backend
     */
    constructor() {
        window.application = this; // save the main class in the window object to be able to access it globally
        this.layout = new Layout(this);

        this.fillTable();
        this.readSettings();
        setInterval(this.readStatus.bind(this), 1000);
    }

    private readSettings(): void {
        new APIRequest(this, "/config")
            .onReceive((resp) => {
                this.settings = resp.split("\n");
            })
            .onFinish(() => {       
                for(let i = 0; i < 9; ++i) {
                    this.layout.setHTML("label-d"+i, "D"+i + (this.settings[i + 3] ? (" - " + this.settings[i+3]) : ""));
                }
            });
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
            html += '<tr><td id="label-d' + i + '">'
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

    private openOrCloseModal(name: string) {
        const el = document.getElementById(name + "-dialog");
        if(el instanceof HTMLElement) Modal.getOrCreateInstance(el).toggle();
    }
   
    public openInfo() { this.openOrCloseModal("info"); }

    public openSettings() {
        new APIRequest(this, "/config")
            .onReceive((resp) => {
                this.openOrCloseModal("settings");
                this.settings = resp.split("\n");
                for(let i = 0; i < 12; ++i) if(this.settings[i] === undefined) this.settings[i] = "";
                this.layout.setFormValue("settings-network-name", this.settings[0]);
                this.layout.setFormValue("settings-network-ssid", this.settings[1]);
                this.layout.setFormValue("settings-network-password", this.settings[2]);
                this.layout.setFormValue("settings-pins-d0", this.settings[3]);
                this.layout.setFormValue("settings-pins-d1", this.settings[4]);
                this.layout.setFormValue("settings-pins-d2", this.settings[5]);
                this.layout.setFormValue("settings-pins-d3", this.settings[6]);
                this.layout.setFormValue("settings-pins-d4", this.settings[7]);
                this.layout.setFormValue("settings-pins-d5", this.settings[8]);
                this.layout.setFormValue("settings-pins-d6", this.settings[9]);
                this.layout.setFormValue("settings-pins-d7", this.settings[10]);
                this.layout.setFormValue("settings-pins-d8", this.settings[11]);
                this.layout.show("settings", true);
            });
    }

    private buildQuery(index: number, param: string, field: string): string {
        if(this.settings[index] != this.layout.getFormValue(field)) return param + "=" + encodeURIComponent(this.layout.getFormValue(field)) + "&";
        else return "";
    }
    public saveSettings() {
        const query = this.buildQuery(0, "name", "settings-network-name")
            + this.buildQuery(1, "ssid", "settings-network-ssid")
            + this.buildQuery(2, "password", "settings-network-password")
            + this.buildQuery(3, "pinName0", "settings-pins-d0")
            + this.buildQuery(4, "pinName1", "settings-pins-d1")
            + this.buildQuery(5, "pinName2", "settings-pins-d2")
            + this.buildQuery(6, "pinName3", "settings-pins-d3")
            + this.buildQuery(7, "pinName4", "settings-pins-d4")
            + this.buildQuery(8, "pinName5", "settings-pins-d5")
            + this.buildQuery(9, "pinName6", "settings-pins-d6")
            + this.buildQuery(10, "pinName7", "settings-pins-d7")
            + this.buildQuery(11, "pinName8", "settings-pins-d8")
        ;
        new APIRequest(this, "/config?" + query)
            .onFinish(() => {
                this.openOrCloseModal("settings");
                this.readSettings();
            });
    }
}
