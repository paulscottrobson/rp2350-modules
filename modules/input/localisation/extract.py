# *******************************************************************************************
# *******************************************************************************************
#
#      Name :      extract.py
#      Purpose :   Extract a scan code set from an XML file
#      Date :      5th August 2025
#      Author :    Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import os,sys,re
from scantousb import *

# *******************************************************************************************
#   
#                                       Extractor class
#
# *******************************************************************************************

class Extractor(object):
    def __init__(self,fileName):
        self.scanToUSB = ScanToUSB()
        self.usbToChars = {}
        self.inPK = False
        for l in [x.strip() for x in open(fileName).readlines()]:                   # Scan through file
            if l[:3].upper() == "<PK":                                              # Look for Physical key records
                self.processOpen(l)
                self.inPK = True
            if l[:5].upper() == "</PK>":
                self.inPK = False
            if self.inPK and l[:7].upper() == "<RESULT":                            # <RESULT entry
                if self.currentUSBCode is not None:                                 # process it if a key current
                    self.processResult(l)

    def processOpen(self,l):
        m = re.search("SC=\"([0-9A-Fa-f]+)\"",l.upper())                            # Find scan code
        assert m is not None,"Can't find scancode "+l
        self.currentUSBCode = self.scanToUSB.get(int(m.group(1),16))                # Convert to USB code
        if self.currentUSBCode is not None:                                         # Create if it exists
            if self.accept(self.currentUSBCode):                                    # If we want it
                assert self.currentUSBCode not in self.usbToChars
                self.usbToChars[self.currentUSBCode] = [ None, None ]
            else:                                                                   # We don't, some we do manually. This is ASCII standard
                self.currentUSBCode = None

    def accept(self,n):
        if n == 0x31:                                                               # Accept the 102 key coded as $31
            return True
        if n >= 0x28 and n <= 0x2B:                                                 # ENTER, ESC, BACKSPACE, TAB
            return False
        if n >= 0x39:                                                               # All the control characters, number keypad and odd things.
            return False
        return True

    def processResult(self,l):
        m = re.search('Text=\"(.*?)\"',l)                                           # Character to use
        if m is not None:
            keyChar = m.group(1)                                                    # Get character and remove XML detritus
            keyChar = keyChar.replace("&quot;",'"').replace("&amp;","&").replace("&lt;","<").replace("&gt;",">")
            m = re.search('With=\"(.*?)\"',l)                                       # Find modifier
            mode = "" if m is None else m.group(1).upper()
            if mode == "":                                                          # Put in shifted or unshifted slot.
                self.usbToChars[self.currentUSBCode][0] = keyChar
            if mode.find("VK_SHIFT") >= 0 and mode.find("VK_CAPITAL") < 0:
                self.usbToChars[self.currentUSBCode][1] = keyChar

    def dump(self):
        keys = [x for x in self.usbToChars.keys()]
        keys.sort()
        for k in keys:
            print("{0:2} : {1}".format(k,self.usbToChars[k]))

    def render(self):
        data = [ 0 ] * 128
        for k in self.usbToChars.keys():
            assert k >= 0 and k < 64
            asc = self.usbToChars[k]
            if asc[0] is not None:
                asc[1] = asc[0] if asc[1] is None else asc[1]
                asc = [ord(x) for x in asc]
                data[k] = asc[0]
                data[k+64] = asc[1]
        return data 

#
#       Outputs the string, which is the locales seperated by commas
#       
#       The local data which is 128 byte records, 1 per locale. The first 64 bytes
#       are the unshifted characters, the second are the shifted characters
#               
if __name__ == "__main__":
    localeList = []
    localeData = []

    for root,dirs,files in os.walk("layouts"):
        for f in files:
            if f.endswith(".xml"):
                e = Extractor(root+os.sep+f)
                localeList.append(f.split(".")[0])
                localeData += e.render()

    print("//\n//\tThis file is automatically generated.\n//")
    print("#include <input_module.h>\n")
    print("#include <input_module_local.h>\n")
    print("char * locales = \"{0}\";\n".format(",".join(localeList)))
    print("const uint16_t USEFLASH localeMapping[] = {{ {0} }};\n\n".format(",".join(["0x{0:02x}".format(n) for n in localeData])))
