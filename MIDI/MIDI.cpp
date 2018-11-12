#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <time.h>
#include <mmsystem.h>
#include <conio.h>
#include <io.h> 
#include "MIDI.h"
#include "tml.h"
#include "tsf.h"
#define HEADER_PARSE 0
#define INITIALIZING 1
#define TRACK_PARSE 2

#pragma warning(disable:4996)
#pragma comment(lib, "winmm.lib")

Track tracksdata[32];

int main() {

	//Initializing
	char * list[1000];
	char name[256];

	char KeyInput = 0;
	int listselect, listn;
	int pitchFix = 0;
	int mode = 1;
	int select = 0;
	int processStatus = HEADER_PARSE;

	int tempo = 180;
	int i = 0, j = 0;
	int fileEnd;
	int timeDivision, timingFormat, timing0TPQ;
	int trackCount;
	double deltaTime;
	double curTime;
	double startTime;
	double Timer;
	int endCount;

	BYTE param1, param2;
	BYTE channel, midiEvent;
	BYTE note, vel;
	BYTE eventStatus;
	BYTE temp;
	META meta, sys;
	Header head;
	HMIDIOUT m_DevHandle = Midi_Open(0);


	puts("머봉이 미디 플레이어 v4.0Release");

	while (true) {

		if (processStatus == HEADER_PARSE) {

			system("cls");
			puts("\n원하는 명령을 선택");
			puts("1. 디렉토리 탐색하기");
			puts("2. 모드 설정하기");
			puts("3. 끝내기");

			scanf("%d", &select);

			getchar();

			if (select == 2) {
				puts("원하는 모드 설정");
				puts("1.ALL Inst Playing");
				puts("2.Piano Only");
				scanf("%d", &mode);
				//	system("cls");
				continue;
			}
			else if (select == 1) {
				puts("검색중");
				system("cls");
				GetMIDIList(list, &listn);

				if (listn == 0) {
					continue;
				}
				else {
					puts("실행하고 싶은 번호를 선택");
					puts("0을 선택하면 취소됩니다.");
					scanf("%d", &listselect);

					getchar();

					if (listselect == 0) {
						puts("실행 취소");
						continue;
					}
					else {

						if (listselect > listn || listselect < 0) {
							puts("그런 번호는 없습니다.");
							continue;
						}
						else {
							strcpy(name, list[listselect]);
							printf("%s\n", name);
						}
					}
				}
			}
			else if (select == 3) {
				puts("미디플 종료");
				break;
			}
			else {
				puts("잘못된 명령");
				continue;
			}


			FILE *fp;
			fp = fopen(name, "rb");

			if (fp == NULL)
			{
				puts("읽기 실패!!");
				continue;
			}
			else {

				if (m_DevHandle == NULL) { return 0; }

				fileEnd = GetFileSize(fp);
				i = 0;
				j = 0;
				while (true) {

					FileInsertByte(head.type, 4, fp);

					FileInsertByte(head.length, 4, fp);

					if (BinaryToNumber(head.type, 4) == 0x4d546864) {

						FileInsertByte(head.format, 2, fp);

						FileInsertByte(head.tracks, 2, fp);

						FileInsertByte(head.tDivision, 2, fp);

						timeDivision = BinaryToNumber(head.tDivision, 2);

						timingFormat = timeDivision >> 15;

						if (timingFormat == 0) {
							timing0TPQ = (timeDivision & 0x7fff);
						}
						else if (timingFormat == 1) {}

					}//MThd Close
					if (BinaryToNumber(head.type, 4) == 0x4d54726b) {


						tracksdata[i].trackLength = BinaryToNumber(head.length, 4);

						tracksdata[i].trackRaw = (BYTE *)malloc(sizeof(BYTE)*(tracksdata[i].trackLength + 1));

						fread(tracksdata[i].trackRaw, sizeof(BYTE), BinaryToNumber(head.length, 4), fp);

					}//MTrk Closes

					if (ftell(fp) >= fileEnd) {

						trackCount = i;

						for (j = 1; j <= trackCount; j++) {

							tracksdata[j].trackTime = ReadVLQ(tracksdata[j].trackRaw, &tracksdata[j].trackLength);
							tracksdata[j].trackEnd = false;
						}
						fclose(fp);
						break;
					}
					else {
						i++;
					}
				}
				processStatus = 1;
			}
		}//Header Parsing
		else if (processStatus == INITIALIZING) {
			endCount = 0;
			startTime = clock();
			curTime = startTime;
			Timer = startTime;

			processStatus = 2;
		}//Timer Setting
		else if (processStatus == TRACK_PARSE) {


			if (endCount == trackCount) {

				for (i = 1; i <= trackCount; i++) {

					free(tracksdata[i].trackRaw);
					tracksdata[i].trackEnd = false;
					printf("채널 %d 닫기 성공!\n", i);
				}

				printf("미디 닫기 성공!\n");

				processStatus = HEADER_PARSE;//처음으로 돌아간다.

			}

			if (Timer <= clock() - 0.001) {
				curTime += 1 * (tempo*timing0TPQ);
				Timer = clock();
			}

			for (i = 1; i <= trackCount; i++) {

				if (startTime + tracksdata[i].trackTime * 1000 <= curTime && tracksdata[i].trackEnd == false) {


					Sub(&eventStatus, tracksdata[i].trackRaw, 1, &tracksdata[i].trackLength);

					if ((eventStatus & 0xf0) == 0xf0) {
						if (eventStatus == 0xff) {

							Sub(&meta.type, tracksdata[i].trackRaw, 1, &tracksdata[i].trackLength);


							meta.length = ReadVLQ(tracksdata[i].trackRaw, &tracksdata[i].trackLength);

							Sub(meta.data, tracksdata[i].trackRaw, meta.length, &tracksdata[i].trackLength);


							if (meta.type == 0x00) {}
							else if (meta.type == 0x01) {} // text
							else if (meta.type == 0x02) {} // copyright
							else if (meta.type == 0x03) {} // sequence track name
							else if (meta.type == 0x04) {} // inst name
							else if (meta.type == 0x05) {} // lyric
							else if (meta.type == 0x06) {} // marker
							else if (meta.type == 0x07) {} // cue
							else if (meta.type == 0x20) {} // midi channel
							else if (meta.type == 0x2f) { tracksdata[i].trackEnd = true; printf("\n%d번 채널 오프\n", i); endCount++; } // end
							else if (meta.type == 0x51) { tempo = 60000000 / BinaryToNumber(meta.data, meta.length); }
							else if (meta.type == 0x54) {}
							else if (meta.type == 0x58) {}
							else if (meta.type == 0x59) {}
							else if (meta.type == 0x7f) {}

						}//0xff
						else if (eventStatus == 0xf0 || eventStatus == 0xf7) {

							sys.length = ReadVLQ(tracksdata[i].trackRaw, &tracksdata[i].trackLength);

							Sub(sys.data, tracksdata[i].trackRaw, sys.length, &tracksdata[i].trackLength);


						}//0xf0 | 0xf7
					}//0xf0
					else { // channel event
						param1 = 0x00;
						if ((eventStatus & 0x80) == 0) {
							param1 = eventStatus;
							eventStatus = tracksdata[i].lastEventType;
						}
						else {
							Sub(&temp, tracksdata[i].trackRaw, 1, &tracksdata[i].trackLength);
							param1 = temp;
							tracksdata[i].lastEventType = eventStatus;
						}
						channel = eventStatus & 0x0f;

						midiEvent = eventStatus >> 4;
						if (midiEvent == 0x08) {
							note = param1;
							Sub(&vel, tracksdata[i].trackRaw, 1, &tracksdata[i].trackLength);
							if (channel == 9) {
								Midi_SendShortMsg(m_DevHandle, eventStatus, note, vel);
								//	printf("\t\t\t\t\tDRUM Set : %d Vel : %d .\n", note, vel);
							}
							else {
								if ((mode == 1 || mode == 2) && mode != 3) {
									Midi_SendShortMsg(m_DevHandle, eventStatus, note + (BYTE)pitchFix, vel);
									//		printf("Channel : %d Note : %d  Vel : %d .\n", channel + 1, note + (BYTE)pitchFix, vel);
								}
							}

						}
						else if (midiEvent == 0x09) {
							note = param1;
							Sub(&vel, tracksdata[i].trackRaw, 1, &tracksdata[i].trackLength);

							if (channel == 9) {
								Midi_SendShortMsg(m_DevHandle, eventStatus, note, vel);
								//	printf("\t\t\t\t\tDRUM Set : %d Vel : %d !\n", note, vel);
							}
							else {
								if ((mode == 1 || mode == 2) && mode != 3) {
									Midi_SendShortMsg(m_DevHandle, eventStatus, note + (BYTE)pitchFix, vel);
									//printf("Channel : %d Note : %d  Vel : %d !\n", channel + 1, note + (BYTE)pitchFix, vel);
								}
							}
						}
						else if (midiEvent == 0x0a) {
							Sub(&param2, tracksdata[i].trackRaw, 1, &tracksdata[i].trackLength);
							Midi_SendShortMsg(m_DevHandle, eventStatus, param1, param2);
						}//Polyphonic aftertouch
						else if (midiEvent == 0x0b) {
							Sub(&param2, tracksdata[i].trackRaw, 1, &tracksdata[i].trackLength);
							Midi_SendShortMsg(m_DevHandle, eventStatus, param1, param2);
						}//Control mode change
						else if (midiEvent == 0x0c) {
							note = param1;
							if (mode == 1) {
								Midi_SendShortMsg(m_DevHandle, eventStatus, note, 0);
							}
						}//Program change
						else if (midiEvent == 0x0d) {
							Midi_SendShortMsg(m_DevHandle, eventStatus, param1, 0);
						}//afterTouch
						else if (midiEvent == 0x0e) {
							Sub(&param2, tracksdata[i].trackRaw, 1, &tracksdata[i].trackLength);
							Midi_SendShortMsg(m_DevHandle, eventStatus, param1, param2);
						}//pitch Controller
						else {
							printf("error");
							return 0;
						}

					}//channel event end

					deltaTime = ReadVLQ(tracksdata[i].trackRaw, &tracksdata[i].trackLength);

					tracksdata[i].trackTime += (float)(60 * deltaTime);

				}//startTime<curtime

			}
		}
	}//Track Parsing


	printf("끝냅니다.");
	system("pause");

	Midi_Close(m_DevHandle);
	return 0;

}




