// dllmain.h : Deklaration der Modulklasse.

class CCOMMyBaySrvModule : public ATL::CAtlDllModuleT< CCOMMyBaySrvModule >
{
public :
	DECLARE_LIBID(LIBID_COMMyBaySrvLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_COMMYBAYSRV, "{E358262F-759E-4648-9336-FCB09BCA5447}")
};

extern class CCOMMyBaySrvModule _AtlModule;
