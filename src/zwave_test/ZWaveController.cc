#include <omnetpp.h>
#include <cmath>
#include <vector>
#include <string>

using namespace omnetpp;

namespace zwavenetwork {

class ZWaveController : public cSimpleModule
{
  protected:
    double frequencyLow;
    double frequencyHigh;
    bool sendCommands;
    std::string commandType;
    std::vector<int> connectedNodes; // Store gate indices

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    bool isValidGateIndex(int index) const;
    void sendCommand(int gateIndex, const std::string& cmd);
    void processIncomingMessage(cMessage *msg, int nodeId);
};

Define_Module(ZWaveController);

void ZWaveController::initialize()
{
    frequencyLow = par("frequencyLow").doubleValue();
    frequencyHigh = par("frequencyHigh").doubleValue();
    sendCommands = par("sendCommands").boolValue();
    commandType = par("commandType").stdstringValue();

    int numGates = gateSize("gate");

    // Identify connected nodes by gate indices
    for (int i = 0; i < numGates; ++i) {
        if (gate("gate$o", i)->isConnected()) {
            connectedNodes.push_back(i);
        }
    }

    EV << "ZWave Controller initialized with FSK frequencies "
       << frequencyLow << " MHz and " << frequencyHigh << " MHz\n";
    EV << "Number of connected nodes: " << connectedNodes.size() << "\n";

    if (sendCommands && !connectedNodes.empty()) {
        scheduleAt(simTime() + 2, new cMessage("sendBatchCommand"));
    }
}

bool ZWaveController::isValidGateIndex(int index) const
{
    return (index >= 0 && index < gateSize("gate") && gate("gate$o", index)->isConnected());
}

void ZWaveController::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (strcmp(msg->getName(), "sendBatchCommand") == 0) {
            // Iterate over connected nodes and send commands based on nodeId
            for (int gateIndex : connectedNodes) {
                // Retrieve nodeId from the connected node
                cModule *nodeModule = gate("gate$i", gateIndex)->getOwnerModule();
                int nodeId = nodeModule->par("nodeId").intValue();

                if (nodeId % 3 == 0) {
                    sendCommand(gateIndex, "ON");           // Example: Turning on a lamp
                }
                else if (nodeId % 3 == 1) {
                    sendCommand(gateIndex, "RaiseTemp");    // Example: Adjusting a thermostat
                }
                else if (nodeId % 3 == 2) {
                    sendCommand(gateIndex, "UnlockDoor");   // Example: Unlocking a door lock
                }
            }
            scheduleAt(simTime() + 10, msg); // Reschedule to send commands periodically
        }
        else if (strcmp(msg->getName(), "start") == 0) {
            // Existing logic for 'start' messages (if any)
        }
    }
    else {
        // Handle incoming messages from nodes
        int arrivalGate = msg->getArrivalGate()->getIndex();
        // Retrieve nodeId from the sender node
        cModule *senderModule = msg->getArrivalGate()->getOwnerModule();
        int nodeId = senderModule->par("nodeId").intValue();
        processIncomingMessage(msg, nodeId);
        delete msg;
    }
}

void ZWaveController::processIncomingMessage(cMessage *msg, int nodeId)
{
    std::string messageName = msg->getName();

    if (messageName == "AlarmTriggered") {
        EV << "Processing Alarm Trigger from Node " << nodeId << "\n";
    }
    else if (messageName == "MotionDetected") {
        EV << "Processing Motion Detection from Node " << nodeId << "\n";
        sendCommand(0, "ON"); // Example action: sending command to gate index 0
    }
    else if (messageName == "LampOn" || messageName == "LampOff" ||
             messageName == "DoorLocked" || messageName == "DoorUnlocked") {
        EV << "Controller received acknowledgment: " << messageName
           << " from Node " << nodeId << "\n";
    }
    else {
        EV << "Controller received unexpected message: " << messageName << "\n";
    }

    // Only process frequency and bit for FSK data messages
    if (msg->hasPar("frequency")) {
        double receivedFrequency = msg->par("frequency").doubleValue();
        int receivedBit = (receivedFrequency == frequencyLow) ? 0 : 1;
        EV << "Received Frequency: " << receivedFrequency << " MHz, Bit: " << receivedBit << "\n";
    }
}

void ZWaveController::sendCommand(int gateIndex, const std::string& cmd)
{
    if (isValidGateIndex(gateIndex)) {
        cMessage *cmdMsg = new cMessage(cmd.c_str());
        try {
            send(cmdMsg, "gate$o", gateIndex);
            EV << "Controller sending command '" << cmd << "' to Gate " << gateIndex << "\n";
        }
        catch (const cRuntimeError& e) {
            EV << "Error sending command: " << e.what() << "\n";
            delete cmdMsg;
        }
    }
}

} // namespace zwavenetwork
