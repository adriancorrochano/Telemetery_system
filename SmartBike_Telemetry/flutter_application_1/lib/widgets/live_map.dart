import 'package:flutter/material.dart';
import 'package:flutter_map/flutter_map.dart';
import 'package:latlong2/latlong.dart';
import '../ride_controller.dart';

class LiveMap extends StatefulWidget {
  final RideController controller;

  const LiveMap({super.key, required this.controller});

  @override
  State<LiveMap> createState() => _LiveMapState();
}

class _LiveMapState extends State<LiveMap> {
  @override
  Widget build(BuildContext context) {
    final points = widget.controller.points
        .map((p) => LatLng(p.lat, p.lon))
        .toList();

    return FlutterMap(
      options: MapOptions(
        center: points.isNotEmpty ? points.last : LatLng(46.0, 8.9),
        zoom: 15.0,
      ),
      children: [
        TileLayer(
          urlTemplate: "https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png",
          subdomains: const ['a', 'b', 'c'],
        ),
        PolylineLayer(
          polylines: [
            Polyline(
              points: points,
              color: Colors.greenAccent,
              strokeWidth: 4,
            )
          ],
        ),
      ],
    );
  }
}
