import 'package:flutter/material.dart';
import '../mqtt_service.dart';
import '../ride_controller.dart';
import 'summary_page.dart';
import '../widgets/speed_hr_chart.dart';
import '../widgets/live_map.dart';

class DashboardPage extends StatefulWidget {
  const DashboardPage({super.key});

  @override
  State<DashboardPage> createState() => _DashboardPageState();
}

class _DashboardPageState extends State<DashboardPage> {
  final MqttService mqtt = MqttService();
  final RideController ride = RideController();

  double liveSpeed = 0;
  int liveBpm = 0;
  double liveG = 0;

  @override
  void initState() {
    super.initState();

    mqtt.onTelemetry = (data) {
      setState(() {
        liveSpeed = (data["spd"] ?? 0).toDouble();
        liveBpm = (data["bpm"] ?? 0).toInt();
        liveG = (data["gf"] ?? 0).toDouble();
      });

      ride.addTelemetry(data);
    };

    mqtt.connect();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text("Smart Bike Dashboard"),
      ),
      body: Column(
        children: [
          // LIVE VALUES
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: [
              StatBox(label: "Speed", value: "${liveSpeed.toStringAsFixed(1)} km/h"),
              StatBox(label: "HR", value: "$liveBpm bpm"),
              StatBox(label: "G-Force", value: liveG.toStringAsFixed(2)),
            ],
          ),

          // MAP
          Expanded(
            child: LiveMap(controller: ride),
          ),

          // SPEED/HR CHART
          SizedBox(
            height: 150,
            child: SpeedHrChart(controller: ride),
          ),

          // START / STOP BUTTONS
          Padding(
            padding: const EdgeInsets.all(12),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                ElevatedButton(
                  onPressed: !ride.recording
                      ? () {
                          setState(() => ride.start());
                        }
                      : null,
                  child: const Text("Start"),
                ),
                const SizedBox(width: 20),
                ElevatedButton(
                  onPressed: ride.recording
                      ? () {
                          setState(() => ride.stop());
                          Navigator.push(
                            context,
                            MaterialPageRoute(
                              builder: (_) => SummaryPage(ride: ride),
                            ),
                          );
                        }
                      : null,
                  child: const Text("Finish"),
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }
}

class StatBox extends StatelessWidget {
  final String label;
  final String value;

  const StatBox({super.key, required this.label, required this.value});

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        Text(label, style: const TextStyle(color: Colors.white70)),
        Text(value, style: const TextStyle(fontSize: 20)),
      ],
    );
  }
}
