// src/firebaseConfig.ts
import { initializeApp } from "firebase/app";
import { getDatabase } from "firebase/database";

const firebaseConfig = {
    apiKey: "AIzaSyCzgFLyA7LiG0Oht8mvgPzaEeNmthAhGYw",
    authDomain: "iot-ble-system.firebaseapp.com",
    databaseURL: "https://iot-ble-system-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "iot-ble-system",
    storageBucket: "iot-ble-system.firebasestorage.app",
    messagingSenderId: "1026718893994",
    appId: "1:1026718893994:web:7e56e5d8029ef4d9890dd3",
    measurementId: "G-S20QMXKD0E"
};

const app = initializeApp(firebaseConfig);
const database = getDatabase(app);

export { database };
