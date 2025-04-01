import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'dart:async';
import 'dart:convert';
//import 'package:http/http.dart' as http;
import 'bluetooth_manager.dart';
import 'package:flutter_dotenv/flutter_dotenv.dart';
import '../models/local_data_repository.dart';

//Author:
//  - Giuliano Taurone

/// DataManager class: manages the data flow between the device and the InfluxDB server.
class DataManager {
  static final DataManager _instance = DataManager._internal();

  factory DataManager() {
    return _instance;
  }

  DataManager._internal();

  BluetoothDevice? connectedDevice = BluetoothManager.getDevice();
  BluetoothCharacteristic? dataCharacteristic;

  // List of files to be transferred
  List<String> fileList = [];
  // Buffer for file data
  String fileBuffer = "";

  int totalFiles = 0;
  int filesTransferred = 0;
  bool batchTransferError = false;

  final StreamController<double> _progressController =
      StreamController<double>.broadcast();
  Stream<double> get progressStream => _progressController.stream;
  // Completer for file data
  Completer<String>? _fileCompleter;
  // InfluxDB configuration
  final String influxDbUrl = dotenv.env['INFLUX_DB_URL'] ?? "";
  final String influxDbToken = dotenv.env['INFLUX_DB_TOKEN'] ?? "";
  final String influxDbOrg = dotenv.env['INFLUX_DB_ORG'] ?? "";
  final String influxDbBucket = dotenv.env['INFLUX_DB_BUCKET'] ?? "";

  Future<void> _setupCharacteristic(BluetoothDevice device) async {
    if (dataCharacteristic != null) return;
    List<BluetoothService> services = await device.discoverServices();
    for (var service in services) {
      for (var characteristic in service.characteristics) {
        if (characteristic.properties.notify &&
            characteristic.properties.write) {
          dataCharacteristic = characteristic;

          return;
        }
      }
    }
    print("⚠️ No notify+write features found!");
  }

  /// Start listening for BLE data to receive both file list and file contents
  Future<void> startListening(BluetoothDevice device) async {
    await _setupCharacteristic(device);
    if (dataCharacteristic == null) return;
    await dataCharacteristic!.setNotifyValue(true);
    dataCharacteristic!.lastValueStream.listen((value) {
      if (value.isEmpty) return;
      final decoded = utf8.decode(value).trim();
      onBleData(decoded);
    });
  }

  /// Request SD file list: send "d" command to deactivate live and then "m"
  Future<void> fetchFileList(BluetoothDevice device) async {
    await _setupCharacteristic(device);
    fileList.clear();
    // Disable live BLE
    await sendCommand("d", "Deactivate live_BLE");

    await sendCommand("m", "SD File List Request");

    await Future.delayed(const Duration(seconds: 5));
  }

  /// Parsing file list: collects lines ending in ".csv"
  void parseFileListData(String data) {
    final lines = data.split('\n');
    for (var line in lines) {
      if (line.contains("End of file list")) {
        print("📂 End of file list.");
        return;
      }
      if (line.toLowerCase().trim().endsWith(".csv")) {
        fileList.add(line.trim());
        print("Added file: ${line.trim()}");
      }
    }
  }

  Future<bool> openFile(String fileName) async {
    if (fileName.isEmpty || dataCharacteristic == null) return false;
    fileBuffer = "";
    _fileCompleter = Completer<String>();

    // Convert the name part to lowercase
    // Assume that fileName has a dot to separate the name and extension
    int dotIndex = fileName.lastIndexOf('.');
    String lowerNamePart =
        (dotIndex != -1)
            ? fileName.substring(0, dotIndex).toLowerCase()
            : fileName.toLowerCase();
    String extPart = (dotIndex != -1) ? fileName.substring(dotIndex) : "";
    String processedFileName = lowerNamePart + extPart;
    await sendCommand("o$processedFileName", "Request file $processedFileName");

    String completeData;
    try {
      completeData = await _fileCompleter!.future;
    } catch (e) {
      print("❌ Exception while reading file $fileName: $e");
      return false;
    }
    bool success = await processFileData(completeData);
    fileBuffer = "";
    return success;
  }

