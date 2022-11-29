#pragma pack(push,1)
/// @brief The first data packet to be sent by the Gripper board. 
struct GripperPacket1 {
    private:
        /// @brief Represents the current angle of the rotate joint, scaled by 10000 to fit inside a short.
        unsigned short rotateAngle;
        /// @brief Represents the target angle of the rotate joint, scaled by 10000 to fit inside a short.
        unsigned short rotateTarget;
        /// @brief Represents the current angle of the pinch joint, scaled by 10000 to fit inside a short.
        unsigned short pinchAngle;
        /// @brief Represents the target angle of the pinch joint, scaled by 10000 to fit inside a short.
        unsigned short pinchTarget;
    public:
        /// @brief Generate the packet
        /// @param rotateAngle The current angle of the rotate joint. This should be in radians.
        /// @param rotateTarget The target angle of the rotate joint. This should be in radians.
        /// @param pinchAngle The current angle of the pinch joint. This should be in radians.
        /// @param pinchTarget The target angle of the pinch joint. This should be in radians.
        GripperPacket1(float rotateAngle, float rotateTarget, float pinchAngle, float pinchTarget) : 
            rotateAngle((unsigned short) (rotateAngle*10000)),
            rotateTarget((unsigned short) (rotateTarget*10000)),
            pinchAngle((unsigned short) (pinchAngle*10000)),
            pinchTarget((unsigned short) (pinchTarget*10000)) {}
        /// @brief Get the current rotate angle
        /// @return The angle in radians.
        float getRotateAngle(){
            return (float) rotateAngle/10000;
        }
        /// @brief Get the target rotate angle
        /// @return The angle in radians.
        float getRotateTarget(){
            return (float) rotateTarget/10000;
        }
        /// @brief Get the current pinch angle
        /// @return The angle in radians.
        float getPinchAngle(){
            return (float) pinchAngle/10000;
        }
        /// @brief Get the target pinch angle
        /// @return The angle in radians.
        float getPinchTarget(){
            return (float) pinchTarget/10000;
        }

};
/// @brief The second data packet to be sent by the Gripper board. 
struct GripperPacket2 {
    private:
        /// @brief Represents the current angle of the lift joint, scaled by 10000 to fit inside a short. 
        unsigned short liftAngle;
        /// @brief Represents the target angle of the lift joint, scaled by 10000 to fit inside a short. 
        unsigned short liftTarget;
        /// @brief Flag representing whether the rotate motor is currently activated. 1 for activated, 0 for inactive.
        bool rotateActivated : 1;
        /// @brief Flag representing whether the rotate motor limit switch is currently pressed. 1 for pressed, 0 otherwise.
        bool rotateLimSwitch : 1;
        /// @brief Flag representing whether the pinchd motor is currently activated. 1 for activated, 0 for inactive.
        bool pinchActivated : 1;
        /// @brief Flag representing whether the pinchd motor limit switch is currently pressed. 1 for pressed, 0 otherwise.
        bool pinchLimSwitch : 1;
        /// @brief Flag representing whether the lift motor is currently activated. 1 for activated, 0 for inactive.
        bool liftActivated : 1;
        /// @brief Flag representing whether the lift motor limit switch is currently pressed. 1 for pressed, 0 otherwise.
        bool liftLimSwitch : 1;
        /// @brief Reprsents the temperature of the rotate motor in Celsius. No scaling applied, so values 0-255 correspond to the actual degrees in C.
        unsigned char rotateTemp;
        /// @brief Reprsents the temperature of the pinch motor in Celsius. No scaling applied, so values 0-255 correspond to the actual degrees in C.
        unsigned char pinchTemp;
        /// @brief Reprsents the temperature of the lift motor in Celsius. No scaling applied, so values 0-255 correspond to the actual degrees in C.
        unsigned char liftTemp;
    public:
        /// @brief Create the second arm packet.
        /// @param liftAngle The lift current angle in radians.
        /// @param liftTarget The lift target angle in radians.
        /// @param rotateActivated Whether or not the rotate motor is currently activated.
        /// @param rotateLimSwitch Whether or not the rotate motor limit switch is currently pressed.
        /// @param pinchActivated Whether or not the pinch motor is currently activated.
        /// @param pinchLimSwitch Whether or not the pinch motor limit switch is currently pressed.
        /// @param liftActivated Whether or not the lift motor is currently activated.
        /// @param liftLimSwitch Whether or not the lift motor limit switch is currently pressed.
        /// @param rotateTemp The current temperature of the rotate motor in Celsius (0-255 range)
        /// @param pinchTemp The current temperature of the pinch motor in Celsius (0-255 range)
        /// @param liftTemp The current temperature of the lift motor in Celsius (0-255 range)
        GripperPacket2(float liftAngle, float liftTarget, bool rotateActivated, bool rotateLimSwitch, bool pinchActivated, bool pinchLimSwitch, bool liftActivated, bool liftLimSwitch, unsigned char rotateTemp, unsigned char pinchTemp, unsigned char liftTemp ) : 
            liftAngle((unsigned short) (liftAngle*10000)),
            liftTarget((unsigned short) (liftTarget*10000)), 
            rotateActivated(rotateActivated),rotateLimSwitch(rotateLimSwitch),pinchActivated(pinchActivated),
            pinchLimSwitch(pinchLimSwitch),liftActivated(liftActivated),liftLimSwitch(liftLimSwitch), 
            rotateTemp(rotateTemp), pinchTemp(pinchTemp), liftTemp(liftTemp){}
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
        /// @brief Get whether or not the rotate motor is activated.
        /// @return 1 for activated, 0 for inactive.
        bool getRotateActivated(){return rotateActivated;}
        /// @brief Get whether or not the rotate limit switch is pressed.
        /// @return 1 for pressed, 0 otherwise
        bool getRotateLimSwitch(){return rotateLimSwitch;}
        /// @brief Get whether or not the pinch motor is activated.
        /// @return 1 for activated, 0 for inactive.
        bool getPinchActivated(){return pinchActivated;}
        /// @brief Get whether or not the pinch limit switch is pressed.
        /// @return 1 for pressed, 0 otherwise
        bool getPinchLimSwitch(){return pinchLimSwitch;}
        /// @brief Get whether or not the lift motor is activated.
        /// @return 1 for activated, 0 for inactive.
        bool getLiftActivated(){return liftActivated;}
        /// @brief Get whether or not the lift limit switch is pressed.
        /// @return 1 for pressed, 0 otherwise
        bool getLiftLimSwitch(){return liftLimSwitch;}

        /// @brief Get the current temperature of the rotate motor
        /// @return The temperature in C ranging from 0-255
        unsigned char getRotateTemp(){return rotateTemp;}
        /// @brief Get the current temperature of the rotate motor
        /// @return The temperature in C ranging from 0-255
        unsigned char getPinchTemp(){return pinchTemp;}
        /// @brief Get the current temperature of the rotate motor
        /// @return The temperature in C ranging from 0-255
        unsigned char getLiftTemp(){return liftTemp;}
};