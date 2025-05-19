#include "Include.h"

void LoadLv2Kernel(const char* fileName, uint32_t type)
{
	PrintLog("LoadLv2Kernel(), fileName = %s\n", fileName);

	if (strlen(fileName) > 24)
	{
		PrintLog("file name too long! Abort!()\n");

		abort();
		return;
	}

	char fullPath[512];
	sprintf(fullPath, "/dev_flash/sys/%s", fileName);

	PrintLog("fullPath = %s\n", fullPath);

	if (!IsFileExist(fullPath))
	{
		PrintLog("file not found! Abort!()\n");

		abort();
		return;
	}

	bool found = false;
	uint64_t offset = 0;

	{
		uint64_t t1 = GetTimeInMs();

		for (uint64_t i = 0; i < 16 * 1024 * 1024; i += 4)
		{
			uint64_t v;
			lv1_read(i, 8, &v);

			if (v == 0x2F6F732F6C76325FULL)
			{
				found = true;
				offset = i - 4;

				break;
			}

			uint64_t t2 = GetTimeInMs();

			if ((t2 - t1) >= 2000)
			{
				lv2_beep_single();
				t1 = t2;
			}
		}
	}

	if (!found)
	{
		PrintLog("offset not found!, Abort!()\n");

		abort();
		return;
	}

	PrintLog("offset = 0x%lx\n", offset);

	{
		char fullPath[512];
		sprintf(fullPath, "/local_sys0/sys/%s", fileName);

		PrintLog("fullPath2 = %s\n", fullPath);

		size_t len = strlen(fullPath);
		lv1_write(offset, len + 1, fullPath);
	}

	if (type == LoadLv2KernelType_e::OtherOS_Fself)
	{
		PrintLog("Patching initial lpar size...\n");

		uint64_t initial_lpar_size_offset = offset + 0x127;
		PrintLog("initial_lpar_size_offset = 0x%lx\n", initial_lpar_size_offset);

		uint8_t v = 0x1B; // 128M
		lv1_write(initial_lpar_size_offset, 1, &v);
	}

	WaitInMs(1000);
	lv2_beep_triple();

	PrintLog("Booting lv2_kernel...\n");
	lv2_boot_lv2_kernel();
}