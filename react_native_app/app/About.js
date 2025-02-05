import React from "react";
import { View, Text, StyleSheet } from "react-native";

const About = () => {
  return (
    <View style={styles.container}>
      <Text style={styles.title}>About This App</Text>
      <Text style={styles.text}>
            This app is developed for the **Target-Strike Game**.
            Which we developed as the EC6020_Embedded System and Design project, where players use IR blasters to shoot at targets. The system usesESP32 micro controllers and Firebase for real-time communication.
            It detects IR beam hits and counts kills in real-time. The app displays the game status and player scores. We are commercializing the game for entertainment centers and events and feel free to contact us for more information and implementation of the game setup.
      </Text>
    </View>
  );
};

const styles = StyleSheet.create({
  container: { flex: 1, justifyContent: "center", alignItems: "center", padding: 20, backgroundColor: "#121212" },
  title: { fontSize: 24, fontWeight: "bold", color: "#E74C3C", marginBottom: 10 },
  text: { fontSize: 16, color: "#FFF", textAlign: "center" },
});

export default About;
