
export interface FNode
{
    nodeType: string;
    value: number|string;
    childNodes: FNode[];
}

export function newFNode(nodeType: string)
{
    return {nodeType:nodeType, value:0, childNodes:new Array<FNode>()};
}

export function getHeaderText(node: FNode): string {
    var left: string;
    var right: string;
    switch (node.nodeType) {
    case "Document":
        return "Flow";
    case "Name":
        return node.value.toString();
    case "Assignment":
        left = "?";
        if (node.childNodes.length > 0) {
            left = getHeaderText(node.childNodes[0]);
        }
        right = "?";
        if (node.childNodes.length > 1) {
            right = getHeaderText(node.childNodes[1]);
        }
        return left + " = " + right;
    default:
        return node.nodeType;
    }
}

