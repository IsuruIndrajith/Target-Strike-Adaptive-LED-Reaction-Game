import React from "react";
import { createStackNavigator } from "@react-navigation/stack";
import { NavigationContainer } from "@react-navigation/native";
import { View, Text, Button, StyleSheet } from "react-native";

import About from "./About";
import Developer from "./Developer";
import Contact from "./Contact";

const Stack = createStackNavigator();

const HomeScreen = ({ navigation }) => {
  return (
    <View style={styles.container}>
      <Text style={styles.title}>Welcome to Target-Strike Game</Text>
      <Button title="About" onPress={() => navigation.navigate("About")} />
      <Button title="Developer" onPress={() => navigation.navigate("Developer")} />
      <Button title="Contact" onPress={() => navigation.navigate("Contact")} />
    </View>
  );
};

export default function App() {
  return (
    <NavigationContainer>
      <Stack.Navigator screenOptions={{ headerStyle: { backgroundColor: "#222" }, headerTintColor: "#fff" }}>
        <Stack.Screen name="Home" component={HomeScreen} />
        <Stack.Screen name="About" component={About} />
        <Stack.Screen name="Developer" component={Developer} />
        <Stack.Screen name="Contact" component={Contact} />
      </Stack.Navigator>
    </NavigationContainer>
  );
}

const styles = StyleSheet.create({
  container: { flex: 1, justifyContent: "center", alignItems: "center", backgroundColor: "#121212" },
  title: { fontSize: 20, fontWeight: "bold", color: "#FFF", marginBottom: 20 },
});
