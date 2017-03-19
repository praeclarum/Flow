/******/ (function(modules) { // webpackBootstrap
/******/ 	// The module cache
/******/ 	var installedModules = {};
/******/
/******/ 	// The require function
/******/ 	function __webpack_require__(moduleId) {
/******/
/******/ 		// Check if module is in cache
/******/ 		if(installedModules[moduleId])
/******/ 			return installedModules[moduleId].exports;
/******/
/******/ 		// Create a new module (and put it into the cache)
/******/ 		var module = installedModules[moduleId] = {
/******/ 			i: moduleId,
/******/ 			l: false,
/******/ 			exports: {}
/******/ 		};
/******/
/******/ 		// Execute the module function
/******/ 		modules[moduleId].call(module.exports, module, module.exports, __webpack_require__);
/******/
/******/ 		// Flag the module as loaded
/******/ 		module.l = true;
/******/
/******/ 		// Return the exports of the module
/******/ 		return module.exports;
/******/ 	}
/******/
/******/
/******/ 	// expose the modules object (__webpack_modules__)
/******/ 	__webpack_require__.m = modules;
/******/
/******/ 	// expose the module cache
/******/ 	__webpack_require__.c = installedModules;
/******/
/******/ 	// identity function for calling harmory imports with the correct context
/******/ 	__webpack_require__.i = function(value) { return value; };
/******/
/******/ 	// define getter function for harmory exports
/******/ 	__webpack_require__.d = function(exports, name, getter) {
/******/ 		Object.defineProperty(exports, name, {
/******/ 			configurable: false,
/******/ 			enumerable: true,
/******/ 			get: getter
/******/ 		});
/******/ 	};
/******/
/******/ 	// getDefaultExport function for compatibility with non-harmony modules
/******/ 	__webpack_require__.n = function(module) {
/******/ 		var getter = module && module.__esModule ?
/******/ 			function getDefault() { return module['default']; } :
/******/ 			function getModuleExports() { return module; };
/******/ 		__webpack_require__.d(getter, 'a', getter);
/******/ 		return getter;
/******/ 	};
/******/
/******/ 	// Object.prototype.hasOwnProperty.call
/******/ 	__webpack_require__.o = function(object, property) { return Object.prototype.hasOwnProperty.call(object, property); };
/******/
/******/ 	// __webpack_public_path__
/******/ 	__webpack_require__.p = "";
/******/
/******/ 	// Load entry module and return exports
/******/ 	return __webpack_require__(__webpack_require__.s = 7);
/******/ })
/************************************************************************/
/******/ ([
/* 0 */
/***/ function(module, exports) {

module.exports = React;

/***/ },
/* 1 */
/***/ function(module, exports) {

module.exports = ReactDOM;

/***/ },
/* 2 */
/***/ function(module, exports) {

"use strict";
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
function getTime() {
    return new Date().getTime() / 1000.0;
}
exports.getTime = getTime;
function newFNode(nodeType) {
    return { nodeType: nodeType, value: 0, childNodes: new Array() };
}
exports.newFNode = newFNode;
function getHeaderText(node) {
    var left;
    var right;
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
exports.getHeaderText = getHeaderText;


/***/ },
/* 3 */
/***/ function(module, exports, __webpack_require__) {

"use strict";
"use strict";
var __extends = (this && this.__extends) || (function () {
    var extendStatics = Object.setPrototypeOf ||
        ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
        function (d, b) { for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p]; };
    return function (d, b) {
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
Object.defineProperty(exports, "__esModule", { value: true });
var React = __webpack_require__(0);
var Flow_1 = __webpack_require__(2);
var FunctionDocs_1 = __webpack_require__(6);
var EvalBox_1 = __webpack_require__(5);
var NodeTree = (function (_super) {
    __extends(NodeTree, _super);
    function NodeTree(props) {
        var _this = _super.call(this, props) || this;
        _this.state = { expanded: false };
        return _this;
    }
    NodeTree.prototype.handleClick = function () {
        console.log('The node was clicked.');
        this.setState({ expanded: !this.state.expanded });
        return 0;
    };
    NodeTree.prototype.render = function () {
        var _this = this;
        var body = null;
        if (this.state.expanded) {
            body = React.createElement("div", { className: "body" }, this.props.node.childNodes.map(function (x, i) {
                return React.createElement(NodeTree, { key: i.toString(), index: i, node: x });
            }));
        }
        var cls = "nodeTree";
        if (this.state.expanded)
            cls += " expanded";
        return (React.createElement("div", { className: cls },
            React.createElement("div", { className: "header", onClick: function (_) { return _this.handleClick(); } }, Flow_1.getHeaderText(this.props.node)),
            body));
    };
    return NodeTree;
}(React.Component));
exports.NodeTree = NodeTree;
var Device = (function (_super) {
    __extends(Device, _super);
    function Device(props) {
        var _this = _super.call(this, props) || this;
        _this.state = {
            flow: {
                documentNode: Flow_1.newFNode("Document"),
                functions: []
            },
            selection: ""
        };
        _this.refresh();
        return _this;
    }
    Device.prototype.refresh = function () {
        var _this = this;
        var xhr = new XMLHttpRequest();
        var url = "document.json";
        xhr.open("GET", url);
        xhr.onload = function (ev) {
            var flow = JSON.parse(xhr.responseText);
            var h = window.location.hash;
            if (h)
                h = h.substr(1);
            _this.setState({ flow: flow, selection: h });
        };
        xhr.send();
    };
    Device.prototype.select = function (thing) {
        this.setState({ flow: this.state.flow, selection: thing });
    };
    Device.prototype.render = function () {
        var _this = this;
        var sel = null;
        var f = this.state.flow.functions.indexOf(this.state.selection);
        if (f >= 0) {
            sel = React.createElement(FunctionDocs_1.FunctionDocs, { name: this.state.flow.functions[f] });
        }
        return React.createElement("div", { className: "pure-g" },
            React.createElement("div", { className: "pure-u-1-5" }),
            React.createElement("div", { className: "pure-u-1-5" },
                React.createElement("nav", null,
                    React.createElement(NodeTree, { index: 0, node: this.state.flow.documentNode }),
                    React.createElement("section", { className: "functionNavs" }, this.state.flow.functions.map(function (x) {
                        var c = "functionNav";
                        if (_this.state.selection == x)
                            c += " selected";
                        return React.createElement("span", { key: x },
                            React.createElement("a", { href: "#" + x, onClick: function (_) { return _this.select(x); }, className: c }, x),
                            " ");
                    })))),
            React.createElement("main", { className: "pure-u-2-5" },
                React.createElement(EvalBox_1.EvalBox, null),
                sel),
            React.createElement("div", { className: "pure-u-1-5" }));
    };
    return Device;
}(React.Component));
exports.Device = Device;


/***/ },
/* 4 */,
/* 5 */
/***/ function(module, exports, __webpack_require__) {

"use strict";
"use strict";
var __extends = (this && this.__extends) || (function () {
    var extendStatics = Object.setPrototypeOf ||
        ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
        function (d, b) { for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p]; };
    return function (d, b) {
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
Object.defineProperty(exports, "__esModule", { value: true });
var React = __webpack_require__(0);
var Flow_1 = __webpack_require__(2);
function getErrorMessage(errorCode) {
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
var LineChart = (function (_super) {
    __extends(LineChart, _super);
    function LineChart(props) {
        var _this = _super.call(this, props) || this;
        _this.startTime = Flow_1.getTime();
        return _this;
    }
    LineChart.prototype.render = function () {
        var _this = this;
        var w = this.props.width;
        var h = this.props.height;
        var sw = this.props.strokeWidth;
        var fill = "none";
        var stroke = this.props.color;
        if (this.props.filled) {
            fill = this.props.color;
            stroke = "none";
        }
        var s = this.props.series;
        var lastI = s.length - 1;
        if (lastI < 0)
            return React.createElement("svg", { width: w, height: h });
        //
        // Measure
        //
        var now = Flow_1.getTime();
        var maxX = now;
        var minX = maxX - 30;
        var minY = 0; // Always include 0 to stabilize graphs
        var maxY = 0;
        var beginI = lastI;
        while (beginI >= 0) {
            var x = s[beginI][0];
            if (x < minX)
                break;
            var y = s[beginI][1];
            minY = Math.min(y, minY);
            maxY = Math.max(y, maxY);
            beginI--;
        }
        if (beginI < lastI)
            beginI++;
        var eps = 1.0e-3;
        if ((maxY - minY) < eps) {
            maxY = minY + eps;
        }
        var dpxdx = (w - sw) / (maxX - minX);
        var dpydy = (h - sw) / (maxY - minY);
        //
        // Draw
        //
        var data = "";
        var getpx = function (x) { return (x - _this.startTime) * dpxdx + sw / 2; };
        var getpy = function (y) { return h - ((y - minY) * dpydy + sw / 2); };
        var moveTo = function (x, y) { return data += "M " + x + " " + y + " "; };
        var lineTo = function (x, y) { return data += "L " + x + " " + y + " "; };
        var end = function () { return data += "z"; };
        var i = beginI;
        var px = getpx(s[i][0]);
        var py = getpy(s[i][1]);
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
        var path = React.createElement("path", { fill: fill, stroke: stroke, strokeWidth: sw, strokeLinejoin: "round", strokeLinecap: "round", d: data, dangerouslySetInnerHTML: { __html: "<animateTransform attributeName='transform'" +
                    "attributeType='XML'" +
                    "type='translate'" +
                    "from='480'" +
                    "to='" + (-dpxdx * 1000) + "'" +
                    "dur='1000s'" +
                    "repeatCount='0'/>" } });
        // {/*<animate attributeType="XML" attributeName="x" from={0} to={dpxdx} dur="1s" repeatCount={1} />*/}
        return React.createElement("svg", { width: w, height: h }, path);
    };
    return LineChart;
}(React.Component));
exports.LineChart = LineChart;
var EvalBox = (function (_super) {
    __extends(EvalBox, _super);
    function EvalBox(props) {
        var _this = _super.call(this, props) || this;
        _this.intervals = [];
        _this.state = { input: "", lastEval: { req: "init", resp: { value: 0, errorCode: 0 } }, log: [] };
        _this.setInterval(function () { return _this.reeval(); }, 1000);
        return _this;
    }
    EvalBox.prototype.setInterval = function (handler, timeout) {
        this.intervals.push(setInterval(handler, timeout));
    };
    EvalBox.prototype.componentWillUnmount = function () {
        this.intervals.forEach(clearTimeout);
    };
    EvalBox.prototype.eval = function (code, force) {
        var _this = this;
        var tcode = code.trim();
        if (!force && tcode === this.state.lastEval.req)
            return;
        var xhr = new XMLHttpRequest();
        var url = "eval";
        // TEST
        // let y = Math.random() * 2 - 0.5;
        // this.state.log.push([getTime(), y]);
        // this.setState ({ input: code, lastEval: { req: tcode, resp: {value:y,errorCode:0} } });
        // REAL
        xhr.open("POST", url);
        xhr.onload = function (_) {
            var resp = JSON.parse(xhr.responseText);
            if (tcode === _this.state.input.trim()) {
                var h = _this.state.log;
                if (resp.errorCode === 0) {
                    h.push([Flow_1.getTime(), resp.value]);
                }
                _this.setState({ lastEval: { req: tcode, resp: resp } });
            }
        };
        xhr.send(tcode);
    };
    EvalBox.prototype.reeval = function () {
        if (this.state.input.trim() !== "")
            this.eval(this.state.input, true);
        return 1;
    };
    EvalBox.prototype.handleChange = function (code) {
        this.setState({ input: code, lastEval: this.state.lastEval });
        this.eval(code, false);
    };
    EvalBox.prototype.render = function () {
        var _this = this;
        var em = React.createElement("div", null);
        var rv = React.createElement("div", null);
        var c = "empty";
        var tinput = this.state.input.trim();
        if (tinput === "") {
            // It's all good            
        }
        else if (this.state.lastEval && tinput === this.state.lastEval.req) {
            var e = this.state.lastEval.resp.errorCode;
            if (e !== 0) {
                c = (e == 4) ? "incomplete" : "error";
                var msg = getErrorMessage(e);
                em = React.createElement("div", { className: "error-message " + c }, msg);
            }
            else {
                c = "ok";
                rv = React.createElement("div", { className: "result-value " + c },
                    React.createElement(LineChart, { width: 480, height: 240, series: this.state.log, color: "#C1FFBE", filled: true, strokeWidth: 4 }),
                    this.state.lastEval.resp.value);
            }
        }
        return React.createElement("form", { className: "eval-box" },
            React.createElement("input", { type: "text", value: this.state.input, autoComplete: "off", autoCorrect: "off", autoCapitalize: "off", spellCheck: false, className: c, onChange: function (e) { return _this.handleChange(e.target.value); } }),
            em,
            rv);
    };
    return EvalBox;
}(React.Component));
exports.EvalBox = EvalBox;


/***/ },
/* 6 */
/***/ function(module, exports, __webpack_require__) {

"use strict";
"use strict";
var __extends = (this && this.__extends) || (function () {
    var extendStatics = Object.setPrototypeOf ||
        ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
        function (d, b) { for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p]; };
    return function (d, b) {
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
Object.defineProperty(exports, "__esModule", { value: true });
var React = __webpack_require__(0);
var docs = {
    "cos": {
        description: React.createElement("section", null,
            "Cosine is the ",
            React.createElement("b", null, "ratio"),
            " of the length of the side of a triangle that is ",
            React.createElement("b", null, "adjacent"),
            " to the given angle ",
            React.createElement("span", { className: "param" }, "\u03B8 "),
            "to the length of the ",
            React.createElement("b", null, "hypotenuse"),
            "."),
        params: [
            { name: "\u03B8", units: "radians", description: React.createElement("p", null, "The angle") }
        ]
    },
    "sin": {
        description: React.createElement("section", null,
            "Sine is the ",
            React.createElement("b", null, "ratio"),
            " of the length of the side of a triangle that is ",
            React.createElement("b", null, "opposite"),
            " of the given angle ",
            React.createElement("span", { className: "param" }, "\u03B8 "),
            "to the length of the ",
            React.createElement("b", null, "hypotenuse"),
            "."),
        params: [
            { name: "\u03B8", units: "radians", description: React.createElement("p", null, "The angle") }
        ]
    },
};
var FunctionDocs = (function (_super) {
    __extends(FunctionDocs, _super);
    function FunctionDocs() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    FunctionDocs.prototype.render = function () {
        var d = docs[this.props.name];
        if (d) {
            var ps = null;
            if (d.params.length > 0) {
                ps = React.createElement("section", null, d.params.map(function (x) {
                    return React.createElement("section", { key: x.name, id: x.name },
                        React.createElement("p", { className: "proto" },
                            React.createElement("span", { className: "param" }, x.name),
                            ": ",
                            React.createElement("span", { className: "units" }, x.units)),
                        x.description);
                }));
            }
            return React.createElement("section", null,
                React.createElement("h1", { className: "fun" }, this.props.name),
                d.description,
                ps);
        }
        else {
            return React.createElement("section", null,
                React.createElement("h1", { className: "fun" }, this.props.name));
        }
    };
    ;
    return FunctionDocs;
}(React.Component));
exports.FunctionDocs = FunctionDocs;


/***/ },
/* 7 */
/***/ function(module, exports, __webpack_require__) {

"use strict";
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var React = __webpack_require__(0);
var ReactDOM = __webpack_require__(1);
var Device_1 = __webpack_require__(3);
ReactDOM.render(React.createElement(Device_1.Device, null), document.getElementById("device"));


/***/ }
/******/ ]);
//# sourceMappingURL=device.js.map