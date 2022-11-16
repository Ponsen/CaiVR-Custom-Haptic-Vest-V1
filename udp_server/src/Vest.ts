export class Vest {
    id: string;
    ip: string;
    port: string;

    motors: number[];
   
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