package zwavenetwork.zwave_test;


simple ZWaveNode
{
    parameters:
        double frequency @unit(MHz);                       // Operating frequency
        double range @unit(m);                             // Communication range
        string deviceType = default("generic");            // Type of device

        double alarmTestInterval @unit(s) = default(0s);          // Interval for alarm testing
        double motionDetectionInterval @unit(s) = default(0s);     // Interval for motion detection

        // Display configuration based on deviceType
        string displayImage = default("");                  // Not used, removed custom images

        @display("i=block/circle,,green;is=vs");              // Default display settings, color set in NED or C++

    gates:
        inout gate[];                                       // Communication gates
}