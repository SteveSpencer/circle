Change Log
==========

> If you read this file in an editor you should switch line wrapping on.

This file contains the release notes (the major changes) since Circle Step30 for quick access. For earlier releases please checkout the respective git tag and look into README.md. More info is attached to the release tags (git cat-file tag StepNN) and is available in the git commit log.

Release 41.1
------------

This is a hotfix release, which fixes the bootloader. It did not work with the recommended firmware on Raspberry Pi 1 and Zero any more.

The 41st Step
-------------

2020-02-04

With this release Circle supports nearly all features on the Raspberry Pi 4, which are known from earlier models. Only OpenGL ES / OpenVG / EGL and the I2C slave support are not available.

On Raspberry Pi 4 models with over 1 GB SDRAM Circle provides a separate *HEAP_HIGH* memory region now. You can use it to dynamically allocate memory blocks with `new` and `malloc()` as known from the low heap. Both heaps can be configured to form a larger unified heap using a system option. Please read the file *doc/new-operator.txt* for details about using and configuring the heaps.

The new *sample/39-umsdplugging* demonstrates how to use `CUSBHCIDevice::RescanDevices()` and `CDevice::RemoveDevice()` to be able to attach USB mass-storage devices (e.g. USB flash drives) and to remove them again on application request without rebooting the system.

