import 'dart:convert';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';

typedef TelemetryCallback = void Function(Map<String, dynamic>);

class MqttService {
  final String host = "broker.hivemq.com";
  final int port = 1883; // MQTT TCP (mobile only)
  final String topic = "portenta/bike/json";

  late MqttServerClient client;
  TelemetryCallback? onTelemetry;

  Future<void> connect() async {
    print("MQTT: Connecting...");

    client = MqttServerClient(host, "flutter_mobile_${DateTime.now().millisecondsSinceEpoch}");
    client.port = port;
    client.keepAlivePeriod = 20;
    client.logging(on: true);

    client.onConnected = () => print("MQTT: Connected!");
    client.onDisconnected = () => print("MQTT: Disconnected!");
    client.onSubscribed = (t) => print("MQTT: Subscribed to $t");

    final connMess = MqttConnectMessage()
      .withClientIdentifier("flutterMobileApp")
      .startClean();

    client.connectionMessage = connMess;

    try {
      await client.connect();
    } catch (e) {
      print("MQTT ERROR: $e");
      client.disconnect();
      return;
    }

    client.subscribe(topic, MqttQos.atLeastOnce);

    client.updates!.listen((messages) {
      final MqttPublishMessage recMess = messages[0].payload as MqttPublishMessage;
      final payload = MqttPublishPayload.bytesToStringAsString(recMess.payload.message);

      print("MQTT RECEIVED: $payload");

      try {
        final jsonData = jsonDecode(payload);
        onTelemetry?.call(jsonData);
      } catch (e) {
        print("Invalid JSON: $e");
      }
    });
  }
}
