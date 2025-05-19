// -O1 -Wall -mcpu=cell -ffreestanding -mno-toc

#pragma GCC diagnostic ignored "-Wunused-function"

#define SECTION_NAME "BadWDSD_Stage1_Test_Do_Section"

#if 0

#define FUNC_DECL __attribute__((noinline, section(SECTION_NAME))) static
#define FUNC_DEF FUNC_DECL

#else

#define FUNC_DECL __attribute__((always_inline, section(SECTION_NAME))) static inline
#define FUNC_DEF FUNC_DECL

#endif

#define LV1_ENABLED 1

typedef char int8_t;
typedef unsigned char uint8_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef int int32_t;
typedef unsigned int uint32_t;

typedef long int64_t;
typedef unsigned long uint64_t;

#define NULL 0

#define eieio()                \
    {                          \
        asm volatile("eieio"); \
        asm volatile("sync");  \
    }

FUNC_DECL void dead();

FUNC_DECL void intr_disable();
FUNC_DECL void intr_enable();

FUNC_DECL uint64_t GetTimeInNs();
FUNC_DECL void WaitInNs(uint64_t ns);

FUNC_DECL uint64_t GetTimeInUs();
FUNC_DECL void WaitInUs(uint64_t us);

FUNC_DECL uint64_t GetTimeInMs();
FUNC_DECL void WaitInMs(uint64_t ms);

FUNC_DECL void memset(void *buf, uint8_t v, uint64_t count);
FUNC_DECL void memcpy(void *dest, const void *src, uint64_t count);

struct func_call_params_s
{
    uint64_t addr;

    uint64_t args[8];
    uint64_t out[8];
};

FUNC_DECL void func_call(struct func_call_params_s *params);

struct sc_packet_s
{
    uint8_t service_id;
    uint32_t communication_tag;
    uint16_t payload_size;

    uint8_t data[256];
};

struct sc_real_packet_header_s
{
    uint8_t service_id;
    uint8_t version;
    uint16_t tag;
    uint8_t res[2];
    uint16_t cksum;
    uint32_t communication_tag;
    uint16_t payload_size[2];
};

FUNC_DECL uint16_t sc_real_packet_header_calc_cksum(struct sc_real_packet_header_s *pkt_hdr);

FUNC_DECL void sc_send_packet(const struct sc_packet_s *in, struct sc_packet_s *out);

FUNC_DECL void sc_triple_beep();
FUNC_DECL void sc_hard_restart();

FUNC_DECL void puts(const char *str);

FUNC_DEF void dead()
{
    while (1)
    {
    }
}

FUNC_DEF void intr_disable()
{
    // li      %r0, 2
    // mtmsrd  %r0, 1

    asm volatile("li 0, 2");
    asm volatile("mtmsrd 0, 1");

    eieio();
}

FUNC_DEF void intr_enable()
{
    // li  %r0, 0
    // ori %r0, %r0, 0x8002
    // eieio
    // sync
    // mtmsrd  %r0, 1

    asm volatile("li 0, 0");
    asm volatile("ori 0, 0, 0x8002");
    eieio();

    asm volatile("mtmsrd 0, 1");
}

// timebase = 79800000

// #define MUL_NS 1000000000
// #define MUL_US 1000000
// #define MUL_MS 1000

#define SYS_TIMEBASE_GET(tb)                          \
    do                                                \
    {                                                 \
        __asm__ volatile("1: mftb %[current_tb];"     \
                         "cmpwi 7, %[current_tb], 0;" \
                         "beq-  7, 1b;"               \
                         : [current_tb] "=r"(tb) :    \
                         : "cr7");                    \
    } while (0)

FUNC_DEF uint64_t GetTimeInNs()
{
    uint64_t my_timebase = 79800000;
    uint64_t MUL_NS = 1000000000;

    uint64_t cur_tb_ns;
    SYS_TIMEBASE_GET(cur_tb_ns);

    cur_tb_ns *= MUL_NS;

    return (cur_tb_ns / my_timebase);
}

FUNC_DEF void WaitInNs(uint64_t ns)
{
    uint64_t start = GetTimeInNs();
    uint64_t end = start + ns;

    while (1)
    {
        uint64_t cur = GetTimeInNs();

        if (cur >= end)
            return;
    }
}

