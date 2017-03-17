
export class FNode
{
    nodeType: string = "Null";
    value: Number;
    childNodes: FNode[];

    constructor(nodeType?: string)
    {
        this.nodeType = nodeType;
        this.childNodes = [];
    }

    static fromJSON(json: string): FNode {
        let n: FNode = JSON.parse(json);
        return n;
    }
}


