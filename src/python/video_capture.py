import cv2
import numpy as np
import time
from typing import Tuple, Optional

class VideoCapture:
    def __init__(self, camera_id: int = 0, resolution: Tuple[int, int] = (640, 480)):
        """
        Initialize the video capture with specified camera and resolution.
        
        Args:
            camera_id: Camera device ID (0 for default camera)
            resolution: Tuple of (width, height) for video resolution
        """
        self.camera = cv2.VideoCapture(camera_id)
        self.camera.set(cv2.CAP_PROP_FRAME_WIDTH, resolution[0])
        self.camera.set(cv2.CAP_PROP_FRAME_HEIGHT, resolution[1])
        self.is_recording = False
        self.recording_start_time = 0
        self.frames = []
        
    def start_recording(self) -> bool:
        """Start recording video frames."""
        if not self.is_recording:
            self.is_recording = True
            self.recording_start_time = time.time()
            self.frames = []
            return True
        return False
    
    def stop_recording(self) -> Optional[np.ndarray]:
        """Stop recording and return the recorded frames as a numpy array."""
        if self.is_recording:
            self.is_recording = False
            return np.array(self.frames)
        return None
    
    def get_frame(self) -> Tuple[bool, Optional[np.ndarray]]:
        """
        Get the current frame from the camera.
        
        Returns:
            Tuple of (success, frame)
        """
        success, frame = self.camera.read()
        if success and self.is_recording:
            self.frames.append(frame)
        return success, frame
    
    def preprocess_frame(self, frame: np.ndarray) -> np.ndarray:
        """
        Preprocess the frame for tremor detection.
        
        Args:
            frame: Input frame from camera
            
        Returns:
            Preprocessed frame
        """
        # Convert to grayscale
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        
        # Apply Gaussian blur to reduce noise
        blurred = cv2.GaussianBlur(gray, (5, 5), 0)
        
        # Apply adaptive thresholding
        thresh = cv2.adaptiveThreshold(
            blurred, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C,
            cv2.THRESH_BINARY_INV, 11, 2
        )
        
        return thresh
    
    def release(self):
        """Release the camera resources."""
        self.camera.release()
        cv2.destroyAllWindows() 