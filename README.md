# 🎯 **Target Strike: Interactive Shooter Game**

## 🚀 **Overview**  
**Target Strike** is a compact embedded system game offering a 🎉 thrilling, 🛡️ ultra-safe, and 🎯 precision-focused experience. It replaces physical projectiles with infrared laser targeting and photodiode detection systems, creating an accessible competitive environment. Features intuitive LED-guided interactions and real-time LCD feedback for dynamic gameplay.

---

## 🤝 **Contributors**  
- **Indrajith E.M.I.** - 2021/E/035  
- **Weerathunga I.S.L.** - 2021/E/055  
- **Koswaththa A.D.** - 2021/E/020  
- **Jayasinghe R.R.G.I.S.** - 2021/E/122  
- **Kasuhalya K.A.P.T.** - 2021/E/070  

---

## ✨ **Key Features**  
- 🔦 **Laser Targeting System**: Eye-safe IR lasers (850nm) with photodiode receivers  
- 💡 **Interactive LED Buttons**: Illuminated controls for target selection/feedback  
- 🎮 **Game Modes**:  
  - *Solo Challenge*: Reaction-time leaderboards  
  - *Duel Mode*: Head-to-head laser battles  
- ⚡ **Adaptive Difficulty**: Auto-adjusts target duration (2-6s) based on performance  
- 📊 **LCD Dashboard**: Real-time scores, game timers, and mode selection  

---

## 🛠️ **Hardware Components**  
1. 📡 **ESP32 Microcontrollers (x2)** - System control/core logic  
2. 🔫 **IR Laser Guns (x2)** - Class 1 safety-rated emitters  
3. 🎯 **Photodiode Targets** - SFH 213 FA sensors with analog detection  
4. 📟 **20x4 LCD Display** - Game interface and statistics  
5. 💡 **LED-Button Modules** - Illuminated pushbuttons (10mm)  
6. 🔊 **Piezo Buzzer** - Multi-tone audio feedback  
7. 🔋 **LiPo Battery Pack** - 7.4V 2000mAh portable power  

---

## 🎮 **Gameplay Flow**  
1. **Setup**  
   - Players choose mode via LED-lit buttons  
   - LCD displays difficulty options (Easy/Medium/Hard)  

2. **Target Activation**  
   - Random photodiodes trigger paired LED indicators  
   - Active targets glow until hit or timeout  

3. **Engagement**  
   - 🔫 Shoot activated targets with IR laser  
   - ✅ Successful hits confirmed by:  
     - LED flash (200ms)  
     - Buzzer confirmation tone  
     - LCD score update  

4. **Scoring**  
   - +10 points per hit (Easy), +15 (Medium), +20 (Hard)  
   - 🏆 Victory fanfare with LCD animation for winner  

---

## ⚠️ **Safety & Specifications**  
- 👁️ **Laser Safety**: Class 1 certified (<0.39mW output)  
- 🔋 **Power System**: 2-hour continuous play, auto-shutdown at 3.2V/cell  
- 🛡️ **Circuit Protection**: Resettable fuses for all power rails  
- 📏 **Dimensions**: 220mm × 160mm × 50mm portable enclosure  

*System achieves 98% shot detection accuracy with <15ms latency using photodiode targeting* 🔥  
