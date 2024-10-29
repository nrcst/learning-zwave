#include <omnetpp.h>
#include <cmath>

using namespace omnetpp;

namespace zwavenetwork {

class ZWaveController : public cSimpleModule
{
protected:
    double frequency;
    double range;
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(ZWaveController);

void ZWaveController::initialize()
{
    frequency = par("frequency").doubleValue();
    range = par("range").doubleValue();
    EV << "ZWave Controller initialized with frequency " << frequency << " MHz and range " << range << " m\n";

    // Schedule the first message
    cMessage *startMsg = new cMessage("start");
    scheduleAt(simTime() + uniform(0, 1), startMsg);
}

void ZWaveController::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        // Send a message to a random node
        int numNodes = getParentModule()->par("numNodes");
        int randomNode = intuniform(0, numNodes - 1);
        cMessage *dataMsg = new cMessage("data");
        send(dataMsg, "gate$o", randomNode);
        EV << "Controller sent message to Node " << randomNode << "\n";

        // Schedule the next message
        cMessage *nextMsg = new cMessage("start");
        scheduleAt(simTime() + exponential(1.0), nextMsg);
    } else {
        // Message received from a node
        EV << "Controller received: " << msg->getName() << " from gate " << msg->getArrivalGate()->getIndex() << "\n";
        delete msg;
    }
}

}
