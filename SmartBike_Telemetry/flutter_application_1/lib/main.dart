import 'package:flutter/material.dart';
import 'pages/dashboard_page.dart';

void main() {
  runApp(const BikeApp());
}

class BikeApp extends StatelessWidget {
  const BikeApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Smart Bike Telemetry',
      theme: ThemeData.dark(),
      home: const DashboardPage(),
    );
  }
}
