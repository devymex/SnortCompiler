/*!
* @file			ctimer.h
* @author		Devymex, Xidian University
* @brief		定义了高精度计时器类CTimer
* @warning		本文件将包含windows.h，若出现命名冲突，请在包此本文件之间增加相关宏定义。
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
*/

#pragma once
#include "windows.h"

/*!
* @addtogroup groupCommon
* @{
*/

/*!
* @brief		小巧方便的计时器类。
* @remark		成员函数均为内联函数，不存在调用开销，计时精度高。
*/
class CTimer
{
public:
	/// @brief		构造函数。
	/// @remark		构造后即启动计时。
	inline CTimer()
	{
		QueryPerformanceFrequency((PLARGE_INTEGER)&m_nFreq);
		QueryPerformanceCounter((PLARGE_INTEGER)&m_nStart);
	}

	/// @brief		获得从计时开始到现在经过的时间。
	/// @return		单位为秒。
	inline double Cur()
	{
		__int64 nCur;
		QueryPerformanceCounter((PLARGE_INTEGER)&nCur);
		return double(nCur - m_nStart) / double(m_nFreq);
	}

	/// @brief		重至计时器，并获得从计时开始到现在经过的时间。
	/// @return		单位为秒。
	inline double Reset()
	{
		__int64 nCur;
		double dCur;

		QueryPerformanceCounter((PLARGE_INTEGER)&nCur);
		dCur = double(nCur - m_nStart) / double(m_nFreq);
		m_nStart = nCur;

		return dCur;
	}
private:
	/// @brief		私有成员，仅供内部使用。
	__int64 m_nFreq;

	/// @brief		私有成员，仅供内部使用。
	__int64 m_nStart;
};

/*!
@}
*/
