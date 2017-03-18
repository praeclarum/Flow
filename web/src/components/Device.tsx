import * as React from "react";

import { FNode, newFNode, getHeaderText } from "../FNode"

import * as $ from "jquery"

export interface NodeTreeProps {
    index: number
    node: FNode
}

export interface NodeTreeState {
    expanded: boolean
}

export class NodeTree extends React.Component<NodeTreeProps, NodeTreeState> {
    constructor (props: NodeTreeProps) {
        super(props);
        this.state = {expanded: false};
    }
    handleClick() {
        console.log('The node was clicked.');
        this.setState({expanded:!this.state.expanded});
        return 0;
    }
    render(): JSX.Element {
        let body = null;
        if (this.state.expanded) {
            body = <div className="body">
                {this.props.node.childNodes.map((x, i) =>
                    <NodeTree key={i.toString()} index={i} node={x}/>
                )}
                </div>
        }
        let cls = "nodeTree";
        if (this.state.expanded)
            cls += " expanded";
        return (<div className={cls}>
            <div className="header" onClick={_=>this.handleClick()}>
                {getHeaderText(this.props.node)}
            </div>
            {body}
            </div>);
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
        this.state = {documentNode: newFNode("Document")};
        this.refresh();
    }
    refresh()
    {
        let xhr = new XMLHttpRequest();
        let url = "document.json";
        xhr.open("GET", url);
        xhr.onload = ev => {
            let n: FNode = JSON.parse(xhr.responseText);
            this.setState ({documentNode: n});
        };
        xhr.send();
    }
    render() {
        return <div className="pure-g">
            <div className="pure-u-1-5"/>
            <div className="pure-u-1-5">
                <nav>
                    <NodeTree index={0} node={this.state.documentNode} />
                </nav>
            </div>
            <div className="pure-u-2-5"/>
            <div className="pure-u-1-5"/>        
        </div>
    }
}
