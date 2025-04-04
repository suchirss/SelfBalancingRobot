import 'dart:convert';
import 'dart:async';
import 'package:arduino_flutter/bluetooth_tuning.dart';
import 'package:arduino_flutter/voice_comms.dart';
import 'package:flutter/material.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';
import 'sonar.dart'; // import sonar class in same folder
import 'dart:math';

// define UUIDs as constants - these should match the Arduino code
const String serviceUUID = "00000000-5EC4-4083-81CD-A10B8D5CF6EC";
const String characteristicUUID = "00000001-5EC4-4083-81CD-A10B8D5CF6EC";

// define object sizes
const double miniMapWidth = 300;
const double miniMapHeight = 450;
const double robotRectWidth = 100;
const double robotRectHeight = 50;
const double rect50 = 50;
const double rect10 = 10;

// initialize sonar instances of type Sonar class
Sonar sonarA = Sonar(-1, "A");
Sonar sonarB = Sonar(-1, "B");
Sonar sonarC = Sonar(-1, "C");
Sonar sonarD = Sonar(-1, "D");
Sonar sonarE = Sonar(-1, "E");
Sonar sonarF = Sonar(-1, "F");

// map secondChar String to sonar instance
Map<String, Sonar> sonarMap = {
  "A": sonarA,
  "B": sonarB,
  "C": sonarC,
  "D": sonarD,
  "E": sonarE,
  "F": sonarF,
};

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key, required this.title});

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  final _ble = FlutterReactiveBle();

  StreamSubscription<DiscoveredDevice>?
      _scanSub; // subscribe to bluetooth scanning stream
  StreamSubscription<ConnectionStateUpdate>?
      _connectSub; // subscribe to bluetooth connection stream
  StreamSubscription<List<int>>? _notifySub;

  List<DiscoveredDevice> _devices = [];
  String? _selectedDeviceId; // will hold the device ID selected for connection
  String?
      _selectedDeviceName; // will hold the device name selected for connection
  var _stateMessage = 'Scanning...'; // displays app status
  QualifiedCharacteristic? _writeCharacteristic;

  bool _isConnected = false; // flag to indicate connection

  // on initialization scan for devices
  @override
  void initState() {
    super.initState();
    _scanSub = _ble.scanForDevices(withServices: []).listen(_onScanUpdate);
  }

  // when terminating cancel all the subscriptions
  @override
  void dispose() {
    _notifySub?.cancel();
    _connectSub?.cancel();
    _scanSub?.cancel();
    super.dispose();
  }

  // update devices that found with "BLE" in their name
  void _onScanUpdate(DiscoveredDevice d) {
    if (d.name.contains("BLE") &&
        !_devices.any((device) => device.id == d.id)) {
      setState(() {
        _devices.add(d);
      });
    }
  }

  // Connect to the devices that was selected by user
  void _connectToDevice() {
    if (_selectedDeviceId != null) {
      setState(() {
        _stateMessage = 'Connecting to $_selectedDeviceName...';
      });

      _connectSub = _ble.connectToDevice(id: _selectedDeviceId!).listen(
        (update) {
          if (update.connectionState == DeviceConnectionState.connected) {
            setState(() {
              _stateMessage = 'Connected to $_selectedDeviceName!';
              _isConnected = true;
            });
            _onConnected(_selectedDeviceId!);
          }
        },
        onError: (error) {
          setState(() {
            _stateMessage = 'Connection error: $error';
          });
        },
      );
    }
  }

  // Handle disconnection
  void _disconnectFromDevice() {
    try {
      if (_notifySub != null) {
        _notifySub?.cancel();
        _notifySub = null;
      }

      if (_connectSub != null) {
        _connectSub?.cancel();
        _connectSub = null;
      }

      setState(() {
        _isConnected = false;
        _stateMessage = 'Disconnected from $_selectedDeviceName.';
        _writeCharacteristic = null;
      });
    } catch (e) {
      setState(() {
        _stateMessage = 'Error during disconnection: $e';
      });
    }
  }

  void _onConnected(String deviceId) {
    final characteristic = QualifiedCharacteristic(
      deviceId: deviceId,
      serviceId: Uuid.parse(serviceUUID), // Use the constant here
      characteristicId: Uuid.parse(characteristicUUID), // Use the constant here
    );

    _writeCharacteristic = characteristic;

    _notifySub = _ble.subscribeToCharacteristic(characteristic).listen((bytes) {
      String stringReceived = Utf8Decoder().convert(bytes);

      if (stringReceived.isNotEmpty) {
        print(stringReceived);
        String firstChar = stringReceived.substring(0, 1);
        // print(firstChar);
        if (firstChar == "#") {
          // print("$stringReceived type #");
          String secondChar = stringReceived.substring(1, 2);
          sonarMap[secondChar]?.decodeString(stringReceived);
          setState(() {});
          print("checking sonar decoding: ");
          sonarMap[secondChar]?.displayString();
        } else if (firstChar == "@") {
          // print("$stringReceived type @");
          setState(() {
            _stateMessage = 'Data received: ${Utf8Decoder().convert(bytes)}';
          });
        }
      } else {
        print("empty string!");
      }

      // if (firstChar == "#") {
      //   print("This is a ultrasonic sensor string");
      //   // decode the string here
      // }
    });
  }

  Future<void> _sendCommand(String command) async {
    if (_writeCharacteristic != null) {
      try {
        await _ble.writeCharacteristicWithResponse(
          _writeCharacteristic!,
          value: utf8.encode(command),
        );
        setState(() {
          _stateMessage = "Command '$command' sent!";
        });
      } catch (e) {
        setState(() {
          _stateMessage = "Error sending command: $e";
        });
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: Text(widget.title),
      ),
      body: Column(
        children: [
          Container(
            padding: const EdgeInsets.all(16.0),
            color: Colors.grey[300],
            width: double.infinity,
            child: Text(
              _stateMessage,
              style: Theme.of(context).textTheme.titleMedium,
              textAlign: TextAlign.center,
            ),
          ),
          if (_devices.isNotEmpty)
            Padding(
              padding: const EdgeInsets.all(0.0),
              child: DropdownButton<String>(
                isExpanded: true,
                hint: const Text("Select a BLE Device"),
                value: _selectedDeviceId,
                items: _devices.map((device) {
                  return DropdownMenuItem(
                    value: device.id,
                    child: Text(device.name),
                  );
                }).toList(),
                onChanged: (value) {
                  setState(() {
                    _selectedDeviceId = value;
                    _selectedDeviceName = _devices
                        .firstWhere((device) => device.id == value)
                        .name;
                  });
                },
              ),
            ),
          if (!_isConnected)
            ElevatedButton(
              onPressed: _selectedDeviceId != null ? _connectToDevice : null,
              child: const Text('Connect'),
            ),
          if (_isConnected)
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                ElevatedButton(
                  onPressed: _disconnectFromDevice,
                  child: const Text('Disconnect'),
                ),
                const SizedBox(width: 10), // Space between buttons
                ElevatedButton(
                  onPressed: _isConnected
                      ? () {
                          Navigator.push(
                            context,
                            MaterialPageRoute(
                              builder: (context) => PIDTuningPage(
                                sendCommand: (command) {
                                  _sendCommand(command);
                                },
                              ),
                            ),
                          );
                        }
                      : null, // Disable button if not connected
                  child: const Text('PID Tuning'),
                ),
                ElevatedButton(
                  onPressed: // _isConnected ?
                      () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(
                        builder: (context) => VoiceCommsPage(
                          sendCommand: (command) {
                            _sendCommand(command);
                          },
                        ),
                      ),
                    );
                  },
                  // : null, // Disable button if not connected
                  child: const Text('Voice Comms'),
                ),
              ],
            ),
          // **************** command buttons ****************
          Expanded(
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                Padding(
                  padding: const EdgeInsets.all(0.0),
                  child: Stack(
                    alignment: Alignment
                        .center, // This ensures that the children are centered
                    children: [
                      Container(
                          height: miniMapHeight,
                          width: miniMapWidth,
                          alignment: Alignment.center,
                          decoration: BoxDecoration(
                              border:
                                  Border.all(color: Colors.black, width: 4))),
                      Container(
                        width: robotRectWidth,
                        height: robotRectHeight,
                        decoration: BoxDecoration(
                            border: Border.all(color: Colors.black, width: 1)),
                        child: Image.asset(
                          'assets/images/self-balancing-robot.png',
                        ),
                      ),
                      // The rectangle positioned relative to the image
                      Positioned(
                        // -------------- LEFT RECTANGLE --------------
                        top: (miniMapHeight - rect50) /
                            2, // Position the rectangle from the top - THIS WILL STAY CONSTANT FOR LEFT RECTANGLE
                        // top: 150,
                        left: (miniMapWidth -
                                rect10 -
                                robotRectWidth -
                                2 * sonarA.get_px()) /
                            2, // Position the rectangle from the left - WRITE FUNCTION TO VARY
                        child: Container(
                          width: rect10,
                          height: rect50,
                          decoration: BoxDecoration(
                            border: Border.all(color: Colors.black, width: 2),
                            color: Colors.deepPurple, // No fill color
                          ),
                        ),
                      ),
                      Positioned(
                        // -------------- RIGHT RECTANGLE --------------
                        top: (miniMapHeight - robotRectHeight) /
                            2, // Position the rectangle from the top - THIS WILL STAY CONSTANT FOR RIGHT RECTANGLE
                        // top: 150,
                        left: (miniMapWidth -
                                rect10 +
                                robotRectWidth +
                                2 * sonarD.get_px()) /
                            2, // Position the rectangle from the left - WRITE FUNCTION TO VARY
                        child: Container(
                          width: rect10,
                          height: rect50,
                          decoration: BoxDecoration(
                            border: Border.all(color: Colors.black, width: 2),
                            color: Colors.deepPurple, // No fill color
                          ),
                        ),
                      ),
                      Positioned(
                        // -------------- TOP RECTANGLE --------------
                        top: (miniMapHeight -
                                robotRectHeight -
                                rect10 -
                                2 * min(sonarB.get_px(), sonarF.get_px())) /
                            2, // Position the rectangle from the top - VARY
                        left: (miniMapWidth - robotRectWidth - rect10) /
                            2, // Position the rectangle from the left - CONSTANT
                        child: Container(
                          width: robotRectWidth + rect10,
                          height: 10,
                          decoration: BoxDecoration(
                            border: Border.all(color: Colors.black, width: 2),
                            color: Colors.deepPurple, // No fill color
                          ),
                        ),
                      ),
                      Positioned(
                        // -------------- BOTTOM RECTANGLE --------------
                        top: (miniMapHeight +
                                rect50 +
                                min(sonarC.get_px(), sonarE.get_px())) /
                            2, // Position the rectangle from the top - VARY
                        left: (miniMapWidth - robotRectWidth - rect10) /
                            2, // Position the rectangle from the left - CONSTANT
                        child: Container(
                          width: 110,
                          height: 10,
                          decoration: BoxDecoration(
                            border: Border.all(color: Colors.black, width: 2),
                            color: Colors.deepPurple, // No fill color
                          ),
                        ),
                      ),
                    ],
                  ),
                ),
                // Joystick Buttons
                Spacer(), // pushes everything below
                Row(
                  mainAxisAlignment: MainAxisAlignment.center,
                  crossAxisAlignment: CrossAxisAlignment.center,
                  children: [
                    // D-Pad (existing)
                    Column(
                      mainAxisSize: MainAxisSize.min,
                      children: [
                        Row(
                          mainAxisAlignment: MainAxisAlignment.center,
                          children: [
                            ElevatedButton(
                              onPressed: _isConnected
                                  ? () => _sendCommand('FORWARD')
                                  : null,
                              child: const Icon(Icons.arrow_upward),
                            ),
                          ],
                        ),
                        const SizedBox(height: 10),
                        Row(
                          mainAxisAlignment: MainAxisAlignment.center,
                          children: [
                            ElevatedButton(
                              onPressed: _isConnected
                                  ? () => _sendCommand('LEFT')
                                  : null,
                              child: const Icon(Icons.arrow_back),
                            ),
                            const SizedBox(width: 10),
                            ElevatedButton(
                              onPressed: _isConnected
                                  ? () => _sendCommand('BACKWARD')
                                  : null,
                              child: const Icon(Icons.arrow_downward),
                            ),
                            const SizedBox(width: 10),
                            ElevatedButton(
                              onPressed: _isConnected
                                  ? () => _sendCommand('RIGHT')
                                  : null,
                              child: const Icon(Icons.arrow_forward),
                            ),
                          ],
                        ),
                      ],
                    ),

                    // Spacer between D-pad and Circle
                    const SizedBox(width: 20),

                    // Circle (customizable)
                    ElevatedButton(
                      onPressed:
                          _isConnected ? () => _sendCommand("STOP") : null,
                      style: ElevatedButton.styleFrom(
                        shape: const CircleBorder(),
                        padding: const EdgeInsets.all(20),
                        backgroundColor: Colors.red, // Button background color
                        foregroundColor:
                            Colors.white, // Text (foreground) color
                        side: const BorderSide(color: Colors.black, width: 2),
                      ),
                      child: const Text(
                        'STOP',
                        style: TextStyle(
                          fontSize: 24,
                          fontWeight: FontWeight.bold,
                        ),
                      ),
                    ),
                  ],
                ),

                Row(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    ElevatedButton(
                      onPressed:
                          _isConnected ? () => _sendCommand('STANDARD') : null,
                      child: const Text('Standard'),
                    ),
                    const SizedBox(width: 10),
                    ElevatedButton(
                      onPressed:
                          _isConnected ? () => _sendCommand('DELAYED') : null,
                      child: const Text('Delayed'),
                    ),
                    const SizedBox(width: 10),
                    ElevatedButton(
                      onPressed:
                          _isConnected ? () => _sendCommand('INVERTED') : null,
                      child: const Text('Inverted'),
                    ),
                  ],
                ),
              ],
            ),
          ),
          // **************** end of command buttons ****************
        ],
      ),
    );
  }
}
