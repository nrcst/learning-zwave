package zwavenetwork.zwave_test;
import ned.IdealChannel;

network ZWaveNetwork
{
    parameters:
        @display("bgb=$playgroundSizeX,$playgroundSizeY;bgp=0,0;bgi=default");
        double frequency @unit(MHz) = default(908.42MHz);
        double range @unit(m) = default(30m);
        double playgroundSizeX @unit(m) = default(100m);
        double playgroundSizeY @unit(m) = default(60m);
        @display("bgb=unit($playgroundSizeX, m), unit($playgroundSizeY, m)");
    
    submodules:
        controller: ZWaveController {
            parameters:
                @display("p=50,30;i=device/antennatower");
        }
        node[8]: ZWaveNode {
            parameters:
                @display("is=s");
        }
    
    connections allowunconnected:
//        node[0].gate++ <--> IdealChannel <--> node[1].gate++;
//        node[1].gate++ <--> IdealChannel <--> node[2].gate++;
//        node[2].gate++ <--> IdealChannel <--> node[3].gate++;
//        node[3].gate++ <--> IdealChannel <--> node[4].gate++;
//        node[4].gate++ <--> IdealChannel <--> node[5].gate++;
//        node[0].gate++ <--> IdealChannel <--> node[6].gate++;
//        node[6].gate++ <--> IdealChannel <--> node[7].gate++;
//    
        for i=0..7 {
            controller.gate++ <--> IdealChannel <--> node[i].gate++;
        }
        
        node[0].gate++ <--> IdealChannel <--> node[1].gate++;
        node[1].gate++ <--> IdealChannel <--> node[2].gate++;
        node[2].gate++ <--> IdealChannel <--> node[3].gate++;
        node[3].gate++ <--> IdealChannel <--> node[4].gate++;
        node[4].gate++ <--> IdealChannel <--> node[5].gate++;
        node[5].gate++ <--> IdealChannel <--> node[6].gate++;
        node[6].gate++ <--> IdealChannel <--> node[7].gate++;
        node[7].gate++ <--> IdealChannel <--> node[0].gate++;
        
        
}