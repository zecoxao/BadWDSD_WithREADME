typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

void entry()
{
    //.bss:000008F0 lv2_prot_info:  dfs 8                   ; lv2_area_addr
    //.bss:000008F8                 dfs 8                   ; lv2_area_size
    //.bss:00000900                 dfs 0x14                ; lv2_area_digest

    typedef void(*check_lv2_area_fn_t)(void* the_appli_loader);
    check_lv2_area_fn_t check_lv2_area_fn = (check_lv2_area_fn_t)0x146A8;

    check_lv2_area_fn((void*)0x3A000);

    asm volatile("stop 0x68");
}