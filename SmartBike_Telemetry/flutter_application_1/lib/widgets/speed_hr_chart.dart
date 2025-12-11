import 'package:fl_chart/fl_chart.dart';
import 'package:flutter/material.dart';
import '../ride_controller.dart';

class SpeedHrChart extends StatefulWidget {
  final RideController controller;

  const SpeedHrChart({super.key, required this.controller});

  @override
  State<SpeedHrChart> createState() => _SpeedHrChartState();
}

class _SpeedHrChartState extends State<SpeedHrChart> {
  @override
  Widget build(BuildContext context) {
    final points = widget.controller.points;

    if (points.length < 2) {
      return const Center(
        child: Text(
          "Ride graph will appear here...",
          style: TextStyle(color: Colors.white54),
        ),
      );
    }

    final start = widget.controller.startTime!;
    final speedSpots = <FlSpot>[];
    final hrSpots = <FlSpot>[];

    for (var p in points) {
      final t = p.time.difference(start).inSeconds.toDouble();
      speedSpots.add(FlSpot(t, p.speed));
      if (p.bpm != null) hrSpots.add(FlSpot(t, p.bpm!.toDouble()));
    }

    return LineChart(
      LineChartData(
        backgroundColor: Colors.black12,
        minY: 0,
        maxY: (points.map((p) => p.speed).reduce((a, b) => a > b ? a : b)) + 5,
        lineTouchData: LineTouchData(enabled: false),
        titlesData: FlTitlesData(
          bottomTitles: AxisTitles(
            sideTitles: SideTitles(showTitles: true, reservedSize: 30),
          ),
          leftTitles: AxisTitles(
            sideTitles: SideTitles(showTitles: true, reservedSize: 40),
          ),
          rightTitles: AxisTitles(
            sideTitles: SideTitles(showTitles: true, reservedSize: 40),
          ),
          topTitles: AxisTitles(),
        ),
        gridData: FlGridData(show: true),
        borderData: FlBorderData(show: true),
        lineBarsData: [
          // SPEED
          LineChartBarData(
            spots: speedSpots,
            isCurved: true,
            color: Colors.lightBlueAccent,
            barWidth: 3,
            dotData: FlDotData(show: false),
          ),
          // HEART RATE
          LineChartBarData(
            spots: hrSpots,
            isCurved: true,
            color: Colors.redAccent,
            barWidth: 2,
            dotData: FlDotData(show: false),
          ),
        ],
      ),
    );
  }
}
