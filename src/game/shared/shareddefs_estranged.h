#ifndef SHARED_DEFS_ESTRANGED_H
#define SHARED_DEFS_ESTRANGED_H

#ifdef _WIN32
#pragma once
#endif

#define ESTRANGED_SCREEN_EXIT_OUTPUT "EISUEO"
#define ESTRANGED_SCREEN_TRIGGER_OUTPUT "EISUTO"
#define ESTRANGED_SCREEN_PERSIST_PARAMETERS "EISPP"
#define ESTRANGED_STATISTICS_ENDPOINT "http://www.example.com/statistics_endpoint"

#ifdef CLIENT_DLL
EXTERN_RECV_TABLE( DT_ColorGradingData );
#else
EXTERN_SEND_TABLE( DT_ColorGradingData );
#endif

struct ColorGradingData_t
{
	ColorGradingData_t();

	DECLARE_CLASS_NOBASE( ColorGradingData_t );
	DECLARE_SIMPLE_DATADESC();
	DECLARE_EMBEDDED_NETWORKVAR();

	CNetworkVar( Vector, contrast );
	CNetworkVar( Vector, brightness );

	CNetworkVar( float, levels_r_min_input);
	CNetworkVar( float, levels_r_max_input);
	CNetworkVar( float, levels_r_min_output);
	CNetworkVar( float, levels_r_max_output);

	CNetworkVar( float, levels_g_min_input);
	CNetworkVar( float, levels_g_max_input);
	CNetworkVar( float, levels_g_min_output);
	CNetworkVar( float, levels_g_max_output);

	CNetworkVar( float, levels_b_min_input);
	CNetworkVar( float, levels_b_max_input);
	CNetworkVar( float, levels_b_min_output);
	CNetworkVar( float, levels_b_max_output);

	CNetworkVar( float, saturation );
	CNetworkVar( float, gamma );
};

#endif //SHARED_DEFS_ESTRANGED_H
