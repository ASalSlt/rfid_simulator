import socket
import threading
import time
import signal
import sys

HOST = "127.0.0.1"
PORT = 1883

subscribers = {}
clients = []
server_socket = None

def signal_handler(sig, frame):
    global server_socket
    print("\nShutting down broker...")
    if server_socket:
        server_socket.close()
    sys.exit(0)

def handle_client(conn):
    try:
        data = conn.recv(1024)
        if not data:
            return
        
        conn.send(b'\x20\x02\x00\x00')  # CONNACK
        clients.append(conn)

        last_ping = time.time()
        while True:
            try:
                conn.settimeout(10)
                data = conn.recv(1024)
                
                if not data:
                    break

                packet_type = data[0] >> 4
                last_ping = time.time()

                if packet_type == 12:  # PINGREQ
                    conn.send(b'\xd0\x00')  # PINGRESP
                elif packet_type == 8:  # SUBSCRIBE
                    topic_len = data[4] << 8 | data[5]
                    topic = data[6:6+topic_len].decode()
                    subscribers.setdefault(topic, []).append(conn)
                    conn.send(b'\x90\x03\x00\x01\x00')
                elif packet_type == 3:  # PUBLISH
                    topic_len = data[2] << 8 | data[3]
                    topic = data[4:4+topic_len].decode()
                    payload = data[4+topic_len:]

                    for t, conns in subscribers.items():
                        if topic.startswith(t.rstrip('#')):
                            for c in conns:
                                try:
                                    c.send(data)
                                except:
                                    pass
            except socket.timeout:
                break

    except Exception as e:
        print("Client error:", e)
    finally:
        if conn in clients:
            clients.remove(conn)
        conn.close()

def start_broker():
    global server_socket
    server_socket = socket.socket()
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind((HOST, PORT))
    server_socket.listen(5)
    print(f"MQTT broker listening on {HOST}:{PORT}")
    print("Press Ctrl+C to stop")

    while True:
        try:
            conn, addr = server_socket.accept()
            threading.Thread(target=handle_client, args=(conn,), daemon=True).start()
        except:
            break

if __name__ == "__main__":
    signal.signal(signal.SIGINT, signal_handler)
    start_broker()