"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const dgram_1 = __importDefault(require("dgram"));
const osc_js_1 = __importDefault(require("osc-js"));
const timers_1 = require("timers");
const Vest_1 = require("./Vest");
const serverPort = 1234;
const Message_Type_Register = 1;
const Message_Type_Drive = 2;
// --------------------creating a udp server --------------------
//devices
let vest;
let sendLoop;
let testLoop;
// creating a udp server
const server = dgram_1.default.createSocket('udp4');
// emits when any error occurs
server.on('error', (error) => {
    console.log('Error: ' + error);
    server.close();
});
// emits on new datagram msg
server.on('message', (msg, info) => {
    debugMessage(msg, info);
    parseMessage(msg, info);
});
//emits when socket is ready and listening for datagram msgs
server.on('listening', () => {
    var address = server.address();
    var port = address.port;
    var family = address.family;
    var ipaddr = address.address;
    console.log('Server is listening at port : ' + port);
    console.log('Server ip : ' + ipaddr);
    console.log('Server is IP4/IP6 : ' + family);
    //announce to devices, in case server restarted
    server.setBroadcast(true);
    server.send("announce", serverPort, "0.0.0.0", (err) => {
        if (err) {
            console.error(err);
        }
    });
});
//emits after the socket is closed using socket.close()
server.on('close', () => {
    console.log('Socket is closed !');
});
server.bind(serverPort);
const debugMessage = (msg, info) => {
    console.log('Data received from client : ' + msg.toString());
    console.log('Received %d bytes from %s:%d\n', msg.length, info.address, info.port);
};
const parseMessage = (msg, info) => {
    try {
        let message = JSON.parse(msg);
        if (message.mtype === Message_Type_Register) {
            vest = new Vest_1.Vest(message.mac, info.address, info.port);
            vest.setMotors(Array(message.motors).fill(0));
            sendMessage("registered", info.port, info.address);
            startSendLoop();
            //startTestLoop()
        }
    }
    catch (e) {
        console.error("error parsing message", e);
    }
};
const sendMessage = (msg, port, address) => {
    server.send(msg, port, address, (error) => {
        if (error) {
            console.error(error);
        }
        else {
            console.log("send " + msg + " to " + address + ":" + port);
        }
    });
};
/* VRCHAT Stuff */
// receive a message via UDP
const osc = new osc_js_1.default({ plugin: new osc_js_1.default.DatagramPlugin() });
/*
address: /avatar/change
types: ,s args: avtr_b6f6cf86-c841-4175-afb0-14c3ba47bdad

address: /avatar/parameters/AngularY
types: ,f args: 0

address: /avatar/parameters/VelocityX
types: ,f args: 0

address: /avatar/parameters/VelocityZ
types: ,f args: -0.07975462079048157

address: /avatar/parameters/Grounded
types: ,T args: true
*/
let maxValue = 1;
osc.on('*', (message, info) => {
    //console.log(JSON.stringify(info))
    console.log("address: " + message.address);
    console.log("types: " + message.types + " args: " + message.args);
    calcInputFreq();
    //TODO: mapping
    if (vest != undefined) {
        if (message.address === "/avatar/parameters/VelocityZ") { //velocity m/s
            let normalized = Math.abs(parseFloat(message.args));
            if (normalized > maxValue) {
                maxValue = normalized;
            }
            if (normalized == 0) { // no infitity plz, thx
                vest.motors[0] = 0;
            }
            else {
                let value = Math.round(normalized / maxValue * 4095);
                vest.motors[0] = value > vest.powerMin ? value : vest.powerMin;
            }
            //send drive commands as soon as a message comes in
            //sendMessage(Message_Type_Drive + "|" + vest.motors, vest.port, vest.ip);
            newMessage = true;
        }
    }
});
let newMessage = false; //only send a message if a value potentially changed
//send drive commands in a loop at a defined frequenzy
const startSendLoop = () => {
    if (sendLoop != undefined) {
        clearInterval(sendLoop);
    }
    sendLoop = (0, timers_1.setInterval)(() => {
        //TODO: dismiss previous message if equal
        if (vest != undefined && newMessage) {
            //if (vest != undefined) {
            //console.log("motors: " + vest.motors)
            sendMessage(Message_Type_Drive + "|" + vest.motors, vest.port, vest.ip);
            newMessage = false;
        }
    }, 20); //50 messages per second max
};
const startTestLoop = () => {
    let currentPower = vest.powerMin;
    let up = true;
    console.log("startTestLoop");
    if (testLoop != undefined) {
        clearInterval(testLoop);
    }
    testLoop = (0, timers_1.setInterval)(() => {
        //TODO: dismiss previous message if equal
        if (vest != undefined) {
            vest.motors[0] = currentPower;
            console.log("motors: " + vest.motors);
            sendMessage(Message_Type_Drive + "|" + vest.motors, vest.port, vest.ip);
        }
        if (up && currentPower < vest.powerMax) {
            currentPower++;
            up = true;
        }
        else {
            up = false;
        }
        if (!up && currentPower > vest.powerMin) {
            currentPower--;
            up = false;
        }
        else {
            up = true;
        }
    }, 20); //50 fps send
};
let times = 0;
const calcInputFreq = () => {
    times++;
};
(0, timers_1.setInterval)(() => {
    console.info("fps: " + times);
    times = 0;
}, 1000);
osc.open({ port: 9001 });
//# sourceMappingURL=index.js.map