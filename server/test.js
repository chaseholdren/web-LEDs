require('dotenv').config();
const Readline = require('@serialport/parser-readline');
require('./firebase/init').default();
const serialport = require('./serial-port/init').default();
const firebase = require("firebase");
const firestoreDatabase = firebase.firestore();

let isSerialPortReady = false;
let sendDataInterval = null;
let currentIndex = 0;

const white = '{"blue":255,"green":255,"red":255}';
const black = '{"blue":0,"green":0,"red":0}';

const tings = [224, 225, 226, 227, 226, 225];

const trySendData = () => {
    if (!isSerialPortReady) return;

    isSerialPortReady = false;

    const currentTing = tings[currentIndex++ % tings.length];

    const ledCount = 255;

    let ledDataString = `${ledCount}`;

    for (let i = 0; i <= ledCount; i++) {
        if (i === currentTing) {
            ledDataString += white;
        } else {
            ledDataString += black;
        }
    }

    console.log('\nSending led data');
    serialport.write(ledDataString);

    clearInterval(sendDataInterval);
}

setInterval(trySendData, 100);

const parser = serialport.pipe(new Readline());

parser.on('data', (data) => {
    console.log(`\nData received: ${data}`);
    if (data.trim() === "OK") {
        isSerialPortReady = true;
    }
})
