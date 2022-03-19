# few-solutions-of-reverse-data-hiding

这大概、可能就会是我的毕业论文中的实验代码。
This could probably be my lab code for the graduation thesis.

## 一点背景介绍(introduction)

可逆信息隐藏，要求是将一段信息隐藏到载体（就是图片）里，但在复原时必须做到同时恢复信息与载体。
Reverse data hiding, it requires to hide/embed some bits into some carriers(in my topic, only pics), 
but you have to gain the bits and carrier both completely while recovery

在现有的研究内(截至到2022.3)，虽然在研究内容里区分了空间域、频率域、压缩域、加密域或者是其他什么域，但具体到解决方案并没有太多的创新。
Within the existing research(By 2022.3), although they distinguished this topic like spatial domain, frequency domain, 
compression domain, encryption domain or other domains. There is not much innovation in the solution.

## 空间域解决方案(solution to spatial domain)
1.无损压缩(lossless compression)
2.直方图位移(histogram shifting)
3.差分扩展(difference expansion)

其中，2和3是将要在论文中分析而实现的代码方案，也是空间域目前的主要研究方向。
Among these, 2 and 3 are the code solutions which will be analysed in my shit thesis, and they are also the main reserch directions in spatial domain.

不要问为什么不做别的方案，和不研究别的域，问就是摆烂:(
DO NOT ASK me why there is still solutions left, or othor domains left. Just go sleep:(

等论文完成时，这里会补上相应的理论说明，但我相信聪明的网友肯定早就已经百度一下了。
When the paper is finished, the corresponding theoretical explanation will be added here, but I believe smart netizens must have google it for a long time.
