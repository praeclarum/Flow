import * as React from "react";

import { Flow, FNode, newFNode, getHeaderText } from "../Flow"

interface Paramdoc {
    name: string;
    units: string;
    description: JSX.Element;
}

interface Fundoc {
    description: JSX.Element
    params: Paramdoc[]
}

interface Fundocs {
    [name: string]: Fundoc;
}

let docs: Fundocs = {
    "cos": {
        description: <section>
                Cosine is the <b>ratio</b> of the length of the side of a triangle
                that is <b>adjacent</b> to the given angle <span className="param">&theta; </span>
                to the length of the <b>hypotenuse</b>.
            </section>,
        params: [
            { name: "\u03B8", units: "radians", description: <p>The angle</p> }
        ]
    },
    "sin": {
        description: <section>
                Sine is the <b>ratio</b> of the length of the side of a triangle
                that is <b>opposite</b> of the given angle <span className="param">&theta; </span>
                to the length of the <b>hypotenuse</b>.
            </section>,
        params: [
            { name: "\u03B8", units: "radians", description: <p>The angle</p> }
        ]
    },
};

export interface FunctionDocsProps {
    name: string;
}

export class FunctionDocs extends React.Component<FunctionDocsProps, undefined> {
    
    render() {
        var d = docs[this.props.name];
        if (d) {
            var ps: JSX.Element = null;
            if (d.params.length > 0) {
                ps = <section>
                        {d.params.map(x =>
                            <section key={x.name} id={x.name}>
                                <p className="proto"><span className="param">{x.name}</span>: <span className="units">{x.units}</span></p>
                                {x.description}
                            </section>)}
                    </section>;
            }
            return <section>
                    <h1 className="fun">{this.props.name}</h1>
                    {d.description}
                    {ps}
                </section>;
        }
        else {
            return <section>
                    <h1 className="fun">{this.props.name}</h1>
                </section>;
        }
    };
}
