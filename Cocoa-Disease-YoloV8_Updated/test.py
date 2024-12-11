import cv2

from ultralytics import YOLO

# Load the YOLOv8 model
model = YOLO("best2.pt")

def webcam_detection():
    cap = cv2.VideoCapture(0)

    # Loop through the video frames
    while cap.isOpened():
        # Read a frame from the video
        success, frame = cap.read()

        if success:
            # Run YOLOv8 inference on the frame
            results = model(frame, conf=0.5)
            #results = model.predict(vid_path, stream=True, show=True, conf=0.8)

            # Visualize the results on the frame
            annotated_frame = results[0].plot()

            yield annotated_frame

def vid_detection(vid_path):
    cap = cv2.VideoCapture(vid_path)

    # Loop through the video frames
    while cap.isOpened():
        # Read a frame from the video
        success, frame = cap.read()

        if success:
            # Run YOLOv8 inference on the frame
            results = model(frame, conf=0.5)
            #results = model.predict(vid_path, stream=True, show=True, conf=0.8)

            # Visualize the results on the frame
            annotated_frame = results[0].plot()

            yield annotated_frame

def img_detection(img_path):
    img = cv2.imread(img_path)
    results = model(img, conf=0.5)
    #results = model.predict(img_path, stream=True, show=True, conf=0.8)
    annotated_frame = results[0].plot()
    yield annotated_frame
    
def pc_detection():
    HOST = '172.20.10.4'  # PC's IP address
    PORT = 9999

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
        #frame = cv2.imdecode(frame, cv2.IMREAD_COLOR)

        # Encode to JPEG and yield as a response
        _, jpeg = cv2.imencode('.jpg', frame)
        yield (b'--frame\r\n'
                b'Content-Type: image/jpeg\r\n\r\n' + jpeg.tobytes() + b'\r\n')
