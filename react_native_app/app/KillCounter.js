import React, { useState, useEffect } from "react";
import { View, Text, StyleSheet } from "react-native";
import { db, ref, onValue } from "./firebaseConfig";

const KillCounter = () => {
  const [killCount, setKillCount] = useState(0);

  useEffect(() => {
    // Reference the 'kills' node in Firebase Realtime Database
    const killsRef = ref(db, "kills");

    // Listen for real-time updates
    const unsubscribe = onValue(killsRef, (snapshot) => {
      if (snapshot.exists()) {
        setKillCount(snapshot.val());
      } else {
        setKillCount(0);
      }
    });

    return () => unsubscribe();
  }, []);

  return (
    <View style={styles.container}>
      <Text style={styles.text}>Kill Count</Text>
      <Text style={styles.count}>{killCount}</Text>
    </View>
  );
};

const styles = StyleSheet.create({
  container: {
    alignItems: "center",
    justifyContent: "center",
    flex: 1,
    backgroundColor: "#121212",
  },
  text: {
    fontSize: 24,
    fontWeight: "bold",
    color: "#FFF",
  },
  count: {
    fontSize: 48,
    fontWeight: "bold",
    color: "#E74C3C",
    marginTop: 10,
  },
});

export default KillCounter;
