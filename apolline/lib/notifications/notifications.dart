// Notification.dart
import 'package:flutter/material.dart';
import 'package:flutter_local_notifications/flutter_local_notifications.dart';
import 'package:timezone/timezone.dart' as tz;
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:geolocator/geolocator.dart';
import 'package:permission_handler/permission_handler.dart';

class NotificationService {
  // Singleton instance.
  static final NotificationService _instance = NotificationService._internal();
  factory NotificationService() => _instance;

  final FlutterLocalNotificationsPlugin _notificationsPlugin =
      FlutterLocalNotificationsPlugin();

  NotificationService._internal();
  FlutterLocalNotificationsPlugin get notificationsPlugin =>
      _notificationsPlugin;

  /// Initializes the notifications plugin and creates the necessary channels.
  Future<void> init() async {
    // Android initialization settings.
    const AndroidInitializationSettings androidInitializationSettings =
        AndroidInitializationSettings('@mipmap/ic_launcher');

    const InitializationSettings initializationSettings =
        InitializationSettings(android: androidInitializationSettings);

    await _notificationsPlugin.initialize(initializationSettings);

    // Create the notification channels.
    await _initializeDailyReminderChannel();
    await _initializeBluetoothChannel();
    await _initializeLocationChannel();
  }

  /// Creates the channel for daily reminders.
  Future<void> _initializeDailyReminderChannel() async {
    const AndroidNotificationChannel dailyChannel = AndroidNotificationChannel(
      'daily_reminder_channel',
      'Daily Reminders',
      description: 'Remember to open the app to sync sensor data',
      importance: Importance.max,
    );
    final androidImplementation =
        _notificationsPlugin
            .resolvePlatformSpecificImplementation<
              AndroidFlutterLocalNotificationsPlugin
            >();
    await androidImplementation?.createNotificationChannel(dailyChannel);
  }

  /// Creates the channel for Bluetooth notifications.
  Future<void> _initializeBluetoothChannel() async {
    const AndroidNotificationChannel bleChannel = AndroidNotificationChannel(
      'ble_channel',
      'Bluetooth',
      description: 'Bluetooth Event Notifications',
      importance: Importance.max,
    );
    final androidImplementation =
        _notificationsPlugin
            .resolvePlatformSpecificImplementation<
              AndroidFlutterLocalNotificationsPlugin
            >();
    await androidImplementation?.createNotificationChannel(bleChannel);
  }

  /// Creates the channel for location notifications.
  Future<void> _initializeLocationChannel() async {
    const AndroidNotificationChannel locationChannel =
        AndroidNotificationChannel(
          'location_channel',
          'Location',
          description: 'Location Services Notifications',
          importance: Importance.max,
        );
    final androidImplementation =
        _notificationsPlugin
            .resolvePlatformSpecificImplementation<
              AndroidFlutterLocalNotificationsPlugin
            >();
    await androidImplementation?.createNotificationChannel(locationChannel);
  }

  /// Schedules a daily notification based on the provided [TimeOfDay].
  Future<void> scheduleDailyNotification(
    int id,
    String title,
    String body,
    TimeOfDay scheduledTime,
  ) async {
    final tz.TZDateTime now = tz.TZDateTime.now(tz.local);
    // Build the scheduled date using today's date and the specified time.
    tz.TZDateTime scheduledDate = tz.TZDateTime(
      tz.local,
      now.year,
      now.month,
      now.day,
      scheduledTime.hour,
      scheduledTime.minute,
    );
    // If the scheduled time has already passed, schedule it for the next day (here adding 10 seconds as an example).
    if (scheduledDate.isBefore(now)) {
      scheduledDate = scheduledDate.add(const Duration(days: 1));
    }
    try {
      await _notificationsPlugin.zonedSchedule(
        id,
        title,
        body,
        scheduledDate,
        const NotificationDetails(
          android: AndroidNotificationDetails(
            'daily_reminder_channel',
            'Daily Reminders',
            channelDescription: 'Remember to open the app to sync sensor data',
            importance: Importance.max,
            priority: Priority.max,
          ),
        ),
        uiLocalNotificationDateInterpretation:
            UILocalNotificationDateInterpretation.absoluteTime,
        androidScheduleMode: AndroidScheduleMode.exactAllowWhileIdle,
        matchDateTimeComponents: DateTimeComponents.time,
      );
      print("Notification scheduled with id $id for: $scheduledDate");
    } catch (e, s) {
      print("Error while scheduling notification$id: $e");
      print(s);
    }
  }

  /// Schedules two daily notifications: one for the morning and one for the evening.
  Future<void> scheduleMorningAndEveningNotifications() async {
    await scheduleDailyNotification(
      3,
      "Good morning!",
      "Open the app to sync sensor data.",
      const TimeOfDay(hour: 9, minute: 00),
    );
    await scheduleDailyNotification(
      4,
      "Good evening!",
      "Open the app to sync sensor data.",
      const TimeOfDay(hour: 22, minute: 59),
    );
  }

  /// Checks if Bluetooth and location services are turned on.
  /// If turned off, shows immediate notifications.
  Future<void> checkBluetoothAndLocation() async {
    // Check Bluetooth status.
    BluetoothAdapterState btState = await FlutterBluePlus.adapterState.first;
    if (btState != BluetoothAdapterState.on) {
      await _notificationsPlugin.show(
        1,
        "Bluetooth is off",
        "Turn Bluetooth on to sync data.",
        const NotificationDetails(
          android: AndroidNotificationDetails(
            'ble_channel',
            'Bluetooth',
            channelDescription: 'Bluetooth Event Notifications',
            importance: Importance.max,
            priority: Priority.max,
          ),
        ),
      );
    }
    // Check location services status.
    bool locationEnabled = await Geolocator.isLocationServiceEnabled();
    if (!locationEnabled) {
      await _notificationsPlugin.show(
        2,
        "Location services off",
        "Turn on location services for a better experience.",
        const NotificationDetails(
          android: AndroidNotificationDetails(
            'location_channel',
            'Location',
            channelDescription: 'Location Services Notifications',
            importance: Importance.max,
            priority: Priority.max,
          ),
        ),
      );
    }
  }

  /// Checks the notification permission status and requests permission if not granted.
  Future<void> checkNotificationPermission() async {
    var status = await Permission.notification.status;
    if (!status.isGranted) {
      var newStatus = await Permission.notification.request();
      if (!newStatus.isGranted) {
        print("Notification permission not granted.");
      }
    }
  }
}
