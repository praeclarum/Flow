import * as React from "react";

import { Flow, FNode, newFNode, getHeaderText } from "../Flow"

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
    flow: Flow;
}

export class Device extends React.Component<DeviceProps, DeviceState> {
    constructor(props: DeviceProps) {
        super(props);
        this.state = {flow:{documentNode: newFNode("Document"),functions:[]}};
        this.refresh();
    }
    refresh()
    {
        let xhr = new XMLHttpRequest();
        let url = "document.json";
        xhr.open("GET", url);
        xhr.onload = ev => {
            let flow: Flow = JSON.parse(xhr.responseText);
            this.setState ({flow: flow});
        };
        xhr.send();
    }
    render() {
        return <div className="pure-g">
            <div className="pure-u-1-5"/>
            <div className="pure-u-1-5">
                <nav>
                    <NodeTree index={0} node={this.state.flow.documentNode} />
                    <section className="functions">
                        {this.state.flow.functions.map(x =>
                            <a className="function">{x} </a>)}
                    </section>
                </nav>
            </div>
            <div className="pure-u-2-5"/>
            <div className="pure-u-1-5"/>        
        </div>
    }
}
