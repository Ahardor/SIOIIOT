import paho.mqtt.client as mqtt
import psycopg2
from datetime import datetime

IP = ""

conn = psycopg2.connect(
    dbname="",
    user="",
    password="",
    host=IP,
    port=""
)
cur = conn.cursor()

def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    client.subscribe("temp")

def on_message(client, userdata, msg):
    current_time = datetime.now()
    message = msg.payload.decode().split(',')
    temperature = message[0]
    source = message[1]
    print(f"Topic: {msg.topic}, id: {source}, temp: {temperature}")

    try:
        cur.execute(
            "INSERT INTO sensor_data (resource, timestamp, temperature) VALUES (%s, %s, %s)",
            (source, current_time, temperature)
        )
        conn.commit()
        print("INSERT OK")
    except Exception as e:
        print(f"INSERT ERROR: {e}")
        conn.rollback()

client = mqtt.Client()

client.on_connect = on_connect
client.on_message = on_message

client.connect(IP, 1883, 60)
client.loop_forever()
