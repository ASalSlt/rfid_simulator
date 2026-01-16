# RFID Simulator

An RFID simulator that uses MQTT for communication.

## System Requirements
- Linux
- GCC 12.1.0 or higher
- CMake 3.20+
- Make
- Python 3.x (only if you use the provisional Python broker)

## Why a bundled Python broker and local Paho builds
To avoid local installations and to support environments without access to external repositories:
- A minimal Python MQTT broker is included (broker/minimal_mqtt_broker.py) for quick, offline testing.
- Paho MQTT (C and C++) is built and installed locally under external/paho_install.

Recommended methodology for normal/production environments:
- Use a proper MQTT broker like Mosquitto (robust, complete).
- Install Paho C/C++ via the system package manager.

## MQTT Broker Options

### Option A — Minimal Python broker (provisional)
Purpose: quick tests without external installs.

Start:
```bash
python3 broker/minimal_mqtt_broker.py
```

Limitations:
- Simplified implementation; not fully MQTT-compliant.
- Limited keep-alive handling; not suitable for real workloads.
- Use only for quick, local tests or fully offline environments.

Stop:
- Ctrl+C in the broker terminal. If it doesn’t stop, kill the process:
  ```bash
  pkill -f minimal_mqtt_broker
  ```

### Option B — Mosquitto (recommended)
Install:
```bash
sudo apt-get install mosquitto mosquitto-clients
```

Run:
```bash
mosquitto -p 1883
```

Why Mosquitto:
- Robust keep-alive/session handling and QoS.
- Full MQTT compliance and stability.
- Production-ready performance.

## Local Paho installation (bundled build)

### 1) Paho MQTT C
```bash
cd external/paho.mqtt.c
mkdir -p build
cd build

cmake .. \
  -DCMAKE_C_COMPILER=/data/asalvatore/dev-tools/gcc-12.1.0/bin/gcc \
  -DCMAKE_BUILD_TYPE=Debug \
  -DPAHO_BUILD_SHARED=OFF \
  -DPAHO_BUILD_STATIC=ON

make -j8
make install DESTDIR=../../../external/paho_install
```

### 2) Paho MQTT C++
```bash
cd external/paho.mqtt.cpp
mkdir -p build
cd build

CC=/data/asalvatore/dev-tools/gcc-12.1.0/bin/gcc \
CXX=/data/asalvatore/dev-tools/gcc-12.1.0/bin/g++ \
cmake .. \
  -DCMAKE_CXX_STANDARD=17 \
  -DCMAKE_BUILD_TYPE=Debug \
  -DPAHO_BUILD_SHARED=OFF \
  -DPAHO_BUILD_STATIC=ON \
  -DPAHO_WITH_SSL=OFF \
  -DPAHO_ENABLE_TESTING=OFF \
  -DCMAKE_PREFIX_PATH=../../../external/paho_install

make -j8
make install DESTDIR=../../../external/paho_install
```

Recommended alternative (for production):
```bash
sudo apt-get install libpaho-mqttpp-dev
```

## Build the project
```bash
mkdir -p build_Debug
cd build_Debug

CC=/data/asalvatore/dev-tools/gcc-12.1.0/bin/gcc \
CXX=/data/asalvatore/dev-tools/gcc-12.1.0/bin/g++ \
cmake ..

make -j8
```

The executable will be generated at: build_Debug/rfid_simulator

## Quick Start

1) Start a broker (recommended):
```bash
mosquitto -p 1883
```
Or use the provisional Python broker:
```bash
python3 broker/minimal_mqtt_broker.py
```

2) Run the simulator:
```bash
./build_Debug/rfid_simulator \
  --address tcp://127.0.0.1:1883 \
  --client-id rfid_simulator \
  --topic rfid/events
```

3) Run the sample subscriber:
```bash
python3 subscriber/subscriber.py
```

## Configuration
Command-line options:
```bash
./build_Debug/rfid_simulator [options]
  --address <url>      MQTT broker address (default: tcp://127.0.0.1:1883)
  --client-id <id>     MQTT client ID (default: rfid_simulator)
  --topic <topic>      MQTT topic (default: rfid/events)
  --help               Show help
```

Examples:
```bash
./build_Debug/rfid_simulator
./build_Debug/rfid_simulator --address tcp://broker.example.com:1883 --topic sensors/rfid
./build_Debug/rfid_simulator --client-id my_rfid_simulator
```

## Troubleshooting
- Port already in use:
  ```bash
  lsof -i :1883
  kill -9 <PID>
  ```
- Broker not running / connection refused: ensure Mosquitto or the Python broker is running.
- GLIBCXX errors at runtime: align your runtime toolchain with the one used for build, or link libstdc++ statically via CMake (-static-libstdc++ -static-libgcc).
