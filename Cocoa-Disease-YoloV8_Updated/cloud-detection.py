import socket
import sys
import cv2
import pickle
import numpy as np
import struct ## new
import zlib
from ultralytics import YOLO

HOST='172.20.10.3'
PORT=9999

s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
print('Socket created')

s.bind((HOST,PORT))
print('Socket bind complete')
s.listen(10)
print('Socket now listening')

conn,addr=s.accept()

data = b""
payload_size = struct.calcsize(">L")
print("payload_size: {}".format(payload_size))

model = YOLO("best.pt")

while True:
    while len(data) < payload_size:
        print("Recv: {}".format(len(data)))
        data += conn.recv(4096)

    print("Done Recv: {}".format(len(data)))
    packed_msg_size = data[:payload_size]
    data = data[payload_size:]
    msg_size = struct.unpack(">L", packed_msg_size)[0]
    print("msg_size: {}".format(msg_size))
    while len(data) < msg_size:
        data += conn.recv(4096)
    frame_data = data[:msg_size]
    data = data[msg_size:]

    frame = pickle.loads(frame_data, fix_imports=True, encoding="bytes")
    frame = cv2.imdecode(frame, cv2.IMREAD_COLOR)

    # YOLO detection
    results = model.predict(frame, conf=0.35)
    annotated_frame = results[0].plot()

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(('172.20.10.4', 9999))
    connection = client_socket.makefile('wb')

    detection = pickle.dumps(annotated_frame, 0)
    size = len(detection)

    print("send Cocoa Disease Dection frame")
    client_socket.sendall(struct.pack(">L", size) + detection)

