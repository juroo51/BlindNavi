#pragma once

#include <string>
#include <sapi.h>

const int TTS()
{
	ISpVoice * pVoice = NULL;

	if (FAILED(::CoInitialize(NULL)))
		return FALSE;
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
	
	if (SUCCEEDED(hr))
	{
		hr = pVoice->Speak(L"Hello", NULL, 0);
		pVoice->Release();
		pVoice = NULL;
	}

	::CoUninitialize();
	return TRUE;
}