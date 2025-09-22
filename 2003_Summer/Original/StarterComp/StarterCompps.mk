
StarterCompps.dll: dlldata.obj StarterComp_p.obj StarterComp_i.obj
	link /dll /out:StarterCompps.dll /def:StarterCompps.def /entry:DllMain dlldata.obj StarterComp_p.obj StarterComp_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del StarterCompps.dll
	@del StarterCompps.lib
	@del StarterCompps.exp
	@del dlldata.obj
	@del StarterComp_p.obj
	@del StarterComp_i.obj
