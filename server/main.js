require('dotenv').config();
const Readline = require('@serialport/parser-readline');
require('./firebase/init').default();
// firebaseInit();
const serialport = require('./serial-port/init').default();
const firebase = require("firebase");
const firestoreDatabase = firebase.firestore();

let isSerialPortReady = false;
let sendDataInterval = null;

firestoreDatabase.collection("ledData").doc("jstJk4unCGziTWsAT3F0")
    .onSnapshot(function (doc) {
        const {
            ledData
        } = doc.data();

        if (isSerialPortReady) {
            trySendData(ledData);
        } else {
            sendDataInterval = setInterval(trySendData, 1000, ledData);
        }

    });

const trySendData = (ledData) => {
    if (!isSerialPortReady) return;

    isSerialPortReady = false;

    ledData = [
        {
            blue: 255,
            green: 0,
            red: 0
        },
        {
            blue: 0,
            green: 255,
            red: 0
        },
        {
            blue: 0,
            green: 0,
            red: 255
        },
    ];

    const ledDataString = JSON.stringify(ledData);
    console.log(`\nSending data: ${ledDataString}`);
    serialport.write(ledDataString);

    clearInterval(sendDataInterval);
}

const parser = serialport.pipe(new Readline());

parser.on('data', (data) => {
    console.log(`\nData received: ${data}`);
    if (data.trim() === "OK") {
        isSerialPortReady = true;
    }
})
