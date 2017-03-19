import * as React from "react";

import { Flow, FNode, newFNode, getHeaderText, getTime } from "../Flow"
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

export type Datum = number[]

export interface LineChartProps {
    series: Datum[];
    color: string;
    filled: boolean;
    strokeWidth: number;
    width: number;
    height: number;
}

export class LineChart extends React.Component<LineChartProps, undefined> {
    startTime: number;
    constructor(props: LineChartProps) {
        super(props);
        this.startTime = getTime();
    }
    render() {
        var w = this.props.width;
        var h = this.props.height;
        var sw = this.props.strokeWidth;
        var fill = "none";
        var stroke = this.props.color;
        if (this.props.filled) {
            fill = this.props.color;
            stroke = "none";
        }
        
        let s = this.props.series;
        var lastI = s.length - 1;
        if (lastI < 0)
            return <svg width={w} height={h} />;

        //
        // Measure
        //
        let now = getTime();
        let maxX = now;
        var minX = maxX - 30;
        let minY = 0; // Always include 0 to stabilize graphs
        let maxY = 0;
        let beginI = lastI;
        while (beginI >= 0) {
            let x = s[beginI][0];
            if (x < minX)
                break;
            let y = s[beginI][1];
            minY = Math.min(y, minY);
            maxY = Math.max(y, maxY);
            beginI--;
        }
        if (beginI < lastI) beginI++;
        let eps = 1.0e-3;
        if ((maxY - minY) < eps) {
            maxY = minY + eps;
        }
        let dpxdx = (w-sw) / (maxX - minX);
        let dpydy = (h-sw) / (maxY - minY);
        //
        // Draw
        //
        var data = "";
        var getpx = (x: number) => (x - this.startTime) * dpxdx + sw/2;
        var getpy = (y: number) => h - ((y - minY) * dpydy + sw/2);
        var moveTo = (x: number, y: number) => data += "M " + x + " " + y + " ";
        var lineTo = (x: number, y: number) => data += "L " + x + " " + y + " ";
        var end = () => data += "z";
        var i = beginI;
        let px = getpx(s[i][0]);
        let py = getpy(s[i][1]);
        if (fill !== "none") {
            py = getpy(0);
        }
        else {
            i++;
        }
        moveTo(px, py);        
        for (; i <= lastI; i++) {
            px = getpx(s[i][0]);
            py = getpy(s[i][1]);
            lineTo(px, py);
        }
        if (fill !== "none") {
            px = getpx(s[lastI][0]) + sw * 0.51;
            py = getpy(0);
            lineTo(px, py);
            end();
        }
        var path = 
                <path fill={fill} stroke={stroke} strokeWidth={sw} strokeLinejoin="round" strokeLinecap="round" d={data}
                    dangerouslySetInnerHTML={{__html: "<animateTransform attributeName='transform'" +
                            "attributeType='XML'" +
                            "type='translate'" +
                            "from='480'" +
                            "to='"+(-dpxdx*1000)+"'" +
                            "dur='1000s'" +
                            "repeatCount='0'/>"}} />
                // {/*<animate attributeType="XML" attributeName="x" from={0} to={dpxdx} dur="1s" repeatCount={1} />*/}
            
        return <svg width={w} height={h}>
                {/*<circle cx={w/2} cy={h/2} r={h/4} fill="red" />*/}
                {path}
            </svg>;
    }
}

export interface EvalBoxProps {
}

export interface EvalBoxState {
    input: string;
    lastEval: {req: string, resp: EvalResponse};
    log: Datum[];
}

export class EvalBox extends React.Component<EvalBoxProps, EvalBoxState> {
    intervals: Array<number> = [];
    constructor(props: EvalBoxProps) {
        super(props);
        this.state = { input: "", lastEval: { req: "init", resp: { value: 0, errorCode: 0 } }, log: [] }
        this.setInterval(()=>this.reeval(), 1000);
    }
    setInterval(handler: any, timeout: number) {
        this.intervals.push(setInterval(handler, timeout));
    }
    componentWillUnmount() {
        this.intervals.forEach(clearTimeout);
    }
    eval(code: string, force: boolean)
    {
        let tcode = code.trim();
        if (!force && tcode === this.state.lastEval.req)
            return;

        let xhr = new XMLHttpRequest();
        let url = "eval";

        // TEST
        // let y = Math.random() * 2 - 0.5;
        // this.state.log.push([getTime(), y]);
        // this.setState ({ input: code, lastEval: { req: tcode, resp: {value:y,errorCode:0} } });

        // REAL
        xhr.open("POST", url);
        xhr.onload = _ => {
            let resp: EvalResponse = JSON.parse(xhr.responseText);
            if (tcode === this.state.input.trim()) {
                var h = this.state.log;
                if (resp.errorCode === 0) {
                    h.push([getTime(), resp.value]);
                }
                this.setState ({ lastEval: { req: tcode, resp: resp } });
            }
        };
        xhr.send(tcode);
    }
    reeval()
    {
        if (this.state.input.trim() !== "")
            this.eval(this.state.input, true);
        return 1;
    }
    handleChange(code: string)
    {
        this.setState ({ input: code, lastEval: this.state.lastEval });
        this.eval(code, false);
    }
    render() {
        let em = <div />
        let rv = <div />
        let c = "empty";
        let tinput = this.state.input.trim();
        if (tinput === "") {
            // It's all good            
        }
        else if (this.state.lastEval && tinput === this.state.lastEval.req) {
            let e = this.state.lastEval.resp.errorCode;
            if (e !== 0) {
                c = (e == 4) ? "incomplete" : "error";
                let msg = getErrorMessage(e);
                em = <div className={"error-message "+c}>{msg}</div>
            }
            else {
                c = "ok";
                rv = <div className={"result-value "+c}>
                        <LineChart width={480} height={240} series={this.state.log} color="#C1FFBE" filled={true} strokeWidth={4} />
                        {this.state.lastEval.resp.value}
                    </div>
            }
        }
        return <form className="eval-box">
                <input type="text" value={this.state.input}
                       autoComplete="off" autoCorrect="off" autoCapitalize="off" spellCheck={false}
                       className={c}
                       onChange={e=>this.handleChange(e.target.value)} />
                {em}
                {rv}
            </form>
    }
}
