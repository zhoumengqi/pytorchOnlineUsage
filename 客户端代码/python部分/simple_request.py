import requests
import argparse
import numpy as np
import cv2
import os,sys,os
# Initialize the PyTorch REST API endpoint URL. 
PyTorch_REST_API_URL = 'http://192.168.17.190:5000/predict'#指定ip和端口
from PIL import Image

def predict_result(outputDir,image_path):
    # Initialize image path
    image = open(image_path, 'rb').read()

    payload = {'image': image}
    # Submit the request.
    r = requests.post(PyTorch_REST_API_URL, files=payload).json()#返回数据
    # Ensure the request was successful.
    if r['success']:
        img_np=np.array(r['result'])
        #print(img_np.shape)256x256
        #二进制数据恢复原图
        im = Image.fromarray(img_np*255).convert('RGB')    
        imo = im.resize((1020,992),resample=Image.BILINEAR)
    
        img = cv2.cvtColor(np.asarray(imo),cv2.COLOR_RGB2BGR)
        #----------这一段是我做的后续处理，可注释
        #去掉锯齿
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        median = cv2.medianBlur(gray,11)
        ret,binary = cv2.threshold(median, 127, 255, cv2.THRESH_BINARY)
        #-------------------------

        #保存图片，若下一行代码报错，则把 .decode('utf-8')放到(image_path)后面
        out_path=outputDir+"out_"+os.path.basename(image_path)
        print(out_path)
        cv2.imwrite(out_path,binary)
        print('保存成功')
        return 1
    # Otherwise, the request failed.
    else:
        print('Request failed')
        return 0
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Seg demo')
    parser.add_argument('--outputDir', type=str, help='test image file',default="./")
    parser.add_argument('--file', type=str, help='test image file',default="C:/Users/Administrator/Desktop/zmq_workspace/BASNet-master/newdata/images/63413.png")
    args = parser.parse_args()
    predict_result(args.outputDir,args.file)