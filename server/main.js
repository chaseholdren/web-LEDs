require('dotenv').config();
const Readline = require('@serialport/parser-readline');
require('./firebase/init')();
const serialport = require('./serial-port/init')();
const firebase = require("firebase");
const firestoreDatabase = firebase.firestore();

let isSerialPortReady = false;

firestoreDatabase.collection("ledData").doc("jstJk4unCGziTWsAT3F0")
    .onSnapshot(function (doc) {
        const {
            ledData
        } = doc.data();
    });

const parser = serialport.pipe(new Readline());

parser.on('data', (data) => {
    console.log(data);
    console.log("hi");
    if (data.trim() === "OK") {
        isSerialPortReady = true;
        console.log("hi2");
        com3port.write("200");
    }
})
