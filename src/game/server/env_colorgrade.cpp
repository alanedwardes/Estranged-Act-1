#include "cbase.h"
#include "shareddefs.h"
#include "shareddefs_estranged.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define SF_GLOBAL_GRADER (1<<0) //Flag: CColorGradingMgr will use this entity when no other color grading entities are nearby

ColorGradingData_t::ColorGradingData_t()
{
	contrast = Vector(1,1,1);
	brightness = Vector(1,1,1);
	
	levels_r_min_input = 0.0;
	levels_r_max_input = 1.0;
	levels_r_min_output = 0.0;
	levels_r_max_output = 1.0;

	levels_g_min_input = 0.0;
	levels_g_max_input = 1.0;
	levels_g_min_output = 0.0;
	levels_g_max_output = 1.0;

	levels_b_min_input = 0.0;
	levels_b_max_input = 1.0;
	levels_b_min_output = 0.0;
	levels_b_max_output = 1.0;

	saturation = 1.0f;
	gamma = 1.0f;
}

BEGIN_SIMPLE_DATADESC( ColorGradingData_t )

	DEFINE_KEYFIELD( contrast,	FIELD_VECTOR,	"contrast" ),
	DEFINE_KEYFIELD( brightness,		FIELD_VECTOR,	"brightness" ),

	DEFINE_KEYFIELD( levels_r_min_input,		FIELD_FLOAT,	"levels_r_min_input" ),
	DEFINE_KEYFIELD( levels_r_max_input,		FIELD_FLOAT,	"levels_r_max_input" ),
	DEFINE_KEYFIELD( levels_r_min_output,		FIELD_FLOAT,	"levels_r_min_output" ),
	DEFINE_KEYFIELD( levels_r_max_output,		FIELD_FLOAT,	"levels_r_max_output" ),

	DEFINE_KEYFIELD( levels_g_min_input,		FIELD_FLOAT,	"levels_g_min_input" ),
	DEFINE_KEYFIELD( levels_g_max_input,		FIELD_FLOAT,	"levels_g_max_input" ),
	DEFINE_KEYFIELD( levels_g_min_output,		FIELD_FLOAT,	"levels_g_min_output" ),
	DEFINE_KEYFIELD( levels_g_max_output,		FIELD_FLOAT,	"levels_g_max_output" ),

	DEFINE_KEYFIELD( levels_b_min_input,		FIELD_FLOAT,	"levels_b_min_input" ),
	DEFINE_KEYFIELD( levels_b_max_input,		FIELD_FLOAT,	"levels_b_max_input" ),
	DEFINE_KEYFIELD( levels_b_min_output,		FIELD_FLOAT,	"levels_b_min_output" ),
	DEFINE_KEYFIELD( levels_b_max_output,		FIELD_FLOAT,	"levels_b_max_output" ),

	DEFINE_KEYFIELD( saturation,		FIELD_FLOAT,	"saturation" ),
	DEFINE_KEYFIELD( gamma,		FIELD_FLOAT,	"gamma" ),

END_DATADESC()

BEGIN_NETWORK_TABLE_NOBASE( ColorGradingData_t, DT_ColorGradingData )
	SendPropVector( SENDINFO( contrast ), 0 ),
	SendPropVector( SENDINFO( brightness ), 0 ),

	SendPropFloat( SENDINFO( levels_r_min_input ), 0 ),
	SendPropFloat( SENDINFO( levels_r_max_input ), 0 ),
	SendPropFloat( SENDINFO( levels_r_min_output ), 0 ),
	SendPropFloat( SENDINFO( levels_r_max_output ), 0 ),

	SendPropFloat( SENDINFO( levels_g_min_input ), 0 ),
	SendPropFloat( SENDINFO( levels_g_max_input ), 0 ),
	SendPropFloat( SENDINFO( levels_g_min_output ), 0 ),
	SendPropFloat( SENDINFO( levels_g_max_output ), 0 ),

	SendPropFloat( SENDINFO( levels_b_min_input ), 0 ),
	SendPropFloat( SENDINFO( levels_b_max_input ), 0 ),
	SendPropFloat( SENDINFO( levels_b_min_output ), 0 ),
	SendPropFloat( SENDINFO( levels_b_max_output ), 0 ),

	SendPropFloat( SENDINFO( saturation ), 0),
	SendPropFloat( SENDINFO( gamma ), 0),
END_NETWORK_TABLE()

class CEnvColorGrade : public CPointEntity
{
	DECLARE_CLASS( CEnvColorGrade, CPointEntity );
public:
	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();

	CEnvColorGrade();
	~CEnvColorGrade();
	bool KeyValue( const char *szKeyName, const char *szValue );

	// Always transmit to clients
	virtual int UpdateTransmitState();
	virtual void Activate( void );

	virtual void Spawn( void );
	void Think( void );
private:
	CNetworkVar( bool, m_bGlobalGrader );
	CNetworkVar( int, m_iInfluenceRadius );
	CNetworkVarEmbedded(ColorGradingData_t, m_ColorGradingData);
};

LINK_ENTITY_TO_CLASS( env_colorgrade, CEnvColorGrade );

BEGIN_DATADESC( CEnvColorGrade )
	DEFINE_THINKFUNC( Think ),
	DEFINE_KEYFIELD( m_iInfluenceRadius, FIELD_INTEGER, "radius" ),
	DEFINE_EMBEDDED( m_ColorGradingData ),
END_DATADESC()

IMPLEMENT_SERVERCLASS_ST( CEnvColorGrade, DT_EnvColorGrade )
	SendPropBool( SENDINFO( m_bGlobalGrader ) ),
	SendPropInt( SENDINFO( m_iInfluenceRadius ) ),
	SendPropDataTable( SENDINFO_DT( m_ColorGradingData ), &REFERENCE_SEND_TABLE( DT_ColorGradingData ) ),
END_SEND_TABLE()

CEnvColorGrade::CEnvColorGrade()
{

}

CEnvColorGrade::~CEnvColorGrade()
{

}

bool CEnvColorGrade::KeyValue( const char *szKeyName, const char *szValue )
{
	return BaseClass::KeyValue( szKeyName, szValue );
}

int CEnvColorGrade::UpdateTransmitState()
{
	return SetTransmitState( FL_EDICT_ALWAYS );
}

void CEnvColorGrade::Activate()
{
	BaseClass::Activate();
}

void CEnvColorGrade::Spawn()
{
	m_bGlobalGrader = HasSpawnFlags( SF_GLOBAL_GRADER );
}

void CEnvColorGrade::Think()
{
	BaseClass::Think();
}