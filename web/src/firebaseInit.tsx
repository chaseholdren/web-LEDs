import firebase from 'firebase';

const firebaseConfig = {
    apiKey: "AIzaSyDIl_SsxnPVi7Ys5I2W9j4pCZiz0fxBOvg",
    authDomain: "dweenoh.firebaseapp.com",
    databaseURL: "https://dweenoh.firebaseio.com",
    projectId: "dweenoh",
    storageBucket: "dweenoh.appspot.com",
    messagingSenderId: "1097424934628"
};

firebase.initializeApp(firebaseConfig);
export const firestore = firebase.firestore();
