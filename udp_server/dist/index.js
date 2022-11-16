"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const dgram_1 = __importDefault(require("dgram"));
const Vest_1 = require("./Vest");
const Message_Type_Register = 1;
// --------------------creating a udp server --------------------
// creating a udp server
let server = dgram_1.default.createSocket('udp4');
//devices
let vest;
// emits when any error occurs
server.on('error', function (error) {
    console.log('Error: ' + error);
    server.close();
});
// emits on new datagram msg
server.on('message', function (msg, info) {
    debugMessage(msg, info);
    parseMessage(msg, info);
});
//emits when socket is ready and listening for datagram msgs
server.on('listening', function () {
    var address = server.address();
    var port = address.port;
    var family = address.family;
    var ipaddr = address.address;
    console.log('Server is listening at port : ' + port);
    console.log('Server ip : ' + ipaddr);
    console.log('Server is IP4/IP6 : ' + family);
});
//emits after the socket is closed using socket.close();
server.on('close', function () {
    console.log('Socket is closed !');
});
server.bind(1234);
function debugMessage(msg, info) {
    console.log('Data received from client : ' + msg.toString());
    console.log('Received %d bytes from %s:%d\n', msg.length, info.address, info.port);
}
function parseMessage(msg, info) {
    let message = JSON.parse(msg);
    if (message.mtype === Message_Type_Register) {
        vest = new Vest_1.Vest(message.mac, info.address, info.port);
    }
}
let sendMessage = function (msg, info) {
    server.send(msg, info.port, info.address, function (error) {
        if (error) {
            console.error(error);
        }
        else {
            console.log('return motor mapping');
        }
    });
};
// const interval = setInterval(function() {
//     server.send("ayy",1234,'192.168.137.10',function(error){
//         if(error){
//           client.close();
//         }else{
//           console.log('Data sent !!!');
//         }
//       });
//   }, 1000);
// setTimeout(function(){
// server.close();
// },8000);
// -------------------- udp client ----------------
// var buffer = require('buffer');
// // creating a client socket
// var client = udp.createSocket('udp4');
// //buffer msg
// var data = Buffer.from('siddheshrane');
// client.on('message', function (msg, info) {
//     console.log('Data received from server : ' + msg.toString());
//     console.log('Received %d bytes from %s:%d\n', msg.length, info.address, info.port);
// });
// //sending msg
// client.send(data,1234,'0.0.0.0',function(error){
//   if(error){
//     client.close();
//   }else{
//     console.log('Data sent !!!');
//   }
// });
// var data1 = Buffer.from('hello');
// var data2 = Buffer.from('world');
// //sending multiple msg
// client.send([data1,data2],1234,'192.168.137.10',function(error){
//   if(error){
//     client.close();
//   }else{
//     console.log('Data sent !!!');
//   }
// });
//# sourceMappingURL=index.js.map