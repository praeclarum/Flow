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
/******/ 	return __webpack_require__(__webpack_require__.s = 5);
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
var Flow_1 = __webpack_require__(4);
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
        _this.state = { flow: { documentNode: Flow_1.newFNode("Document"), functions: [] } };
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
            _this.setState({ flow: flow });
        };
        xhr.send();
    };
    Device.prototype.render = function () {
        return React.createElement("div", { className: "pure-g" },
            React.createElement("div", { className: "pure-u-1-5" }),
            React.createElement("div", { className: "pure-u-1-5" },
                React.createElement("nav", null,
                    React.createElement(NodeTree, { index: 0, node: this.state.flow.documentNode }),
                    React.createElement("section", { className: "functions" }, this.state.flow.functions.map(function (x) {
                        return React.createElement("a", { className: "function" },
                            x,
                            " ");
                    })))),
            React.createElement("div", { className: "pure-u-2-5" }),
            React.createElement("div", { className: "pure-u-1-5" }));
    };
    return Device;
}(React.Component));
exports.Device = Device;


/***/ },
/* 3 */,
/* 4 */
/***/ function(module, exports) {

"use strict";
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
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
/* 5 */
/***/ function(module, exports, __webpack_require__) {

"use strict";
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var React = __webpack_require__(0);
var ReactDOM = __webpack_require__(1);
var Device_1 = __webpack_require__(2);
ReactDOM.render(React.createElement(Device_1.Device, null), document.getElementById("device"));


/***/ }
/******/ ]);
//# sourceMappingURL=device.js.map