import 'dart:convert';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';

typedef TelemetryCallback = void Function(Map<String, dynamic>);

class MqttService {
  final String broker = "broker.hivemq.com";
  final int port = 1883;
  final String topic = "portenta/bike/json";

  late MqttServerClient client;
  TelemetryCallback? onTelemetry;

  Future<void> connect() async {
    client = MqttServerClient.withPort(broker, "bike_app_${DateTime.now().millisecondsSinceEpoch}", port);
    client.logging(on: false);
    client.keepAlivePeriod = 20;

    client.onConnected = () => print("MQTT Connected");
    client.onDisconnected = () => print("MQTT Disconnected");

    try {
      await client.connect();
    } catch (e) {
      print("Error connecting MQTT: $e");
      client.disconnect();
      return;
    }

    client.subscribe(topic, MqttQos.atLeastOnce);

    client.updates!.listen((List<MqttReceivedMessage<MqttMessage>> events) {
      final MqttPublishMessage message = events[0].payload as MqttPublishMessage;
      final payload = MqttPublishPayload.bytesToStringAsString(message.payload.message);

      try {
        final data = jsonDecode(payload);
        if (onTelemetry != null) onTelemetry!(data);
      } catch (e) {
        print("Invalid JSON: $payload");
      }
    });
  }
}
