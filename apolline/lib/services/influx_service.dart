import 'package:influxdb_client/api.dart';

class InfluxService {
  late final InfluxDBClient client;
  late final WriteService writeService;
  final String org;
  final String bucket;

  InfluxService({
    required String url,
    required String token,
    required this.org,
    required this.bucket,
    int batchSize = 500,
    int flushInterval = 10000, // flush interval in milliseconds
  }) {
    client = InfluxDBClient(url: url, token: token, org: org, bucket: bucket);
    // Set the write options on the client.
    // Note: flushInterval is expected to be an int (milliseconds).
    writeService = client.getWriteService();
    writeService.writeOptions = WriteOptions(
      batchSize: batchSize,
      flushInterval: flushInterval,
    );
    // Obtain the write service without any named parameters.
    // writeService = client.getWriteService();
  }

  /// Writes a list of InfluxDB Points.
  Future<void> writePoints(List<Point> points) async {
    // writePoints expects a list of points.
    writeService.write(points);
    await writeService.flush();
  }

  void close() {
    client.close();
  }
}
