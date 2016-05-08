#include <iostream>
#include <fstream>
#include <bitset>
using namespace std;

struct mp3_header {
	unsigned char version,
		layer,
		crc,
		bitrate,
		sampling_rate,
		padding_bit,
		private_bit,
		channel_mode,
		mode_extension,
		copyright,
		original_data,
		emphasis;
};

mp3_header extract_header(unsigned char buffer[])
{
	mp3_header header = {};

	header.version = (buffer[1] >> 3) & 0x03;
	header.layer = (buffer[1] >> 1) & 0x01;
	header.crc = (buffer[1] & 0x01);

	header.bitrate = (buffer[2] >> 4) & 0x0F;
	header.sampling_rate = (buffer[2] >> 2) & 0x03;
	header.padding_bit = (buffer[2] >> 1) & 0x01;
	header.private_bit = (buffer[2] & 0x01);

	header.channel_mode = (buffer[3] >> 6) & 0x03;
	header.mode_extension = (buffer[3] >> 4) & 0x03;
	header.copyright = (buffer[3] >> 3) & 0x01;
	header.original_data = (buffer[3] >> 2) & 0x01;
	header.emphasis = (buffer[3] & 0x03);

	return header;
}

/*
	returns 0 for bad, 1 for free or bitrate otherwise
*/
int get_bitrate(unsigned char ver, unsigned char layer, unsigned char value)
{
	switch (value)
	{
	case 0: return 0;
	case 1:
		if (ver == 0x03 && layer == 0x03)      return 32;
		else if (ver == 0x03 && layer == 0x02) return 32;
		else if (ver == 0x03 && layer == 0x01) return 32;
		else if (ver == 0x02 && layer == 0x03) return 32;
		else return 8;
		break;
	case 2:
		if (ver == 0x03 && layer == 0x03)      return 64;
		else if (ver == 0x03 && layer == 0x02) return 48;
		else if (ver == 0x03 && layer == 0x01) return 40;
		else if (ver == 0x02 && layer == 0x03) return 48;
		else return 16;
		break;
	case 3:
		if (ver == 0x03 && layer == 0x03)      return 96;
		else if (ver == 0x03 && layer == 0x02) return 56;
		else if (ver == 0x03 && layer == 0x01) return 48;
		else if (ver == 0x02 && layer == 0x03) return 56;
		else return 24;
		break;
	case 4:
		if (ver == 0x03 && layer == 0x03)      return 128;
		else if (ver == 0x03 && layer == 0x02) return 64;
		else if (ver == 0x03 && layer == 0x01) return 56;
		else if (ver == 0x02 && layer == 0x03) return 64;
		else return 32;
		break;
	case 5:
		if (ver == 0x03 && layer == 0x03)      return 160;
		else if (ver == 0x03 && layer == 0x02) return 80;
		else if (ver == 0x03 && layer == 0x01) return 64;
		else if (ver == 0x02 && layer == 0x03) return 80;
		else return 40;
		break;
	case 6:
		if (ver == 0x03 && layer == 0x03)      return 192;
		else if (ver == 0x03 && layer == 0x02) return 96;
		else if (ver == 0x03 && layer == 0x01) return 80;
		else if (ver == 0x02 && layer == 0x03) return 96;
		else return 48;
		break;
	case 7:
		if (ver == 0x03 && layer == 0x03)      return 224;
		else if (ver == 0x03 && layer == 0x02) return 112;
		else if (ver == 0x03 && layer == 0x01) return 96;
		else if (ver == 0x02 && layer == 0x03) return 112;
		else return 56;
		break;
	case 8:
		if (ver == 0x03 && layer == 0x03)      return 256;
		else if (ver == 0x03 && layer == 0x02) return 128;
		else if (ver == 0x03 && layer == 0x01) return 112;
		else if (ver == 0x02 && layer == 0x03) return 128;
		else return 64;
		break;
	case 9:
		if (ver == 0x03 && layer == 0x03)      return 288;
		else if (ver == 0x03 && layer == 0x02) return 160;
		else if (ver == 0x03 && layer == 0x01) return 128;
		else if (ver == 0x02 && layer == 0x03) return 144;
		else return 80;
		break;
	case 0xA:
		if (ver == 0x03 && layer == 0x03)      return 320;
		else if (ver == 0x03 && layer == 0x02) return 192;
		else if (ver == 0x03 && layer == 0x01) return 160;
		else if (ver == 0x02 && layer == 0x03) return 160;
		else return 96;
		break;
	case 0xB:
		if (ver == 0x03 && layer == 0x03)      return 352;
		else if (ver == 0x03 && layer == 0x02) return 224;
		else if (ver == 0x03 && layer == 0x01) return 192;
		else if (ver == 0x02 && layer == 0x03) return 176;
		else return 112;
		break;
	case 0xC:
		if (ver == 0x03 && layer == 0x03)      return 384;
		else if (ver == 0x03 && layer == 0x02) return 256;
		else if (ver == 0x03 && layer == 0x01) return 224;
		else if (ver == 0x02 && layer == 0x03) return 192;
		else return 128;
		break;
	case 0xD:
		if (ver == 0x03 && layer == 0x03)      return 416;
		else if (ver == 0x03 && layer == 0x02) return 320;
		else if (ver == 0x03 && layer == 0x01) return 256;
		else if (ver == 0x02 && layer == 0x03) return 224;
		else return 144;
		break;
	case 0xE:
		if (ver == 0x03 && layer == 0x03)      return 448;
		else if (ver == 0x03 && layer == 0x02) return 384;
		else if (ver == 0x03 && layer == 0x01) return 320;
		else if (ver == 0x02 && layer == 0x03) return 256;
		else return 160;
		break;
	}
	
	return 1;
}

