import React, { useState, useEffect } from "react";
import { View, Text, Button, StyleSheet } from "react-native";
import { database } from "./firebaseConfig";
import { ref, onValue, set } from "firebase/database";

export default function App() {
    const [killCount, setKillCount] = useState(0);

    useEffect(() => {
        const killsRef = ref(database, "game/kills");

        // Listen for real-time changes in Firebase
        const unsubscribe = onValue(killsRef, (snapshot) => {
            const data = snapshot.val();
            setKillCount(data || 0);
        });

        return () => unsubscribe();  // ✅ Proper cleanup
    }, []);

    const incrementKills = () => {
        const killsRef = ref(database, "game/kills");
        set(killsRef, killCount + 1)
            .then(() => console.log("Kill count updated!"))
            .catch((error) => console.error("Update failed:", error));
    };

    return (
        <View style={styles.container}>
            <Text style={styles.title}>Target Strike</Text>
            <Text style={styles.count}>Kills: {killCount}</Text>
            <Button title="Add Kill" onPress={incrementKills} />
        </View>
    );
}

const styles = StyleSheet.create({
    container: { flex: 1, justifyContent: "center", alignItems: "center" },
    title: { fontSize: 24, fontWeight: "bold", marginBottom: 20 },
    count: { fontSize: 30, color: "red", marginBottom: 20 },
});
