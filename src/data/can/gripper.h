struct GripperPacket1 {
    unsigned short rotateAngle;
    unsigned short rotateTarget;
    unsigned short pinchAngle;
    unsigned short pinchTarget;
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
    unsigned short liftAngle;
    unsigned short liftTarget;
    unsigned int flags : 6;
    unsigned char rotateTemp;
    unsigned char pinchTemp;
    unsigned char liftTemp;
    GripperPacket2(float liftAngle, float liftTarget, bool rotateActivated, bool rotateLimSwitch, bool pinchActivated, bool pinchLimSwitch, bool liftActivated, bool liftLimSwitch, unsigned char rotateTemp, unsigned char pinchTemp, unsigned char liftTemp ) : 
        liftAngle((unsigned short) (liftAngle*10000)),
        liftTarget((unsigned short) (liftTarget*10000)), flags(0), rotateTemp(rotateTemp), pinchTemp(pinchTemp), liftTemp(liftTemp) {
            if(rotateActivated) flags|=0b1;
            if(rotateLimSwitch) flags|=0b01;
            if(pinchActivated) flags|=0b001;
            if(pinchLimSwitch) flags|=0b0001;
            if(liftActivated) flags|=0b00001;
            if(liftLimSwitch) flags|=0b000001;
        }
    float getLiftAngle(){
        return (float) liftAngle/10000;
    }
    float getLiftTarget(){
        return (float) liftTarget/10000;
    }
    bool getRotateActivated(){return (flags&0b1)>0;}
    bool getRotateLimSwitch(){return (flags&0b01)>0;}
    bool getPinchActivated(){return (flags&0b001)>0;}
    bool getPinchLimSwitch(){return (flags&0b0001)>0;}
    bool getLiftActivated(){return (flags&0b00001)>0;}
    bool getLiftLimSwitch(){return (flags&0b000001)>0;}
    unsigned char getRotateTemp(){return rotateTemp;}
    unsigned char getPinchTemp(){return pinchTemp;}
    unsigned char getLiftTemp(){return liftTemp;}
};