/*
	returns 0 for reserved or sampling frequency otherwise
*/
int get_sampling_freq(unsigned char ver, unsigned char value)
{
	switch (ver)
	{
	case 0: // MPEG V2.5
		switch (value)
		{
		case 0: return 11025;
		case 1: return 12000;
		case 2: return 8000;
		}
		break;
	case 2: // MPEG V2
		switch (value)
		{
		case 0: return 22050;
		case 1: return 24000;
		case 2: return 16000;
		}
		break;
	case 3: // MPEG V1
		switch (value)
		{
		case 0: return 44100;
		case 1: return 48000;
		case 2: return 32000;
		}
		break;
	}

	return 0;
}

void process_header(mp3_header header)
{
	int bitrate = get_bitrate(header.version, header.layer, header.bitrate);
	int sampling_freq = get_sampling_freq(header.version, header.sampling_rate);

	cout << "Version ID ........ ";
	switch (header.version)
	{
	case 0: cout << "MPEG V2.5"; break;
	case 1: cout << "RESERVED"; break;
	case 2: cout << "MPEG V2"; break;
	case 3: cout << "MPEG V1"; break;
	default: cout << "N/A"; break;
	}

	cout << "\nLayer ............. ";
	switch (header.layer)
	{
	case 0: cout << "RESERVED"; break;
	case 1: cout << "Layer III"; break;
	case 2: cout << "Layer II"; break;
	case 3: cout << "Layer I"; break;
	default: cout << "N/A"; break;
	}

	cout << "\nProtection Bit .... ";
	switch (header.crc)
	{
	case 0: cout << "CRC Protected"; break;
	case 1: cout << "Not Protected"; break;
	default: cout << "N/A"; break;
	}

	cout << "\nBitrate ........... ";
	switch (bitrate)
	{
	case 0: cout << "BAD"; break;
	case 1: cout << "FREE"; break;
	default: cout << bitrate << " kbps"; break;
	}

	cout << "\nSampling Rate ..... ";
	switch (sampling_freq)
	{
	case 0: cout << "N/A"; break;
	default: cout << sampling_freq << " Hz"; break;
	}

	cout << "\nPadding Bit ....... ";
	header.padding_bit ? cout << "SET" : cout << "NOT SET";

	cout << "\nPrivate Bit ....... ";
	header.private_bit ? cout << "SET" : cout << "NOT SET";

	cout << "\nChannel Mode ...... ";
	switch (header.channel_mode)
	{
	case 0: cout << "STEREO"; break;
	case 1: cout << "JOINT STEREO"; break;
	case 2: cout << "DUAL CHANNEL"; break;
	case 3: cout << "MONO"; break;
	default: cout << "N/A"; break;
	}

	cout << "\nMode Extenion ..... " << bitset<8>(header.mode_extension);

	cout << "\nCopyright ......... ";
	header.copyright ? cout << "YES" : cout << "NO";

	cout << "\nOriginal .......... ";
	header.original_data ? cout << "ORIGINAL" : cout << "COPY";

	cout << "\nEmphasis .......... ";
	switch (header.emphasis)
	{
	case 0: cout << "NONE"; break;
	case 1: cout << "50/15 ms"; break;
	case 2: cout << "RESERVED"; break;
	case 3: cout << "CCIT J.17"; break;
	}

	cout << endl;
}

int main()
{
	ifstream input("sample.mp3", ifstream::binary);

	if (!input) {
		cout << "Could not open file..." << endl;
		return 1;
	}

	const int BUFF_SIZE =  4;
	unsigned char buff[BUFF_SIZE];
	bool sync_frame_found = false;
	mp3_header header = {};
	int index = 0;

	input.seekg(0, ios_base::beg);

	while(input)
	{
		input.read((char *)buff, BUFF_SIZE);

		if (buff[0] == 0xFF && (buff[1] & 0xE0) == 0xE0) {
			sync_frame_found = true;
			break;
		}
	}

	input.close();

	if (!sync_frame_found)
	{
		cout << "Could not find Sync Frame ... " << endl;
		return 2;
	}

	header = extract_header(buff);
	process_header(header);
	
	return 0;
}