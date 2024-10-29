#include <omnetpp.h>
#include <cmath>

using namespace omnetpp;

namespace zwavenetwork {

class ZWaveNode : public cSimpleModule
{
protected:
    double frequency;
    double range;
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(ZWaveNode);

void ZWaveNode::initialize()
{
    frequency = par("frequency").doubleValue();
    range = par("range").doubleValue();
    EV << "ZWave Node " << getIndex() << " initialized with frequency " << frequency << " MHz and range " << range << " m\n";
}

void ZWaveNode::handleMessage(cMessage *msg)
{
    EV << "Node " << getIndex() << " received: " << msg->getName() << "\n";

    // Forward the message to a random neighbor (including the controller)
    int numGates = gateSize("gate");
    int randomGate = intuniform(0, numGates - 1);

    cMessage *fwdMsg = msg->dup();
    send(fwdMsg, "gate$o", randomGate);
    EV << "Node " << getIndex() << " forwarded message to gate " << randomGate << "\n";

    delete msg;
}

}
