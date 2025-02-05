import React from "react";
import { View, StyleSheet } from "react-native";
import KillCounter from "./KillCounter";

const KillCountScreen = () => {
  return (
    <View style={styles.screen}>
      <KillCounter />
    </View>
  );
};

const styles = StyleSheet.create({
  screen: {
    flex: 1,
    backgroundColor: "#1E1E1E",
  },
});

export default KillCountScreen;
