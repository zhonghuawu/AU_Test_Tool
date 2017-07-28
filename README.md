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
Corresponding to simple factory design pattern, a TestFunc is considered as a product, 
TestFuncFactory create product namely concrete TestFunc. TestFuncProcessor is considered as 
customer, get product (TestFunc) from TestFuncFactory. If TestFuncProcessor get product 
successfully, it can execute the test function in main function.
Process procedure steps:
1. new TestFuncProcessor object tfProcessor in main function.
2. tfProcessor loadIniFile. new ini object about toolcfg.ini file and get buildpath and 
processtype. If failed, exit.
3. tfProcessor getTestFunc. get TestFunc instance from TestFuncFactory according to
buildpath and processtype. If failed, exit.
4. tfProcessor initEnv. get information required by TestFunc (TestFunc::getInfo) and load 
library and create context according context template (TestFunc::initEnv).
5. tfProcessor runTest (TestFunc::testFunc)
6. tfProcessor finalEnv (TestFunc::finalEnv).
7. delete tfProcessor. free TestFunc for freeing information required by TestFunc. free ini.
Notice:
TestFunc::getInfo must be executed previously than TestFunc::initEnv. Because 
TestFunc::initEnv needs some source information loaded by TestFunc::getInfo.
If many times loop testing expected about one function tested, the repetition testing will 
be executed in TestFunc::testFunc inside.
#### 2.2.2 TestFunc
Abstract class TestFunc only define some pure virtual methods. AUTestFunc and 
IAUTestFunc derived from TestFunc. They are abstract class too, but they will implement some 
function according to specific requirements.
#### 2.2.3 AUTestFunc
AUTestFunc derives from TestFunc and implements initEnv and finalEnv methods, because 
AU function load and release environment is fixed. AUTestFunc has a TmuContext_t context 
member attribute and has getter and setter function corresponding to context.
#### 2.2.4 IAUTestFunc
IAUTestFunc derives from TestFunc. It has an IAU_ContextHandle context member 
attribute and has setter and getter function corresponding to context. Because different IAU 
function load different library and release different environment, classes derived from
IAUTestFunc must implement initEnv and finalEnv member methods respectively.
#### 2.2.5 Concrete TestFunc
One class derived from AUTestFunc or IAUTestFunc is concrete class of TestFunc. This kind 
class is specific implementation to test function tested. Every concrete class contains the 
information requied by function tested respectively as its private member attributes. It must 
implement the virtual methods from parent class. 
Virtual method getInfo is implemented to get information required by function tested from 
formal parameter pIni.
Virtual method testFunc is implemented to run testing program. Function tested should be 
invoked in this method and should use informations got by getInfo as parameters.
Virtual destructor is used to release the information got by getInfo.
Above three virtual methods must be implemented explicitly by every concrete class.
Other two virtual methods initEnv and finalEnv are supposed to be overwrote if need be. For 
example, AUTestUpdate overwrites initEnv because TmuSetPropertyEx must be invoked
previously before TmuUpdateEx is being tested. AUTestUpdate::initEnv call parent methed 
AUTestFunc::initEnv to initialize AU environment and call AUTestSet::testFunc to set 
properties about TmuUpdateEx. IAUTestUpdate overwrites initEnv and finalEnv because 
IAU_Update and IAU_Duplicate load different dynamic link library (iau.dll and iaurelay.dll on 
windows platform and iau.so and iaurelay.so on linux platform).
#### 2.2.6 Inheritance relationship
The TestFunc inheritance relationship is illustrated as follow: <br>
![image](https://github.com/zhonghuawu/AU_Test_Tool/blob/master/image/inheritance.png)
#### 2.2.7 TestFuncFactory
TestFuncFactory is designed by class singleton pattern. It has only some static methods, 
but not has non-static methods. It can create TestFunc instance according to buildpath and 
processtype. TestFuncProcessor object will get TestFunc from TestFuncFactory through calling 
TestFuncProcessor::getTestFunc methods.
TestFuncFactory::createTestFunc uses buildpath firstly to detect whether iAU library exists 
in this build path. If exists, it will invoke TestFuncFactory::createIAUTestFunc to create 
IAUTestFunc instance. Otherwise, it will invoke Test FuncFactory::createAUTestFunc to create 
AUTestFunc instance.
#### 2.2.8 TestFuncProcessor
TestFuncProcessor encapsulate testing function process procedure. It get TestFunc object 
from TestFuncFactory, and executing test function orderly.
Testing result will record into log file (Test.log) and console using log function. As far as 
possible to simplify the logging way, it only provides four interface: LogInit, LogTime, 
LogResult, LogFinal. At the beginning of Process procedure, LogInit was called to initialize log 
environment. At the end, LogFinal was called to release log environment. LogTime is used to 
record current time. LogResult is overloaded to record pure string and the point of xml 
document. It is used frequently to record current event information. 
### 2.3 Log
Testing result will record into log file (Test.log default) and console using log function. As 
far as possible to simplify the logging way, only four interfaces were provided: LogInit, 
LogTime, LogResult, LogFinal. At the beginning of Process procedure, LogInit was called to 
initialize log environment. At the end, LogFinal was called to release log environment. LogTime 
is used to record current time. LogResult is overloaded to record pure string and the point of 
xml document. It is used frequently to record current event information. 

## 3 Source file structure
For the reason of using simple factory design pattern, there are many source file. One 
tested function is regard as one kind of product. One kind of product is designed as one class.
One class is written into a head file (declaration) and a source file (implementation). 
According to the thought of simple factory pattern, products information should be hidden. 
And the factory and the processor should be exposed. So, the folder AU contains source files 
about function tested from AU; the folder IAU contains source files about function tested from 
IAU; abstract class TestFunc will be settled in root directory, so do TestFuncFactory and 
TestFuncProcessor. 
The folder simpleini contains source files for sparing configure ini.
The folder tinyxml contains source files for sparing xml.
The folder TestLog contains source files for logging.

## 4 How to use
### 4.1 Usage
The usage of tool is very simple. The important and key things is configuring the toolcfg.ini 
file. All information required by tool are from configuration file. 
WIN32 platform:
Using VS2008 to compile project. Generating an executable file AU_Test_Tool.exe in path 
AU_Test_Tool/debug/.
Usage in windows console:
AU_Test_Tool.exe -h //help information
AU_Test_Tool.exe [-u] [-m] [-l] [cfgname.ini] // toolcfg.ini default if omitted.
LINUX platform:
Using make tool and g++ to compile project. Makefile is placed in
AU_Test_Tool/AU_Test_Tool/.
Using make command can compile project automatically. Using make -clean option can 
clean procedure file.
More option of make command, please look at Makefile. 
Generation an executable file AU_Test_Tool in the same path with Makefile.
Usage in Linux terminal:
./AU_Test_Tool -h //help information
./AU_Test_Tool [-u] [-m] [-l] [cfgname.ini] // toolcfg.ini default if omitted.

### 4.2 Configuration file 
Configuration file is very important for tool. All information required by tool is from it.
Default configuration file is toolcfg.ini. Specified configuration file can be used if the file 
name should be introduced to AU_Test_Tool.
The toolcfg.ini sample as [this](https://github.com/zhonghuawu/AU_Test_Tool/blob/master/AU_Test_Tool/toolcfg.ini).
Not all the sections will be used. Not all the sections will be required from different 
function tested. The whole configuration file will be loaded into memory when the tool start. 
The parts required by function tested will be used. Other will be ignored. So according to 
function tested, the sections required (or maybe called “used”) must be configured exactly.
[BuildPath] section provides buildPath of AU/iAU.
[ProcessType] section provides processName. ProcessName can be one of [Update | 
Duplicate | Set | Get | Rollback | NULL].
[Property] section is optional. If processName is Get, this section provides properties 
which should be got.
[AUConfiguration] section provides serverUrl and correlated information about serverUrl
for AU. This section also provides properties information if the processName is one of [Update 
| Duplicat | Set].
[IAUConfiguration] section provides serverUrl and correlated information about serverUrl
for IAU Update. 
[IAURelayConfiguration] section provides serverUrl and correlated information about 
serverUrl for IAU Duplicate.
[ItemsInfo] section provides Items Information for AU. If an item i should be ignored, you 
can just annotate Item[i].class. 
[ProductsInfo] section provides products information for IAU. If a product i should be 
ignored, you can just annotate Product[i].MainPro. if a sub product N from product i should be 
ignored, you can just annotate Product[i].SubProN. Tool reads products information required 
by iAU and transfer them to xml format.
Notice:
The configuration file support to Chinese characters. The path with Chinese characters 
configured by file can be recognized exactly. The configuration file can recognize octal(0nn), 
decimal(nn) and hexadecimal(0xnn) number accurately.

### 4.3 Log file (Test.log)
Log file name is Test.log default. The Test.log file will be opened by appending ways at the 
beginning of testing process procedure. Log will record beginning time and ending time and 
the result of function tested.
## 5 Advantages
Using simple factory design pattern to design the tool bring some advantages. Interfaceoriented programming bring high extensibility. It brings high isolation ability that One function 
tested is encapsulated into one class. They reduce codes coupling that the part of loading ini is 
separated from main features and some utils is wrote into util.cpp file.
### 5.1 High Extensibility
If some new function of AU wanted to be tested later, it can be added into the tool. Firstly, 
building a new class extended from AUTestFunc. Secondly, implementing some virtual 
methods. Lastly, informing TestFuncFactory to create this new TestFunc instance.
### 5.2 High isolation
One function tested is encapsulated into one class. There is no connection between two 
function tested. High isolation ability can be ensured. But the connection between 
AUTestUpdate, AUTestDuplicat and AUTestSet is existence. It will not influence the isolation 
ability in fact, because AUTestUpdate and AUTestDuplication contain one AUTestSet instance.

### 5.3 Lower code repeatability
The similarity of functions tested is getInfo. So, the ways of getInfo are wrote into 
AULoadIni.cpp and IAULoadIni.cpp. Some useful utils are wrote into util.cpp source file, such 
as mySleep function for sleeping current process.
## 6 Extension
### 6.1 Multiprocess
### 6.2 More categories function tested from AU or iAU
If others function of AU and iAU want to be tested, you should define a new class (named 
**Test***) that should be derived from AUTestFunc for function tested from AU or derived 
from IAUTestFunc for one from iAU. The new class should be similar with one of classes from 
third layer of inheritance relationship graph in section 2.2.6. And then, you should inform 
TestFuncFactory how to create its instance.
### 6.3 More categories library
If others library similar with AU or iAU want to be tested, you should define a new class
(named **TestFunc) that should be derived from TestFunc. The new class should be similar 
with AUTestFunc or IAUTestFunc from second layer of inheritance relationship graph in section 
2.2.6. You should create some new concrete class derived from new class (**TestFunc) and 
inform TestFuncFactory how to create its instance.
## 7 Notice
1. The Log path can be introduced into LogInit as formal parameter to alter the log path.
The default value is current path if nothing is introduced to LogInit. And the log file
name is Test.log default defined in TestLog class as static constant attribute. It can be 
changed.
2. The member method initEnv of AUTestUpdate and AUTestDuplicate will call 
AUTestSet::testFunc. These two class contain an AUTestSet instance respectively. It 
exists some invoking in the same level.
3. Tool reads items information (from [ItemsInfo] section) required by AU and products 
information (from [ProductsInfo] section) required by iAU from configuration file and 
uses ‘.’ and digitals appeared in key from above two section to discriminate different 
items or products. So, ‘.’ and digitals appeared in key from [ItemsInfo] and 
[ProductsInfo] section must be paid more attentions to.