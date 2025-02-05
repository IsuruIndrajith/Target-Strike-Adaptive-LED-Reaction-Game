import { initializeApp } from "firebase/app";
import { getDatabase, ref, onValue } from "firebase/database";

// Your Firebase configuration
const firebaseConfig = {
  apiKey: "AIzaSyCNpl9gQ-fkZ9xbbtbTjiNFJqvQTyMlFQg",
  authDomain: "reactnativeapp-bb3d3.firebaseapp.com",
  databaseURL: "https://reactnativeapp-bb3d3-default-rtdb.firebaseio.com/",
  projectId: "reactnativeapp-bb3d3",
  storageBucket: "reactnativeapp-bb3d3.firebasestorage.app",
  messagingSenderId: "439980361430",
  appId: "YOUR_APP_ID"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const db = getDatabase(app);

export { db, ref, onValue };
