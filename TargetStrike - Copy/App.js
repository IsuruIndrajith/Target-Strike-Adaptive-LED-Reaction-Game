import React, { useState, useEffect } from "react";
import { View, Text, TouchableOpacity, StyleSheet, ActivityIndicator, Modal, ScrollView, Image } from "react-native";
import { database } from "./firebaseConfig";
import { ref, onValue, set } from "firebase/database";
import { MaterialIcons } from '@expo/vector-icons';

export default function App() {
    const [killCount, setKillCount] = useState(0);
    const [loading, setLoading] = useState(true);
    const [showAbout, setShowAbout] = useState(false);

    useEffect(() => {
        const killsRef = ref(database, "kills");

        // Listen for real-time changes in Firebase
        const unsubscribe = onValue(
            killsRef,
            (snapshot) => {
                const data = snapshot.val();
                console.log("Fetched data from Firebase:", data); // Debugging log
                setKillCount(data || 0);
                setLoading(false); // Data fetched, stop loading
            },
            (error) => {
                console.error("Error fetching data:", error);
                setLoading(false); // Stop loading even if there's an error
            }
        );

        return () => unsubscribe(); // ✅ Proper cleanup
    }, []);

    const incrementKills = () => {
        const killsRef = ref(database, "kills");
        set(killsRef, killCount + 1)
            .then(() => console.log("Kill count updated!"))
            .catch((error) => console.error("Update failed:", error));
    };

    return (
        <View style={styles.container}>
            <View style={styles.header}>
                <Text style={styles.title}>Target Strike</Text>
                <TouchableOpacity onPress={() => setShowAbout(true)}>
                    <MaterialIcons name="info" size={24} color="#fff" />
                </TouchableOpacity>
            </View>

            {loading ? (
                <ActivityIndicator size="large" color="#4CAF50" />
            ) : (
                <View style={styles.gameContainer}>
                    <View style={styles.scoreCard}>
                        <Text style={styles.scoreLabel}>TOTAL STRIKES</Text>
                        <Text style={styles.count}>{killCount}</Text>
                    </View>

                    {/* Target Image */}
                    <Image source={require('./assets/target.png')} style={styles.targetImage} />

                    <TouchableOpacity
                        style={styles.strikeButton}
                        onPress={incrementKills}>
                        <Text style={styles.strikeButtonText}>STRIKE TARGET</Text>
                    </TouchableOpacity>
                </View>
            )}

            {/* About Modal */}
            <Modal
                visible={showAbout}
                animationType="slide"
                transparent={true}>
                <View style={styles.modalContainer}>
                    <View style={styles.modalContent}>
                        <Text style={styles.modalTitle}>About Target Strike</Text>
                        <ScrollView style={styles.modalScroll}>
                            <Text style={styles.modalText}>
                                Target Strike is an adaptive reaction game that tests your reflexes and timing.
                            </Text>
                            <Text style={styles.modalSubtitle}>How to Play:</Text>
                            <Text style={styles.modalText}>
                                • Select the One player or Two player 🤖{'\n'}
                                • Select the game mode{'\n'}
                                ( Easy 10s | Medium 7s | Hard 5s ){'\n'}
                                • Watch for the target 🎯{'\n'}
                                • Strike as quickly as you can when the target appears 🔫{'\n'}
                                • Score points for accurate hits 🥳{'\n'}
                                • Challenge yourself and others to beat your high score 🎮
                            </Text>
                            <Text style={styles.modalSubtitle}>Version:</Text>
                            <Text style={styles.modalText}>2.0.0</Text>
                        </ScrollView>
                        <TouchableOpacity
                            style={styles.closeButton}
                            onPress={() => setShowAbout(false)}>
                            <Text style={styles.closeButtonText}>Close</Text>
                        </TouchableOpacity>
                    </View>
                </View>
            </Modal>
        </View>
    );
}

const styles = StyleSheet.create({
    container: {
        flex: 1,
        backgroundColor: '#1a1a1a',
    },
    header: {
        flexDirection: 'row',
        justifyContent: 'space-between',
        alignItems: 'center',
        padding: 20,
        paddingTop: 50,
        backgroundColor: '#2d2d2d',
    },
    title: {
        fontSize: 28,
        fontWeight: 'bold',
        color: '#fff',
    },
    gameContainer: {
        flex: 1,
        justifyContent: 'center',
        alignItems: 'center',
        padding: 20,
    },
    scoreCard: {
        backgroundColor: '#2d2d2d',
        padding: 20,
        borderRadius: 15,
        alignItems: 'center',
        marginBottom: 30,
        width: '100%',
        elevation: 5,
    },
    scoreLabel: {
        fontSize: 16,
        color: '#4CAF50',
        marginBottom: 10,
    },
    count: {
        fontSize: 48,
        fontWeight: 'bold',
        color: '#4CAF50',
    },
    targetImage: {
        width: 255,
        height: 400,
        marginBottom: 20,
    },
    strikeButton: {
        backgroundColor: '#4CAF50',
        paddingVertical: 15,
        paddingHorizontal: 30,
        borderRadius: 25,
        elevation: 5,
    },
    strikeButtonText: {
        color: '#fff',
        fontSize: 18,
        fontWeight: 'bold',
    },
    modalContainer: {
        flex: 1,
        justifyContent: 'center',
        alignItems: 'center',
        backgroundColor: 'rgba(0,0,0,0.5)',
    },
    modalContent: {
        backgroundColor: '#2d2d2d',
        borderRadius: 20,
        padding: 20,
        width: '90%',
        maxHeight: '80%',
    },
    modalTitle: {
        fontSize: 24,
        fontWeight: 'bold',
        color: '#fff',
        marginBottom: 15,
        textAlign: 'center',
    },
    modalScroll: {
        marginVertical: 15,
    },
    modalSubtitle: {
        fontSize: 18,
        fontWeight: 'bold',
        color: '#4CAF50',
        marginTop: 15,
        marginBottom: 10,
    },
    modalText: {
        fontSize: 16,
        color: '#fff',
        lineHeight: 24,
        marginBottom: 10,
    },
    closeButton: {
        backgroundColor: '#4CAF50',
        padding: 15,
        borderRadius: 25,
        alignItems: 'center',
        marginTop: 15,
    },
    closeButtonText: {
        color: '#fff',
        fontSize: 16,
        fontWeight: 'bold',
    },
});