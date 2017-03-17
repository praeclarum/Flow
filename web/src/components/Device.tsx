import * as React from "react";

import { FNode } from "../FNode"

export interface NodeHeaderProps {
    node: FNode
}

export interface NodeBodyProps {
    node: FNode
}

export class NodeHeader extends React.Component<NodeHeaderProps, undefined> {
    render() {
        return <span>{this.props.node.nodeType}</span>
    }
}

export class NodeBody extends React.Component<NodeBodyProps, undefined> {
    render(): JSX.Element {
        return <div>
            <div>{this.props.node.value}</div>
            {(this.props.node.childNodes.length > 0)?
            <ul>
            {this.props.node.childNodes.map((x, i) =>
                <li>
                    <NodeHeader node={x} />
                </li>
            )}
            </ul>:"no children"}
        </div>;
    }
}


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
        return <div><nav>
            <div className="nav-wrapper">
                <a href="#" className="brand-logo">Flow {this.state.documentNode.childNodes.length}</a>
                    <ul id="nav-mobile" className="right hide-on-med-and-down">
                </ul>
            </div>
        </nav>
        <div className="container">
            <ul className="collapsible" data-collapsible="expandable">
            {this.state.documentNode.childNodes.map((x, i) =>
                <li>
                    <div className="collapsible-header"><NodeHeader node={x} /></div>
                    <div className="collapsible-body"><NodeBody node={x} /></div>
                </li>
            )}
            </ul>
        </div>
        </div>
    }
}
