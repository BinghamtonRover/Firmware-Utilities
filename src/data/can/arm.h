#pragma pack(push,1)
/// @brief The first data packet to be sent by the Arm board. 
struct ArmPacket1 {
    private:
        /// @brief Represents the current angle of the swivel joint, scaled by 10000 to fit inside a short.
        unsigned short swivelAngle;
        /// @brief Represents the target angle of the swivel joint, scaled by 10000 to fit inside a short.
        unsigned short swivelTarget;
        /// @brief Represents the current angle of the extend joint, scaled by 10000 to fit inside a short.
        unsigned short extendAngle;
        /// @brief Represents the target angle of the extend joint, scaled by 10000 to fit inside a short.
        unsigned short extendTarget;
    public:
        /// @brief Generate the packet
        /// @param swivelAngle The current angle of the swivel joint. This should be in radians.
        /// @param swivelTarget The target angle of the swivel joint. This should be in radians.
        /// @param extendAngle The current angle of the extend joint. This should be in radians.
        /// @param extendTarget The target angle of the extend joint. This should be in radians.
        ArmPacket1(float swivelAngle, float swivelTarget, float extendAngle, float extendTarget) : 
            swivelAngle((unsigned short) (swivelAngle*10000)),
            swivelTarget((unsigned short) (swivelTarget*10000)),
            extendAngle((unsigned short) (extendAngle*10000)),
            extendTarget((unsigned short) (extendTarget*10000)) {}
        /// @brief Get the current swivel angle
        /// @return The angle in radians.
        float getSwivelAngle(){
            return (float) swivelAngle/10000;
        }
        /// @brief Get the target swivel angle
        /// @return The angle in radians.
        float getSwivelTarget(){
            return (float) swivelTarget/10000;
        }
        /// @brief Get the current extend angle
        /// @return The angle in radians.
        float getExtendAngle(){
            return (float) extendAngle/10000;
        }
        /// @brief Get the target extend angle
        /// @return The angle in radians.
        float getExtendTarget(){
            return (float) extendTarget/10000;
        }

};
/// @brief The second data packet to be sent by the Arm board. 
struct ArmPacket2 {
    private:
        /// @brief Represents the current angle of the lift joint, scaled by 10000 to fit inside a short. 
        unsigned short liftAngle;
        /// @brief Represents the target angle of the lift joint, scaled by 10000 to fit inside a short. 
        unsigned short liftTarget;
        /// @brief Flag representing whether the swivel motor is currently activated. 1 for activated, 0 for inactive.
        bool swivelActivated : 1;
        /// @brief Flag representing whether the swivel motor limit switch is currently pressed. 1 for pressed, 0 otherwise.
        bool swivelLimSwitch : 1;
        /// @brief Flag representing whether the extendd motor is currently activated. 1 for activated, 0 for inactive.
        bool extendActivated : 1;
        /// @brief Flag representing whether the extendd motor limit switch is currently pressed. 1 for pressed, 0 otherwise.
        bool extendLimSwitch : 1;
        /// @brief Flag representing whether the lift motor is currently activated. 1 for activated, 0 for inactive.
        bool liftActivated : 1;
        /// @brief Flag representing whether the lift motor limit switch is currently pressed. 1 for pressed, 0 otherwise.
        bool liftLimSwitch : 1;
        /// @brief Reprsents the temperature of the swivel motor in Celsius. No scaling applied, so values 0-255 correspond to the actual degrees in C.
        unsigned char swivelTemp;
        /// @brief Reprsents the temperature of the extend motor in Celsius. No scaling applied, so values 0-255 correspond to the actual degrees in C.
        unsigned char extendTemp;
        /// @brief Reprsents the temperature of the lift motor in Celsius. No scaling applied, so values 0-255 correspond to the actual degrees in C.
        unsigned char liftTemp;
    public:
        /// @brief Create the second arm packet.
        /// @param liftAngle The lift current angle in radians.
        /// @param liftTarget The lift target angle in radians.
        /// @param swivelActivated Whether or not the swivel motor is currently activated.
        /// @param swivelLimSwitch Whether or not the swivel motor limit switch is currently pressed.
        /// @param extendActivated Whether or not the extend motor is currently activated.
        /// @param extendLimSwitch Whether or not the extend motor limit switch is currently pressed.
        /// @param liftActivated Whether or not the lift motor is currently activated.
        /// @param liftLimSwitch Whether or not the lift motor limit switch is currently pressed.
        /// @param swivelTemp The current temperature of the swivel motor in Celsius (0-255 range)
        /// @param extendTemp The current temperature of the extend motor in Celsius (0-255 range)
        /// @param liftTemp The current temperature of the lift motor in Celsius (0-255 range)
        ArmPacket2(float liftAngle, float liftTarget, bool swivelActivated, bool swivelLimSwitch, bool extendActivated, bool extendLimSwitch, bool liftActivated, bool liftLimSwitch, unsigned char swivelTemp, unsigned char extendTemp, unsigned char liftTemp ) : 
            liftAngle((unsigned short) (liftAngle*10000)),
            liftTarget((unsigned short) (liftTarget*10000)), swivelTemp(swivelTemp), extendTemp(extendTemp), liftTemp(liftTemp),
            swivelActivated(swivelActivated),swivelLimSwitch(swivelLimSwitch),extendActivated(extendActivated),
            extendLimSwitch(extendLimSwitch),liftActivated(liftActivated),liftLimSwitch(liftLimSwitch)
             {}
        /// @brief Get the lift current angle.
        /// @return The angle in radians.
        float getLiftAngle(){
            return (float) liftAngle/10000;
        }
        /// @brief Get the lift target angle.
        /// @return The angle in radians.
        float getLiftTarget(){
            return (float) liftTarget/10000;
        }
        /// @brief Get whether or not the swivel motor is activated.
        /// @return 1 for activated, 0 for inactive.
        bool getSwivelActivated(){return swivelActivated;}
        /// @brief Get whether or not the swivel limit switch is pressed.
        /// @return 1 for pressed, 0 otherwise
        bool getSwivelLimSwitch(){return swivelLimSwitch;}
        /// @brief Get whether or not the extend motor is activated.
        /// @return 1 for activated, 0 for inactive.
        bool getExtendActivated(){return extendActivated;}
        /// @brief Get whether or not the extend limit switch is pressed.
        /// @return 1 for pressed, 0 otherwise
        bool getExtendLimSwitch(){return extendLimSwitch;}
        /// @brief Get whether or not the lift motor is activated.
        /// @return 1 for activated, 0 for inactive.
        bool getLiftActivated(){return liftActivated;}
        /// @brief Get whether or not the lift limit switch is pressed.
        /// @return 1 for pressed, 0 otherwise
        bool getLiftLimSwitch(){return liftLimSwitch;}

        /// @brief Get the current temperature of the swivel motor
        /// @return The temperature in C ranging from 0-255
        unsigned char getSwivelTemp(){return swivelTemp;}
        /// @brief Get the current temperature of the swivel motor
        /// @return The temperature in C ranging from 0-255
        unsigned char getExtendTemp(){return extendTemp;}
        /// @brief Get the current temperature of the swivel motor
        /// @return The temperature in C ranging from 0-255
        unsigned char getLiftTemp(){return liftTemp;}
};