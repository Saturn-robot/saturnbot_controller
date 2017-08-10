# 写在前面
-----
本文是根据ros\_arduino\_bridge功能包的官方文档翻译修改而来，目的是为了便于广大网友更方便的学习使用该功能包。本文主要针对indigo-devel版本进行介绍，也会捎带介绍一下其他的版本。文中存在我的一些主观看法，如您有其他观点可以与我交流，我的email：<chaoyanglius@outlook.com>。这篇博文篇幅有些冗长，希望各位网友耐心阅读。

# 概述
------
使用这个功能包时，注意选择相应的版本，因为不同的版本之间有些并不能兼容。indigo-devel branch是针对ROS Indigo以及更高版本的，它使用的是Catkin编译系统，它对于ROS Hydro也是兼容的；而master 分支目前还在测试中，可能在某些板子或电脑上无法正常运行，我曾今使用过这个分支，但是出了很多问题，如串口（serial）就无法与主机正常交流、波特率的大小设置等问题。本文主要针对indigo-devel版本进行介绍，也会捎带介绍一下其他的版本。

这个ROS功能包集包括了Arduino库（ROSArduinoBridge）和一系列用来控制基于Arduino的ROS功能包，它使用的是标准的ROS消息和服务。这个功能包集并不依赖于ROS串口。
这个功能包集的功能包括：

- 可以直接支持ping声呐和Sharp红外线传感器
- 也可以从通用的模拟和数字信号的传感器读取数据
- 可以控制数字信号的输出
- 支持PWM伺服机
- 如果使用所要求的硬件的话，可以配置基本的控制
- 如果你的Arduino编程基础好的话，并且具有python基础的话，你就可以很自由的改动代码来满足你的硬件要求

这个功能包集包括一个兼容不同驱动的机器人的基本控制器（`base controller`），它可以接收ROS Twist类型的消息，可以发布里程数据到个人电脑。这个控制器（`base controller`）要求使用一个电机控制器和编码器来读取里程数据。目前indigo-devel版本的功能包集支持下面的控制器硬件：

