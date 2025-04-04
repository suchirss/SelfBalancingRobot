import 'package:flutter/material.dart';
import 'package:speech_to_text/speech_to_text.dart' as stt;

class VoiceCommsPage extends StatefulWidget {
  final Function(String)? sendCommand;

  const VoiceCommsPage({Key? key, this.sendCommand}) : super(key: key);

  @override
  _VoiceCommsPageState createState() => _VoiceCommsPageState();
}

class _VoiceCommsPageState extends State<VoiceCommsPage> {
  String _statusText = "Awaiting command";
  late stt.SpeechToText _speech;
  bool _isListening = false;
  String _lastRecognized = ""; // Store raw recognized text

  @override
  void initState() {
    super.initState();
    _speech = stt.SpeechToText();
  }

  void _startListening() async {
    bool available = await _speech.initialize();
    if (available) {
      setState(() => _isListening = true);
      _speech.listen(
        onResult: (result) {
          setState(() {
            _lastRecognized = result.recognizedWords.trim().toLowerCase();
            _statusText = _lastRecognized;
          });
        },
      );
    } else {
      setState(() {
        _statusText = "Speech recognition not available";
      });
    }
  }

  void _stopListening() {
    _speech.stop();
    setState(() => _isListening = false);
  }

  void _sendCommand() {
    const validCommands = {
      "forward": "FORWARD",
      "backward": "BACKWARD",
      "left": "LEFT",
      "right": "RIGHT",
      "stop": "STOP",
    };

    if (validCommands.containsKey(_lastRecognized)) {
      final commandToSend = validCommands[_lastRecognized]!;
      widget.sendCommand?.call(commandToSend);
      setState(() {
        _statusText = "Command sent!";
      });
    } else {
      setState(() {
        _statusText = "This word cannot be sent.";
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Voice Commands"),
        backgroundColor: Theme.of(context).colorScheme.primary,
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          crossAxisAlignment: CrossAxisAlignment.center,
          children: [
            // Hold to Record Button
            GestureDetector(
              onLongPress: _startListening,
              onLongPressUp: _stopListening,
              child: AnimatedContainer(
                duration: Duration(milliseconds: 200),
                height: MediaQuery.of(context).size.height * 0.15,
                width: MediaQuery.of(context).size.height * 0.15,
                decoration: BoxDecoration(
                  color: _isListening ? Colors.redAccent : Colors.blueAccent,
                  shape: BoxShape.circle,
                  boxShadow: [
                    BoxShadow(
                      color: Colors.black26,
                      blurRadius: 10,
                      offset: Offset(0, 4),
                    ),
                  ],
                ),
                alignment: Alignment.center,
                child: Icon(
                  Icons.mic,
                  color: Colors.white,
                  size: 40,
                ),
              ),
            ),
            SizedBox(height: 20),

            // Transcribed Text or Status
            Text(
              _statusText,
              textAlign: TextAlign.center,
              style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold),
            ),
            SizedBox(height: 20),

            // Send Button
            ElevatedButton(
              onPressed: _sendCommand,
              child: Text("Send"),
            ),
          ],
        ),
      ),
    );
  }
}