FUNC_DEF uint64_t GetTimeInUs()
{
    uint64_t my_timebase = 79800000;
    uint64_t MUL_US = 1000000;

    uint64_t cur_tb_us;
    SYS_TIMEBASE_GET(cur_tb_us);

    cur_tb_us *= MUL_US;

    return (cur_tb_us / my_timebase);
}

FUNC_DEF void WaitInUs(uint64_t us)
{
    uint64_t start = GetTimeInUs();
    uint64_t end = start + us;

    while (1)
    {
        uint64_t cur = GetTimeInUs();

        if (cur >= end)
            return;
    }
}

FUNC_DEF uint64_t GetTimeInMs()
{
    uint64_t my_timebase = 79800000;
    uint64_t MUL_MS = 1000;

    uint64_t cur_tb_ms;
    SYS_TIMEBASE_GET(cur_tb_ms);

    cur_tb_ms *= MUL_MS;

    return (cur_tb_ms / my_timebase);
}

FUNC_DEF void WaitInMs(uint64_t ms)
{
    uint64_t start = GetTimeInMs();
    uint64_t end = start + ms;

    while (1)
    {
        uint64_t cur = GetTimeInMs();

        if (cur >= end)
            return;
    }
}

FUNC_DEF void memset(void *buf, uint8_t v, uint64_t count)
{
    if (((uint64_t)buf % 4) != 0)
    {
        volatile uint8_t *buff = (volatile uint8_t *)buf;

        for (uint64_t i = 0; i < count; ++i)
            buff[i] = v;
    }
    else
    {
        volatile uint32_t *buff = (volatile uint32_t *)buf;

        for (uint64_t i = 0; i < (count / 4); ++i)
            buff[i] = v;

        {
            uint32_t x = (count % 4);
            uint32_t i = (count / 4);

            if (x != 0)
            {
                uint32_t shiftCount = (x * 8);

                uint32_t mask = 0xffffffff;
                mask <<= shiftCount;

                uint32_t value = buff[i];
                value &= ~mask;

                value |= (((uint32_t)v) << shiftCount);

                buff[i] = value;
            }
        }
    }
}

