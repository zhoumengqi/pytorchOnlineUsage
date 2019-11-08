import os,sys,flask
from skimage import transform
import torch
import torchvision
from torch.autograd import Variable
import torch.nn as nn
import torch.nn.functional as F
from torch.utils.data import Dataset, DataLoader
from torchvision import transforms#, utils
# import torch.optim as optim

import numpy as np
from PIL import Image
import glob,numpy
from skimage import io, transform, color
import torchvision.transforms as T
app = flask.Flask(__name__)
model = None
use_gpu = True
model_dir = './xxx.pth'#这里是训练好的pytorch模型地址


def normPRED(d):
    ma = torch.max(d)
    mi = torch.min(d)
    dn = (d-mi)/(ma-mi)
    return dn
def load_model():
    """Load the pre-trained model, you can use your model just as easily.
    """
    global model
    model = BASNet(3,1)#初始化网络结构,BASNet 是我自己的,需要替换成你自己的
    model.load_state_dict(torch.load(model_dir))#加载权重
    if torch.cuda.is_available():#使用GPU
        model.cuda()
    model.eval()
    if use_gpu:
        model.cuda()
@app.route("/predict", methods=["POST"])
def predict():
    # Initialize the data dictionary that will be returned from the view.
    data = {"success": False}

    # Ensure an image was properly uploaded to our endpoint.
    if flask.request.method == 'POST':
        print("接收到post请求")
        if flask.request.files.get("image"):
            print("接收到字节流")
            # Read the image in PIL format
            image = flask.request.files["image"].read()
            import io
            image = Image.open(io.BytesIO(image))#从字节流还原图片
            #print(image.size)
            image=numpy.array(image)
            print("正在转换图片")
            image = transform.resize(image,(256,256),mode='constant')#这里需要resize成你的模型需要的输入大小
            image[:,:,0] = (image[:,:,0]-0.485)/0.229
            image[:,:,1] = (image[:,:,1]-0.456)/0.224
            image[:,:,2] = (image[:,:,2]-0.406)/0.225
            #例如([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])就是Imagenet dataset的标准化系数（RGB三个通道对应三组系数）。数据集给出的均值和标准差系数，每个数据集都不同的，都是数据集提供方给出的
            image=image.transpose([2,0,1])#调换通道的顺序
            image = image[np.newaxis,:]#扩维
            #在这之前，整理一下你的输入数据，确保符合模型,变成 [1,w,h,c] 图片个数、宽、高、通道数
            image=torch.from_numpy(image).float()
            if torch.cuda.is_available():
                image = Variable(image.cuda())
            else:
                image = Variable(image)
            print("开始输入模型预测")
            d1,d2,d3,d4,d5,d6,d7,d8 = model(image)#根据你的模型输出结果，改成你自己的,如果你只返回图片，就用一个变量接受就好了
            
            
            #----------------------------------
            # normalization  这一段是对模型输出做后处理，如果不需要可以直接注释掉
            pred = d1[:,0,:,:]
            pred = normPRED(pred)
            #----------------------------------
            
            #--------接下来的两步是必须的
            pred = pred.squeeze()
            pred_np = pred.cpu().data.numpy()

            data['result']=pred_np.tolist()
            # Indicate that the request was a success.
            data["success"] = True
            del d2,d3,d4,d5,d6,d7,d8  #删除没用的变量，防止内存泄漏，这一步可有可无
            print("返回结果")
    # Return the data dictionary as a JSON response.
    return flask.jsonify(data)
if __name__ == '__main__':
    load_model()
    app.run(host='0.0.0.0')#在这里host=0.0.0.0 表示任何ip可以访问，还可以指定port端口，默认是5000
