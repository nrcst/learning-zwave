package zwavenetwork.zwave_test;

simple ZWaveController
{
    parameters:
        int nodeId = default(-1); 
        double frequency @unit(MHz);
        double range @unit(m);
        double frequencyLow @unit(MHz);
        double frequencyHigh @unit(MHz);
        bool sendCommands = default(false);
        int commandNode = default(-1);
        string commandType = default("");
        @display("i=device/accesspoint");
    gates:
        inout gate[5]; 
}