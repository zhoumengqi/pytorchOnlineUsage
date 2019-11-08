## 效果是 c++传过来一张图片，返回一张模型预测好的结果图
这个工程目的是实现c++可以调用远程服务器模型进行预测的功能

需求是，本地c++需要调用模型预测结果，模型是pytorch写的，放在服务端，所以需要通过网络进行传输

最终实现的框架是，使用python 的falsk 将模型一直加载进服务器GPU,对外发布为服务，

本地c++ 调用py脚本，然后发送数据，接收数据，(这里调用py脚本生成的exe,并不是直接调py,这样做的好处是可以脱离py环境运行)

**使用步骤**

1.开启服务器端服务 CUDA_VISIBLE_DEVICES=3 python test-service.py

2.pyinstaller -D simple_request.py 生成exe

3.自己写c++代码调用exe,详细介绍如下

本文件夹包含了两部分，客户端代码，和服务端代码

下面分别介绍

**客户端代码**

	包含两部分

	c++代码  --传图片及参数使用举例
	python部分 --是c++代码后台要调用的
		注意:这里并不是直接调用python代码，而是调用python转成的exe文件，目的是为了脱离python环境运行
			如果有修改python代码的需要，请重新生成exe,才能生效
			步骤如下
				1.在python环境下安装 pip install pyinstaller (若已安装，请忽略)
				2.进入py文件目录,运行 pyinstaller -D simple_request.py  生成build 和 dist文件夹
				  dist文件夹里面有调用的exe


**服务端代码**

	在BASNet-master 里面
		开启预测服务步骤
		CUDA_VISIBLE_DEVICES=3 python	test-service.py
		解释 CUDA_VISIBLE_DEVICES=3 指定GPU,防止多人使用冲突，
		开启后，默认占用的端口号是5000，可在test-service.py 修改
		若要别的电脑访问，需要服务器开启5000端口
若有不懂或者调试不出来，可私信我

![关注我](./image/AI浅析二维码.jpg)
