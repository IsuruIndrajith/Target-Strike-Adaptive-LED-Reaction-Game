import React from "react";
import { View, Text, StyleSheet } from "react-native";

const Contact = () => {
  return (
    <View style={styles.container}>
      <Text style={styles.title}>Contact Information</Text>
      <Text style={styles.text}>Email: your-email@example.com</Text>
      <Text style={styles.text}>LinkedIn: linkedin.com/in/yourprofile</Text>
      <Text style={styles.text}>GitHub: github.com/yourusername</Text>
    </View>
  );
};

const styles = StyleSheet.create({
  container: { flex: 1, justifyContent: "center", alignItems: "center", padding: 20, backgroundColor: "#121212" },
  title: { fontSize: 24, fontWeight: "bold", color: "#2ECC71", marginBottom: 10 },
  text: { fontSize: 16, color: "#FFF", textAlign: "center", marginVertical: 5 },
});

export default Contact;