void GetMIDIList(char ** list, int * listn) {

	long h_file;
	int i = 0;
	FILE_SEARCH file_search;

	if ((h_file = _findfirst(".\\*.mid", &file_search)) == -1L) {
		printf("아무것도 없어용\n");
		*listn = 0;
	}
	else {

		do {
			i++;
			list[i] = (char *)malloc(sizeof(char)*(sizeof(file_search.name) + 1));
			strcpy(list[i], file_search.name);
			printf("%d %s\n", i, list[i]);

		} while (_findnext(h_file, &file_search) == 0);
		*listn = i;
		_findclose(h_file);
	}
}

HMIDIOUT Midi_Open(WORD wDevsNum)

{
	WORD      wMaxDevNum = 0;
	MMRESULT  uiMidiOpen = 0;
	HMIDIOUT  m_DevHandle = NULL;
	wMaxDevNum = midiInGetNumDevs();
	if (wDevsNum >= wMaxDevNum)
	{
		wDevsNum = 0;
	}
	uiMidiOpen = midiOutOpen(&m_DevHandle, wDevsNum,
		(DWORD)(MidiOutProc),
		(DWORD)NULL, CALLBACK_FUNCTION);
	if (uiMidiOpen != MMSYSERR_NOERROR)

	{
		return NULL;
	}
	return m_DevHandle;

}
LRESULT Midi_Close(HMIDIOUT m_DevOutHandle)
{
	MMRESULT Result;
	Result = midiOutClose(m_DevOutHandle);

	if (Result != MMSYSERR_NOERROR)
	{
		return FALSE;
	}
	else {
		m_DevOutHandle = NULL;
	}
	return TRUE;
}
void Midi_SendShortMsg(HMIDIOUT m_DevOutHandle, BYTE byteState, BYTE byteNote, BYTE byteValo)
{

	MIDIShort  sMsg;

	sMsg.stMidi_Data.byteVelocity = byteValo;
	sMsg.stMidi_Data.byteNote = byteNote;
	sMsg.stMidi_Data.byteState = byteState;
	sMsg.stMidi_Data.byteNULL = 0;
	midiOutShortMsg(m_DevOutHandle, sMsg.dwMidi_Data);
}//MIDI LIB

