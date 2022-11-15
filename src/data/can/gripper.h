
struct GripperPacket1 {
    private:
        unsigned short rotateAngle;
        unsigned short rotateTarget;
        unsigned short pinchAngle;
        unsigned short pinchTarget;
    public:
        GripperPacket1(float rotateAngle, float rotateTarget, float pinchAngle, float pinchTarget) : 
            rotateAngle((unsigned short) (rotateAngle*10000)),
            rotateTarget((unsigned short) (rotateTarget*10000)),
            pinchAngle((unsigned short) (pinchAngle*10000)),
            pinchTarget((unsigned short) (pinchTarget*10000)) {}
        float getRotateAngle(){
            return (float) rotateAngle/10000;
        }
        float getRotateTarget(){
            return (float) rotateTarget/10000;
        }
        float getPinchAngle(){
            return (float) pinchAngle/10000;
        }
        float getPinchTarget(){
            return (float) pinchTarget/10000;
        }

};
struct GripperPacket2 {
    private:
        unsigned short liftAngle;
        unsigned short liftTarget;
        bool rotateActivated : 1;
        bool rotateLimSwitch : 1;
        bool pinchActivated : 1;
        bool pinchLimSwitch : 1;
        bool liftActivated : 1;
        bool liftLimSwitch : 1;
        unsigned char rotateTemp;
        unsigned char pinchTemp;
        unsigned char liftTemp;
    public:
        GripperPacket2(float liftAngle, float liftTarget, bool rotateActivated, bool rotateLimSwitch, bool pinchActivated, bool pinchLimSwitch, bool liftActivated, bool liftLimSwitch, unsigned char rotateTemp, unsigned char pinchTemp, unsigned char liftTemp ) : 
            liftAngle((unsigned short) (liftAngle*10000)),
            liftTarget((unsigned short) (liftTarget*10000)), rotateTemp(rotateTemp), pinchTemp(pinchTemp), liftTemp(liftTemp),
            rotateActivated(rotateActivated),rotateLimSwitch(rotateActivated),pinchActivated(pinchActivated),
            pinchLimSwitch(pinchLimSwitch),liftActivated(liftActivated),liftLimSwitch(liftLimSwitch) {
            }
        float getLiftAngle(){
            return (float) liftAngle/10000;
        }
        float getLiftTarget(){
            return (float) liftTarget/10000;
        }
        bool getRotateActivated(){return rotateActivated;}
        bool getRotateLimSwitch(){return rotateLimSwitch;}
        bool getPinchActivated(){return pinchActivated;}
        bool getPinchLimSwitch(){return pinchLimSwitch;}
        bool getLiftActivated(){return liftActivated;}
        bool getLiftLimSwitch(){return liftLimSwitch;}
        unsigned char getRotateTemp(){return rotateTemp;}
        unsigned char getPinchTemp(){return pinchTemp;}
        unsigned char getLiftTemp(){return liftTemp;}
};