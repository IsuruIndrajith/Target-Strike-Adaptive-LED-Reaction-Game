import React from "react";
import { View, Text, StyleSheet } from "react-native";

const Developer = () => {
  return (
    <View style={styles.container}>
      <Text style={styles.title}>Developer Info</Text>
      <Text style={styles.text}>App Developed by Isuru Indrajith</Text>
      <Text style={styles.text}>B.Sc.(Hons) Computer Engineering | Undergraduate | UOJ</Text>
      <Text style={styles.text}>Passionate about IoT & AI</Text>
    </View>
  );
};

const styles = StyleSheet.create({
  container: { flex: 1, justifyContent: "center", alignItems: "center", padding: 20, backgroundColor: "#121212" },
  title: { fontSize: 24, fontWeight: "bold", color: "#3498DB", marginBottom: 10 },
  text: { fontSize: 16, color: "#FFF", textAlign: "center", marginVertical: 5 },
});

export default Developer;