void Midi_AllChannelSoundOff(HMIDIOUT m_DevOutHandle)
{

	BYTE channel;

	for (channel = 0; channel < 16; channel++)
	{

		Midi_SendShortMsg(m_DevOutHandle, (BYTE)(0xB0 + channel), 0x78, 0);

	}

}




void DebugBinary(BYTE * debug, int cnt) {
	printf("\n");
	for (int i = 0; i < cnt; i++) {

		printf("%02X   ", *(debug + i));

	}
	printf("\n");

}

void Sub(BYTE * out, BYTE * word, int cnt, int * tracklength)
{
	//int i;
	if ((*tracklength) < cnt) return;

	memcpy(out, word, cnt);
	(*tracklength) -= cnt;
	memcpy(word, word + cnt, *tracklength);
}


int GetFileSize(FILE *p) {
	int end;

	fseek(p, 0l, 2);
	end = ftell(p);
	fseek(p, 0l, 0);

	return end;
}

int BinaryToNumber(BYTE * raw, int cnt) {
	int res = 0;
	int i;
	if (cnt == 1) { return *raw; }
	else {
		for (i = 1; i <= cnt; i++) {
			res += raw[i - 1] << (cnt - i) * 8;
		}
	}
	return res;
}

void FileInsertByte(BYTE * raw, int cnt, FILE * fp) {

	fread(raw, sizeof(BYTE), cnt, fp);
	/*for (i = 0; i < cnt; i++) {
		tmp = fgetc(fp);
		if (tmp != EOF) {
			*(raw + i) = tmp;
		}
	}*/
}

int ReadVLQ(BYTE * raw, int * tracklength) {

	int res = 0;
	int i;
	BYTE temp[8];

	for (i = 0; i < 7; i++) {
		Sub(temp, raw, 1, tracklength);
		int c = BinaryToNumber(temp, 1);
		res = res << 7;
		res = res | (c & 0x7F);
		if ((c & 0x80) != 0x80) {
			return res;
		}
	}
	return res;
}


