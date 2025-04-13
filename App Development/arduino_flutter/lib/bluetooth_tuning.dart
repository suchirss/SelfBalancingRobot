import 'package:flutter/material.dart';

class PIDTuningPage extends StatefulWidget {
  final Function(String) sendCommand; // Function to send commands

  const PIDTuningPage({Key? key, required this.sendCommand}) : super(key: key);

  @override
  _PIDTuningPageState createState() => _PIDTuningPageState();
}

class _PIDTuningPageState extends State<PIDTuningPage> {
  double _kp = 12.0;
  double _ki = 400.0;
  double _kd = 0.01;

  final TextEditingController _kpController = TextEditingController();
  final TextEditingController _kiController = TextEditingController();
  final TextEditingController _kdController = TextEditingController();

  @override
  void initState() {
    super.initState();
    _updateControllers();
  }

  @override
  void dispose() {
    _kpController.dispose();
    _kiController.dispose();
    _kdController.dispose();
    super.dispose();
  }

  void _updateControllers() {
    _kpController.text = _kp.toStringAsFixed(3);
    _kiController.text = _ki.toStringAsFixed(3);
    _kdController.text = _kd.toStringAsFixed(3);
  }

  void _updateValue(String param, double change) {
    setState(() {
      if (param == "KP") {
        _kp = (_kp + change).clamp(0, 50);
        _kpController.text = _kp.toStringAsFixed(3);
      } else if (param == "KI") {
        _ki = (_ki + change).clamp(0, 1000);
        _kiController.text = _ki.toStringAsFixed(3);
      } else if (param == "KD") {
        _kd = (_kd + change).clamp(0.001, 1.0);
        _kdController.text = _kd.toStringAsFixed(3);
      }
    });
  }

  void _onTextSubmitted(String param, String newValue) {
    double? parsed = double.tryParse(newValue);
    if (parsed != null) {
      setState(() {
        if (param == "KP") {
          _kp = parsed.clamp(0, 50);
          _kpController.text = _kp.toStringAsFixed(3);
        } else if (param == "KI") {
          _ki = parsed.clamp(0, 1000);
          _kiController.text = _ki.toStringAsFixed(3);
        } else if (param == "KD") {
          _kd = parsed.clamp(0.001, 1.0);
          _kdController.text = _kd.toStringAsFixed(3);
        }
      });
    }
  }

  void _sendValue(String param) {
    String command;
    if (param == "KP") {
      command = "KP:$_kp";
    } else if (param == "KI") {
      command = "KI:$_ki";
    } else {
      command = "KD:$_kd";
    }
    widget.sendCommand(command); // Send only the selected parameter
  }

  Widget _buildControl(
      String label, TextEditingController controller, double step) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.spaceBetween,
      children: [
        Text("$label:",
            style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold)),
        IconButton(
          icon: Icon(Icons.arrow_downward),
          onPressed: () => _updateValue(label, -step),
        ),
        SizedBox(
          width: 80,
          child: TextField(
            textAlign: TextAlign.center,
            controller: controller,
            keyboardType: TextInputType.number,
            decoration: InputDecoration(border: OutlineInputBorder()),
            onSubmitted: (newValue) => _onTextSubmitted(label, newValue),
          ),
        ),
        IconButton(
          icon: Icon(Icons.arrow_upward),
          onPressed: () => _updateValue(label, step),
        ),
        ElevatedButton(
          onPressed: () => _sendValue(label), // Send only this value
          child: Text("SEND"),
        ),
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text("PID Tuning")),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          children: [
            _buildControl("KP", _kpController, 0.1),
            SizedBox(height: 10),
            _buildControl("KI", _kiController, 10),
            SizedBox(height: 10),
            _buildControl("KD", _kdController, 0.01),
          ],
        ),
      ),
    );
  }
}
