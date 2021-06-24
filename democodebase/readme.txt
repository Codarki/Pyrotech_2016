
To compile:

1. Microsoft Visual Studio 2010

2. This codebase was developed using boost C++ library version 1.46.1 (http://www.boost.org/users/history/version_1_46_1.html)
   This library is not included. Boost must be installed to directory "SDK\boost" (next to fmod) so the subfolders in it are:
   boost\
   libs\

3. This codebase was developed using Microsoft DirectX SDK (June 2010) (http://www.microsoft.com/en-us/download/details.aspx?id=6812)
   Default installation should be sufficient. Install after installing Visual Studio, and library paths should be set by installer.

4. Open demo2008\demo2008.sln

5. Hit compile (Build -> Build Solution). The executable demo2008.exe (or demo2008.dll) is built in ".\demo2008\exe" directory. This is the working directory, project demo2008's Debugging property "Working Directory" is set to "$(ProjectDir)exe".
   
------------
   
Run from batch (.bat) files in directory ".\demo2008\exe\". Example batch file contains:
"demo2008.exe -verbose -convert -window -res=1280x720 -timeline=data/timelines/creation.xml"

------------

Run from debugger:

1. "Set as StartUp Project" demo2008.

2. Set "Command Arguments" parameter in demo2008's debug properties. Hit run (Debug -> Start Debugging). Example arguments:
   -verbose -convert -window -res=1280x720 -timeline=data/timelines/raytrace.xml
   
------------

Command line parameters:
-aa					Anti-aliasing value. NOT supported.
-convert			Performs content pipeline conversion of all assets from .\demo2008\src_data\ to .\demo2008\exe\data\
-demoname			Sets the demoname.
-res=1280x720		Sets the resolution.
-serialize_only		Skips the full loading of mesh models, and loads only the binary dump of vertex buffers.
					This parameter is a hack, meant for release builds, and must be run in sequence after a run with -convert.
-sync_params		Some sync parameter.
-timeline			Relative path from working directory to timeline XML file.
-verbose			Enables logging to VERBOSE.txt in working directory.
-window				Runs in windowed mode.

------------

Do NOT modify data under .\demo2008\exe\data\ directory. Always modify under .\demo2008\src-data\, and the conversion will propagate the changes.

------------

Shortcuts:
	
	See .\demo2008\src-data\shortcuts.txt

------------

Hard-coded files:

"srd-data/curves/curves_test.xml"	All spline curves. There are multiple types of curves:

	catmull_rom		float spline
	catmull_rom2	vector2f spline
	catmull_rom3	vector3f spline
	catmull_rom4	vector4f spline
	step_curve		"step" interpolation text curve. (used for animating camera shots)
	
	There is more information at "documentation.txt" section "3. Curves reference". Also plenty of examples in the file itself.


"srd-data/shared_textures.xml"		All shared textures. 

	These are only suitable for programmers for setting up more complex render-to-texture
	and texture feedback to material scenarios. These textures can be referenced by some
	timeline sequences, and by any material texture parameter.
	TODO: specify which sequences, and the syntax of material's shared texture.
	
All files under "src-data/materials"	Material XML files.

	All files under this directory are loaded, recursively. All materials have unique name.
	TODO: Describe how materials work.

------------

Archiver:
if Escape.dat exists, it will override all files found under .\exe\data\ directory.

---

Publishing demo:

1. Run timeline with -convert command line parameter. This will create all the required data files.

2. Run delete_p3d_models.bat in the exe directory. This will delete all the .p3d_model files. It will leave .geometry files behind.

3. Run archiver_uncompressed.exe. This will create data.dat from data directory.

4. From Visual Studio, go to properties of demo2008 project
  4.1 Change Configuration Properties -> General -> Configuration Type to: DLL
  4.2 Change Configuration Properties -> Linker -> General -> Output File to: .DLL
  4.3 Change demo name in demo2008/dll_main.cpp to correct name.
  4.4 Compile the dll
  
5. Edit Gui project:
  5.1 Edit Form1.vb to load demoname.dll
  5.2 Compile Gui

6. Copy following files to a directory:
  6.1 data.dat
  6.2 fmodex.dll
  6.3 Microsoft.DirectX.Direct3D.dll
  6.3 demo2008.dll, rename to demoname.dll
  6.4 Gui executable, rename to demoname.exe
  
  
  
  