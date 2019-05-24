gui:
	cd build && msbuild guiManager.vcxproj
cli:
	cd build && msbuild clilocate.vcxproj

prepare:
	cd build && cmake .. 
clean:
	cd build && msbuild mediaManager.sln -t:Clean
initDB:
	cd db && del media.db && del media.db-journal && copy mediaBk.db media.db