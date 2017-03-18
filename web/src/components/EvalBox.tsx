import * as React from "react";

import { Flow, FNode, newFNode, getHeaderText } from "../Flow"
import { FunctionDocs } from "./FunctionDocs"


interface EvalResponse
{
    value: number;
}

export interface EvalBoxProps {
}

export interface EvalBoxState {
    input: string;
    lastEval: {req: string, resp: EvalResponse};
}

export class EvalBox extends React.Component<EvalBoxProps, EvalBoxState> {
    constructor(props: EvalBoxProps) {
        super(props);
        this.state = { input: "", lastEval: { req: "init", resp: {value: 0} } }
    }
    eval(code: string)
    {
        let xhr = new XMLHttpRequest();
        let url = "eval";
        xhr.open("POST", url);
        xhr.onload = _ => {
            let resp: EvalResponse = JSON.parse(xhr.responseText);
            if (code === this.state.input) {
                this.setState ({ input: code, lastEval: { req: code, resp: resp } });
            }
        };
        xhr.send(code);
    }
    handleChange(code: string)
    {
        this.setState ({ input: code, lastEval: this.state.lastEval });
        this.eval(code);
    }
    render() {
        let res = null;
        if (this.state.input === this.state.lastEval.req) {
            res = <div>{this.state.lastEval.resp.value}</div>
        }
        return <form className="pure-form">
                <input type="text" value={this.state.input} onChange={e=>this.handleChange(e.target.value)} />
                {res}
            </form>
    }
}
