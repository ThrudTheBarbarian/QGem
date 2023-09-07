# QGem

Implement a daemon view-service, a library to talk to the daemon, and a
'desktop' app that provides the user-visible workspace.

This is the upper half of the STxe system, as laid out 
in the block diagram below:
````
                                   +--------------+                   
                                   |              |                   
+--------------+          +--------+----+         |    + - - - - - - +
|    Client    |<---+     |             |         |                   
+--------------+    |     |             |         |    |             |
                    |     |             |  OpenGL |                   
                    |     |     Pi      |  Render |    |     68K     |
+--------------+    |     |             |         |       emulation   
|    Client    <----+---->|   running   |<- - - - + - >|             |
+--------------+    |     |             |         |       (Cyclone)   
                    |     |    GEMd     |         |    |             |
                    |     |             |         |                   
+--------------+    |     |             +---------+    |             |
|    Client    |<---+     |             |                             
+--------------+          +-------------+              + - - - - - - +
                                 ^                                    
                                 |                                    
                                 |                                    
                                 |                                    
                                 v                                    
+-------------+        +------------------+                           
|             |<------>|  SMI interface   |                           
|             |        +------------------+                           
|             |                                                       
|             |                                                       
|             |                                    +----------------+ 
|             |                               +--->| Expansion Slot | 
|             |        +------------------+   |    +----------------+ 
|    FPGA     |<------>|      RP2040      |<--+                       
|             |        +------------------+   |    +----------------+ 
|             |                               +--->| Expansion Slot | 
|             |                                    +----------------+ 
|             |                                                       
|             |                                                       
|             |        +--------------------------------------------+ 
|             |<------>|              XL/XE interface               | 
+-------------+        +--------------------------------------------+ 
````
# "XE Uber Alles"

The main purpose of the project is to extend the XL/XE bus via an FPGA that
can understand the bus protocol, parse out the video data accesses, and 
reconstruct the video onto an HDMI display, via a raspberry Pi.

To do this, the FPGA parses the XL/XE bus traffic, and sends packetised data
over to the Pi via its Secondary Memory Interface bus (SMI). This is a high
bandwidth, low latency connection with DMA to user-space.

The Pi then converts the data to a suitable texture format and blits it to the
screen, providing a realtime HD output without having to do anything to the
original computer.

# Hardware

- The FPGA has 8MB of PSRAM attached to it, and can map that into the XL memory map, on a page-by-page basis. 
- The FPGA also connects to an RP2040, which acts as a system controller, and also allows for easy update of the FPGA bitstream via the RP2040 virtual USB disk method
- The RP2040 also provides 5 (2 shown) slots which implement SPI and UART connections, each of which can DMA data directly into the memory map of the XL/XE.
- The RP2040 also controls an OLED display for infornational purposes, and some navigation buttons to help set things up.

# Software

Because the hardware can control what is in the XE/XL memory, it can load applications and data in a flash - so the software provides a 'desktop' like environment to easily allow programs to be run on the XL, mirroring the local filesystem on the Pi.

Given that this is an Atari, it seemed appropriate to make that user interface be GEM, which (finally, sorry) is what this software package is all about. The code here implements an "X-server"-like daemon called 'GEMd' that opens up a unix server socket (/tmp/gemd) that allows applications to connect and issue GEM commands to.

These commands are wrapped in a 'C' library that implements the original GEM function calls, serialises them, sends them over the socket to be acted on, and reports back any status. To the client applicaiton, it might as well be just running in a GEM environment.

With all of that in place, maybe the next step is to get Cyclone (an arm assembly 68k emulator) linked up to the GEM API and make it possible to run original PRG files from the Atari ST/TT. We'll see, this is pretty ambitious already :)
