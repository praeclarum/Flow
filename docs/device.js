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
var FNode_1 = __webpack_require__(4);
var NodeHeader = (function (_super) {
    __extends(NodeHeader, _super);
    function NodeHeader() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    NodeHeader.prototype.render = function () {
        return React.createElement("span", null, this.props.node.nodeType);
    };
    return NodeHeader;
}(React.Component));
exports.NodeHeader = NodeHeader;
var NodeBody = (function (_super) {
    __extends(NodeBody, _super);
    function NodeBody() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    NodeBody.prototype.render = function () {
        return React.createElement("div", null,
            React.createElement("div", null, this.props.node.value),
            (this.props.node.childNodes.length > 0) ?
                React.createElement("ul", null, this.props.node.childNodes.map(function (x, i) {
                    return React.createElement("li", null,
                        React.createElement(NodeHeader, { node: x }));
                })) : "no children");
    };
    return NodeBody;
}(React.Component));
exports.NodeBody = NodeBody;
var Device = (function (_super) {
    __extends(Device, _super);
    function Device(props) {
        var _this = _super.call(this, props) || this;
        _this.state = { documentNode: new FNode_1.FNode("Document") };
        _this.refresh();
        return _this;
    }
    Device.prototype.refresh = function () {
        var _this = this;
        var xhr = new XMLHttpRequest();
        var url = "document.json";
        xhr.open("GET", url);
        xhr.onload = function (ev) {
            var n = FNode_1.FNode.fromJSON(xhr.responseText);
            _this.setState({ documentNode: n });
        };
        xhr.send();
    };
    Device.prototype.render = function () {
        return React.createElement("div", null,
            React.createElement("nav", null,
                React.createElement("div", { className: "nav-wrapper" },
                    React.createElement("a", { href: "#", className: "brand-logo" },
                        "Flow ",
                        this.state.documentNode.childNodes.length),
                    React.createElement("ul", { id: "nav-mobile", className: "right hide-on-med-and-down" }))),
            React.createElement("div", { className: "container" },
                React.createElement("ul", { className: "collapsible", "data-collapsible": "expandable" }, this.state.documentNode.childNodes.map(function (x, i) {
                    return React.createElement("li", null,
                        React.createElement("div", { className: "collapsible-header" },
                            React.createElement(NodeHeader, { node: x })),
                        React.createElement("div", { className: "collapsible-body" },
                            React.createElement(NodeBody, { node: x })));
                }))));
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
var FNode = (function () {
    function FNode(nodeType) {
        this.nodeType = "Null";
        this.nodeType = nodeType;
        this.childNodes = [];
    }
    FNode.fromJSON = function (json) {
        var n = JSON.parse(json);
        return n;
    };
    return FNode;
}());
exports.FNode = FNode;


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