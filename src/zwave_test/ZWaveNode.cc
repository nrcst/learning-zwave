#include <omnetpp.h>
#include <cmath>
#include <string>
#include <vector>

using namespace omnetpp;

namespace zwavenetwork {

class ZWaveNode : public cSimpleModule
{
  protected:
    double frequency;
    double range;
    double frequencyLow;
    double frequencyHigh;
    std::string deviceType;
    int nodeId; // Added nodeId
    cMessage *selfMsg;

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void sendToController(const std::string& eventName);
    void sendStatusUpdate(const std::string& status);
};

Define_Module(ZWaveNode);

void ZWaveNode::initialize()
{
    frequency = par("frequency").doubleValue();
    range = par("range").doubleValue();
    frequencyLow = par("frequencyLow").doubleValue();
    frequencyHigh = par("frequencyHigh").doubleValue();
    deviceType = par("deviceType").stdstringValue();
    nodeId = par("nodeId").intValue(); // Initialize nodeId

    EV << "ZWave Node " << nodeId << " (" << deviceType << ") initialized with frequencies "
       << frequencyLow << " MHz and " << frequencyHigh << " MHz\n";
    getDisplayString().setTagArg("t", 0, deviceType.c_str());

    // Schedule initial event if the device is a sensor
    if (deviceType == "sensor") {
        selfMsg = new cMessage("motionDetected");
        double nextEventTime = simTime().dbl() + par("motionDetectionInterval").doubleValue();
        if (nextEventTime < 200) {
            scheduleAt(nextEventTime, selfMsg);
        }
    }
}

void ZWaveNode::handleMessage(cMessage *msg)
{
    std::string msgName = msg->getName();

    if (msgName == "ON" || msgName == "OFF") {
        EV << "Node " << nodeId << " (" << deviceType << ") received command: " << msgName << "\n";
        sendStatusUpdate(msgName == "ON" ? "LampOn" : "LampOff");
    }
    else if (msgName == "RaiseTemp" || msgName == "ReduceTemp") {
        EV << "Node " << nodeId << " (Thermostat) adjusting temperature\n";
        sendStatusUpdate(msgName);
    }
    else if (msgName == "LockDoor" || msgName == "UnlockDoor") {
        EV << "Node " << nodeId << " (Door Lock) received command: " << msgName << "\n";
        sendStatusUpdate(msgName == "UnlockDoor" ? "DoorUnlocked" : "DoorLocked");
    }
    else if (msg->isSelfMessage() && deviceType == "sensor") {
        // Reschedule motion detection if within the 200s limit
        EV << "Sensor Node " << nodeId << " detected motion!\n";
        sendToController("MotionDetected");

        double nextEventTime = simTime().dbl() + par("motionDetectionInterval").doubleValue();
        if (nextEventTime < 200) {
            scheduleAt(nextEventTime, msg);
        }
        else {
            // Drop from the event queue before deleting
            msg->setContextPointer(nullptr);
            cancelAndDelete(msg); // Safely cancel and delete
            selfMsg = nullptr;
        }
    }
    else {
        delete msg;
    }
}

void ZWaveNode::sendToController(const std::string& eventName)
{
    cMessage *eventMsg = new cMessage(eventName.c_str());
    send(eventMsg, "gate$o", 0); // Assuming gate index 0 connects to controller
    EV << "Node " << nodeId << " sending '" << eventName << "' to controller.\n";
}

void ZWaveNode::sendStatusUpdate(const std::string& status)
{
    sendToController(status);
}

} // namespace zwavenetwork
