const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
const firebase = require("firebase");

// Initialize Firebase
var config = {
    apiKey: "AIzaSyDIl_SsxnPVi7Ys5I2W9j4pCZiz0fxBOvg",
    authDomain: "dweenoh.firebaseapp.com",
    databaseURL: "https://dweenoh.firebaseio.com",
    projectId: "dweenoh",
    storageBucket: "dweenoh.appspot.com",
    messagingSenderId: "1097424934628"
};
firebase.initializeApp(config);
const db = firebase.firestore();

db.collection("ledData").doc("PJXXO3q2N1pr10D3Yn5c")
    .onSnapshot(function (doc) {
        const {
            leds
        } = doc.data();
        console.log("leds: ", leds);
    });


const onOpen = (port) => {
    port.write("50");
}

// list serial ports:
SerialPort.list(function (err, ports) {
    ports.forEach(function (port) {
        console.log(port.comName);
    });
});

const com3port = new SerialPort('COM3', {
    baudRate: 115200,
    autoOpen: false,
});

com3port.open(function (err) {
    if (err) {
        return console.log('Error opening port: ', err.message)
    }
    // setTimeout(() => {
    //     onOpen(com3port);
    // }, 3000);
});

// Open errors will be emitted as an error event
com3port.on('error', function (err) {
    console.log('Error: ', err.message)
})

com3port.on('data', function (data) {
    console.log('Data:', data);
});

const parser = com3port.pipe(new Readline());

parser.on('data', (data) => {
    console.log(data);
    console.log("hi");
    if (data.trim() === "setup4") {
        console.log("hi2");
        // com3port.write("200");
    }
})

// The open event is always emitted
com3port.on('open', function () {
    // open logic
    onOpen(com3port);
})
