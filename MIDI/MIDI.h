typedef struct _finddata_t  FILE_SEARCH;

typedef struct
{
	BYTE byteState;
	BYTE byteNote;
	BYTE byteVelocity;
	BYTE byteNULL;
}MIDIShortST;

typedef union
{

	DWORD        dwMidi_Data;

	MIDIShortST  stMidi_Data;

}MIDIShort;

typedef struct {

	BYTE *trackRaw;
	BYTE lastEventType;
	float trackTime;
	int trackLength;
	bool trackEnd;


}Track;

typedef struct {

	BYTE data[100];
	BYTE type;
	int length;


}META;

typedef struct {

	BYTE type[4];
	BYTE length[4];
	BYTE format[2];
	BYTE tracks[2];
	BYTE tDivision[2];


}Header;


void CALLBACK MidiOutProc(HMIDIOUT MidiOut, UINT Msg, DWORD Instance, DWORD Param1, DWORD Param2) {}
int GetFileSize(FILE *p);
int BinaryToNumber(BYTE * raw, int cnt);
void DebugBinary(BYTE * debug, int cnt);
void FileInsertByte(BYTE * raw, int cnt, FILE * fp);
int ReadVLQ(BYTE * raw, int * treacklength);
void Sub(BYTE * out, BYTE * word, int cnt, int * tracklength);
void Midi_AllChannelSoundOff(HMIDIOUT m_DevOutHandle);
void Midi_SendShortMsg(HMIDIOUT m_DevOutHandle, BYTE byteState, BYTE byteNote, BYTE byteValo);
LRESULT Midi_Close(HMIDIOUT m_DevOutHandle);
HMIDIOUT Midi_Open(WORD wDevsNum);
void GetMIDIList(char ** list, int * listn);
