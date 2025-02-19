"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.Vest = void 0;
class Vest {
    constructor(id, ip, port) {
        this.powerMin = 1350;
        this.powerMax = 4095;
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