package zwavenetwork.zwave_test;
import ned.IdealChannel;

network ZWaveNetwork
{
    parameters:
        int numNodes = default(20);
        double frequency @unit(MHz) = default(908.42MHz);
        double range @unit(m) = default(30m);
        double playgroundSizeX @unit(m) = default(100m);
        double playgroundSizeY @unit(m) = default(100m);
        @display("bgb=unit($playgroundSizeX, m), unit($playgroundSizeY, m)");
    submodules:
        controller: ZWaveController {
            parameters:
                @display("p=50,30;i=device/antennatower");
        }
        node[numNodes]: ZWaveNode {
            parameters:
                @display("is=s");
        }
    
    connections allowunconnected:
        // First row horizontal connections (nodes 0-4)
        node[0].gate++ <--> IdealChannel <--> node[1].gate++;
        node[1].gate++ <--> IdealChannel <--> node[2].gate++;
        node[2].gate++ <--> IdealChannel <--> node[3].gate++;
        node[3].gate++ <--> IdealChannel <--> node[4].gate++;
        
        // Second row horizontal connections (nodes 5-9)
        node[5].gate++ <--> IdealChannel <--> node[6].gate++;
        node[6].gate++ <--> IdealChannel <--> node[7].gate++;
        node[7].gate++ <--> IdealChannel <--> node[8].gate++;
        node[8].gate++ <--> IdealChannel <--> node[9].gate++;
        
        // Vertical connections between rows
        node[0].gate++ <--> IdealChannel <--> node[5].gate++;
        node[1].gate++ <--> IdealChannel <--> node[6].gate++;
        node[2].gate++ <--> IdealChannel <--> node[7].gate++;
        node[3].gate++ <--> IdealChannel <--> node[8].gate++;
        node[4].gate++ <--> IdealChannel <--> node[9].gate++;
        
        // Controller connections to each row
        controller.gate++ <--> IdealChannel <--> node[2].gate++; // Connect to middle of first row
        controller.gate++ <--> IdealChannel <--> node[7].gate++; // Connect to middle of second row
}