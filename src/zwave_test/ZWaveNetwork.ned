package zwavenetwork.zwave_test;
import ned.IdealChannel;

network ZWaveNetwork
{
    parameters:
        int numNodes = default(20);
        double frequency @unit(MHz) = default(908.42MHz);
        double range @unit(m) = default(30m);  // Approximate range for ZWave
        double playgroundSizeX @unit(m) = default(100m);
        double playgroundSizeY @unit(m) = default(100m);
        @display("bgb=unit($playgroundSizeX, m), unit($playgroundSizeY, m)");
    submodules:
        controller: ZWaveController {
            @display("p=50,50;i=device/antennatower");
        }
        node[numNodes]: ZWaveNode {
            @display("p=,,m,0.5,0.5;is=s");
        }
    connections allowunconnected:
        // Koneksi Controller ke Node
        for i=0..numNodes-1 {
            controller.gate++ <--> IdealChannel <--> node[i].gate++;
        }

        // Koneksi Node ke Node (Mesh) - Menggunakan ekspresi untuk simulasi nested loop
        for i=0..numNodes-1, for j=i+1..numNodes-1 {
            node[i].gate++ <--> IdealChannel <--> node[j].gate++;}
}