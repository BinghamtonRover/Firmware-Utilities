struct ArmPacket1 {
    unsigned short swivelAngle;
    unsigned short swivelTarget;
    unsigned short extendAngle;
    unsigned short extendTarget;
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
    unsigned short liftAngle;
    unsigned short liftTarget;
    unsigned int flags : 6;
    unsigned char swivelTemp;
    unsigned char extendTemp;
    unsigned char liftTemp;
    ArmPacket2(float liftAngle, float liftTarget, bool swivelActivated, bool swivelLimSwitch, bool extendActivated, bool extendLimSwitch, bool liftActivated, bool liftLimSwitch, unsigned char swivelTemp, unsigned char extendTemp, unsigned char liftTemp ) : 
        liftAngle((unsigned short) (liftAngle*10000)),
        liftTarget((unsigned short) (liftTarget*10000)), flags(0), swivelTemp(swivelTemp), extendTemp(extendTemp), liftTemp(liftTemp) {
            if(swivelActivated) flags|=0b1;
            if(swivelLimSwitch) flags|=0b01;
            if(extendActivated) flags|=0b001;
            if(extendLimSwitch) flags|=0b0001;
            if(liftActivated) flags|=0b00001;
            if(liftLimSwitch) flags|=0b000001;
        }
    float getLiftAngle(){
        return (float) liftAngle/10000;
    }
    float getLiftTarget(){
        return (float) liftTarget/10000;
    }
    bool getSwivelActivated(){return (flags&0b1)>0;}
    bool getSwivelLimSwitch(){return (flags&0b01)>0;}
    bool getExtendActivated(){return (flags&0b001)>0;}
    bool getExtendLimSwitch(){return (flags&0b0001)>0;}
    bool getLiftActivated(){return (flags&0b00001)>0;}
    bool getLiftLimSwitch(){return (flags&0b000001)>0;}
    unsigned char getSwivelTemp(){return swivelTemp;}
    unsigned char getExtendTemp(){return extendTemp;}
    unsigned char getLiftTemp(){return liftTemp;}
};