const firebase = require("firebase");

const firestoreDatabase = firebase.firestore();

const initLedData = () => {
    const ledData = [];

    for (let i = 0; i <= 256; i++) {
        const isRed = Math.round(Math.random()) === 1;
        ledData.push({
            red: isRed ? 255 : 0,
            blue: 0,
            green: 0,
        })
    }

    firestoreDatabase.collection("ledData").add({
        ledData
    });
}

exports.default = initLedData;
