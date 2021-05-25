# AudioConverter
LINUX
-----
1. Whereever possible, you should use the packages supplied by your Linux
distribution.(needed libraries - mp3lame, Threads)

2. Download the source from  - https://github.com/gulipalli123/AudioConverter.git

3. Go to WavToMp3 folder and open a terminal and run
	    $ cmake .
		$ make
This should generate the binary in bin folder inside WavToMP3

4.Now you can run the executable and provide the path to the folder where wav files are present
$ ./WavToMp3 path/to/wav files

5. the generated mp3 files can be found in the same folder.

WINDOWS
-------
1.  Download the source from  - https://github.com/gulipalli123/AudioConverter.git

2. Create a cmake cross-platform project in MS Visual Studio

3. Copy the WavToMp3 folder content to the project

4. You can reuse the CMakeSettings.json file in the repo

5. Use the vcpkg to load any missing packages e.g. mp3lame, dirent(important - 
choose package according to the architecture - e.g. mp3lame_x64-windows or mp3lame_x86-windows)

6. vcpkg commands - .\vcpkg install mp3lame:x64-windows, .\vcpkg integrate install

7. save the cmake file will do the configuration and generation

8. there is option to build in the visual studio -select x64-Debug as build options

9. You can find the binary or executable in -  /WavToMp3/out/build/x64-Debug/WavToMp3.exe

10. Open command line and run exe by providing the wav files path
	$ ./WavToMp3 path/to/wav files

11. the generated mp3 files can be found in the same folder.

12. If you want to build for linux in visual studio - 
	1. Connect the remote linux host to VS- https://www.youtube.com/watch?v=Y5Els11sY1A
	2. select Linux-Debug instead of x64-Debug in the build options
	3. Your binary will be created in the remote linux machine.
	