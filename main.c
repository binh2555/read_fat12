#include "main.h"


int main(void)
{
	const bootSectorInfor bootSectorInformation = getInformationBootSector();
	const uint32_t numOfEntry = bootSectorInformation.numOfRootDirectoryEntries;
	const uint32_t NumOfFatEntry = (bootSectorInformation.numOfSecPerFAT * SIZE_PER_SECTOR * EIGHT_BIT) / BIT_PER_ENTRY_FAT;
	directoryEntrypointReal rootEntryPoint[numOfEntry];
	uint16_t FatEntry[NumOfFatEntry];
	managerLinkedList manager = {NULL, NULL, 0};

    memset(FatEntry, 0, NumOfFatEntry);


	createRootDirectory(rootEntryPoint, numOfEntry, &bootSectorInformation);
	createFatTable(FatEntry, NumOfFatEntry, &bootSectorInformation);
	//createStackDirection(&manager, entryPoint);

	browserRootDirectory(rootEntryPoint, numOfEntry);

	browserSubDirectoy(&rootEntryPoint[10], &FatEntry, &bootSectorInformation);

	printf("\noke");

	return 0;
}
