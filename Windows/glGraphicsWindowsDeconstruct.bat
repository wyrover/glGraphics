cd ..

copy "%cd%\glGraphics.vcxproj" "%cd%\Windows\glGraphics.vcxproj"
copy "%cd%\glGraphics.vcxproj.filters" "%cd%\Windows\glGraphics.vcxproj.filters"
copy "%cd%\glGraphics.vcxproj.users" "%cd%\Windows\glGraphics.vcxproj.users"

del "freeglut.h"
del "freeglut.lib"
del "freeglut_ext.h"
del "freeglut_std.h"
del "glGraphics.vcxproj"
del "glGraphics.vcxproj.filters"
del "glGraphics.vcxproj.users"
del "glut.h"
del "glut32.dll"
del "osGraphics.h"
del "gl_main_void"
del "glGraphicsCompile.bash"

cd Windows
