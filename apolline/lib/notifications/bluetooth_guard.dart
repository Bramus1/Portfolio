import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';

class BluetoothGuard extends StatelessWidget {
  final Widget child;
  const BluetoothGuard({super.key, required this.child});

  @override
  Widget build(BuildContext context) {
    return StreamBuilder<BluetoothAdapterState>(
      stream: FlutterBluePlus.adapterState,
      builder: (context, snapshot) {
        final adapterState = snapshot.data;
        // If Bluetooth is not on, show a full-screen error overlay.
        if (adapterState != BluetoothAdapterState.on) {
          return Scaffold(
            body: Container(
              width: double.infinity,
              height: double.infinity,
              color: Colors.black87,
              alignment: Alignment.center,
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: const [
                  Icon(
                    Icons.bluetooth_disabled,
                    size: 80,
                    color: Colors.redAccent,
                  ),
                  SizedBox(height: 20),
                  Padding(
                    padding: EdgeInsets.symmetric(horizontal: 20),
                    child: Text(
                      "Bluetooth is turned off.\nPlease enable Bluetooth to continue.",
                      textAlign: TextAlign.center,
                      style: TextStyle(fontSize: 22, color: Colors.white),
                    ),
                  ),
                ],
              ),
            ),
          );
        }
        // Otherwise, show the regular child widget.
        return child;
      },
    );
  }
}
