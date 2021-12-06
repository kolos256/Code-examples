import serial
import matplotlib.pyplot as plt
import time

class serialprotocol1:
    def __init__(self, serial, byteorder='little'):
        self.serial = serial
        self.byteorder = byteorder

    def _header(self, addr, cmdcode):
        return bytearray(addr.to_bytes(1, self.byteorder) + cmdcode.to_bytes(1, self.byteorder))

    def exchange(self, addr, cmdcode, datatotx = bytearray(), bytestorx = 0):
        self.serial.write(self._header(addr, cmdcode) + datatotx)
        bytestorx += 1
        rxdata = self.serial.read(bytestorx)
        if len(rxdata) < bytestorx:
            raise Exception(f'wrong ansver: {rxdata}')
        if rxdata[0] != addr:
            raise Exception(f'wrong confirmation: {rxdata}')
        return rxdata

    def sendcmd(self, addr, cmdcode):
        self.exchange(addr, cmdcode)
    
    def sendcmdS16(self, addr, cmdcode, data16):
        self.exchange(addr, cmdcode, data16.to_bytes(2, self.byteorder, signed=True))

    def sendcmdS32(self,addr, cmdcode, data32):
        self.exchange(addr, cmdcode, data32.to_bytes(4, self.byteorder, signed=True))

    def readcmdS8(self, addr, cmdcode):
        return int.from_bytes( 
            self.exchange(addr, cmdcode, bytestorx=1)[1:2],
            self.byteorder,
            signed=True
        )

    def readcmdS16(self, addr, cmdcode):
        return int.from_bytes( 
            self.exchange(addr, cmdcode, bytestorx=2)[1:3],
            self.byteorder,
            signed=True
        )
        
    def readcmdS32(self, addr, cmdcode):
        return int.from_bytes( 
            self.exchange(addr, cmdcode, bytestorx=4)[1:5],
            self.byteorder,
            signed=True
        )


class System1:
    def __init__(self, serialprotocol):
        self.serialprotocol = serialprotocol

    def motorsinit(self):
        self.serialprotocol.sendcmd(1, 1)
    
    def setpwm(self, pwm):
        self.serialprotocol.sendcmdS16(1, 2, pwm)

    def setdbgpointer(self, memaddr):
        return self.serialprotocol.sendcmdS32(1, 4, memaddr)

    def getdbgpointer(self):
        return self.serialprotocol.readcmdS32(1, 5)

    def dbgpointertobuf1(self):
        self.serialprotocol.sendcmd(1, 6)

    def getdbgpointerdata(self):
        return self.serialprotocol.readcmdS8(1, 7)

    def get16dbgpointerdata(self):
        return self.serialprotocol.readcmdS16(1, 8)

    def incdbgpointer(self, inc):
        pointer = self.getdbgpointer()
        self.setdbgpointer(pointer+inc)
    
    def startproc1(self):
        self.serialprotocol.sendcmd(1, 10)

    def startproc2(self):
        self.serialprotocol.sendcmd(1, 11)
    
    def starttest1(self, plotdata=True):
        pwm1data=[]
        hdrv1data=[]
        pwm2data=[]
        hdrv2data=[]
        plottime=[]
        self.startproc1()
        time.sleep(2)
        self.dbgpointertobuf1()
        result = []
        for i in range(1024):
            data=self.getdbgpointerdata()
            self.incdbgpointer(1)
            result.append(data)
            pwm1data.append((data>>0)&1)
            hdrv1data.append((data>>1)&1)
            pwm2data.append((data>>2)&1)
            hdrv2data.append((data>>3)&1)
            plottime.append(i)
        if plotdata:
            fig, axs = plt.subplots(4)
            fig.suptitle('Test1')
            axs[0].plot(plottime, pwm1data, marker='o')
            axs[0].title.set_text('pwm1data')
            axs[1].plot(plottime, hdrv1data, marker='o')
            axs[1].title.set_text('hdrv1data')
            axs[2].plot(plottime, pwm2data, marker='o')
            axs[2].title.set_text('pwm2data')
            axs[3].plot(plottime, hdrv2data, marker='o')
            axs[3].title.set_text('hdrv2data')
            plt.show()
        return result

    def starttest2(self, plotdata=True):
        adcdata1=[]
        plottime=[]
        self.startproc2()
        time.sleep(2)
        self.dbgpointertobuf1()
        result = []
        for i in range(512):
            data=self.get16dbgpointerdata()
            result.append(data)
            self.incdbgpointer(2)
            adcdata1.append(data)
            plottime.append(i)
        if plotdata:
            plt.plot(plottime, adcdata1, marker='o')
            plt.title("Test2")
            plt.show()
        return result

if __name__ == '__main__':
    System = System1(serialprotocol1(serial.Serial('COM3', 927600, timeout=1)))
    System.starttest1()
    System.starttest2()
