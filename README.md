# AU_Test_Tool
AU Test Tool: automating testing AU and iAU function.

At the period of trendmicro intern, I finished this project by myself.

using c++ polymorphism and simple factory design pattern for enhancing project expansibility.

## 1 Objective
Provide guide for using the tool AU_Test_Tool. Provide design document in detail for enhancing tool functions and improving tool features. Descript design idea and implementation in detail.
## 2 Testing frame design
### 2.1 Purpose
Design a tool for testing AU and iAU uniformly. The tool integrates testing AU and testing iAU function. Because of the difference of AU interface and iAU interface, the tool need encapsulate the uniform interface.
### 2.2 Implementation
#### 2.2.1 Process procedure
Simple factory design pattern be used for the tool. The follow frame illustrates the design idea.
![image](https://github.com/zhonghuawu/AU_Test_Tool/blob/master/image/frame.png)
![image](https://github.com/zhonghuawu/AU_Test_Tool/blob/master/image/inheritance.png)