import * as React from "react";

import { Flow, FNode, newFNode, getHeaderText } from "../Flow"
import { FunctionDocs } from "./FunctionDocs"
import { EvalBox } from "./EvalBox"

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
    selection: string;
}

export class Device extends React.Component<DeviceProps, DeviceState> {
    constructor(props: DeviceProps) {
        super(props);
        this.state = {
            flow: {
                documentNode: newFNode("Document"),
                functions:[]
            },
            selection: ""
        };
        this.refresh();
    }
    refresh()
    {
        let xhr = new XMLHttpRequest();
        let url = "document.json";
        xhr.open("GET", url);
        xhr.onload = ev => {
            let flow: Flow = JSON.parse(xhr.responseText);
            let h = window.location.hash;
            if (h) h = h.substr(1);
            this.setState ({flow: flow, selection: h});
        };
        xhr.send();
    }
    select(thing: string)
    {
        this.setState ({flow: this.state.flow, selection: thing});
    }
    render() {
        var sel = null;
        var f = this.state.flow.functions.indexOf(this.state.selection);
        if (f >= 0) {
            sel = <FunctionDocs name={this.state.flow.functions[f]} />
        }

        return <div className="pure-g">
            <div className="pure-u-1-5"/>
            <div className="pure-u-1-5">
                <nav>
                    <NodeTree index={0} node={this.state.flow.documentNode} />
                    <section className="functionNavs">
                        {this.state.flow.functions.map(x => {
                            var c = "functionNav";
                            if (this.state.selection == x)
                                c += " selected";
                            return <span key={x}><a href={"#"+x} onClick={_=>this.select(x)} className={c}>{x}</a> </span>;
                        })}
                    </section>
                </nav>
            </div>
            <main className="pure-u-2-5">
                <EvalBox />
                {sel}
            </main>
            <div className="pure-u-1-5"/>        
        </div>
    }
}
