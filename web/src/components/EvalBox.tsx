import * as React from "react";

import { Flow, FNode, newFNode, getHeaderText } from "../Flow"
import { FunctionDocs } from "./FunctionDocs"


interface EvalResponse
{
    value: number;
    errorCode: number;
}

function getErrorMessage(errorCode: number)
{
    switch (errorCode) {
    case 0:
        return "Success";
    case 2:
        return "Out of memory";
    case 3:
        return "Syntax error";
    case 4:
        return "Incomplete";
    case 1:
    default:
        return "Unknown error";
    }
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
        this.state = { input: "", lastEval: { req: "init", resp: { value: 0, errorCode: 0 } } }
    }
    eval(code: string)
    {
        let xhr = new XMLHttpRequest();
        let url = "eval";
        this.setState ({ input: code, lastEval: { req: code, resp: {value:42,errorCode:4} } });
        // xhr.open("POST", url);
        // xhr.onload = _ => {
        //     let resp: EvalResponse = JSON.parse(xhr.responseText);
        //     if (code === this.state.input) {
        //         this.setState ({ input: code, lastEval: { req: code, resp: resp } });
        //     }
        // };
        // xhr.send(code);
    }
    handleChange(code: string)
    {
        this.setState ({ input: code, lastEval: this.state.lastEval });
        this.eval(code);
    }
    render() {
        let em = <div />
        let rv = <div />
        let c = "empty";
        if (this.state.input.trim() === "") {
            // It's all good            
        }
        else if (this.state.lastEval && this.state.input === this.state.lastEval.req) {
            let e = this.state.lastEval.resp.errorCode;
            if (e !== 0) {
                c = (e == 4) ? "incomplete" : "error";
                let msg = getErrorMessage(e);
                em = <div className={"error-message "+c}>{msg}</div>
            }
            else {
                c = "ok";
                rv = <div className={"result-value "+c}>{this.state.lastEval.resp.value}</div>
            }
        }
        return <form className="eval-box">
                <input type="text" value={this.state.input}
                       className={c}
                       onChange={e=>this.handleChange(e.target.value)} />
                {em}
                {rv}
            </form>
    }
}
