import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:permission_handler/permission_handler.dart';
import 'data_manager.dart';
import 'package:flutter_local_notifications/flutter_local_notifications.dart';

//Author:
// - Giuliano Taurone

class BluetoothManager {
  static final List<ScanResult> _scanResults = [];
  static BluetoothDevice? connectedDevice;

  // Local notification plugin (configured in main.dart)
  static FlutterLocalNotificationsPlugin _notifPlugin =
      FlutterLocalNotificationsPlugin();

  static void setNotificationPlugin(FlutterLocalNotificationsPlugin plugin) {
    _notifPlugin = plugin;
  }

  static Future<void> requestPermissions() async {
    await [
      Permission.bluetooth,
      Permission.bluetoothScan,
      Permission.bluetoothConnect,
      Permission.locationWhenInUse,
    ].request();
  }

  static Future<void> startScan(Function(List<ScanResult>) onScanUpdate) async {
    await requestPermissions();
    _scanResults.clear();
    // Start scanning for 10 seconds
    await FlutterBluePlus.startScan(timeout: const Duration(seconds: 10));
    FlutterBluePlus.scanResults.listen((results) {
      for (ScanResult result in results) {
        String name =
            result.device.name.isNotEmpty
                ? result.device.name
                : result.device.advName;
        // Filter only devices whose name starts with "APO"
        if (name.startsWith("APO") &&
            !_scanResults.any(
              (d) => d.device.remoteId == result.device.remoteId,
            )) {
          _scanResults.add(result);
        }
      }
      onScanUpdate(List.from(_scanResults));
    });
    await Future.delayed(const Duration(seconds: 10));
    await FlutterBluePlus.stopScan();
  }

  static Future<void> connectToDevice(BluetoothDevice device) async {
    try {
      // Connection with autoConnect false for more stability
      await device.connect(
        autoConnect: false,
      ); // not possible to connect to a device with autoConnect: true
      connectedDevice = device;
      DataManager().connectedDevice = device;
      print(
        "✅ Connected to ${device.name.isNotEmpty ? device.name : device.advName}",
      );
    /*   await device.requestMtu(512);
      print("MTU set to ${await device.mtu}"); */
      // Start listening to BLE data (file list and file content)
      await DataManager().startListening(device);
      // Ascolta lo stato di connessione per notificare disconnessioni
      device.connectionState.listen((connectionState) {
        print("Connection state update: $connectionState");
        if (connectionState == BluetoothConnectionState.disconnected) {
          print(
            "🔴 Disconnection detected for ${device.name.isNotEmpty ? device.name : device.advName}",
          );
          _showDisconnectionNotification(device);
        }
      });
    } catch (e) {
      print("❌Connection error: $e");
    }
  }

  static Future<void> disconnectDevice() async {
    if (connectedDevice != null) {
      try {
        await connectedDevice!.disconnect();
      } catch (e) {
        print("⚠️ Error while disconnecting: $e");
      }
      DataManager().connectedDevice = null;
      connectedDevice = null;
      print("🔴 Disconnected from device");
    }
  }

  static BluetoothDevice? getDevice() {
    return connectedDevice;
  }

  static Future<void> _showDisconnectionNotification(
    BluetoothDevice device,
  ) async {
    const AndroidNotificationDetails androidDetails =
        AndroidNotificationDetails(
          'ble_channel',
          'Bluetooth',
          channelDescription: 'Notifiche per eventi Bluetooth',
          importance: Importance.max,
          priority: Priority.high,
          ongoing: false,
          autoCancel: true,
        );
    const NotificationDetails details = NotificationDetails(
      android: androidDetails,
    );
    await _notifPlugin.show(
      0,
      "BLE disconnected",
      "The device ${device.name.isNotEmpty ? device.name : device.advName} is disconnected.",
      details,
    );
  }
}
