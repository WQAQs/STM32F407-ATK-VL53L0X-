实验器材:
	探索者STM32F407开发板
	
实验目的:
	学习ATK-VL53L0X激光测距模块的使用,实现距离校准、普通测量模式和中断测量模式的功能,距离校准数据会存储在24C02上.
	
硬件资源:
	1,DS0、DS1(连接在PF9\PF10)
	2,串口1(波特率:115200,PA9/PA10连接在板载USB转串口芯片CH340上面)
	3,ALIENTEK 2.8/3.5/4.3/7寸TFTLCD模块(通过FSMC驱动,FSMC_NE4接LCD片选/A10接RS) 
	4,按键KEY0(PE4)/KEY1(PE3)/KEY_UP(PA0)
	5,24C02(IIC连接在PB6/PB7上面) 
	6,ATK-VL53L0X模块一个（连接在PB11(SCL),PB10(SDA),PC0（XSH),PF6(INT)上面），也可直接插在板上ATK-MODULE的接口。
	
实验现象:
    本实验功能简介：本实验用于测试ATK-VL53L0X激光测距模块，总共包含三大项测试：
	
	1，校准测试-通过KEY_UP按键进入此项测试。该功能实现对传感器测量误差的校准，进入
           测试后，会看到LCD屏幕提示，提示需要一个白色的目标（可以是白纸），且白色的目
           标需与模块保持在100mm的距离，确认目标和距离无误后，这时可按下LCD屏幕提示
           的 KEY1按键执行校准操作，若不想执行校准可按下KEY_UP返回主菜单。按下KEY1按
           键后，LCD 屏幕会显示开始校准，这时校准需要一定的时间，若校准成功，则会将校准
           的数据存入到24C02上。否则，将不会进行数据保存，校准过程结束后，会自动返回主
           菜单页面。
        2，普通测量测试-此测试是使用Single ranging（单次测量）工作模式，采用轮询方式，读取
           测量数据，通过KEY1按键进入此项测试。此项测试包含4个精度模式子项，其中包括：
           默认、高精度、长距离、高速。通过屏幕提示的KEY1按键操作，进行精度模式的切换。
           选择好精度模式后，按下KEY0按键可进入此模式测试。进入测试后，若之前成功校准过，
           会将保存在24C02的校准数据写进模块上，测量的数据通过轮询方式采集。数据采集结
           束后，数据显示在屏幕上、同时也打印在串口调试助手上。单击按下KEY_UP，可返回精
           度模式选项页面，双击按下，则返回主菜单页面。
        3，中断测量测试-此测试是使用Continuous ranging（连续测量）工作模式，采用中断方式，
           读取测量数据。通过KEY0按键进入此项测试。此项测试包含4个精度模式子项，其中包
           括：默认、高精度、长距离、高速。通过LCD屏幕提示的KEY1按键操作，进行精度模式
           的切换。选择好精度模式后，按下KEY0按键可进入此模式测试。进入测试后，若之前成
           功校准过，会将保存在24C02的校准数据写进模块上。在LCD屏幕上，会显示距离上下
           限阀值，当测出的距离在上下限阀值范围之间，模块不触发测距工作，中断没有输出，若
           在阀值外，则启动测距工作，测量结束会触发一次中断，通过中断读取测量数据，距离数
           据会显示在屏幕上，同时也会打印在串口调试助手上。单击按下KEY_UP，可返回精度模
           式选择页面，双击按下，则返回到主菜单页面。
	
         （关于工作模式和精度的介绍，请看ATK-VL53L0X激光传感器模块用户手册_V1.0）

  
注意事项:
	1,4.3寸和7寸屏需要比较大电流,USB供电可能不足,请用外部电源适配器(推荐外接12V 1A电源).
	2,本例程在LCD_Init函数里面(在ILI93xx.c),用到了printf,如果不初始化串口1,将导致液晶无法显示!!  
    3,校准过程中可能会受环境温度、红外线以及光线的影响，导致校准数据不准。
    4,由于模块是属于光学器件，传感器表层的不洁净，会容易导致测量数据不佳。所以模块在使用前，保持传感器表层的清洁度，工作时请勿用手去触摸，以免导致模块工作不正常。
	
-------------------------------------------------------------------------------------------
◆其他重要连接：
  开发板光盘资料下载地址（视频+文档+源码等）：http://www.openedv.com/posts/list/13912.htm
  添加微信公众平台方法：http://www.openedv.com/posts/list/45157.htm
  

◆友情提示：如果您想以后及时免费的收到正点原子所有开发板资料更新增加通知，请关注微信公众平台：

 2种添加方法：（动动手提升您的学习效率，惊喜不断哦）
（1）打开微信->添加朋友->公众号->输入“正点原子”->点击关注
（2）打开微信->添加朋友->输入“alientek_stm32"->点击关注

 具体微信添加方法，请参考帖子：http://www.openedv.com/posts/list/45157.htm

 

			正点原子@ALIENTEK 
			广州市星翼电子科技有限公司
			电话：020-38271790
			传真：020-36773971
			购买：http://eboard.taobao.com
			公司网站：www.alientek.com
			技术论坛：www.openedv.com
					
					
					
					
					
					
					
					
					
					
					
					