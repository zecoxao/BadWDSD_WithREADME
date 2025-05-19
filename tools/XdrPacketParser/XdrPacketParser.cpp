#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/stat.h>

struct XdrCmd_s
{
    uint32_t value;
};

static const uint32_t XdrCmd_Start_Mask = 0xF0000000;
static const uint32_t XdrCmd_Start_ShiftCount = 28;

static const uint32_t XdrCmd_Scmd_Mask = 0xC000000;
static const uint32_t XdrCmd_Scmd_ShiftCount = 26;

static const uint32_t XdrCmd_Sid_Mask = 0x3FC0000;
static const uint32_t XdrCmd_Sid_ShiftCount = 18;

static const uint32_t XdrCmd_Sadr_Mask = 0x3FC00;
static const uint32_t XdrCmd_Sadr_ShiftCount = 10;

static const uint32_t XdrCmd_Junk1_Mask = 0x200;
static const uint32_t XdrCmd_Junk1_ShiftCount = 9;

static const uint32_t XdrCmd_Swd_Mask = 0x1FE;
static const uint32_t XdrCmd_Swd_ShiftCount = 1;

static const uint32_t XdrCmd_Junk2_Mask = 0x1;
static const uint32_t XdrCmd_Junk2_ShiftCount = 0;

uint32_t XdrCmd_GetValue(struct XdrCmd_s* cmd)
{
    return cmd->value;
}

void XdrCmd_SetValue(struct XdrCmd_s* cmd, uint32_t value)
{
    cmd->value = value;
}

uint8_t XdrCmd_GetStart(struct XdrCmd_s* cmd)
{
    return (cmd->value & XdrCmd_Start_Mask) >> XdrCmd_Start_ShiftCount;
}

void XdrCmd_SetStart(struct XdrCmd_s* cmd, uint8_t value)
{
    cmd->value &= ~XdrCmd_Start_Mask;
    cmd->value |= (value << XdrCmd_Start_ShiftCount) & XdrCmd_Start_Mask;
}

uint8_t XdrCmd_GetScmd(struct XdrCmd_s* cmd)
{
    return (cmd->value & XdrCmd_Scmd_Mask) >> XdrCmd_Scmd_ShiftCount;
}

void XdrCmd_SetScmd(struct XdrCmd_s* cmd, uint8_t value)
{
    cmd->value &= ~XdrCmd_Scmd_Mask;
    cmd->value |= ((uint32_t)value << XdrCmd_Scmd_ShiftCount) & XdrCmd_Scmd_Mask;
}

uint8_t XdrCmd_GetSid(struct XdrCmd_s* cmd)
{
    return (cmd->value & XdrCmd_Sid_Mask) >> XdrCmd_Sid_ShiftCount;
}

void XdrCmd_SetSid(struct XdrCmd_s* cmd, uint8_t value)
{
    cmd->value &= ~XdrCmd_Sid_Mask;
    cmd->value |= ((uint32_t)value << XdrCmd_Sid_ShiftCount) & XdrCmd_Sid_Mask;
}

uint8_t XdrCmd_GetSadr(struct XdrCmd_s* cmd)
{
    return (cmd->value & XdrCmd_Sadr_Mask) >> XdrCmd_Sadr_ShiftCount;
}

void XdrCmd_SetSadr(struct XdrCmd_s* cmd, uint8_t value)
{
    cmd->value &= ~XdrCmd_Sadr_Mask;
    cmd->value |= ((uint32_t)value << XdrCmd_Sadr_ShiftCount) & XdrCmd_Sadr_Mask;
}

uint8_t XdrCmd_GetJunk1(struct XdrCmd_s* cmd)
{
    return (cmd->value & XdrCmd_Junk1_Mask) >> XdrCmd_Junk1_ShiftCount;
}

void XdrCmd_SetJunk1(struct XdrCmd_s* cmd, uint8_t value)
{
    cmd->value &= ~XdrCmd_Junk1_Mask;
    cmd->value |= ((uint32_t)value << XdrCmd_Junk1_ShiftCount) & XdrCmd_Junk1_Mask;
}

uint8_t XdrCmd_GetSwd(struct XdrCmd_s* cmd)
{
    return (cmd->value & XdrCmd_Swd_Mask) >> XdrCmd_Swd_ShiftCount;
}

void XdrCmd_SetSwd(struct XdrCmd_s* cmd, uint8_t value)
{
    cmd->value &= ~XdrCmd_Swd_Mask;
    cmd->value |= ((uint32_t)value << XdrCmd_Swd_ShiftCount) & XdrCmd_Swd_Mask;
}

