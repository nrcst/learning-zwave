package zwavenetwork.zwave_test;

simple ZWaveController
{
    parameters:
        bool sendCommands = default(false);
        int commandNode = default(-1);
        string commandType = default("");

        double frequency @unit(MHz);
        double range @unit(m);

        @display("i=block/broadcast;is=l");
    gates:
        inout gate[];    
}