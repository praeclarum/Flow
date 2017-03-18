import * as React from "react";

import { Flow, FNode, newFNode, getHeaderText } from "../Flow"
import { FunctionDocs } from "./FunctionDocs"


export interface EvalBoxProps {
}

export interface EvalBoxState {
    input: string;
    response: {input: string, output:number};
}

export class EvalBox extends React.Component<EvalBoxProps, EvalBoxState> {
    constructor(props: EvalBoxProps) {
        super(props);
        this.state = { input: "", response: { input: "", output: 0 } }
    }
    eval(code: string)
    {
        let xhr = new XMLHttpRequest();
        let url = "eval";
        xhr.open("POST", url);
        xhr.onload = _ => {
            let value = parseFloat(xhr.responseText);
            if (code === this.state.input) {
                this.setState ({ response: { input: code, output: value } });
            }
        };
        xhr.send(code);
    }
    handleChange(code: string)
    {
        this.setState ({input: code, response: this.state.response});
        this.eval(code);
    }
    render() {
        let res = null;
        if (this.state.input === this.state.response.input) {
            res = <div>{this.state.response.output}</div>
        }
        return <form className="pure-form">
                <input type="text" value={this.state.input} onChange={e=>this.handleChange(e.target.value)} />
            </form>
    }
}
