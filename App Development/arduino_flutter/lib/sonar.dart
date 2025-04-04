import 'package:flutter/cupertino.dart';

class Sonar {
  static const double MAX = 1000.0; // large value beyond mini map maximum

  // private variables
  double _cm;
  late double _px;
  String _ID;

  Sonar(this._cm, this._ID) {
    _px = 0; // initialize px to 0 to avoid late initialization error
  }

  // setter function for _cm
  void set_cm(double distance) {
    if (distance == -1) {
      _cm = MAX;
    } else {
      _cm = distance;
    }
  }

  // getter function for _cm
  double get_cm() {
    return _cm;
  }

  void set_px(inputCM) {
    if (_ID == "B" || _ID == "F" || _ID == "C" || _ID == "E") {
      if (inputCM > 100) {
        _px = 200;
      } else {
        _px = 2.0 * inputCM;
      }
    } else if (_ID == "A" || _ID == "D") {
      if (inputCM > 50) {
        _px = 100;
      } else {
        _px = 2.0 * inputCM;
      }
    }
  }

  // setter function for _ID
  void set_ID(String inputID) {
    _ID = inputID;
  }

  double get_px() {
    return _px;
  }

  // getter function for _ID
  String get_ID() {
    return _ID;
  }

  void decodeString(String encodedString) {
    String numberSubstring = encodedString.substring(2);
    double number = double.tryParse(numberSubstring) ?? 0.0;
    set_cm(number);
    set_px(get_cm());
  }

  void displayString() {
    print("sonar $_ID reading $_cm => cm $_px px");
  }
}

// // validation for sonar.dart class
// void main() {
//   Sonar sonarJ = Sonar(100, "J");
//   sonarJ.decodeString("#J23");

//   sonarJ.displayString();
// }
