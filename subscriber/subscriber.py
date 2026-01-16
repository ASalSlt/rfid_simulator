import sys
import os

sys.path.insert(
    0,
    os.path.join(os.path.dirname(__file__), "vendor")
)

import paho.mqtt.client as mqtt
print("Paho MQTT loaded from:", mqtt.__file__)

BROKER = "127.0.0.1"
PORT = 1883
TOPIC = "rfid/events"

def on_connect(client, userdata, flags, rc, properties=None):
    print("Connected with result code", rc)
    client.subscribe(TOPIC)

def on_message(client, userdata, msg):
    print(f"Topic: {msg.topic} | Payload: {msg.payload.decode()}")

client = mqtt.Client(callback_api_version=mqtt.CallbackAPIVersion.VERSION2)
client.on_connect = on_connect
client.on_message = on_message


try:
    client.connect(BROKER, PORT, 60)
    client.loop_forever()
except ConnectionRefusedError:
    print(f"Error: Connection to {BROKER}:{PORT} cannot be completed")
    print("Be sure that MQTT broker is running")