  // Replace your processFileData method with the following:
  Future<bool> processFileData(String data) async {
    // Split the content into lines (handling any \r and \n)
    List<String> lines = data.split(RegExp(r'[\r\n]+'));
    int totalLines = 0;
    int emptyLines = 0;
    int invalidLines = 0;
    for (String line in lines) {
      totalLines++;
      line = line.trim();
      if (line.isEmpty || line.startsWith('#')) {
        emptyLines++;
        continue;
      }
      // If the line contains an end-of-transfer marker, stop reading further.
      if (line.toLowerCase().contains("end of file") ||
          line.toLowerCase().contains("end_of_transfer")) {
        print("📂 End of transfer detected.");
        break;
      }
      // Split the line using ";" as a separator.
      List<String> values = line.split(';').map((v) => v.trim()).toList();
      // Expect exactly 21 columns (adjust if needed)
      if (values.length != 21) {
        invalidLines++;
        print("⚠️ Incomplete CSV row (columns: ${values.length}) - '$line'");
        continue;
      }
      // Create a DataPoint.
      // You can decide whether to use the provided timestamp (values[0])
      // or generate one here. In this example we use values[0].
      DataPoint point = DataPoint(
        timestamp: values[0],
        sensorData:
            line, // In a more advanced version, you could format a payload.
      );
      await LocalDataRepository().insertDataPoint(point);
    }
    print(
      "Total lines: $totalLines, empty/header: $emptyLines, invalid: $invalidLines",
    );
    return true;
  }

  // Modify processBatchTransfer to only process files (i.e. open file, process data, archive file) without uploading directly:
  Future<void> processBatchTransfer() async {
    if (fileList.isEmpty) {
      print("⚠️ No files to transfer.");
      await sendCommand("c", "Reactivation live_BLE");
      return;
    }

    totalFiles = fileList.length;
    filesTransferred = 0;
    bool allSuccess = true;

    // Create a copy of the file list.
    List<String> filesToProcess = List.from(fileList);

    for (String file in filesToProcess) {
      print("Processing file: $file");
      bool result = await openFile(file);
      if (!result) {
        allSuccess = false;
        break;
      } else {
        // Optionally archive the file on the device.
        await sendCommand("G$file", "SD File Archiviation");
      }
      filesTransferred++;
      double progress = filesTransferred / totalFiles;
      _progressController.add(progress);
      print("✅ File $filesTransferred/$totalFiles processed");
      await Future.delayed(const Duration(seconds: 1));
    }

    if (allSuccess) {
      print("✅ Batch transfer completed.");
      final now = DateTime.now().toUtc();
      await sendCommand(
        "H${now.year};${now.month};${now.day};${now.hour};${now.minute};${now.second}",
        "RTC sync",
      );
    } else {
      print("❌ Batch transfer error; directory NOT deleted.");
    }

    await sendCommand("c", "Reactivation live_BLE");
    fileList.clear();
    _progressController.add(0.0);
  }

  /// Handles data received via BLE.
  /// If it contains "End of file", it completes the file buffer.
  void onBleData(String data) {
    // If data contains ".csv" or file list marker, handle as file list
    if (data.toLowerCase().trim().endsWith(".csv") ||
        data.contains("end of file list.")) {
      parseFileListData(data);
    } else {
      // Append each block of data into the buffer
      fileBuffer += "$data\n";

      // If the block contains the end-of-transfer marker,
      // we complete the Completer
      if (data.toUpperCase().contains("END_OF_TRANSFER") ||
          data.toLowerCase().contains("end of file")) {
        print("✅ End of file received.");
        if (_fileCompleter != null && !_fileCompleter!.isCompleted) {
          _fileCompleter!.complete(fileBuffer);
        }
      }
    }
  }

  // Buffer for partial lines
  /* String partialData = "";

  void onBleData(String data) {
    partialData += data; // accumulate data (no immediate '\n' added here)

    // Process complete lines only
    while (partialData.contains('\n')) {
      int newlineIndex = partialData.indexOf('\n');
      String completeLine = partialData.substring(0, newlineIndex).trim();
      partialData = partialData.substring(newlineIndex + 1);

      // Check for end-of-transfer
      if (completeLine.toUpperCase().contains("END_OF_TRANSFER") ||
          completeLine.toLowerCase().contains("end of file")) {
        print("✅ End of file received.");
        if (_fileCompleter != null && !_fileCompleter!.isCompleted) {
          _fileCompleter!.complete(fileBuffer);
          fileBuffer = "";
          partialData = ""; // reset partialData
          return;
        }
      }

      // Append to file buffer (with newline)
      fileBuffer += completeLine + '\n';
    }
  }
 */
  /// Send a command to the device via BLE.
  Future<void> sendCommand(String cmd, String desc) async {
    if (dataCharacteristic == null) {
      print("❌ Feature not initialized (command$cmd not sent)!");
      return;
    }
    try {
      await dataCharacteristic!.write(
        utf8.encode("$cmd\n"),
        withoutResponse: true,
      );
      await Future.delayed(const Duration(seconds: 1));
      print("✅ Command $cmd sent ($desc)");
    } catch (e) {
      print("❌ Error sending command $cmd: $e");
    }
  }
}
