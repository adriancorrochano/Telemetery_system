import 'package:flutter/material.dart';
import '../ride_controller.dart';

class SummaryPage extends StatelessWidget {
  final RideController ride;

  const SummaryPage({super.key, required this.ride});

  @override
  Widget build(BuildContext context) {
    final dist = ride.totalDistance();
    final avgSpd = ride.avgSpeed();
    final maxSpd = ride.maxSpeed();
    final avgHr = ride.avgBpm();
    final maxHr = ride.maxBpm();

    final duration =
        ride.points.last.time.difference(ride.startTime!).inSeconds;

    return Scaffold(
      appBar: AppBar(
        title: const Text("Ride Summary"),
      ),
      body: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          children: [
            Text("Total Time: ${duration}s"),
            Text("Distance: ${dist.toStringAsFixed(2)} km"),
            Text("Avg Speed: ${avgSpd.toStringAsFixed(1)} km/h"),
            Text("Max Speed: ${maxSpd.toStringAsFixed(1)} km/h"),
            Text("Avg HR: ${avgHr.toStringAsFixed(0)} bpm"),
            Text("Max HR: ${maxHr.toStringAsFixed(0)} bpm"),
          ],
        ),
      ),
    );
  }
}
