import 'package:postgres/postgres.dart';

class SendData {
  late double temp;
  late String source;

  SendData({required this.temp, required this.source});
}

class PostgresWriter {
  late Connection connection;
  static const String table = "temp";

  Future<void> init() async {
    connection = await Connection.open(
        Endpoint(
          host: '',
          database: '',
          username: '',
          password: '',
        ),
        settings: ConnectionSettings(
          sslMode: SslMode.disable,
        ));
  }

  void send(SendData data) async {
    var insert = await connection.execute(
      Sql.named(
          "INSERT INTO $table (source_id, timestamp, temp) VALUES (@source, @time, @temp)"),
      parameters: {
        'time': DateTime.now().toIso8601String().replaceFirst("T", " "),
        'temp': data.temp,
        'source': data.source,
      },
    );

    print("Affected rows: ${insert.affectedRows}");
  }
}
