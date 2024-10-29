package zwavenetwork.zwave_test;
package zwavenetwork;

simple ZWaveNode
{
    parameters:
        double frequency @unit(MHz);
        double range @unit(m);
        @display("i=device/wifilaptop");
    gates:
        inout gate[];
}