- [Pololu VNH5019 dual motor controller shield](http://www.pololu.com/catalog/product/2502) or [Pololu MC33926 dual motor shield](http://www.pololu.com/catalog/product/2503).
- [Robogaia Mega Encoder shield](http://www.robogaia.com/two-axis-encoder-counter-mega-shield-version-2.html) or on-board wheel encoder counters.

**NOTE:** Robogaia Mega Encoder shield 仅适用于Arduino Mega，而板上编码计数器(ARDUINO_ENC_COUNTER)目前仅支持Arduino Uno，这些是官方文档上的说法。但是实际上只要你有一定的编程基础，你就可以随心所欲，想让它支持什么硬件就支持什么硬件。

# 关于ROS官方文档
-----------
在这个ROS wiki [ros\_arduino\_bridge](http://www.ros.org/wiki/ros_arduino_bridge)里可以找到一个标准的ROS风格的文档，但是貌似这个wiki里面的信息并不全面，应该说很不全面，不会有什么实质性的帮助。也许是因为在它的github仓库里有详细的说明，所以它才偷懒了吧！所以如果你想要了解关于该功能包的详细内容，请直接查看它的github仓库里的[README文件](https://github.com/hbrobotics/ros_arduino_bridge)。当然里面全是E文，所以如果你觉得有困难的话，可以委屈一下继续阅读我的拙文，当然这也是我写这篇博文的一个原因，我十分乐意为广大网友翻译英文资料，如有翻译不当的地方，欢迎各位批评指正，我的e-mail是<chaoyanglius@outlook.com>。

# 系统要求
---------
Python 串口：使用下面的命令可以在Ubuntu下安装python-serial功能包
```bash
$ sudo apt-get install python-serial
```
在非Ubuntu系统下，可以使用：
```bash
$ sudo pip install --upgrade pyserial
```
或者
```bash
$ sudo easy_install -U pyserial
```

这个功能包集可以在兼容Arduino的控制器上进行读取传感器上的数据，以及控制[PWM](https://en.wikipedia.org/wiki/Pulse-width_modulation)伺服机。但是你必须具备上面所说的被支持的硬件（电动机控制器和编码器），你才能使用这个功能包集中的基本控制器（`base controller`）。

**NOTE**:如果你没有这些硬件设备，你仍然有办法使用这个功能包，当然这需要一些相应的编程基础，具体方法请查看本文档结尾的NOTES部分。

要使用这个基本控制器（`base controller`），你还得为你的电动机控制器和编码器安装合适的库。对于上面提到过的Pololu VNH5019 Dual Motor Shield，它的相应的库可以在这里找到：

<https://github.com/pololu/Dual-VNH5019-Motor-Shield>

对于Pololu MC33926 Dual Motor Shield，则在这里可以找到对应的库：

<https://github.com/pololu/dual-mc33926-motor-shield>

Robogaia Mega Encoder shield的库可以在这里找到；

<http://www.robogaia.com/uploads/6/8/0/9/6809982/__megaencodercounter-1.3.tar.gz>

这些库应该被安装到你的标准Arduino sketchbook/libraries路径下面。最后，这个功能包集假设你使用的Arduino IDE的版本是1.0或以上，我使用的是1.6.8。

# 在你的Linux下准备串口
------------
你的Arduino很可能是通过接口`/dev/ttyACM#` 或者 `/dev/ttyUSB#`来连接你的Linux系统的。这里的`#`可以是0,1,2等数字，当然这根据你连接的设备数量而定。得到这个数字`#`最简单的方式就是拔掉所有的USB设备，然后插上你的Arduino，然后运行下面这个命令：

```bash
$ ls /dev/ttyACM*
```

或者

```bash
$ ls /dev/ttyUSB*
```

但愿上面这两个命令的有一个可以返回你想要的结果（例如/dev/ttyACM0），而另一个可能会返回这个错误"No such file or directory"。反正只要有一个命令可以得到结果就行。

然后你需要确保你对这个接口有访问的权限。假设你的Arduino连接的是`/dev/ttyACM0`，那么就运行下面这个命令：

```bash
$ ls -l /dev/ttyACM0
```

然后你就可以看到类似于下面的输出结果：

> crw-rw---- 1 root dialout 166, 0 2013-02-24 08:31 /dev/ttyACM0

我们注意到在上面的结果中，只有root和"dialout"组才有读写权限。因此，你需要成为`dialout`（关于dialout你可以参考[这里](http://bbs.csdn.net/topics/391944286)）组的一个成员。你仅仅需要马上加入这组中，并且它会对你之后插入的所有的USB设备都起作用。

你可以运行这个命令来加入dialout组：

```bash
$ sudo usermod -a -G dialout your_user_name
```

在这个命令中`your_user_name`就是你在Linux下登录的用户名。之后你可能需要注销登录，然后再重新登录进去，或者就简单的重启一下电脑。

当你执行完上面的操作之后，你可以运行下面的命令查看一下：

```bash
$ groups
```

然后如果你可以在列出的组中找到dialout，这就说明你已经加入到dialout中了。


# 安装ros\_arduino\_bridge功能包集
-----------

```bash
$ cd ~/catkin_workspace/src
$ git clone https://github.com/hbrobotics/ros_arduino_bridge.git
$ cd ~/catkin_workspace
$ catkin_make
```

这个被提供的Arduino库叫做ROSArduinoBridge（前面已经提到过），你可以在 ros\_arduino\_firmware功能包中找到。这个sketch（这是arduino对使用arduino IDE编辑出来的程序的专门叫法）是针对上面所要求的硬件，但是也可以被用于Arduino其他版本的板子（例如Uno），你可以不使用基础控制器（`base controller`），你可以在文档末尾的NOTES部分找到详细的说明。

你可以按照下面的步骤安装ROSArduinoBridge库：

```bash
$ cd SKETCHBOOK_PATH
```
这里`SKETCHBOOK_PATH`是指你的Arduino sketchbook路径。

```bash
$ cp -rp `rospack find ros_arduino_firmware`/src/libraries/ROSArduinoBridge ROSArduinoBridge
```

最后一个命令就是把ROSArduinoBridge sketch文件拷贝到你的sketchbook文件夹下面。如果你之前已经拷贝过这个sketch，那么下次拷贝的时候请先删除之前的sketch：

	cd SKETCHBOOK_PATH
	rm -R ROSArduinoBridge
	
下一节描述的是如何配置，编译并上传这个sketch。

# 加载ROSArduinoBridge的Sketch
-----------

- 如果你正在使用基础控制器功能包（`base controller`），那么你必须确保你已经在Arduino sketchbook/libraries文件夹里面安装了合适的电动机控制器和编码器的相关库。
- 启动ArduinoIDE(1.0及以上)，然后载入ROSArduinoBridgesketch。你可以这样找到他们File->Sketchbook->ROSArduinoBridge

**NOTE:**如果你还没有安装要求的基础控制硬件，但是你仍然想使用这些代码，那么你可以参考文档末尾的NOTES部分。

你可以通过去掉或保留相关的宏定义声明，来选择你想用的电动机控制器，同时你还要将其他电动机控制器的宏声明注释掉。默认选择的是 Pololu VNH5019 driver。

同样，你也可以用相同的方法选择你想使用的编码器。默认选择的是Pololu VNH5019 driver。

如果你想让你的基础控制器控制PWM伺服机的话，就把下面这两行：

```c
//#define USE_SERVOS
#undef USE_SERVOS
```
改成：
```c
#define USE_SERVOS
//#undef USE_SERVOS
```
之后你就必须修改头文件`servos.h`改变其中的N_SERVOS参数，另外你还需要根据你的伺服机所接的引脚修改相应的引脚数字。

一切都准备好后你就可以把这个sketch编译并上传到你的Arduino板子上上了。

# 固件程序命令
----------
这个ROSArduino库接受单字母命令来轮询传感器、控制伺服机、驱动机器人以及读取编码器。这些命令可以通过串口接口来发送给Arduino，比如Arduino的串口监视器。

**NOTE:**在尝试这些命令之前，把串口监视器的波特率设置为57600然后把行结束符设置为“Carriage return（回车）”和“Both NL & CR”（NL和CR）。这些设置选项在串口监视器右下角的两个下拉菜单中。如下图所示：

![serial_monitor](http://img.blog.csdn.net/20160508122310948)

命令列表可以在头文件commands.h中找到。目前的列表包括这些命令：

```c
#define ANALOG_READ    'a'
#define GET_BAUDRATE   'b'
#define PIN_MODE       'c'
#define DIGITAL_READ   'd'
#define READ_ENCODERS  'e'
#define MOTOR_SPEEDS   'm'
#define PING           'p'
#define RESET_ENCODERS 'r'
#define SERVO_WRITE    's'
#define SERVO_READ     't'
#define UPDATE_PID     'u'
#define DIGITAL_WRITE  'w'
#define ANALOG_WRITE   'x'
```

例如如果你想从模拟引脚pin3读取数据，你就可以使用这个命令：

> a 3

如果你想改变数字引脚pin3的模式为`OUTPUT`，就发送这个命令：

> c 3 1

要得到目前编码器的计数，你可以用这个命令：

> e

把机器人以每秒20个encoder ticks的速度向前移动，可以使用命令：

> m 20 20

**NOTE:**这些命令有些需要有对应的参数，比如`a 3`中`3`代表模拟引脚3，其实这些命令的参数很多都是不言自明的，如果有些命令比较隐晦（比如`m 20 20`中的两个参数究竟那个参数是左轮，那个是右轮并未说明），你可以通过查看源码了解相应参数的意义，这里也就不在一一赘述了。

PS:**我在此强调一遍，务必要进行[这一节](#固件程序命令)以及[下一节](#测试你的电线连接)的测试，否则你不能保证python-serial（后面会说到）与主机交流正常。**

# 测试你的电线连接
在一个差速轮式机器人上，电动机使用两个极性相反的接头来连接到电动机控制器上的。同样地，连接到编码器上的A/B端也是互斥的。然而，你仍然需要确保这样两个要求：

- 当给一个正向的速度，轮子向前移动；
- 而当轮子向前移动时，编码器计数增加。

把你的机器人放在一个小块儿上（**此处原文为：*placing your robot on blocks*，按我的理解是这样的：把机器人放在一个块儿上，使得它的轮子可以悬空，以便于测试**），你可以使用串口监视器来测试上述的两个要求。你可以使用`m`命令来启动电动机，使用`e`命令来获取编码器计数，以及使用`r`命令来将编码器重置为0。你要明白，在固件层，电动机的速度是按照编码器每秒的tick数来表示的，这样可以使编码器很容易解析（理解）它。假如轮子转速是每秒4000个编码器计数，那么命令`m 20 20`会以一个非常小的速度移动小车。（默认设置轮子仅仅移动2秒，你可以通过修改源码中的`AUTO_STOP_INTERVAL`改变这个值）。另外你还要知道，第一个参数是左轮的速度，另一个参数是右轮的速度。类似地，当使用`e`命令时，第一个被返回的数是左轮的编码器计数，第二个数是右轮的编码器计数。

最后，你可以先使用`r`命令把编码器计数清零，然后通过手动粗略地旋转轮子一整圈，再通过`e`命令来验证获取到的编码器计数是否为预期的结果。

# 配置ros\_arduino\_python节点

既然你的Arduino已经可以按要求运行sketch了，那么现在你就可以在你的PC上把它配置为ROS节点。你可以在通过编辑ros\_arduino\_python/config路径下的YAML文件定义的机器人尺寸，PID参数，以及传感器配置信息。所以首先进入到这个路径中：

```bash
$ roscd ros_arduino_python/config
```

你可以把这个提供的配置文件拷贝一份然后再修改（名字可以自定义）：

```bash
$ cp arduino_params.yaml my_arduino_params.yaml
```

现在你可以使用任何你所喜欢的编辑器来编辑这个配置文件的副本（my_arduino_params.yaml），打开之后应该是这个样子：

```YAML
# For a direct USB cable connection, the port name is typically
# /dev/ttyACM# where is # is a number such as 0, 1, 2, etc
# For a wireless connection like XBee, the port is typically
# /dev/ttyUSB# where # is a number such as 0, 1, 2, etc.

port: /dev/ttyACM0
baud: 57600
timeout: 0.1

rate: 50
sensorstate_rate: 10

use_base_controller: False
base_controller_rate: 10

# For a robot that uses base_footprint, change base_frame to base_footprint
base_frame: base_link

# === Robot drivetrain parameters
#wheel_diameter: 0.146
#wheel_track: 0.2969
#encoder_resolution: 8384 # from Pololu for 131:1 motors
#gear_reduction: 1.0
#motors_reversed: True

# === PID parameters
#Kp: 10
#Kd: 12
#Ki: 0
#Ko: 50
#accel_limit: 1.0

# === Sensor definitions.  Examples only - edit for your robot.
#     Sensor type can be one of the following:
#	  * Ping
#	  * GP2D12
#	  * Analog
#	  * Digital
#	  * PololuMotorCurrent
#	  * PhidgetsVoltage
#	  * PhidgetsCurrent (20 Amp, DC)


sensors: {
  #motor_current_left:   {pin: 0, type: PololuMotorCurrent, rate: 5},
  #motor_current_right:  {pin: 1, type: PololuMotorCurrent, rate: 5},
  #ir_front_center:      {pin: 2, type: GP2D12, rate: 10},
  #sonar_front_center:   {pin: 5, type: Ping, rate: 10},
  onboard_led:           {pin: 13, type: Digital, rate: 5, direction: output}
}

# Joint name and configuration is an example only
joints: {
    head_pan_joint: {pin: 3, init_position: 0, init_speed: 90, neutral: 90, min_position: -90, max_position: 90, invert: False, continuous: False},
    head_tilt_joint: {pin: 5, init_position: 0, init_speed: 90, neutral: 90, min_position: -90, max_position: 90, invert: False, continuous: False}
}
```
**NOTE:**不要在你的.yaml文件里使用tab，否则的话这个语法解析器会无法加载它。如果你要缩进的话必须用空格代替tab键。

**ALSO:**当定义你的传感器参数时，列表中的最后一个传感器的行尾（花括号后面）后没有逗号，但是其余的行尾必须有逗号。

现在让我们一起浏览一下这个文件的每一部分。

***接口设置***

这个接口要么是/dev/ttyACM0，要么是/dev/ttyUSB0，视情况而定，而且前面我们也已经提到过这个。

其中MegaRobogaiaPololu的Arudino sketch默认是以57600的波特率连接的。

***轮询速率***

**跳出ROS loop运行的速率主要就取决于这个速率参数（默认为50Hz），这个默认值足以满足大多数情况。在任何情况下，它应该至少与你的传感器的最大速率（下面我们会说到）一样快才行。**

*sensorstate_rate*决定了多久发布一个所有传感器的集合列表，每个传感器也以各自的速率在各自的主题上发布消息。

*use_base_controller*参数默认为False。你可以把它设置为True（假设你有文中所要求的硬件设施）。下面你将必须设置PID参数。


*base_controller_rate*参数决定了多久发布一次里程计读取信息。

***定义传感器***

*sensors*参数按照定义了传感器的名字和参数的字典。（你可以给你的传感器起任何你喜欢的名字，但是你必须要知道传感器的名字也会成为那个传感器所对应主题的名字。

这四个最重要的参数分别是：*pin*，*type*，*rate*，*direction*。*rate*定义了你每秒想轮询一次那个传感器多少次。例如，一个电压传感器可能每秒仅仅被轮询一次（或者仅仅每两秒一次），而一个声呐传感器可能每秒被轮询20次。*type*必须是列表中被列出来的（**注意区分大小写！**）。*direction*默认是*input*，所以如果你想将它定义为*output*，就将这个direction单独设为*output*。在上面的例子中，Arduino LED（pin13）将会以每秒两次的速率被点亮或熄灭。

***设置Drivetrain（驱动系统）和PID参数***

为了使用基础控制器（base controller），你必须去掉它的注释并且设置机器人的drivetrain和PID参数。示例中drivetrain参数是直径6英寸的驱动轮，距离11.5英寸。注意在ROS中使用米作为距离单位，所以一定要换算单位。示例中的编码器的分辨率（每转的tick数）规格来自于Pololu 131:1电动机。为你的电动机/编码器组合设置合理的数值。如果你的轮子可以向后转，那么就把* motors_reversed*设置为true，否则的话就设置为False。

PID参数比较难设置，你可以先按照示例中的值设置。但是在你第一次发送转弯命令的时候，一定要把你的机器人放在小块儿上。

# 启动ros_arduino_python节点

你现在看一下路径ros\_arduino\_python/launch下的文件arduino.launch。正如你所看到的，它指向一个名叫my_arduino_params.yaml的文件（之前创建的）。

```xml
<launch>
   <node name="arduino" pkg="ros_arduino_python" type="arduino_node.py" output="screen" clear_params="true">
      <rosparam file="$(find ros_arduino_python)/config/my_arduino_params.yaml" command="load" />
   </node>
</launch>
```

如果你的配置文件命名不同，那么就把这里的文件名参数(my_arduino_params.yaml)修改成你的配置文件的名字。

当你连接好Arduino并运行MegaRobogaiaPololu sketch时，使用下面的参数来启动 ros_arduino_python node节点：

```bash
$ roslaunch ros_arduino_python arduino.launch
```

**NOTE:**启动之前，千万不要打开Arduino IDE 的串口监视器，因为串口监视器会与该节点争夺串口资源。

你应该看到下面这样的输出：

> process[arduino-1]: started with pid [6098]
> Connecting to Arduino on port /dev/ttyUSB0 ...
> Connected at 57600
> Arduino is ready.
> [INFO] [WallTime: 1355498525.954491] Connected to Arduino on port / > dev/ttyUSB0 at 57600 baud
> [INFO] [WallTime: 1355498525.966825] motor\_current\_right {'rate': 5, > 'type': 'PololuMotorCurrent', 'pin': 1}
> [INFO]
> etc

如果你在你的机器人上装的有Ping声呐而且你也在配置文件里面定义了它们，它们就会闪一下来告诉你已经连接成功。

# 查看传感器数据

你可以回显传感器状态主题来查看所有传感器的数据：

```bash
$ rostopic echo /arduino/sensor_state
```

回显相应传感器的主题名字你就可以查看任何指定的传感器数据：

```bash
rostopic echo /arduino/sensor/sensor_name
```

例如，你有一个叫做
 ir\_front\_center的传感器，那么你可以运行下面的命令来查看相应的数据：

```bash
$ rostopic echo /arduino/sensor/ir_front_center
```

你也可以使用`rxqrt`来将这系列数据用图像的形式表示出来：

```bash
$ rxplot -p 60 /arduino/sensor/ir_front_center/range
```
# 发送Twist命令与查看里程计数据

ros\_arduino\_python功能包也定义了一些ROS服务，如下所示：

**digital\_set\_direction**-设置数字引脚的方向

```bash 
$ rosservice call /arduino/digital_set_direction pin direction
```

这里`pin`是引脚数字，`direction`为0代表输入，1代表输出。

**digital\_write**-给数字引脚发送高低电平（LOW为0，HIGH为1）

```bash
$ rosservice call /arduino/digital_write pin value
```

同样，这里`pin`是引脚数字，`value`是电平高低（LOW为0，HIGH为1）。

**servo\_write**-设置伺服机位置

```bash
$ rosservice call /arduino/servo_write id pos
```

这里id是伺服机的索引号（定义在Arduino sketch中的servos.h）并且`pos`是以弧度为单位（0-3.14），头文件servos.h中具体是这样写的：

```bash
byte servoInitPosition [N_SERVOS] = { 90, 90 }; // [0, 180] degrees
```
**servo_read **-读取伺服机的位置

```bash
$ rosservice call /arduino/servo_read id
```

# 使用板上编码器计数（仅支持ArduinoUno）

对于Arduino Uno，这个固件程序支持板上的编码器计数。这样的话，编码器就直接可以连接到Arduino板上，而不用借助任何额外的编码器设备（例如RoboGaia encoder shield）

对于速度，这个代码可以直接找到Atmega328p的接口和中断管脚，而这一功能的实现必须依赖Atmega328p（Arduino Uno）。（尽管它也可能兼容其他电路板或AVR单片机芯片） 

为了使用这个板上编码器计数，按照下面的要求来将编码器连接到Arduino Uno：

```
Left wheel encoder A output -- Arduino UNO pin 2
Left wheel encoder B output -- Arduino UNO pin 3

Right wheel encoder A output -- Arduino UNO pin A4
Right wheel encoder B output -- Arduino UNO pin A5
```
这时你需要在Arduino sketch中做相应的修改，通过下面的方式来取消使用RoboGaia encoder shield的代码，启用板上编码器的代码。

```c
/* The RoboGaia encoder shield */
//#define ROBOGAIA
/* Encoders directly attached to Arduino board */
#define ARDUINO_ENC_COUNTER
```

这时你就可以编译并上传到Arduino上了。

# 注意
-------
如果你没有文档中所要求的硬件来运行这个基础控制器，但是你仍然想使用其他兼容Arduino的控制器来读取传感器以及控制PWM伺服机，那么请按照下面的步骤：

首先，你需要编辑你的ROSArduinoBridge sketch，在文件的最前面，将这两行：

```c
#define USE_BASE
//#undef USE_BASE
```

改成这样:

```c
//#define USE_BASE
#undef USE_BASE
```

**NOTE**:你还需要将文件encoder_driver.ino中的这一行注释掉：

```c
#include "MegaEncoderCounter.h"
```
这时你就可以编译并上传你的代码了。

然后，编辑你的 my\_arduino\_params.yaml文件，确保参数`use_base_controller`被设为False。这样你就完成了。

# 可能遇到的错误

我在使用这个功能包的时候遇到了不少错误，因此我特意将这些错误及解决方法总结出来并与大家分享:[ros_arduino_bridge功能包集的使用错误及解决方法总结](http://blog.csdn.net/github_30605157/article/details/51621965)。
