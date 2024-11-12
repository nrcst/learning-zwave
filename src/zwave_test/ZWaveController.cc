// ZWaveController.cc
#include <omnetpp.h>
#include <vector>
#include <string>

using namespace omnetpp;

namespace zwavenetwork {

class ZWaveController : public cSimpleModule
{
  protected:
    double frequency;
    double range;
    bool sendCommands;
    int commandNode;
    std::string commandType;
    std::vector<int> connectedNodes;

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void sendCommand(int nodeId, const std::string& cmd);
    void processIncomingMessage(cMessage *msg, int nodeId);
};

Define_Module(ZWaveController);

void ZWaveController::initialize()
{
    frequency = par("frequency").doubleValue();
    range = par("range").doubleValue();
    sendCommands = par("sendCommands").boolValue();
    commandNode = par("commandNode").intValue();
    commandType = par("commandType").stdstringValue();

    // Gather connected nodes
    int numGates = gateSize("gate");
    for (int i = 0; i < numGates; ++i) {
        if (gate("gate$o", i)->isConnected()) {
            connectedNodes.push_back(i);
        }
    }

    EV << "ZWave Controller initialized with frequency " << frequency
       << " MHz and range " << range << " m\n";
    EV << "Connected nodes: " << connectedNodes.size() << "\n";

    if (sendCommands && !connectedNodes.empty()) {
        scheduleAt(simTime() + 2, new cMessage("sendCommand"));
    }
}

void ZWaveController::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        // Handle self-messages like sending periodic commands
        if (strcmp(msg->getName(), "sendCommand") == 0) {
            // Send command to the specified node
            if (commandNode >= 0 && commandNode < connectedNodes.size()) {
                int gateIndex = connectedNodes[commandNode];
                sendCommand(gateIndex, commandType);
                EV << "Controller sent command '" << commandType
                   << "' to Node " << commandNode << " via gate " << gateIndex << "\n";
            } else {
                EV << "Invalid commandNode index\n";
            }

            // Schedule the next command
            scheduleAt(simTime() + 10, msg);
        }
    }
    else {
        // Handle incoming messages from nodes
        int arrivalGate = msg->getArrivalGate()->getIndex();
        EV << "Controller received: " << msg->getName()
           << " from Gate " << arrivalGate << "\n";

        // Process the incoming message
        processIncomingMessage(msg, arrivalGate);

        delete msg;
    }
}

void ZWaveController::sendCommand(int nodeId, const std::string& cmd)
{
    if (nodeId >= 0 && nodeId < gateSize("gate")) {
        cMessage *cmdMsg = new cMessage(cmd.c_str());
        EV << "Controller sending command '" << cmd << "' to Gate " << nodeId << "\n";
        send(cmdMsg, "gate$o", nodeId);
    }
}

void ZWaveController::processIncomingMessage(cMessage *msg, int nodeId)
{
    // Identify the original sender node based on gate index
    int senderNodeId = nodeId; // In this setup, gate index corresponds to node index

    std::string messageName = msg->getName();

    if (messageName == "AlarmTriggered") {
        EV << "Processing Alarm Trigger from Node " << senderNodeId << "\n";
        // Implement additional processing, e.g., notify an app or activate other devices
    }
    else if (messageName == "MotionDetected") {
        EV << "Processing Motion Detection from Node " << senderNodeId << "\n";
        // Example Action: Turn on light node[0]
        sendCommand(0, "ON");
    }
    // Add more message types as needed
}

} // namespace zwavenetwork
