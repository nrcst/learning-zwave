#include <omnetpp.h>
#include <string>

using namespace omnetpp;

namespace zwavenetwork {

class ZWaveNode : public cSimpleModule
{
  protected:
    double frequency;
    double range;
    std::string deviceType;
    cMessage *selfMsg;

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void sendToController(const std::string& eventName);
    void forwardMessage(cMessage *msg, int incomingGateIndex);
};

Define_Module(ZWaveNode);

void ZWaveNode::initialize()
{
    frequency = par("frequency").doubleValue();
    range = par("range").doubleValue();
    deviceType = par("deviceType").stdstringValue();

    // Schedule motion detection for sensor nodes
    if (deviceType == "sensor") {
        selfMsg = new cMessage("motionDetected");
        scheduleAt(simTime() + par("motionDetectionInterval").doubleValue(), selfMsg);
    }

    EV << "ZWave Node " << getIndex() << " initialized as " << deviceType
       << " device with frequency " << frequency << " MHz\n";
}

void ZWaveNode::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        // Handle self-generated events
        if (deviceType == "sensor" && strcmp(msg->getName(), "motionDetected") == 0) {
            EV << "Sensor Node " << getIndex() << " detected motion!\n";
            sendToController("MotionDetected");

            // Reschedule the next motion detection
            scheduleAt(simTime() + par("motionDetectionInterval").doubleValue(), msg);
        }
    }
    else {
        int incomingGateIndex = msg->getArrivalGate()->getIndex();

        std::string msgName = msg->getName();
        EV << "Node " << getIndex() << " (" << deviceType << ") received message: "
           << msgName << " from gate " << incomingGateIndex << "\n";

        if (deviceType == "light") {
            if (msgName == "ON") {
                EV << "Light Node " << getIndex() << " turned ON\n";
                getDisplayString().setTagArg("i", 1, "yellow"); // Change color to indicate ON
            }
            else if (msgName == "OFF") {
                EV << "Light Node " << getIndex() << " turned OFF\n";
                getDisplayString().setTagArg("i", 1, "grey"); // Change color to indicate OFF
            }
        }
        else {
            // For non-light devices, process commands or forward as needed
            // Example: Forward the message to all other gates except the incoming one
            forwardMessage(msg, incomingGateIndex);
            return; // Message has been forwarded; no need to delete
        }

        delete msg; // Delete the message after processing
    }
}

void ZWaveNode::sendToController(const std::string& eventName)
{
    // Create and send a message to the controller via gate[0]
    cMessage *eventMsg = new cMessage(eventName.c_str());
    EV << "Node " << getIndex() << " sending '" << eventName << "' to controller.\n";
    send(eventMsg, "gate$o", 0); // Assuming controller is connected to gate index 0
}

void ZWaveNode::forwardMessage(cMessage *msg, int incomingGateIndex)
{
    // Forward the message to all gates except the one it arrived on
    int numGates = gateSize("gate");
    for (int i = 0; i < numGates; ++i) {
        if (i != incomingGateIndex) {
            cMessage *forwardedMsg = msg->dup(); // Duplicate the message for each forward
            EV << "Node " << getIndex() << " forwarding message '" << forwardedMsg->getName()
               << "' to gate " << i << "\n";
            send(forwardedMsg, "gate$o", i);
        }
    }

    delete msg; // Delete the original message after forwarding
}

}