FUNC_DEF void memcpy(void *dest, const void *src, uint64_t count)
{
    if ((((uint64_t)dest % 4) != 0) || (((uint64_t)src % 4) != 0))
    {
        volatile uint8_t *destt = (volatile uint8_t *)dest;
        const volatile uint8_t *srcc = (const volatile uint8_t *)src;

        for (uint64_t i = 0; i < count; ++i)
            destt[i] = srcc[i];
    }
    else
    {
        volatile uint32_t *destt = (volatile uint32_t *)dest;
        const volatile uint32_t *srcc = (const volatile uint32_t *)src;

        for (uint64_t i = 0; i < (count / 4); ++i)
            destt[i] = srcc[i];

        {
            uint32_t x = (count % 4);
            uint32_t i = (count / 4);

            if (x != 0)
            {
                uint32_t shiftCount = (x * 8);

                uint32_t mask = 0xffffffff;
                mask <<= shiftCount;

                uint32_t value = destt[i];
                value &= ~mask;

                value |= (srcc[i] << shiftCount);

                destt[i] = value;
            }
        }
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"

FUNC_DEF void func_call(struct func_call_params_s *params)
{
    uint64_t lr;
    asm volatile("mflr %0" : "=r"(lr)::);

    uint64_t ctr;
    asm volatile("mfctr %0" : "=r"(ctr)::);

    asm volatile("mr 3, %0" ::"r"(params->args[0]) :);
    asm volatile("mr 4, %0" ::"r"(params->args[1]) :);
    asm volatile("mr 5, %0" ::"r"(params->args[2]) :);
    asm volatile("mr 6, %0" ::"r"(params->args[3]) :);
    asm volatile("mr 7, %0" ::"r"(params->args[4]) :);
    asm volatile("mr 8, %0" ::"r"(params->args[5]) :);
    asm volatile("mr 9, %0" ::"r"(params->args[6]) :);
    asm volatile("mr 10, %0" ::"r"(params->args[7]) :);

    asm volatile("mtctr %0" ::"r"(params->addr) :);

    asm volatile("addi 1, 1, -16");
    asm volatile("bctrl");
    asm volatile("addi 1, 1, 16");

    asm volatile("mr %0, 10" : "=r"(params->out[7])::);
    asm volatile("mr %0, 9" : "=r"(params->out[6])::);
    asm volatile("mr %0, 8" : "=r"(params->out[5])::);
    asm volatile("mr %0, 7" : "=r"(params->out[4])::);
    asm volatile("mr %0, 6" : "=r"(params->out[3])::);
    asm volatile("mr %0, 5" : "=r"(params->out[2])::);
    asm volatile("mr %0, 4" : "=r"(params->out[1])::);
    asm volatile("mr %0, 3" : "=r"(params->out[0])::);

    asm volatile("mtctr %0" ::"r"(ctr) :);

    asm volatile("mtlr %0" ::"r"(lr) :);
}

#pragma GCC diagnostic pop

FUNC_DEF uint16_t sc_real_packet_header_calc_cksum(struct sc_real_packet_header_s *pkt_hdr)
{
    uint8_t *ptr;
    uint32_t sum;

    ptr = (uint8_t *)pkt_hdr;
    sum = 0;

    for (int32_t i = 0; i < 6; i++)
        sum += *ptr++;

    sum += 0x8000;

    return sum & 0xffff;
}

FUNC_DEF void sc_send_packet(const struct sc_packet_s *in, struct sc_packet_s *out)
{
    if (in->payload_size > 256)
    {
        // printf("payload too big!\n");
        dead();
    }

    // 0x24000000000
    uint64_t sb_base_addr = 0x24;
    sb_base_addr <<= 36;

    //
    uint64_t send_packet_counter_addr[2];

    // cell
    // 0x2400008DFF0
    send_packet_counter_addr[0] = sb_base_addr;
    send_packet_counter_addr[0] |= 0x8DFF0;

    // sc
    // 0x2400008CFF4
    send_packet_counter_addr[1] = sb_base_addr;
    send_packet_counter_addr[1] |= 0x8CFF4;

    // 0x2400008D000
    uint64_t send_packet_data_addr = sb_base_addr;
    send_packet_data_addr |= 0x8D000;

    // 0x2400008E100
    uint64_t send_packet_kick_addr = sb_base_addr;
    send_packet_kick_addr |= 0x8E100;

    //
    uint64_t recieve_packet_counter_addr[1];

    // sc
    // 0x2400008DFF4
    recieve_packet_counter_addr[0] = sb_base_addr;
    recieve_packet_counter_addr[0] |= 0x8DFF4;

    // 0x2400008C000
    uint64_t recieve_packet_data_addr = sb_base_addr;
    recieve_packet_data_addr |= 0x8C000;

    //

    volatile uint32_t *send_packet_counter_cell = (volatile uint32_t *)send_packet_counter_addr[0];

    volatile uint32_t *send_packet_data = (volatile uint32_t *)send_packet_data_addr;
    volatile uint32_t *send_packet_kick = (volatile uint32_t *)send_packet_kick_addr;

    //

    volatile uint32_t *recieve_packet_counter_sc = (volatile uint32_t *)recieve_packet_counter_addr[0];

    volatile uint32_t *recieve_packet_data = (volatile uint32_t *)recieve_packet_data_addr;

    //

    {
        uint16_t tag = 0x69;

#if 1

        {
            uint8_t buf[512];
            memset(buf, 0, 512);

            struct sc_real_packet_header_s in_real_pkt_hdr;

            in_real_pkt_hdr.service_id = in->service_id;
            in_real_pkt_hdr.version = 1;

            in_real_pkt_hdr.tag = tag;

            in_real_pkt_hdr.res[0] = 0;
            in_real_pkt_hdr.res[1] = 0;

            in_real_pkt_hdr.cksum = sc_real_packet_header_calc_cksum(&in_real_pkt_hdr);

            in_real_pkt_hdr.communication_tag = in->communication_tag;

            in_real_pkt_hdr.payload_size[0] = in->payload_size;
            in_real_pkt_hdr.payload_size[1] = in->payload_size;

            uint64_t curSize = 0;

            memcpy(&buf[curSize], &in_real_pkt_hdr, sizeof(struct sc_real_packet_header_s));
            curSize += sizeof(struct sc_real_packet_header_s);

            uint64_t payload_size = in_real_pkt_hdr.payload_size[0];

            memcpy(&buf[curSize], &in->data[0], payload_size);
            curSize += payload_size;

            // padding...

            uint64_t align = 4;
            uint64_t zz = (payload_size % align);

            if (zz != 0)
            {
                uint64_t y = (align - zz);

                memset(&buf[curSize], 0, y);
                curSize += y;
            }

            // cksum...

            uint32_t cksum = 0;

            for (int32_t i = 0; i < curSize; i++)
                cksum -= buf[i];

            cksum = cksum & 0xffff;

            memcpy(&buf[curSize], &cksum, sizeof(uint32_t));
            curSize += sizeof(uint32_t);

            {
                uint32_t *p = (uint32_t *)buf;

                for (uint32_t i = 0; i < (512 / 4); ++i)
                    send_packet_data[i] = p[i];
            }
        }

#else

        {
            tag = 0x1620;

            send_packet_data[0] = 0x16011620;
            send_packet_data[1] = 0x0000804d;
            send_packet_data[2] = 0x00000001;
            send_packet_data[3] = 0x00080008;
            send_packet_data[4] = 0x20290a00;
            send_packet_data[5] = 0x000001b6;
            send_packet_data[6] = 0x0000fdcb;
        }

#endif

        {
            uint32_t value = *send_packet_counter_cell;

            value = value + 1;
            value &= 0xffff;
            value = (value << 16) | value;

            *send_packet_counter_cell = value;
        }

        uint32_t old_recieve_packet_counter_sc = *recieve_packet_counter_sc;

        eieio();
        *send_packet_kick = 0x1;
        eieio();

        while (1)
        {
            if (*recieve_packet_counter_sc != old_recieve_packet_counter_sc)
                break;
        }

        struct sc_real_packet_header_s out_real_pkt_hdr;

        {
            uint32_t *p = (uint32_t *)&out_real_pkt_hdr;

            for (uint32_t i = 0; i < 4; i++)
                p[i] = recieve_packet_data[i];
        }

        if (out_real_pkt_hdr.tag != tag)
            dead();

        if (out != NULL)
        {
            out->service_id = out_real_pkt_hdr.service_id;
            out->communication_tag = out_real_pkt_hdr.communication_tag;

            out->payload_size = out_real_pkt_hdr.payload_size[0];

            {
                uint32_t *p = (uint32_t *)out->data;

                for (uint32_t i = 0; i < (256 / 4); ++i)
                    p[i] = recieve_packet_data[i + 4];
            }
        }
    }
}

FUNC_DEF void sc_triple_beep()
{
    struct sc_packet_s pkt;

    pkt.service_id = 0x16;
    pkt.communication_tag = 1;

    pkt.payload_size = 8;

    pkt.data[0] = 0x20;
    pkt.data[1] = 0x29;
    pkt.data[2] = 0x0a;
    pkt.data[3] = 0x00;

    pkt.data[4] = 0x00;
    pkt.data[5] = 0x00;

    pkt.data[6] = 0x01;
    pkt.data[7] = 0xb6;

    sc_send_packet(&pkt, NULL);
}

FUNC_DEF void sc_hard_restart()
{
    struct sc_packet_s pkt;

    pkt.service_id = 0x13;
    pkt.communication_tag = 1;

    pkt.payload_size = 4;

    pkt.data[0] = 0x11;
    pkt.data[1] = 0x00;
    pkt.data[2] = 0x00;
    pkt.data[3] = 0x02;

    sc_send_packet(&pkt, NULL);
}

FUNC_DEF void puts(const char *str)
{
#if LV1_ENABLED
    struct func_call_params_s params;
    params.addr = 0x2BA460; // lv1_puts
    params.args[0] = (uint64_t)str;

    func_call(&params);
#endif

    //


}

__attribute__((noinline, section(SECTION_NAME))) void stage_main()
{
    WaitInMs(4000);
    sc_triple_beep();

    WaitInMs(1000);
    sc_hard_restart();

    //uint64_t result = *((uint64_t *)0x100);

    //asm volatile("mr 3, %0" ::"r"(result) :);
}