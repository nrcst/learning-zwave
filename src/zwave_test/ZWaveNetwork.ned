package zwavenetwork.zwave_test;

import ned.IdealChannel;

network ZWaveNetwork
{
    parameters:
        double frequency @unit(MHz) = default(908.42MHz);
        double range @unit(m) = default(30m);
        double playgroundSizeX @unit(m) = default(100m);
        double playgroundSizeY @unit(m) = default(100m);
        double frequencyLow @unit(MHz) = default(908.40MHz);
        double frequencyHigh @unit(MHz) = default(908.42MHz);
        @display("bgb=1140,630,0,,0;bgi=background/smarthome,s");

    submodules:
        controller: ZWaveController {
            @display("p=528,319;i=device/antennatower");
        }
        node0: ZWaveNode {
            parameters:
                nodeId = 0;
                @display("p=402,157;is=s");
        }

        node1: ZWaveNode {
            parameters:
                nodeId = 1;
                @display("is=s;p=84,269");
        }

        node2: ZWaveNode {
            parameters:
                nodeId = 2;
                @display("p=697,254;is=s");
        }

        node3: ZWaveNode {
            parameters:
                nodeId = 3;
                @display("is=s;p=845,157");
        }

        node4: ZWaveNode {
            parameters:
                nodeId = 4;
                @display("is=s;p=1060,158");
        }

        node5: ZWaveNode {
            parameters:
                nodeId = 5;
                @display("is=s;p=1060,444");
        }

        node6: ZWaveNode {
            parameters:
                nodeId = 6;
                @display("is=s;p=298,452");
        }

        node7: ZWaveNode {
            parameters:
                nodeId = 7;
                @display("is=s;p=694,452");
        }

        node8: ZWaveNode {
            parameters:
                nodeId = 8;
                @display("is=s;p=85,444");
        }

        node9: ZWaveNode {
            parameters:
                nodeId = 9;
                @display("is=s;p=662,157");
        }

    connections allowunconnected:
        controller.gate++ <--> IdealChannel <--> node0.gate++;
        controller.gate++ <--> IdealChannel <--> node1.gate++;
        controller.gate++ <--> IdealChannel <--> node2.gate++;
        controller.gate++ <--> IdealChannel <--> node3.gate++;
        controller.gate++ <--> IdealChannel <--> node4.gate++;
        controller.gate++ <--> IdealChannel <--> node5.gate++;
        controller.gate++ <--> IdealChannel <--> node6.gate++;
        controller.gate++ <--> IdealChannel <--> node7.gate++;
        controller.gate++ <--> IdealChannel <--> node8.gate++;
        controller.gate++ <--> IdealChannel <--> node9.gate++;

        node0.gate++ <--> IdealChannel <--> node1.gate++;
        node1.gate++ <--> IdealChannel <--> node2.gate++;
        node2.gate++ <--> IdealChannel <--> node3.gate++;
        node3.gate++ <--> IdealChannel <--> node4.gate++;
        node4.gate++ <--> IdealChannel <--> node5.gate++;
        node5.gate++ <--> IdealChannel <--> node6.gate++;
        node6.gate++ <--> IdealChannel <--> node7.gate++;
        node7.gate++ <--> IdealChannel <--> node8.gate++;
        node8.gate++ <--> IdealChannel <--> node9.gate++;
        node9.gate++ <--> IdealChannel <--> node0.gate++;
}
