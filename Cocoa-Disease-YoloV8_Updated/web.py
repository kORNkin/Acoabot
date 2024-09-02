from flask import Flask, Response, render_template, jsonify, request
#from video_dectect import vid_detection, img_detection
import cv2
from test import vid_detection, img_detection, webcam_detection

app = Flask(__name__)
app.config['SECRET_KEY'] = 'kornkin'

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

# Mock data for GPS and infected plants
gps_data = "8.772249, 99.923447"
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

if __name__ == "__main__":
    app.run(debug=True)