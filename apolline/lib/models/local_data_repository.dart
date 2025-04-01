// local_data_repository.dart
import 'dart:async';
import 'package:sqflite/sqflite.dart';
import 'package:path/path.dart';
import 'package:path_provider/path_provider.dart';

class DataPoint {
  int? id;
  final String timestamp; //  the sensor's timestamp (ISO string or epoch)
  final String sensorData; // the full CSV line or formatted InfluxDB payload
  final int synchronized; // 0 = not yet uploaded, 1 = uploaded

  DataPoint({
    this.id,
    required this.timestamp,
    required this.sensorData,
    this.synchronized = 0,
  });

  Map<String, dynamic> toMap() {
    return {
      'id': id,
      'timestamp': timestamp,
      'sensorData': sensorData,
      'synchronized': synchronized,
    };
  }

  factory DataPoint.fromMap(Map<String, dynamic> map) {
    return DataPoint(
      id: map['id'],
      timestamp: map['timestamp'],
      sensorData: map['sensorData'],
      synchronized: map['synchronized'],
    );
  }
}

/// Repository for local persistence using SQFLite.
class LocalDataRepository {
  static final LocalDataRepository _instance = LocalDataRepository._internal();
  factory LocalDataRepository() => _instance;
  LocalDataRepository._internal();

  Database? _database;

  Future<Database> get database async {
    if (_database != null) return _database!;
    _database = await _initDatabase();
    return _database!;
  }

  Future<Database> _initDatabase() async {
    final documentsDirectory = await getApplicationDocumentsDirectory();
    String path = join(documentsDirectory.path, 'sensor_data.db');
    return await openDatabase(path, version: 1, onCreate: _onCreate);
  }

  Future _onCreate(Database db, int version) async {
    await db.execute('''
      CREATE TABLE DataPoints (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        timestamp TEXT NOT NULL,
        sensorData TEXT NOT NULL,
        synchronized INTEGER DEFAULT 0
      )
    ''');
  }

  Future<int> insertDataPoint(DataPoint point) async {
    final db = await database;
    return await db.insert('DataPoints', point.toMap());
  }

  Future<List<DataPoint>> getUnsyncedData() async {
    final db = await database;
    final List<Map<String, dynamic>> maps = await db.query(
      'DataPoints',
      where: 'synchronized = ?',
      whereArgs: [0],
    );
    return List.generate(maps.length, (i) => DataPoint.fromMap(maps[i]));
  }

  Future<int> markDataAsSynced(List<int> ids) async {
    final db = await database;
    return await db.update(
      'DataPoints',
      {'synchronized': 1},
      where: 'id IN (${List.filled(ids.length, '?').join(',')})',
      whereArgs: ids,
    );
  }

  Future<int> deleteOldData() async {
    final db = await database;
    DateTime cutoff = DateTime.now().subtract(Duration(days: 7));
    return await db.delete(
      'DataPoints',
      where: "datetime(timestamp) < datetime(?) AND synchronized = 1",
      whereArgs: [cutoff.toIso8601String()],
    );
  }
}
