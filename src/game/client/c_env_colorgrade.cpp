#include "cbase.h"
#include "colorgrade_manager.h"
#include "c_env_colorgrade.h"

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

BEGIN_NETWORK_TABLE_NOBASE( ColorGradingData_t, DT_ColorGradingData )

	RecvPropVector( RECVINFO( contrast ) ),
	RecvPropVector( RECVINFO( brightness ) ),

	RecvPropFloat( RECVINFO( levels_r_min_input ) ),
	RecvPropFloat( RECVINFO( levels_r_max_input ) ),
	RecvPropFloat( RECVINFO( levels_r_min_output ) ),
	RecvPropFloat( RECVINFO( levels_r_max_output ) ),

	RecvPropFloat( RECVINFO( levels_g_min_input ) ),
	RecvPropFloat( RECVINFO( levels_g_max_input ) ),
	RecvPropFloat( RECVINFO( levels_g_min_output ) ),
	RecvPropFloat( RECVINFO( levels_g_max_output ) ),

	RecvPropFloat( RECVINFO( levels_b_min_input ) ),
	RecvPropFloat( RECVINFO( levels_b_max_input ) ),
	RecvPropFloat( RECVINFO( levels_b_min_output ) ),
	RecvPropFloat( RECVINFO( levels_b_max_output ) ),

	RecvPropFloat( RECVINFO( saturation ) ),
	RecvPropFloat( RECVINFO( gamma ) ),

END_NETWORK_TABLE()

IMPLEMENT_CLIENTCLASS_DT( C_EnvColorGrade, DT_EnvColorGrade, CEnvColorGrade )
	RecvPropBool( RECVINFO( m_bGlobalGrader )),
	RecvPropInt( RECVINFO( m_iInfluenceRadius ) ),
	RecvPropDataTable( RECVINFO_DT( m_ColorGradingData ), 0, &REFERENCE_RECV_TABLE(DT_ColorGradingData) ),
END_RECV_TABLE()

C_EnvColorGrade::C_EnvColorGrade()
{

}

C_EnvColorGrade::~C_EnvColorGrade()
{
	RemoveFromManager();
}

void C_EnvColorGrade::Simulate()
{

}

void C_EnvColorGrade::ClientThink()
{

}

int C_EnvColorGrade::GetInfluenceRadius()
{
	return m_iInfluenceRadius;
}

ColorGradingData_t C_EnvColorGrade::GetColorGradingData()
{
	return m_ColorGradingData;
}

void C_EnvColorGrade::AddToManager()
{
	if (m_bAddedToManager)
	{
		return;
	}

	if (m_iInfluenceRadius <= 0)
	{
		return;
	}

	CColorGradingMgr *manager = g_pColorGradingMgr;

	if (!manager)
	{
		return;
	}

	manager->AddGrader(this);
	m_bAddedToManager = true;
}

void C_EnvColorGrade::RemoveFromManager()
{
	if (!m_bAddedToManager)
	{
		return;
	}

	CColorGradingMgr *manager = g_pColorGradingMgr;

	if (!manager)
	{
		return;
	}

	manager->RemoveGrader(this);
	m_bAddedToManager = false;
}

void C_EnvColorGrade::OnDataChanged( DataUpdateType_t updateType )
{
	AddToManager();
}

bool C_EnvColorGrade::IsGlobalGrader()
{
	return m_bGlobalGrader;
}