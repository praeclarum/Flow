import * as React from "react";

import { FNode } from "../FNode"

export interface DeviceProps {
}

export interface DeviceState {
    documentNode: FNode;
}

export class Device extends React.Component<DeviceProps, DeviceState> {
    constructor(props: DeviceProps) {
        super(props);
        this.state = {documentNode: new FNode("Document")};
        this.refresh();
    }
    refresh()
    {
        let xhr = new XMLHttpRequest();
        let url = "document.json";
        xhr.open("GET", url);
        xhr.onload = ev => {
            let n = FNode.fromJSON(xhr.responseText);
            this.setState ({documentNode: n});
        };
        xhr.send();
    }
    render() {
        return <nav>
            <div className="nav-wrapper">
                <a href="#" className="brand-logo">Flow {this.state.documentNode.childNodes.length}</a>
                    <ul id="nav-mobile" className="right hide-on-med-and-down">
                </ul>
            </div>
        </nav>
    }
}
