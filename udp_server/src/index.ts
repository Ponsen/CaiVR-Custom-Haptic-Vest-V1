import udp from "dgram"
import OSC from "osc-js"
import { setInterval } from "timers"

import { Vest } from './Vest'

const Message_Type_Register = 1

// --------------------creating a udp server --------------------

//devices
let vest: Vest
let sendLoop:NodeJS.Timer
let testLoop:NodeJS.Timer 

// creating a udp server
const server = udp.createSocket('udp4')

// emits when any error occurs
server.on('error', (error) => {
    console.log('Error: ' + error)
    server.close()
})

// emits on new datagram msg
server.on('message', (msg, info) => {
    debugMessage(msg, info)
    parseMessage(msg, info)
})

//emits when socket is ready and listening for datagram msgs
server.on('listening', () => {
    var address = server.address()
    var port = address.port
    var family = address.family
    var ipaddr = address.address
    console.log('Server is listening at port : ' + port)
    console.log('Server ip : ' + ipaddr)
    console.log('Server is IP4/IP6 : ' + family)
})

//emits after the socket is closed using socket.close()
server.on('close', () => {
    console.log('Socket is closed !')
})

server.bind(1234)

const debugMessage = (msg, info) => {
    console.log('Data received from client : ' + msg.toString())
    console.log('Received %d bytes from %s:%d\n', msg.length, info.address, info.port)
}

const parseMessage = (msg, info) => {
    try {
        let message = JSON.parse(msg)
        if (message.mtype === Message_Type_Register) {
            vest = new Vest(message.mac, info.address, info.port)
            //TODO: send num motors and set
            vest.setMotors(Array(2).fill(0))
            sendMessage("registered", info.port, info.address)
            startSendLoop()
            //startTestLoop()
        }
    } catch (e) {
        console.error("error parsing message", e)
    }
}

const sendMessage = (msg, port, address) =>  {
    server.send(msg, port, address, (error) => {
        if (error) {
            console.error(error)
        } else {
            //console.log('message sent successfully')
        }

    })
}


/* VRCHAT Stuff */

// receive a message via UDP
const osc = new OSC({ plugin: new OSC.DatagramPlugin() })

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
    console.log("address: " + message.address)
    console.log("types: " + message.types + " args: " + message.args)

    calcInputFreq()

    //TODO: mapping
    if (vest != undefined) {
        if (message.address === "/avatar/parameters/VelocityZ") { //velocity m/s
            let normalized =  Math.abs(parseFloat(message.args))
            if (normalized > maxValue) {
                maxValue = normalized
            }
            if (normalized == 0) { // no infitity plz, thx
                vest.motors[0] = 0;
            } else {
                vest.motors[0] = Math.round(normalized / maxValue * 4095)
            }
        }
    }
})

const startSendLoop = () => {
    if (sendLoop !=  undefined) {
        clearInterval(sendLoop);
    }
    sendLoop = setInterval(() => {
        //TODO: dismiss previous message if equal
        if (vest != undefined) {
            console.log("motors: " + vest.motors)
            sendMessage("drive|" + vest.motors, vest.port, vest.ip);
        }
    }, 20) //50 fps send
}



const startTestLoop = () => {
    let currentPower = vest.powerMin
    let up = true
    console.log("startTestLoop")
    
    if (testLoop !=  undefined) {
        clearInterval(testLoop)
    }
    
    testLoop = setInterval(() => {
        //TODO: dismiss previous message if equal
        if (vest != undefined) {
            vest.motors[0] = currentPower
            console.log("motors: " + vest.motors)
            sendMessage("drive|" + vest.motors, vest.port, vest.ip)
        }
        if (up && currentPower < vest.powerMax) {
            currentPower++
            up = true
        } else {
            up = false
        }
        
        
        if (!up && currentPower > vest.powerMin){
            currentPower--
            up = false
        } else {
            up = true
        }
    }, 20) //50 fps send
}

let times = 0;

const calcInputFreq = () => {
    times++;
}

setInterval(() => {
    console.info("fps: " + times)
    times = 0
}, 1000)

osc.open({ port: 9001 })