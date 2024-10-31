#include <omnetpp.h>
#include <cmath>

using namespace omnetpp;

namespace zwavenetwork {

class ZWaveController : public cSimpleModule
{
protected:
    double frequency;
    double range;
    int numGates;  // Store the number of available gates

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    bool isValidGateIndex(int index) const;  // Helper function to validate gate indices
};

Define_Module(ZWaveController);

void ZWaveController::initialize()
{
    frequency = par("frequency").doubleValue();
    range = par("range").doubleValue();

    // Get the number of gates actually connected
    numGates = gateSize("gate");

    EV << "ZWave Controller initialized with frequency " << frequency
       << " MHz and range " << range << " m\n";
    EV << "Number of connected gates: " << numGates << "\n";

    // Validate that we have connections
    if (numGates == 0) {
        EV << "Warning: Controller has no connected gates!\n";
        return;
    }

    // Schedule the first message
    cMessage *startMsg = new cMessage("start");
    scheduleAt(simTime() + uniform(0, 1), startMsg);
}

bool ZWaveController::isValidGateIndex(int index) const
{
    return (index >= 0 && index < numGates && gate("gate$o", index)->isConnected());
}

void ZWaveController::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        // Safety check - ensure we have gates to send to
        if (numGates == 0) {
            EV << "No gates available to send messages\n";
            delete msg;
            return;
        }

        // Generate a random node index within the valid range
        int attempts = 0;
        int maxAttempts = 10;  // Prevent infinite loops
        int randomNode;

        do {
            randomNode = intuniform(0, numGates - 1);
            attempts++;
        } while (!isValidGateIndex(randomNode) && attempts < maxAttempts);

        if (attempts >= maxAttempts) {
            EV << "Could not find a valid gate to send message after " << maxAttempts << " attempts\n";
            delete msg;
            return;
        }

        // Create and send the message
        cMessage *dataMsg = new cMessage("data");

        try {
            // Attempt to send the message
            send(dataMsg, "gate$o", randomNode);
            EV << "Controller sent message to Node " << randomNode << "\n";

            // Schedule the next message
            cMessage *nextMsg = new cMessage("start");
            scheduleAt(simTime() + exponential(1.0), nextMsg);
        }
        catch (const cRuntimeError& e) {
            EV << "Error sending message: " << e.what() << "\n";
            delete dataMsg;  // Clean up if send fails
        }
    }
    else {
        // Message received from a node
        int arrivalGate = msg->getArrivalGate()->getIndex();
        EV << "Controller received: " << msg->getName()
           << " from gate " << arrivalGate << "\n";
        delete msg;
    }
}

}  // namespace zwavenetwork
