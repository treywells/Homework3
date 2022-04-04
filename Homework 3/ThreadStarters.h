#pragma once

inline DWORD _stdcall DiskThreadStarter(LPVOID p) {
	Disk* d = (Disk*)p;
	d->Run();
	return 0;
}

inline DWORD _stdcall SearchThreadStarter(LPVOID p) {
	Search* s = (Search*)p;
	s->Run();
	return 0;
}

inline DWORD _stdcall StatThreadStarter(LPVOID p) {
	Stats* p = (Stats*)p;
	p->Run();
}