Some support for the QEMU semihosting interface has been added, like the possibility to exit QEMU on `halt()`, optionally with a specific exit status (`set_qemu_exit_status()`). This may be used to automate tests. There is a new class `CQEMUHostFile`, which allows reading and writing files (including stdin / stdout) on the host system, where QEMU is running on. See the directory *addon/qemu/*, the new sample in *addon/qemu/hostlogdemo/* and the file file *doc/qemu.txt* for info on using QEMU with Circle with the semihosting API.

The Circle build system checks dependencies of source files with header files now and automatically rebuilds the required object files. You will not need to clean the whole project after editing a header file any more. You have to append the (last) line `-include $(DEPS)` to an existing Makefile to enable this feature in your project. The dependencies check may be globally disabled, by defining `CHECK_DEPS = 0` in Config.mk.

If you want to modify a system option in *include/circle/sysconfig.h*, explicitly changing this file is not required any more, which makes it easier to include Circle as a git submodule. All system options can be defined in *Config.mk* this way:


```
DEFINE += -DARM_ALLOW_MULTI_CORE
DEFINE += -DNO_CALIBRATE_DELAY
```

Finally a project file for the Geany IDE is provided in *tools/template/*. The recommended toolchain is based on GNU C 9.2.1 now. As announced the Bluetooth support has been removed for legal reasons.

Release 40.1
------------

2019-12-15

This intermediate release mostly adds FIQ support for AArch64 on the Raspberry Pi 4. This has been implemented using an additional ARM stub file *armstub8-rpi4.bin*, which is loaded by the firmware. The configuration file *config.txt*, provided in *boot/*, is required in any case for AArch64 operation now. Please read the file *boot/README* for detailed information on preparing a SD card with the firmware for using it with Circle applications.

*boot/Makefile* downloads a specific firmware revision now. With continuous Raspberry Pi firmware development there may occur compatibility problems between a Circle release and a new firmware revision, which may lead to confusion, if they are not detected and fixed immediately. To overcome this situation a specific tested firmware revision is downloaded by default now. This firmware revision reference will be updated with each new Circle release.

Further news in this release are:

* User timer (class CUserTimer) supported on Raspberry Pi 4
* LittlevGL support in *addon/littlevgl/* updated to v6.1.1
* FatFs support in *addon/fatfs/* updated to R0.14

Finally the system option *SCREEN_HEADLESS* has been added to *include/circle/sysconfig.h*. It can be defined to allow headless operation of sample programs especially on the Raspberry Pi 4, which would otherwise fail to start without notice, if there is not a display attached (see the end of next section for a description of the problem).

The 40th Step
-------------

2019-09-02

This Circle release adds support for the Raspberry Pi 4 Model B. All features supported by previous releases on Raspberry Pi 1-3 should work on Raspberry Pi 4 except:

* Accelerated graphics support
* FIQ on AArch64
* User timer (class CUserTimer)
* I2C slave (class CI2CSlave)
* USB RescanDevices() and RemoveDevice()

The Raspberry Pi 4 provides a number of new features. Not all of these are supported yet. Support for the following features is planned to be added later:

* USB 3.0 hubs
* High memory (over 1 GByte)
* additional peripherals (SPI, I2C, UART)

The Raspberry Pi 4 has a new xHCI USB host controller and a non-USB Gigabit Ethernet controller. This requires slight modifications to existing applications. The generic USB host controller class is named `CUSBHCIDevice` and has to be included from `<circle/usb/usbhcidevice.h>` now. The Ethernet controller driver is automatically loaded, if the TCP/IP network library is used, but has to be loaded manually otherwise (see *sample/06-ethernet*).

Please note that there is a different behavior regarding headless operation on the Raspberry Pi 4 compared to earlier models, where the frame buffer initialization succeeds, even if there is no display connected. On the Raspberry Pi 4 there must be a display connected, the initialization of the class CBcmFrameBuffer (and following of the class CScreenDevice) will fail otherwise. Most of the Circle samples are not aware of this and may fail to run without a connected display. You have to modify CKernel::Initialize() for headless operation so that `m_Screen.Initialize()` is not called. `m_Serial` (or maybe class `CNullDevice`) can be used as logging target in this case.

Release 39.2
------------

2019-07-08

This is another intermediate release, which collects the recent changes to the project as a basis for the planned Raspberry Pi 4 support in Circle 40.

News in this release are:

* LittlevGL embedded GUI library (by Gabor Kiss-Vamosi) supported (in addon/littlevgl/).

* The SD card access (EMMC) performance has been remarkable improved.

* USB mass-storage devices (e.g. flash drives) can be removed from the running system now. The application has to call `CDevice::RemoveDevice()` for the device object of the USB mass-storage device, which will be removed afterwards. The file system has to be unmounted before by the application. If FatFs (in addon/fatfs/) is used, the device removal is announced by calling `disk_ioctl (pdrv, CTRL_EJECT, 0)`, where pdrv is the physical drive number (e.g. 1 for the first USB mass-storage device).

* Network initialization can be done in background now to speed-up system initialization. If `CNetSubSystem::Initialize()` is called with the parameter FALSE, it returns quickly without waiting for the Ethernet link to come up and for DHCP to be bound. The network must not be accessed, before `CNetSubSystem::IsRunning()` returns TRUE. This has to be ensured by the application.

Please note that the rudimentary Bluetooth support is deprecated now. There are legal reasons, why it cannot be developed further and because it is currently of very limited use, it will probably be removed soon.

Release 39.1
------------

2019-03-20

This intermediate release comes with a new in-memory-update (chain boot) function and some improvements in detail. Furthermore it is the basis for AArch64 support in the [circle-stdlib](https://github.com/smuehlst/circle-stdlib) project.

The in-memory-update function allows starting a new (Circle-based) kernel image without writing it out to the SD card. This is used to implement a HTTP- and TFTP-based bootloader with Web front-end in *sample/38-bootloader*. Starting larger kernel images with it is much quicker, compared with the serial bootloader. See the *README* file in this directory for details.

The ARM Generic Timer is supported now on Raspberry Pi 2 and 3 as a replacement for the BCM2835 System Timer. This should improve performance and allows using QEMU with AArch64. The system option *USE_PHYSICAL_COUNTER* is enabled by default now.

The relatively rare resource of DMA channels is assigned dynamically now. Lite DMA channels are supported. This allows to use DMA for scrolling the screen much quicker.

There is a new make target "install". If you define `SDCARD = /path` with the full path of your SD card mount point in *Config.mk*, the built kernel image can be copied directly to the SD card. There is a second optional configuration file *Config2.mk* now. Because some Circle-based projects overwrite the file *Config.mk* for configuration, you can set additional non-volatile configuration variables using this new file.

The 39th Step
-------------

2019-02-28

Circle supports the following accelerated graphics APIs now:

* OpenGL ES 1.1 and 2.0
* OpenVG 1.1
* EGL 1.4
* Dispmanx

This has been realized by (partially) porting the Raspberry Pi [userland libraries](https://github.com/raspberrypi/userland), which use the VC4 GPU to render the graphics. Please see the *addon/vc4/interface/* directory and the *README* file in this directory for more details. This support is limited to AArch32 and cannot be built on Raspbian.

The accelerated graphics support requires support for <math.h> functions. To provide this, the *libm.a* standard library is linked now, in case `STDLIB_SUPPORT = 1` is set (default). You need an appropriate toolchain so that it works. See the *Building* section for a link. You may use the <math.h> functions in your own applications too now.

Circle does not support normal USB hot-plugging, but there is a new feature, which allows to detect newly attached USB devices on application request. You can call CDWHCIDevice::ReScanDevices() now, while the application is running, to accomplish this.

The 38th Step
-------------

2019-01-04

The entire Circle project has been ported to the AArch64 architecture. Please see the *AArch64* section below for details! The 32-bit support is still available and will be maintained and developed further.

Moreover a driver for the BMP180 digital pressure sensor has been added to *addon/sensor/*.

Circle creates a short beautified build log now.

The 37th Step
-------------

2018-12-01

In this step the USB gamepad drivers have been totally revised. The PS3, PS4, Xbox 360 Wired, Xbox One and Nintendo Switch Pro gamepads are supported now, including LEDs, rumble and gyroscope. All these drivers support a unique class interface and mapping for the button and axis controls. This should simplify the development of gamepad applications and is used in the new *sample/37-showgamepad*. See the *README* file in this directory for details.

The touchpad of the PS4 gamepad can be used as a mouse device to control normal mouse applications. To implement this, a new unique mouse interface class has been added, which is used by the USB mouse driver too. Therefore existing mouse applications have to be updated as follows:

* Include <circle/input/mouse.h> instead of <circle/usb/usbmouse.h>
* Class of the mouse device is *CMouseDevice* instead of *CUSBMouseDevice*
* Name of the first mouse device is "mouse1" instead of "umouse1"

Finally with this release Circle supports the new Raspberry Pi 3 Model A+.

Release 36.1
------------

2018-11-14

This hotfix release fixes race conditions in the FIQ synchronization, which affected using the FIQ together with EnterCritical() or the class CSpinLock.

The 36th Step
-------------

2018-10-21

In this step the class *CUserTimer* has been added to Circle, which implements a fine grained user programmable interrupt timer. It can generate up to 500000 interrupts per second (if used with FIQ).

*CUserTimer* is used in the new *sample/36-softpwm* to implement Software PWM (Pulse Width Modulation), which can be used to control the brightness of a LED or a servo. See the *README* file in this directory for details.

This release of Circle comes with an updated version of the FatFs module (in *addon/fatfs*). Furthermore there have been fixes to the touchscreen driver and the bootloader tools. Finally there are the new system options *SAVE_VFP_REGS_ON_IRQ* and *SAVE_VFP_REGS_ON_FIQ* in *include/circle/sysconfig.h*, which can be enabled if your application uses floating point registers in interrupt handlers.

You may be interested in the [Alpha GDB server](https://github.com/farjump/Alpha_Raspberry_Pi_Circle) by Farjump, which can be used to source-level debug single-core Circle applications with the GNU debugger (GDB).

Release 35.1
------------

2018-05-15

This intermediate release is done to allow a new release of the [circle-stdlib project](https://github.com/smuehlst/circle-stdlib), which provides C and C++ standard library support for Circle and which has been extended with SSL/TLS support by porting [mbed TLS](https://tls.mbed.org/).

Another new feature in this release is a driver for LCD dot-matrix displays with HD44780 controller in the *addon/display/* subdirectory.

The 35th Step
-------------

2018-04-26

In this step a client for the MQTT IoT protocol has been added to Circle. It is demonstrated in *sample/35-mqttclient*. See the *README* file in this directory for details.

Furthermore a serial bootloader is available now and can be started directly from the Circle build environment. The well-known bootloader by David Welch is used here. See the file *doc/eclipse-support.txt* for instructions on using the bootloader with or without the Eclipse IDE.

There is an incompatible change in behaviour of the CSocket API. Previously when a TCP connection received a FIN (disconnect) and the application called CSocket::Receive(), it did not get an error so far. Because of this, the disconnect has not been detectable. Now an error will be returned in this case. This may cause problems with existing user applications using TCP. You may have to check this.

Release 34.1
------------

2018-03-28

This intermediate release was necessary, because the new Raspberry Pi 3 Model B+ became available. Circle supports it now.

Beside this a driver for the Auxiliary SPI master (SPI1) has been added. The sample/23-spisimple has been updated to support this too. The class CDMAChannel supports the 2D mode now. Furthermore there are some minor improvements in the network subsystem.

The 34th Step
-------------

2018-01-16

In this step a VCHIQ audio service driver and a VCHIQ interface driver have been added to Circle in the *addon/vc4/* subdirectory. They have been ported from Linux and allow to output sound via the HDMI connector. You need a HDMI display which is capable of output sound, to use this.

There is a new Linux kernel driver emulation library in *addon/linux/* which allows to use the VCHIQ interface driver from Linux with only a few changes.

The new *sample/34-sounddevices* can be used to demonstrate the HDMI sound and integrates different sound devices (PWM, I2S, VCHIQ). See the *README* file in this directory for details.

There is a new system option *USE_PHYSICAL_COUNTER* in *include/circle/sysconfig.h* which enables the use of the CPU internal physical counter on the Raspberry Pi 2 and 3, which should improve the system performance, especially if the scheduler is used. It cannot be used with QEMU and that's why it is not enabled by default.

The 33rd Step
-------------

2017-11-18

The main expenditure in this step was spent to prepare Circle for external projects which allow to develop applications which are using C and C++ standard library features. Please see the file *doc/stdlib-support.txt* for details.

Furthermore a syslog client has been added, which allows to send log messages to a syslog server. This is demonstrated in *sample/33-syslog*. See the *README* file in this directory for details.

Circle applications will be linked using the standard library libgcc.a by default now. This library should come with your toolchain. Only if you have problems with that, you can fall back to the previous handling using the setting *STDLIB_SUPPORT=0* in the files *Config.mk* or *Rules.mk*.

If you are using an USB mouse with the mouse cursor in your application, you have to add a call to *CUSBMouseDevice::UpdateCursor()* in your application's main loop so that that cursor can be updated. Please see *sample/10-usbmouse* for an example.

In a few cases it may be important, that the Circle type *boolean* is now equivalent to the C++ type *bool*, which only takes one byte. *boolean* was previously four bytes long.

Release 32.1
------------

2017-10-29

This hotfix release fixes possible SPI DMA errors in the class CSPIMasterDMA.

The 32nd Step
-------------

2017-10-28

In this step a console class has been added to Circle, which allows the easy use of an USB keyboard and a screen together as one device. The console class provides a line editor mode and a raw mode and is demonstrated in *sample/32-i2cshell*, a command line tool for interactive communication with I2C devices. If you are often experimenting with I2C devices, this may be a tool for you. See the *README* file in this directory for details.

Furthermore the Circle USB HCI driver provides an improved compatibility for low-/full-speed USB devices (e.g. keyboards, some did not work properly). Because this update changes the overall system timing, it is not enabled by default to be compatible with existing applications. You should enable the system option *USE_USB_SOF_INTR*, if the improved compatibility is important for your application.

The system options can be found in the file *include/circle/sysconfig.h*. This file has been completely revised and each option is documented now.

Finally there are some improvements in the SPI0 master drivers (polling and DMA) included in this step.

The 31st Step
-------------

2017-08-26

In this step HTTP client support has been added to Circle and is demonstrated in *sample/31-webclient*. See the *README* file in this directory for details.

Furthermore the I2S sound device is supported now with up to 192 KHz sample rate (24-bit I2S audio, tested with PCM5102A DAC). *sample/29-miniorgan* has been updated to be able to use this feature.

The addon/ subdirectory contains a port of the FatFs file system driver (by ChaN), drivers for the HC-SR04 and MPU-6050 sensor devices and special samples to be used with QEMU now.

Finally there are some fixes in the TCP protocol handler included in this step.

Release 30.3
------------

2017-08-01

With the firmware from 2017-07-28 on the ActLED didn't worked on RPi 3 any more and the official touchscreen on RPi 2 and 3. This has been fixed.

Release 30.2
------------

2017-07-30

Multicore support didn't work with recent firmware versions. This has been fixed.

Release 30.1
------------

2017-05-03

This hotfix release fixes an invalid section alignment with -O0. This caused a crash while calling the constructors of static objects in sysinit().

The 30th Step
-------------

2017-04-11

In this step FIQ (fast interrupt) support has been added to Circle. This is used to implement the class CGPIOPinFIQ, which allows fast interrupt-driven event capture from a GPIO pin and is demonstrated in *sample/30-gpiofiq*. See the *README* file in this directory for details.

FIQ support is also used in the class CSerialDevice, which allows an interrupt-driven access to the UART0 device. *sample/29-miniorgan* has been updated to use the UART0 device (at option) as a serial MIDI interface.

Finally in this step QEMU support has been added. See the file *doc/qemu.txt* for details!