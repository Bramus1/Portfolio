import 'dart:async';
import 'package:http/http.dart' as http;
import 'package:influxdb_client/api.dart';
import 'local_data_repository.dart';
import '../managers/bluetooth_manager.dart';
import 'package:flutter_dotenv/flutter_dotenv.dart';

class FileProgress {
  final int current;
  final int total;
  FileProgress({required this.current, required this.total});
}

class SyncManager {
  final LocalDataRepository repository = LocalDataRepository();

  LocalDataRepository getRepository() {
    return repository;
  }

  // Progress streams for UI updates.
  final StreamController<double> _progressController =
      StreamController<double>.broadcast();
  Stream<double> get progressStream => _progressController.stream;

  final StreamController<FileProgress> _fileProgressController =
      StreamController<FileProgress>.broadcast();
  Stream<FileProgress> get fileProgressStream => _fileProgressController.stream;

  // InfluxDB configuration from environment variables.
  final String influxDbUrl = dotenv.env['INFLUX_DB_URL'] ?? "";
  final String influxDbToken = dotenv.env['INFLUX_DB_TOKEN'] ?? "";
  final String influxDbOrg = dotenv.env['INFLUX_DB_ORG'] ?? "";
  final String influxDbBucket = dotenv.env['INFLUX_DB_BUCKET'] ?? "";

  Future<void> synchronizeBatch() async {
    // 1. Retrieve unsynced data from the local database.
    List<DataPoint> unsynced = await repository.getUnsyncedData();
    int totalPoints = unsynced.length;
    if (totalPoints == 0) {
      print("No unsynced data to transfer.");
      _progressController.add(1.0);
      _fileProgressController.add(FileProgress(current: 0, total: 0));
      return;
    }

    // 2. Convert unsynced DataPoints into InfluxDB Points.
    List<Point> points = [];
    // Determine sensor name and version from BluetoothManager.getDevice()
    String sensorName = "UnknownSensor";
    String version = "v1.0";
    var device = BluetoothManager.getDevice();
    if (device != null) {
      String fullName = device.name.isNotEmpty ? device.name : device.advName;
      // Use only the first part (e.g., "APO010")
      sensorName = fullName.split('_')[0];
      // Extract version info (e.g., "APO010_v5.1_93C2" -> "v5.1")
      final RegExp versionRegExp = RegExp(r'_v([\d.]+)');
      final match = versionRegExp.firstMatch(fullName);
      if (match != null) {
        version = "v${match.group(1)!}";
      }
    }

    for (DataPoint point in unsynced) {
      List<String> values = point.sensorData.split(';');
      if (values.length != 21) {
        print(
          "❌ Data point id ${point.id} has ${values.length} fields; expected 21. Skipping.",
        );
        continue;
      }

      // Build the InfluxDB Point.
      Point influxPoint = Point("sensor_data")
          .addTag("sensor", sensorName)
          .addTag("version", version)
          .addField("horodatage", values[0])
          .addField("PM1.0(ug/m3)", double.tryParse(values[1]) ?? 0)
          .addField("PM2.5(ug/m3)", double.tryParse(values[2]) ?? 0)
          .addField("PM10(ug/m3)", double.tryParse(values[3]) ?? 0)
          .addField("N0.3(#/cm3)", double.tryParse(values[4]) ?? 0)
          .addField("N0.5(#/cm3)", double.tryParse(values[5]) ?? 0)
          .addField("N1(#/cm3)", double.tryParse(values[6]) ?? 0)
          .addField("N2.5(#/cm3)", double.tryParse(values[7]) ?? 0)
          .addField("N5(#/cm3)", double.tryParse(values[8]) ?? 0)
          .addField("N10(#/cm3)", double.tryParse(values[9]) ?? 0)
          .addField("T_DPS310(C)", double.tryParse(values[10]) ?? 0)
          .addField("P_DPS310(Pa)", double.tryParse(values[11]) ?? 0)
          .addField("H_HDC1080(%)", double.tryParse(values[12]) ?? 0)
          .addField("battery_level", double.tryParse(values[13]) ?? 0)
          .addField("T_AM2320_EXT(C)", double.tryParse(values[14]) ?? 0)
          .addField("H_AM2320_EXT(%)", double.tryParse(values[15]) ?? 0)
          .addField("Latitude", double.tryParse(values[16]) ?? 0)
          .addField("Longitude", double.tryParse(values[17]) ?? 0)
          .addField("Altitude", double.tryParse(values[18]) ?? 0)
          .addField("vitesse", double.tryParse(values[19]) ?? 0)
          .addField("nb_de_satellites", double.tryParse(values[20]) ?? 0);

      // Convert the timestamp from field 0 by splitting the string.
      try {
        List<String> parts = values[0].split('_');
        if (parts.length != 6) {
          throw FormatException("Unexpected timestamp format");
        }
        DateTime timestamp = DateTime(
          int.parse(parts[0]),
          int.parse(parts[1]),
          int.parse(parts[2]),
          int.parse(parts[3]),
          int.parse(parts[4]),
          int.parse(parts[5]),
        );

        // Convert to Unix timestamp in seconds.
        influxPoint.time(timestamp.toUtc().millisecondsSinceEpoch ~/ 1000);
      } catch (e) {
        print(
          "Invalid timestamp format for DataPoint id ${point.id}: ${values[0]}. Skipping.",
        );
        continue;
      }
      points.add(influxPoint);
    }

    // 3. Manually split the points list into batches.
    final int chunkSize = 500;
    int totalBatches = (points.length + chunkSize - 1) ~/ chunkSize;
    int batchCounter = 0;

    for (int i = 0; i < points.length; i += chunkSize) {
      int endIndex = i + chunkSize;
      if (endIndex > points.length) endIndex = points.length;
      List<Point> batchPoints = points.sublist(i, endIndex);
      // Convert points to line protocol with second precision.
      String payload = batchPoints
          .map((p) => p.toLineProtocol(WritePrecision.s))
          .join('\n');

      try {
        final response = await http.post(
          Uri.parse(
            "$influxDbUrl/api/v2/write?org=$influxDbOrg&bucket=$influxDbBucket&precision=s",
          ),
          headers: {
            "Authorization": "Token $influxDbToken",
            "Content-Type": "text/plain",
          },
          body: payload,
        );
        if (response.statusCode == 204) {
          print("✅ Batch ${batchCounter + 1} upload successful.");
          // Mark this batch's DataPoints as synced.
          try {
            List<int> batchIds =
                unsynced.sublist(i, endIndex).map((p) => p.id!).toList();
            await repository.markDataAsSynced(batchIds);
          } catch (e) {
            print("❌ Error marking batch ${batchCounter + 1} as synced: $e");
          }
        } else {
          print(
            "❌ Error during batch ${batchCounter + 1} upload: ${response.statusCode} - ${response.body}",
          );
        }
      } catch (e) {
        print("❌ Exception during batch ${batchCounter + 1} upload: $e");
      }

      batchCounter++;
      // Update progress based on batches sent.
      double progressFraction = batchCounter / totalBatches;
      _progressController.add(progressFraction);
      _fileProgressController.add(
        FileProgress(current: batchCounter, total: totalBatches),
      );
    }

    // 4. Reset progress indicators.
    _progressController.add(0.0);
    _fileProgressController.add(FileProgress(current: 0, total: 0));
    print("✅ Batch synchronization completed.");
  }
}
