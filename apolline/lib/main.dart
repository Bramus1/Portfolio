import 'dart:async';
import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:flutter_background/flutter_background.dart';
import 'package:flutter_local_notifications/flutter_local_notifications.dart';
import 'managers/bluetooth_manager.dart';
import 'managers/data_manager.dart';
import 'package:flutter_dotenv/flutter_dotenv.dart';
import 'package:timezone/data/latest_all.dart' as tzData;
import 'package:timezone/timezone.dart' as tz;
import 'notifications/notifications.dart';
import 'notifications/bluetooth_guard.dart';
import '/models/sync_manager.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  // Load environment variables
  await dotenv.load(fileName: ".env");
  tzData.initializeTimeZones();
  tz.setLocalLocation(tz.getLocation('Europe/Rome'));

  await NotificationService().init();
  await NotificationService().checkNotificationPermission();
  await NotificationService().scheduleMorningAndEveningNotifications();
  await NotificationService().checkBluetoothAndLocation();

  // Enable background execution to reduce battery consumption
  final androidConfig = FlutterBackgroundAndroidConfig(
    notificationTitle: "APO BLE SD Uploader",
    notificationText: "Running in background",
    notificationImportance: AndroidNotificationImportance.max,
    enableWifiLock: true,
  );
  if (await FlutterBackground.hasPermissions) {
    await FlutterBackground.initialize(androidConfig: androidConfig);
    await FlutterBackground.enableBackgroundExecution();
  }
  BluetoothManager.setNotificationPlugin(
    NotificationService().notificationsPlugin,
  );

  runApp(MyApp(notifications: NotificationService().notificationsPlugin));
}

class MyApp extends StatefulWidget {
  final FlutterLocalNotificationsPlugin notifications;
  const MyApp({super.key, required this.notifications});

  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  // Hold the current language code; default to English.
  String _language = 'en';

  // Custom translation map.
  final Map<String, Map<String, String>> translations = {
    'en': {
      'select_ble_device': 'Select BLE device (APO*)',
      'rescan': 'Rescan',
      'connect_device': 'Connect device',
      'cancel': 'Cancel',
      'connect': 'Connect',
      'language': 'Language',
      'sync_in_progress': 'Sync in progress...',
      'synchronization_completed': 'Synchronization completed',
      'device_disconnected': 'Device disconnected',
      'sd_sync_in_progress': 'SD sync in progress...',
      'sync_complete_no_files': 'Sync complete, no files to process',
    },
    'fr': {
      'select_ble_device': 'Sélectionnez un appareil BLE (APO*)',
      'rescan': 'Rescan',
      'connect_device': 'Connecter appareil',
      'cancel': 'Annuler',
      'connect': 'Connecter',
      'language': 'Langue',
      'sync_in_progress': 'Synchronisation en cours...',
      'synchronization_completed': 'Synchronisation terminée',
      'sync_complete_no_files':
          'Synchronisation terminée, aucun fichier à traiter',
      'sd_sync_in_progress': 'Synchronisation SD en cours...',
    },
  };

  // Helper method to get the translation for a given key.
  String t(String key) {
    return translations[_language]?[key] ?? key;
  }

  // Function to update the language.
  void _changeLanguage(String lang) {
    setState(() {
      _language = lang;
    });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'APO BLE SD Uploader',
      debugShowCheckedModeBanner: false,
      theme: ThemeData(primarySwatch: Colors.deepPurple),
      // Pass the translation function and language change callback down to the first screen.
      home: BluetoothGuard(
        child: DeviceSelectionScreen(
          notifications: widget.notifications,
          t: t,
          onLanguageChange: _changeLanguage,
          currentLanguage: _language,
        ),
      ),
    );
  }
}

class DeviceSelectionScreen extends StatefulWidget {
  final FlutterLocalNotificationsPlugin notifications;
  final String Function(String key) t;
  final void Function(String lang) onLanguageChange;
  final String currentLanguage;
  const DeviceSelectionScreen({
    super.key,
    required this.notifications,
    required this.t,
    required this.onLanguageChange,
    required this.currentLanguage,
  });

  @override
  _DeviceSelectionScreenState createState() => _DeviceSelectionScreenState();
}

class _DeviceSelectionScreenState extends State<DeviceSelectionScreen> {
  List<ScanResult> scanResults = [];
  bool isScanning = false;

  @override
  void initState() {
    super.initState();
    _startScan();
  }

  Future<void> _startScan() async {
    setState(() {
      scanResults.clear();
      isScanning = true;
    });
    await BluetoothManager.startScan((results) {
      if (!mounted) return;
      setState(() {
        scanResults = results;
      });
    });
    setState(() {
      isScanning = false;
    });
  }

