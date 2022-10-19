import time
from websocket import create_connection
from websocket import ABNF
from test_pb2 import Pose

ws = create_connection("ws://127.0.0.1:9900/api/ws/")

pose = Pose()
pose.position.x = 1;
pose.position.y = 2;
pose.orientation.qx = 3;
pose.orientation.qy = 4;
pose.orientation.qz = 5;
print(pose)

ws.send(pose.SerializeToString() , ABNF.OPCODE_BINARY)

time.sleep(1)

