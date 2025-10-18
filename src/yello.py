from inference_sdk import InferenceHTTPClient
import cv2

# Initialize Roboflow client
CLIENT = InferenceHTTPClient(
    api_url="https://detect.roboflow.com",
    api_key="bXxNgm3iZYPjAApRc8qO"   # replace with your actual API key
)

MODEL_ID = "construction-waste-segregation-bw374/9"   # replace with your model ID

# Open webcam
cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # Run inference via Roboflow API
    result = CLIENT.infer(frame, model_id=MODEL_ID)

    # Draw detections on the frame
    for pred in result["predictions"]:
        x, y, w, h = pred["x"], pred["y"], pred["width"], pred["height"]
        class_name = pred["class"]
        conf = pred["confidence"]

        # Convert center x,y to top-left and bottom-right
        x1 = int(x - w / 2)
        y1 = int(y - h / 2)
        x2 = int(x + w / 2)
        y2 = int(y + h / 2)

        # Draw bounding box and label
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
        cv2.putText(frame, f"{class_name} {conf:.2f}",
                    (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX,
                    0.6, (0, 255, 0), 2)
        print(f"{class_name}: {x},{y}; {w},{h}")

    # Show the frame
    cv2.imshow("Roboflow YOLOv8 Detection", frame)

    # Quit on 'q'
    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

cap.release()
cv2.destroyAllWindows()