uint8_t XdrCmd_GetJunk2(struct XdrCmd_s* cmd)
{
    return (cmd->value & XdrCmd_Junk2_Mask) >> XdrCmd_Junk2_ShiftCount;
}

void XdrCmd_SetJunk2(struct XdrCmd_s* cmd, uint8_t value)
{
    cmd->value &= ~XdrCmd_Junk2_Mask;
    cmd->value |= ((uint32_t)value << XdrCmd_Junk2_ShiftCount) & XdrCmd_Junk2_Mask;
}

size_t get_file_size(FILE *f)
{
	size_t old_off = ftell(f);

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);

	fseek(f, old_off, SEEK_SET);
	return size;
}

int main(int argc, char **argv)
{
	if (argc != 2 || strlen(argv[1]) == 0)
	{
		printf("args: <file.txt>\n");
		return 0;
	}

	FILE* f = fopen(argv[1], "rb");

	if (f == NULL)
	{
		printf("file not found!\n");
		return 0;
	}

	size_t fsize = get_file_size(f);
	printf("fsize = %lu\n", fsize);

	uint8_t* rawData = (uint8_t*)malloc(fsize);
	fread(rawData, 1, fsize, f);

	fclose(f);
	f = NULL;
	
	// find data count

	size_t dataCount = 0;
	size_t firstDataOffset = 0;

	for (size_t i = 0; i < fsize; i++)
	{
		if (rawData[i] == 'D' && rawData[i + 1] == '0')
		{
			if (dataCount == 0)
				firstDataOffset = i;

			++dataCount;
		}
	}

	printf("dataCount = %lu\n", dataCount);

	// parse data

	struct ParsedData_s
	{
		bool value;
	};

	ParsedData_s* d0_ParsedData = new ParsedData_s[dataCount];
	ParsedData_s* d1_ParsedData = new ParsedData_s[dataCount];

	{
		size_t curOffset = firstDataOffset;

		for (size_t i = 0; i < dataCount; i++)
		{
			curOffset += 3;
			d0_ParsedData[i].value = (rawData[curOffset] == '1');
			curOffset += 2;

			curOffset += 3;
			d1_ParsedData[i].value = (rawData[curOffset] == '1');
			curOffset += 2;
		}
	}

	{
		uint32_t result = 0;

		// find where d0 is high
		size_t firstD0HighOffset = 0;

		for (size_t i = 0; i < dataCount; i++)
		{
			if (d0_ParsedData[i].value)
			{
				firstD0HighOffset = i;
				break;
			}
		}

		printf("firstD0HighOffset = %lu\n", firstD0HighOffset);

		if (firstD0HighOffset == 0)
		{
			printf("d0 high not found!\n");
			abort();
		}

		bool prevClkValue = true;
		uint32_t clkCount = 0;

		for (size_t i = firstD0HighOffset; i < dataCount; i++)
		{
			bool curClkValue = d0_ParsedData[i].value;

			if (prevClkValue && !curClkValue)
			{
				if (clkCount == 32)
					break;

				uint32_t x = d1_ParsedData[i].value ? 0 : 1;

				result |= (x << (31 - clkCount));

				++clkCount;
			}

			prevClkValue = curClkValue;
		}

		printf("clkCount = %u\n", clkCount);
		printf("result = 0x%x\n", result);

		printf("\n");

		printf("Parse:\n");

		XdrCmd_s cmd;
		XdrCmd_SetValue(&cmd, result);

		printf("Start = 0x%x\n", (uint32_t)XdrCmd_GetStart(&cmd));
		printf("Scmd = 0x%x\n", (uint32_t)XdrCmd_GetScmd(&cmd));
		printf("Sid = 0x%x\n", (uint32_t)XdrCmd_GetSid(&cmd));
		printf("Sadr = 0x%x\n", (uint32_t)XdrCmd_GetSadr(&cmd));
		printf("Junk1 = 0x%x\n", (uint32_t)XdrCmd_GetJunk1(&cmd));
		printf("Swd = 0x%x\n", (uint32_t)XdrCmd_GetSwd(&cmd));
		printf("Junk2 = 0x%x\n", (uint32_t)XdrCmd_GetJunk2(&cmd));
	}

	delete[] d1_ParsedData;
	delete[] d0_ParsedData;

	free(rawData);

	return 0;
}