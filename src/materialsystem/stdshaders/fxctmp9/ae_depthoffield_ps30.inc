#include "shaderlib/cshader.h"
class ae_depthoffield_ps30_Static_Index
{
public:
	ae_depthoffield_ps30_Static_Index( )
	{
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
#endif // _DEBUG
		return 0;
	}
};
#define shaderStaticTest_ae_depthoffield_ps30 0
class ae_depthoffield_ps30_Dynamic_Index
{
private:
	int m_nDEBUG_VIS;
#ifdef _DEBUG
	bool m_bDEBUG_VIS;
#endif
public:
	void SetDEBUG_VIS( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nDEBUG_VIS = i;
#ifdef _DEBUG
		m_bDEBUG_VIS = true;
#endif
	}
	void SetDEBUG_VIS( bool i )
	{
		m_nDEBUG_VIS = i ? 1 : 0;
#ifdef _DEBUG
		m_bDEBUG_VIS = true;
#endif
	}
public:
	ae_depthoffield_ps30_Dynamic_Index()
	{
#ifdef _DEBUG
		m_bDEBUG_VIS = false;
#endif // _DEBUG
		m_nDEBUG_VIS = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllDynamicVarsDefined = m_bDEBUG_VIS;
		Assert( bAllDynamicVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nDEBUG_VIS ) + 0;
	}
};
#define shaderDynamicTest_ae_depthoffield_ps30 psh_forgot_to_set_dynamic_DEBUG_VIS + 0
