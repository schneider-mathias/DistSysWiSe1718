// dllmain.h : Declaration of module class.

class CCOMMyCasinoSrvModule : public ATL::CAtlDllModuleT< CCOMMyCasinoSrvModule >
{
public :
	DECLARE_LIBID(LIBID_COMMyCasinoSrvLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_COMMYCASINOSRV, "{FF428121-8B0F-49EC-AB16-5550EB924138}")
};

extern class CCOMMyCasinoSrvModule _AtlModule;
