#define MUL_NS 1000000000
#define MUL_US 1000000
#define MUL_MS 1000

extern uint64_t GetTimeInNs();
extern void WaitInNs(uint64_t ns);

extern uint64_t GetTimeInUs();
extern void WaitInUs(uint64_t us);

extern uint64_t GetTimeInMs();
extern void WaitInMs(uint64_t ms);

extern void Sleep(uint32_t secs);

extern bool IsFileExist(const char* path);
extern size_t GetFileSize(FILE* f);

extern bool FlashIsNor();

extern bool TargetIsCEX();
extern bool TargetIsDEX();

extern bool TargetIsDECR();

extern void NorWrite(uint64_t offset, const void* data, uint64_t size);
extern void NorRead(uint64_t offset, void* data, uint64_t size);