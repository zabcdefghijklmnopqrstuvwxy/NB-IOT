
attrib +h .\MDK-ARM\BC20\*.hex
attrib +h .\MDK-ARM\BC20\*.map
attrib +h .\MDK-ARM\BC20\*.sct

del /Q /A-H .\MDK-ARM\BC20\*.*

attrib -h .\MDK-ARM\BC20\*.hex
attrib -h .\MDK-ARM\BC20\*.map
attrib -h .\MDK-ARM\BC20\*.sct