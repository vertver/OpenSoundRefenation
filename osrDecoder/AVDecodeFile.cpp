/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR entry-point
**********************************************************
* AVDecodeFile.h
* FFMpeg decoder implementation
*********************************************************/
#include "stdafx.h"

extern "C"
{
#include <libavcodec/avcodec.h>
}

extern DLL_API HMODULE hAVCodec;

#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096

VOID
DecodeFrame(
	AVCodecContext* pCodecContext,
	AVPacket* pPacket,
	AVFrame* pFrame,
	HANDLE hFile
)
{
	int i = 0;
	int iChannel = 0;
	int iRet = 0;
	int iDataSize = 0;

	// send packet to context
	iRet = avcodec_send_packet(pCodecContext, pPacket);

	if (iRet < 0)
	{
		DEBUG_BREAK;
	}

	while (iRet >= 0)
	{
		// receive frame from context
		iRet = avcodec_receive_frame(pCodecContext, pFrame);
		if (iRet == AVERROR(EAGAIN) || iRet == AVERROR_EOF) { return; }
		else if (iRet < 0) { DEBUG_BREAK; }

		// get bps
		iDataSize = av_get_bytes_per_sample(pCodecContext->sample_fmt);
		if (iDataSize < 0) { DEBUG_BREAK; }

		for (i = 0; i < pFrame->nb_samples; i++)
		{
			for (iChannel = 0; iChannel < pCodecContext->channels; iChannel++)
			{
				// write file
				DWORD dwSizeTemp = 0;
				WriteFile(hFile, pFrame->data[iChannel] + iDataSize*i, iDataSize, &dwSizeTemp, NULL);
				YieldProcessor();
			}
		}
	}
}

VOID
AVReader::OpenFileToBuffer(
	LPCWSTR lpPath, 
	LPCWSTR* lpTempPath,
	LPDWORD dwSize,
	DWORD dwFormat
)
{
	SYSTEMTIME sysTime = { NULL };
	GetSystemTime(&sysTime);

	// set file in temp dir
	std::wstring lpTempFile = GetTempDirectory() + std::wstring(L"\\expdata_num") +
		std::to_wstring(sysTime.wMonth) + std::to_wstring(sysTime.wDay) + 
		std::to_wstring(sysTime.wHour) + std::to_wstring(sysTime.wMinute + sysTime.wSecond + sysTime.wMilliseconds) + L"m.raw";

	// create it
	HANDLE hTempFile = CreateFileW(lpTempFile.c_str(), GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!hTempFile || hTempFile == (HANDLE)LONG_PTR(-1))
	{
		if (!THROW3(L"Application can't save this file because file handle is invalid. Continue?")) { return; }
	}

	*lpTempPath = lpTempFile.c_str();

	// open our file
	HANDLE hFile = CreateFileW(lpPath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!hTempFile || hTempFile == (HANDLE)LONG_PTR(-1))
	{
		if (!THROW3(L"Application can't open this file because file handle is invalid. Continue?")) { return; }
	}

	DWORD dwWrittenTemp = NULL;
	
	int ret = 0;
	DWORD len = 0;
	const AVCodec* codec = nullptr;
	AVCodecContext* c = NULL;
	AVCodecParserContext* parser = NULL;
	uint8_t inbuf[AUDIO_INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
	uint8_t* data = nullptr;
	size_t   data_size;
	AVPacket* pkt = nullptr;
	AVFrame* decoded_frame = NULL;

	// alloc packet
	pkt = av_packet_alloc();

	switch (dwFormat)
	{
	// stable working only on 4, 5, 6 and 7
	case 1:
		codec = avcodec_find_decoder(AV_CODEC_ID_ALAC);		// needy for QT codec
		break;
	case 2:
		codec = avcodec_find_decoder(AV_CODEC_ID_FLAC);
		break;
	case 3:
		codec = avcodec_find_decoder(AV_CODEC_ID_APE);
		break;
	case 4:
		codec = avcodec_find_decoder(AV_CODEC_ID_MP2);
		break;
	case 5:
		codec = avcodec_find_decoder(AV_CODEC_ID_MP3);
		break;
	case 6:
		codec = avcodec_find_decoder(AV_CODEC_ID_AAC);
		break;
	case 7:
		codec = avcodec_find_decoder(AV_CODEC_ID_AC3);
		break;
	default:
		break;
	}

	// init parser by codec id
	parser = av_parser_init(codec->id);
	if (!parser) { DEBUG_BREAK; }

	// allocate codec
	c = avcodec_alloc_context3(codec);
	if (!c) { DEBUG_BREAK; }

	if (avcodec_open2(c, codec, NULL) < 0)
	{
		DEBUG_BREAK;
	}

	DWORD dwWritten = 0;

	// read first
	data = inbuf;
	ReadFile(hFile, inbuf, AUDIO_INBUF_SIZE, &dwWritten, NULL);
	data_size = dwWritten;

	while (data_size > 0)
	{
		// allocate frame if 0
		if (!decoded_frame)
		{
			if (!(decoded_frame = av_frame_alloc()))
			{
				DEBUG_BREAK;
			}
		}

		// parse to packet
		ret = av_parser_parse2(parser, c, &pkt->data, &pkt->size, data, (int)data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);

		if (ret < 0)
		{
			DEBUG_BREAK;
		}

		data += ret;
		data_size -= ret;

		// decode if packet include size
		if (pkt->size)
		{
			DecodeFrame(c, pkt, decoded_frame, hTempFile);
		}

		if (data_size < AUDIO_REFILL_THRESH)
		{
			// move data to pointer
			memmove_s(inbuf, sizeof(inbuf), data, data_size);
			data = inbuf;
			ReadFile(hFile, data + data_size, AUDIO_INBUF_SIZE - ((DWORD)data_size), &len, NULL);
			if (len > 0) { data_size += len; }
		}
	}

	pkt->data = nullptr;
	pkt->size = 0;

	// flush data
	DecodeFrame(c, pkt, decoded_frame, hTempFile);
	
	// close file handles (read and write)
	CloseHandle(hFile);
	CloseHandle(hTempFile);

	// free all AV stuff
	avcodec_free_context(&c);
	av_parser_close(parser);
	av_frame_free(&decoded_frame);
	av_packet_free(&pkt);

	// #NOTE: if MP3 - float 32/little endian
	// WMF use 16 bit/little endian
}
