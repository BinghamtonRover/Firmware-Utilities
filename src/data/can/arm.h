struct ArmPacket1 {
    private:
        unsigned short swivelAngle;
        unsigned short swivelTarget;
        unsigned short extendAngle;
        unsigned short extendTarget;
    public:
        ArmPacket1(float swivelAngle, float swivelTarget, float extendAngle, float extendTarget) : 
            swivelAngle((unsigned short) (swivelAngle*10000)),
            swivelTarget((unsigned short) (swivelTarget*10000)),
            extendAngle((unsigned short) (extendAngle*10000)),
            extendTarget((unsigned short) (extendTarget*10000)) {}
        float getSwivelAngle(){
            return (float) swivelAngle/10000;
        }
        float getSwivelTarget(){
            return (float) swivelTarget/10000;
        }
        float getExtendAngle(){
            return (float) extendAngle/10000;
        }
        float getExtendTarget(){
            return (float) extendTarget/10000;
        }

};
struct ArmPacket2 {
    private:
        unsigned short liftAngle;
        unsigned short liftTarget;
        bool swivelActivated : 1;
        bool swivelLimSwitch : 1;
        bool extendActivated : 1;
        bool extendLimSwitch : 1;
        bool liftActivated : 1;
        bool liftLimSwitch : 1;
        unsigned char swivelTemp;
        unsigned char extendTemp;
        unsigned char liftTemp;
    public:
        ArmPacket2(float liftAngle, float liftTarget, bool swivelActivated, bool swivelLimSwitch, bool extendActivated, bool extendLimSwitch, bool liftActivated, bool liftLimSwitch, unsigned char swivelTemp, unsigned char extendTemp, unsigned char liftTemp ) : 
            liftAngle((unsigned short) (liftAngle*10000)),
            liftTarget((unsigned short) (liftTarget*10000)), swivelTemp(swivelTemp), extendTemp(extendTemp), liftTemp(liftTemp),
            swivelActivated(swivelActivated),swivelLimSwitch(swivelLimSwitch),extendActivated(extendActivated),
            extendLimSwitch(extendLimSwitch),liftActivated(liftActivated),liftLimSwitch(liftLimSwitch)
             {}
        float getLiftAngle(){
            return (float) liftAngle/10000;
        }
        float getLiftTarget(){
            return (float) liftTarget/10000;
        }
        bool getSwivelActivated(){return swivelActivated;}
        bool getSwivelLimSwitch(){return swivelLimSwitch;}
        bool getExtendActivated(){return extendActivated;}
        bool getExtendLimSwitch(){return extendLimSwitch;}
        bool getLiftActivated(){return liftActivated;}
        bool getLiftLimSwitch(){return liftLimSwitch;}
        unsigned char getSwivelTemp(){return swivelTemp;}
        unsigned char getExtendTemp(){return extendTemp;}
        unsigned char getLiftTemp(){return liftTemp;}
};