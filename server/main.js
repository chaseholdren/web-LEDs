require('dotenv').config();
const Readline = require('@serialport/parser-readline');
require('./firebase/init').default();
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

        if (sendDataInterval !== null) clearInterval(sendDataInterval);

        if (isSerialPortReady) {
            trySendData(ledData);
        } else {
            sendDataInterval = setInterval(trySendData, 1000, ledData);
        }

    });

const trySendData = (ledData) => {
    if (!isSerialPortReady) return;

    isSerialPortReady = false;

    let ledDataString = `${ledData.length}`;

    for (const ledDataObject of ledData) {
        ledDataString += JSON.stringify(ledDataObject);

    }

    console.log('\nSending led data');
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