  Future<void> _connectDevice(BluetoothDevice device) async {
    bool? ok = await showDialog<bool>(
      context: context,
      barrierDismissible: false,
      builder: (context) {
        return AlertDialog(
          title: Text(widget.t('connect_device')),
          content: Text(
            "Connect to ${device.name.isNotEmpty ? device.name : device.advName}?",
          ),
          actions: [
            TextButton(
              onPressed: () => Navigator.pop(context, false),
              child: Text(widget.t('cancel')),
            ),
            TextButton(
              onPressed: () => Navigator.pop(context, true),
              child: Text(widget.t('connect')),
            ),
          ],
        );
      },
    );
    if (ok == true) {
      await BluetoothManager.connectToDevice(device);
      Navigator.pushReplacement(
        context,
        MaterialPageRoute(
          builder:
              (context) => BackgroundScreen(
                notifications: widget.notifications,
                intervalMinutes: 1000, // Periodic synchronization interval
                t: widget.t, // Pass the translation function
              ),
        ),
      );
    }
  }

  // Function to display a language selection dialog.
  void _selectLanguage() {
    showDialog(
      context: context,
      builder: (context) {
        return AlertDialog(
          title: Text(widget.t('language')),
          content: DropdownButton<String>(
            value: widget.currentLanguage,
            items: [
              DropdownMenuItem(value: 'en', child: Text('English')),
              DropdownMenuItem(value: 'fr', child: Text('Français')),
            ],
            onChanged: (value) {
              if (value != null) {
                widget.onLanguageChange(value);
                Navigator.pop(context);
              }
            },
          ),
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.t('select_ble_device')),
        actions: [
          IconButton(icon: Icon(Icons.language), onPressed: _selectLanguage),
        ],
      ),
      body: Column(
        children: [
          if (isScanning) const LinearProgressIndicator(),
          Expanded(
            child: ListView.builder(
              itemCount: scanResults.length,
              itemBuilder: (context, index) {
                final result = scanResults[index];
                String name =
                    result.device.name.isNotEmpty
                        ? result.device.name
                        : result.device.advName;
                return ListTile(
                  title: Text(name),
                  subtitle: Text(result.device.id.id),
                  onTap: () => _connectDevice(result.device),
                );
              },
            ),
          ),
          ElevatedButton(
            onPressed: isScanning ? null : _startScan,
            child: Text(widget.t('rescan')),
          ),
        ],
      ),
    );
  }
}

class BackgroundScreen extends StatefulWidget {
  final FlutterLocalNotificationsPlugin notifications;
  final int intervalMinutes;
  final String Function(String key) t; // translation function
  const BackgroundScreen({
    Key? key,
    required this.notifications,
    this.intervalMinutes = 1000,
    required this.t,
  }) : super(key: key);

  @override
  _BackgroundScreenState createState() => _BackgroundScreenState();
}

class _BackgroundScreenState extends State<BackgroundScreen> {
  Timer? _syncTimer;
  String statusMessage = '';
  final SyncManager syncManager = SyncManager();

  @override
  void initState() {
    super.initState();
    // Initialize with a translated message
    statusMessage = widget.t('device_connected');
    _startImmediateSync();
    _syncTimer = Timer.periodic(Duration(minutes: widget.intervalMinutes), (
      timer,
    ) async {
      if (BluetoothManager.getDevice() != null) {
        setState(() {
          statusMessage = widget.t('sync_in_progress');
        });
        var device = BluetoothManager.getDevice();
        if (device != null) {
          await DataManager().fetchFileList(device);
          await DataManager().processBatchTransfer();
        }
        await syncManager.synchronizeBatch();
        setState(() {
          statusMessage = widget.t('synchronization_completed');
        });
      } else {
        setState(() {
          statusMessage = widget.t('device_disconnected');
        });
        timer.cancel();
        Navigator.pushReplacement(
          context,
          MaterialPageRoute(
            builder:
                (context) => DeviceSelectionScreen(
                  notifications: widget.notifications,
                  t: widget.t,
                  onLanguageChange: (lang) {},
                  currentLanguage: 'en',
                ),
          ),
        );
      }
    });
  }

  void _startImmediateSync() async {
    syncManager.getRepository().deleteOldData();
    if (BluetoothManager.getDevice() != null) {
      setState(() {
        statusMessage = widget.t('sd_sync_in_progress');
      });
      var device = BluetoothManager.getDevice();
      if (device != null) {
        await DataManager().fetchFileList(device);
        await DataManager().processBatchTransfer();
      }
      await syncManager.synchronizeBatch();
      setState(() {
         statusMessage = widget.t('synchronization_completed');
      });
    }
  }

  @override
  void dispose() {
    _syncTimer?.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text(widget.t('device_connected'))),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Text(statusMessage),
            const SizedBox(height: 20),
            StreamBuilder<FileProgress>(
              stream: syncManager.fileProgressStream,
              builder: (context, snapshot) {
                if (!snapshot.hasData) {
                  return const CircularProgressIndicator();
                }
                final progress = snapshot.data!;
                if (progress.total == 0) {
                  return Text(widget.t('sync_complete_no_files'));
                }
                double fraction = progress.current / progress.total;
                return Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    LinearProgressIndicator(value: fraction),
                    const SizedBox(height: 10),
                    Text(
                      "File ${progress.current} of ${progress.total} processed",
                    ),
                    Text("${(fraction * 100).toStringAsFixed(1)}% complete"),
                  ],
                );
              },
            ),
          ],
        ),
      ),
    );
  }
}
