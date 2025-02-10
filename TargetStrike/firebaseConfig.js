// firebaseConfig.js
import { initializeApp } from "firebase/app";
import { getDatabase } from "firebase/database";

const firebaseConfig = {
  apiKey: "AIzaSyCJko2UI_5QcnrQYbIry182wGoCuCUrLRo",
  authDomain: "targetstrike-63c04.firebaseapp.com",
  databaseURL: "https://targetstrike-63c04-default-rtdb.firebaseio.com",
  projectId: "targetstrike-63c04",
  storageBucket: "targetstrike-63c04.firebasestorage.app",
  messagingSenderId: "402423000686",
  appId: "1:402423000686:web:3dcb1a87621b001784807f"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const database = getDatabase(app);

export { database };