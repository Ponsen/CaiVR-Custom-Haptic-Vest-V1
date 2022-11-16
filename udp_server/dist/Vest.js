"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.Vest = void 0;
class Vest {
    constructor(id, ip, port) {
        this.id = id;
        this.ip = ip;
        this.port = port;
    }
    setMotors(motors) {
        this.motors = motors;
    }
    getMotors() {
        return this.motors;
    }
}
exports.Vest = Vest;
//# sourceMappingURL=Vest.js.map