import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';

class MQTTReader {
  late MqttServerClient client;
  Map<String, List<Function>> callbacks = {};

  Future<void> init() async {
    client = MqttServerClient.withPort('127.0.0.1', 'dart_client', 1883);
    var state = await client.connect();

    if (state == MqttConnectionState.connected) {
      print('client connected');
    } else {
      print('client connection failed - $state');
    }

    client.updates!.listen((messages) {
      for (var message in messages) {
        var topic = message.topic;
        var payload = (message.payload as MqttPublishMessage).payload;

        if (callbacks.containsKey(topic)) {
          for (var i in callbacks[topic]!) {
            i(MqttPublishPayload.bytesToStringAsString(payload.message));
          }
        }
      }
    });
  }

  void publish(String topic, String message) {
    var msg = MqttClientPayloadBuilder();
    msg.addString(message);

    client.publishMessage(
      topic,
      MqttQos.atMostOnce,
      msg.payload!,
    );
  }

  void subscribe(String topic) {
    client.subscribe(topic, MqttQos.atMostOnce);
  }

  void addListener(String topic, Function callback) {
    if (!callbacks.containsKey(topic)) {
      callbacks[topic] = [];
    }
    callbacks[topic]!.add(callback);
  }
}
