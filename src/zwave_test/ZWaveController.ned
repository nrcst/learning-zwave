package zwavenetwork.zwave_test;

simple ZWaveController
{
    parameters:
        double frequency @unit(MHz);
        double range @unit(m);
        @display("i=device/antennatower");
    gates:
        inout gate[];
}