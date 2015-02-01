call "glGraphicsWindowsDeconstruct.bat"

cd ..

copy "%cd%\Windows\freeglut.h" "%cd%\freeglut.h"
copy "%cd%\Windows\freeglut.lib" "%cd%\freeglut.lib"
copy "%cd%\Windows\freeglut_ext.h" "%cd%\freeglut_ext.h"
copy "%cd%\Windows\freeglut_std.h" "%cd%\freeglut_std.h"
copy "%cd%\Windows\glGraphics.vcxproj" "%cd%\glGraphics.vcxproj"
copy "%cd%\Windows\glGraphics.vcxproj.filters" "%cd%\glGraphics.vcxproj.filters"
copy "%cd%\Windows\glGraphics.vcxproj.user" "%cd%\glGraphics.vcxproj.user"
copy "%cd%\Windows\glut.h" "%cd%\glut.h"
copy "%cd%\Windows\glut32.dll" "%cd%\glut32.dll"
copy "%cd%\Windows\osGraphics.h" "%cd%\osGraphics.h"
copy "%cd%\Windows\glOSFunctions.h" "%cd%\glOSFunctions.h"
copy "%cd%\Windows\glOSFunctions.cpp" "%cd%\glOSFunctions.cpp"

cd Windows