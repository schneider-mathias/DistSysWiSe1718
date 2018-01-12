// dllmain.h : Deklaration der Modulklasse.

class CCOMMyBaySrvModule : public ATL::CAtlDllModuleT< CCOMMyBaySrvModule >
{
public :
	DECLARE_LIBID(LIBID_COMMyBaySrvLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_COMMYBAYSRV, "{B7324A10-BD80-4396-9278-1E3A5C81401E}")
};

extern class CCOMMyBaySrvModule _AtlModule;
