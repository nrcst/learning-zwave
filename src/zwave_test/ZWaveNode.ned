package zwavenetwork.zwave_test;

simple ZWaveNode
{
    parameters:
        int nodeId; 
        double frequency @unit(MHz);
        double range @unit(m);
        double frequencyLow @unit(MHz);
        double frequencyHigh @unit(MHz);
        string deviceType;
        double alarmTestInterval @unit(s) = default(0s);         
        double motionDetectionInterval @unit(s) = default(0s);
        @display("i=block/circle,,green;is=vs");
    gates:
        inout gate[];
}