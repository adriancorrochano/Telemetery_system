import 'package:latlong2/latlong.dart';

class RidePoint {
  final double lat;
  final double lon;
  final double speed;
  final int? bpm;
  final double g;
  final DateTime time;

  RidePoint({
    required this.lat,
    required this.lon,
    required this.speed,
    required this.bpm,
    required this.g,
    required this.time,
  });
}

class RideController {
  bool recording = false;
  DateTime? startTime;
  List<RidePoint> points = [];

  void start() {
    recording = true;
    points.clear();
    startTime = DateTime.now();
  }

  void stop() {
    recording = false;
  }

  void addTelemetry(Map<String, dynamic> data) {
    if (!recording) return;

    double? lat = data["lat"]?.toDouble();
    double? lon = data["lon"]?.toDouble();
    double? spd = data["spd"]?.toDouble();

    if (lat == null || lon == null || spd == null) return;

    points.add(
      RidePoint(
        lat: lat,
        lon: lon,
        speed: spd,
        bpm: data["bpm"],
        g: data["gf"]?.toDouble() ?? 0.0,
        time: DateTime.now(),
      ),
    );
  }

  double totalDistance() {
    if (points.length < 2) return 0;

    final dist = Distance();
    double total = 0;

    for (int i = 1; i < points.length; i++) {
      total += dist(
        LatLng(points[i - 1].lat, points[i - 1].lon),
        LatLng(points[i].lat, points[i].lon),
      );
    }
    return total / 1000; // km
  }

  double avgSpeed() {
    if (points.isEmpty) return 0;
    return points.map((p) => p.speed).reduce((a, b) => a + b) / points.length;
  }

  double maxSpeed() {
    if (points.isEmpty) return 0;
    return points.map((p) => p.speed).reduce((a, b) => a > b ? a : b);
  }

  double avgBpm() {
    final bpms = points.where((p) => p.bpm != null).map((p) => p.bpm!).toList();
    if (bpms.isEmpty) return 0;
    return bpms.reduce((a, b) => a + b) / bpms.length;
  }

  int maxBpm() {
    final bpms = points.where((p) => p.bpm != null).map((p) => p.bpm!).toList();
    if (bpms.isEmpty) return 0;
    return bpms.reduce((a, b) => a > b ? a : b);
  }
}
