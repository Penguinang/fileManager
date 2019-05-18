main:
	cd build && msbuild mediaManager.sln
prepare:
	cd build && cmake .. 
clean:
	cd build && msbuild mediaManager.sln -t:Clean
initDB:
	cd db && del media.db && del media.db-journal && copy mediaBk.db media.db