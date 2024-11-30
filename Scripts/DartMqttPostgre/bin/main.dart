import 'package:dart_mqtt_postgre/mqtt.dart';
import 'package:dart_mqtt_postgre/postgres.dart';

void main() async {
  var reader = MQTTReader();
  await reader.init();

  var writer = PostgresWriter();
  await writer.init();

  reader.subscribe('temp1');
  reader.addListener('temp1', (payload) {
    print(payload);
    writer.send(SendData(
      temp: double.parse(payload),
      source: "1",
    ));
  });

  reader.subscribe('temp2');
  reader.addListener('temp2', (payload) {
    print(payload);
    writer.send(SendData(
      temp: double.parse(payload),
      source: "2",
    ));
  });
}
