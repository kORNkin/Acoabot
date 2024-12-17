from flask import Flask, Response, render_template, jsonify, request
from video_dectect import vid_detection, img_detection
import cv2
from cocoa_disease_detection import vid_detection, img_detection, webcam_detection
# import smbus
import serial
import time
import os

import threading
import numpy as np
import struct 
import zlib

app = Flask(__name__)
app.config['SECRET_KEY'] = 'kornkin'

# Raspberry Pi 4's I2C bus number

if os.path.exists("/dev/ttyUSB0"):
    ser = serial.Serial ("/dev/ttyUSB0", 9600)
    print("USB0 connect!")
else: 
    ser = serial.Serial ("/dev/ttyUSB1", 9600)
    print("USB1 connect!")

time.sleep(3)
ser.reset_input_buffer()

# Shared variable for GPS data
gps_data = "No GPS data received"  # Default value until data is read

# Background thread to continuously read data from Arduino
def read_serial_data():
    global gps_data
    while True:
        if ser.in_waiting > 0:
            try:
                # Read and decode the line
                line = ser.readline().decode('utf-8').strip()
                
                # Check if the line starts with "GPS:"
                if line.startswith("GPS:"):
                    gps_data = line[4:].strip()  # Extract everything after "GPS:"
                    print(f"Updated GPS data: {gps_data}")  # Optional: log the received GPS data
            except Exception as e:
                print(f"Error reading from serial: {e}")

# Start the background thread
thread = threading.Thread(target=read_serial_data, daemon=True)
thread.start()

# Function to send command to Arduino
def send_data(data):
    ser.write(bytes(data + '\n', encoding='utf-8'))
    
@app.route('/send_command/<command>')
def handle_command(command):
    send_data(command)  # Corrected function name
    return f"Command '{command}' sent to Arduino."
    
# def generate_pc_detection():
#     yolo_out = pc_detection()
#     for detection_ in yolo_out:
#         ret, buffer = cv2.imencode('.jpg', detection_)

#         frame = buffer.tobytes()
#         yield (b'--frame\r\n'
#                     b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

def generate_webcam():
    yolo_out = webcam_detection()
    for detection_ in yolo_out:
        ret, buffer = cv2.imencode('.jpg', detection_)

        frame = buffer.tobytes()
        yield (b'--frame\r\n'
                    b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

def generate_frames(path_x = ''):
    yolo_out = vid_detection(path_x)
    for detection_ in yolo_out:
        ret, buffer = cv2.imencode('.jpg', detection_)

        frame = buffer.tobytes()
        yield (b'--frame\r\n'
                    b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')
        
def generate_img(path_x = ''):
    yolo_out = img_detection(path_x)
    for detection_ in yolo_out:
        ret, buffer = cv2.imencode('.jpg', detection_)

        frame = buffer.tobytes()
        yield (b'--frame\r\n'
                    b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/webcam')
def webcam():
    return Response(generate_webcam(), mimetype = 'multipart/x-mixed-replace; boundary=frame')

@app.route('/video')
def video():
    return Response(generate_frames(path_x = 'video/10.mp4'), mimetype = 'multipart/x-mixed-replace; boundary=frame')

@app.route('/video/<int:num>')
def video_dynamic(num):

    return Response(generate_frames(path_x = f'video/{str(num)}.mp4'), mimetype = 'multipart/x-mixed-replace; boundary=frame')

@app.route('/image')
def img():
    return Response(generate_img(path_x = 'img/3.jpg'), mimetype = 'multipart/x-mixed-replace; boundary=frame')

@app.route('/image/<int:num>')
def img_dynamic(num):
    return Response(generate_img(path_x = f'img/{str(num)}.jpg'), mimetype = 'multipart/x-mixed-replace; boundary=frame')

infected_data = {
    "infected": 0,
    "blackpod": 0,
    "frostypod": 0,
    "mirid": 0
}

@app.route('/get_gps', methods=['GET'])
def get_gps():
    return jsonify({'gps': gps_data})

@app.route('/get_infected_data', methods=['GET'])
def get_infected_data():
    return jsonify(infected_data)

@app.route('/move', methods=['POST'])
def move():
    direction = request.json.get('direction')
    print(f"Moving {direction}")
    # Here you would include code to send a command to the robot
    return jsonify({'status': 'success'})

@app.route('/set_mode', methods=['POST'])
def set_mode():
    mode = request.json.get('mode')
    print(f"Setting mode to {mode}")
    # Here you would include code to set the mode on the robot
    return jsonify({'status': 'success'})

@app.route('/camera_set_mode', methods=['POST'])
def camera_set_mode():
    mode = request.json.get('mode')
    print(f"Setting mode to {mode}")
    # Here you would include code to set the mode on the robot
    if mode == "stabilize":
        handle_command()
    elif mode == "manual":
        handle_command()
    return jsonify({'status': 'success'})

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=7000, debug=True)
