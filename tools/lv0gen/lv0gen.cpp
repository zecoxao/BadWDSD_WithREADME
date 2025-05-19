#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/stat.h>

#define round_up(x, n) (-(-(x) & -(n)))

size_t get_file_size(FILE *f)
{
    size_t old_off = ftell(f);

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);

    fseek(f, old_off, SEEK_SET);
    return size;
}

uint16_t endswap16(uint16_t v)
{
    return __bswap_16(v);
}

uint32_t endswap32(uint32_t v)
{
    return __bswap_32(v);
}

uint64_t endswap64(uint64_t v)
{
    return __bswap_64(v);
}

struct Zelf_Header_s
{
public:
    uint64_t magic;

    uint64_t original_size;
    uint64_t compressed_size;
};

bool SearchAndReplace(void *in_data, uint64_t dataSize, const void *in_searchData, uint64_t searchDataSize, const void *in_replaceData, uint64_t replaceDataSize)
{
    uint8_t *data = (uint8_t *)in_data;

    const uint8_t *searchData = (const uint8_t *)in_searchData;
    const uint8_t *replaceData = (const uint8_t *)in_replaceData;

    for (uint64_t i = 0; i < dataSize; ++i)
    {
        if (!memcmp(&data[i], searchData, searchDataSize))
        {
            memcpy(&data[i], replaceData, replaceDataSize);
            return true;
        }
    }

    return false;
}

void lv0gen(const char *inFilePath, const char *outFilePath, const char *stage2jFilePath)
{
    printf("lv0gen()\n");

    printf("inFilePath = %s\n", inFilePath);
    printf("outFilePath = %s\n", outFilePath);

    printf("stage2jFilePath = %s\n", stage2jFilePath);

    FILE *inFile = fopen(inFilePath, "rb");
    FILE *outFile = fopen(outFilePath, "wb");
    FILE *stage2jFile = fopen(stage2jFilePath, "rb");

    if (inFile == NULL || outFile == NULL || stage2jFile == NULL)
    {
        printf("open file failed!\n");

        abort();
        return;
    }

    size_t inFileSize = get_file_size(inFile);
    printf("inFileSize = %lu\n", inFileSize);

    uint8_t *inData = (uint8_t *)malloc(inFileSize);

    if (inData == NULL)
    {
        printf("malloc failed!\n");

        abort();
        return;
    }

    fread(inData, 1, inFileSize, inFile);
    fclose(inFile);

    size_t stage2jFileSize = get_file_size(stage2jFile);
    printf("stage2jFileSize = %lu\n", stage2jFileSize);

    if (stage2jFileSize > 32)
    {
        printf("bad stage2j file size!\n");

        abort();
        return;
    }

    uint8_t *stage2jData = (uint8_t *)malloc(stage2jFileSize);

    if (stage2jData == NULL)
    {
        printf("stage2jData failed!\n");

        abort();
        return;
    }

    fread(stage2jData, 1, stage2jFileSize, stage2jFile);
    fclose(stage2jFile);

    uint8_t *outData = (uint8_t *)malloc(inFileSize);

    if (outData == NULL)
    {
        printf("malloc failed!\n");

        abort();
        return;
    }

    memcpy(outData, inData, inFileSize);

    {
        uint8_t searchData[] = {0x38, 0x60, 0x01, 0x00, 0x7C, 0x69, 0x03, 0xA6, 0x4E, 0x80, 0x04, 0x20, 0x60, 0x00, 0x00, 0x00};

        printf("Installing stage2j...\n");

        if (!SearchAndReplace(outData, inFileSize, searchData, 16, stage2jData, stage2jFileSize))
        {
            printf("install failed!\n");

            abort();
            return;
        }
    }

    fwrite(outData, 1, inFileSize, outFile);

    free(outData);

    free(stage2jData);
    free(inData);

    fclose(outFile);
}

struct SceHeader_s
{
    uint32_t magic;
    uint32_t version;
    uint16_t attribute;
    uint16_t category;
    uint32_t ext_header_size;
    uint64_t file_offset;
    uint64_t file_size;
};

struct SceMetaInfo_s
{
    uint64_t key[2];
    uint8_t key_pad[16];
    uint64_t iv[2];
    uint8_t iv_pad[16];
};

struct SceMetaHeader_s
{
    uint64_t sign_offset;
    uint32_t sign_algorithm; // 1 = ECDSA160, 2 = HMACSHA1, 3 = SHA1, 5 = RSA2048, 6 = HMACSHA256 (?not used?)
    uint32_t section_entry_num;
    uint32_t key_entry_num;
    uint32_t optional_header_size;
    uint64_t pad;
};

struct SceMetaSectionHeader_s
{
    uint64_t segment_offset;
    uint64_t segment_size;
    uint32_t segment_type;   // 1 = shdr, 2 = phdr, 3 = sceversion
    uint32_t segment_id;     // 0,1,2,3,etc for phdr, always 3 for shdrs, sceversion shdr number for sceversion
    uint32_t sign_algorithm; // 1 = ECDSA160 (not used), 2 = HMACSHA1, 3 = SHA1, 5 = RSA2048 (not used), 6 = HMACSHA256
    uint32_t sign_idx;
    uint32_t enc_algorithm;  // 1 = none, 2 = aes128cbccfb, 3 = aes128ctr
    uint32_t key_idx;        // -1 when enc_algorithm = none
    uint32_t iv_idx;         // -1 when enc_algorithm = none
    uint32_t comp_algorithm; // 1 = plain, 2 = zlib
};

struct SceMetaKey_s
{
    uint64_t key[2];
};

struct ElfHeader_s
{
    uint8_t e_ident[16];  /* ELF identification */
    uint16_t e_type;      /* object file type */
    uint16_t e_machine;   /* machine type */
    uint32_t e_version;   /* object file version */
    uint64_t e_entry;     /* entry point address */
    uint64_t e_phoff;     /* program header offset */
    uint64_t e_shoff;     /* section header offset */
    uint32_t e_flags;     /* processor-specific flags */
    uint16_t e_ehsize;    /* ELF header size */
    uint16_t e_phentsize; /* size of program header entry */
    uint16_t e_phnum;     /* number of program header entries */
    uint16_t e_shentsize; /* size of section header entry */
    uint16_t e_shnum;     /* number of section header entries */
    uint16_t e_shstrndx;  /* section name string table index */
} __attribute__((packed));

struct ElfPhdr_s
{
    uint32_t p_type;   /* Segment type */
    uint32_t p_flags;  /* Segment flags */
    uint64_t p_offset; /* Segment file offset */
    uint64_t p_vaddr;  /* Segment virtual address */
    uint64_t p_paddr;  /* Segment physical address */
    uint64_t p_filesz; /* Segment size in file */
    uint64_t p_memsz;  /* Segment size in memory */
    uint64_t p_align;  /* Segment alignment */
};

#include "../Aes/Aes.h"
#include "../Aes/Aes.c"

void DecryptLv0Self(void *inDest, const void *inSrc)
{
    printf("DecryptLv0Self()\n");

    uint8_t *dest = (uint8_t *)inDest;
    const uint8_t *src = (const uint8_t *)inSrc;

    uint64_t curSrcOffset = 0;

    struct SceHeader_s sceHeader;
    memcpy(&sceHeader, &src[curSrcOffset], sizeof(struct SceHeader_s));

    if (endswap32(sceHeader.magic) != 0x53434500)
    {
        printf("SCE magic check failed!, 0x%x\n", endswap32(sceHeader.magic));

        abort();
        return;
    }

    printf("sceHeader:\n");
    printf("ext_header_size = 0x%x\n", endswap32(sceHeader.ext_header_size));
    printf("file_offset = 0x%lx\n", endswap64(sceHeader.file_offset));
    printf("file_size = 0x%lx\n", endswap64(sceHeader.file_size));

    // erk=CA7A24EC38BDB45B 98CCD7D363EA2AF0 C326E65081E0630C B9AB2D215865878A
    // riv=F9205F46F6021697 E670F13DFA726212

    uint64_t meta_key[4];
    meta_key[0] = endswap64(0xCA7A24EC38BDB45B);
    meta_key[1] = endswap64(0x98CCD7D363EA2AF0);
    meta_key[2] = endswap64(0xC326E65081E0630C);
    meta_key[3] = endswap64(0xB9AB2D215865878A);

    uint64_t meta_iv[2];
    meta_iv[0] = endswap64(0xF9205F46F6021697);
    meta_iv[1] = endswap64(0xE670F13DFA726212);

    WORD meta_aes_key[60];
    aes_key_setup((const uint8_t *)meta_key, meta_aes_key, 256);

    curSrcOffset = 0x200;

    SceMetaInfo_s metaInfo;
    aes_decrypt_cbc(

        &src[curSrcOffset],
        sizeof(struct SceMetaInfo_s),

        (uint8_t *)&metaInfo,

        meta_aes_key,
        256,

        (const uint8_t *)meta_iv);

    curSrcOffset += sizeof(struct SceMetaInfo_s);

    printf("metaInfo:\n");
    printf("metaInfo.key[0] = 0x%lx\n", endswap64(metaInfo.key[0]));
    printf("metaInfo.key[1] = 0x%lx\n", endswap64(metaInfo.key[1]));

    printf("metaInfo.iv[0] = 0x%lx\n", endswap64(metaInfo.iv[0]));
    printf("metaInfo.iv[1] = 0x%lx\n", endswap64(metaInfo.iv[1]));

    WORD meta_header_key[60];
    aes_key_setup((const uint8_t *)metaInfo.key, meta_header_key, 128);

    uint64_t metasSize = endswap64(sceHeader.file_offset) - sizeof(struct SceHeader_s) + endswap32(sceHeader.ext_header_size) + sizeof(struct SceMetaInfo_s);
    printf("metasSize = %lu\n", metasSize);

    uint8_t metasBuf[16384];
    
    aes_decrypt_ctr(

        &src[curSrcOffset],
        metasSize,

        metasBuf,

        meta_header_key,
        128,

        (const uint8_t *)metaInfo.iv
    
    );

    SceMetaHeader_s* metaHeader = (SceMetaHeader_s*)&metasBuf[0];

    printf("metaHeader:\n");
    printf("section_entry_num = %u\n", endswap32(metaHeader->section_entry_num));
    printf("key_entry_num = %u\n", endswap32(metaHeader->key_entry_num));

    SceMetaSectionHeader_s* metaSectionHeaders = (SceMetaSectionHeader_s*)&metasBuf[sizeof(struct SceMetaHeader_s)];

    for (uint32_t i = 0; i < endswap32(metaHeader->section_entry_num); ++i)
    {
        SceMetaSectionHeader_s *h = &metaSectionHeaders[i];
    
        printf("section_headers[%u]:\n", i);
        printf("segment_id = %u, segment_offset = 0x%lx, segment_size = 0x%lx, enc_algorithm = %u, key_idx = %u, iv_idx = %u\n", 
            endswap32(h->segment_id), endswap64(h->segment_offset), endswap64(h->segment_size), endswap32(h->enc_algorithm), endswap32(h->key_idx), endswap32(h->iv_idx));
    }

    SceMetaKey_s* metaKeys = (SceMetaKey_s*)&metasBuf[sizeof(struct SceMetaHeader_s) + (endswap32(metaHeader->section_entry_num) * sizeof(struct SceMetaSectionHeader_s))];

    for (uint32_t i = 0; i < endswap32(metaHeader->key_entry_num); ++i)
    {
        SceMetaKey_s *k = &metaKeys[i];
    
        printf("keys[%u]: ", i);

        printf("key[0] = 0x%lx", endswap64(k->key[0]));
        printf(", key[1] = 0x%lx\n", endswap64(k->key[1]));
    }

    ElfHeader_s* elfHeader = (ElfHeader_s*)&src[0x90];

    memcpy(dest, elfHeader, sizeof(ElfHeader_s));
    memcpy(dest + endswap64(elfHeader->e_phoff), &src[0x90 + endswap64(elfHeader->e_phoff)], endswap16(elfHeader->e_phentsize) * endswap16(elfHeader->e_phnum));

    ElfPhdr_s* elfPhdrs = (ElfPhdr_s*)(dest + endswap64(elfHeader->e_phoff));

    for (uint16_t i = 0; i < endswap16(elfHeader->e_phnum); ++i)
    {
        ElfPhdr_s* phdr = &elfPhdrs[i];

        printf("decrypting phdr %u...\n", (uint32_t)i);

        SceMetaSectionHeader_s *h = &metaSectionHeaders[i];

        SceMetaKey_s* key = &metaKeys[endswap32(h->key_idx)];
        SceMetaKey_s* iv = &metaKeys[endswap32(h->iv_idx)];

        WORD aes_key[60];
        aes_key_setup((const uint8_t *)key->key, aes_key, 128);

        aes_decrypt_ctr(

            &src[endswap64(h->segment_offset)],
            endswap64(h->segment_size),
    
            dest + endswap64(phdr->p_offset),
    
            aes_key,
            128,
    
            (const uint8_t *)iv->key
        
        );
    }
}

void lv0decrypt(const char *inFilePath, const char *outFilePath)
{
    printf("lv0decrypt()\n");

    printf("inFilePath = %s\n", inFilePath);
    printf("outFilePath = %s\n", outFilePath);

    FILE *inFile = fopen(inFilePath, "rb");
    FILE *outFile = fopen(outFilePath, "wb");

    if (inFile == NULL || outFile == NULL)
    {
        printf("open file failed!\n");

        abort();
        return;
    }

    size_t inFileSize = get_file_size(inFile);
    printf("inFileSize = %lu\n", inFileSize);

    uint8_t *inData = (uint8_t *)malloc(inFileSize);

    if (inData == NULL)
    {
        printf("malloc failed!\n");

        abort();
        return;
    }

    fread(inData, 1, inFileSize, inFile);
    fclose(inFile);

    size_t outFileSize = (inFileSize * 2);
    uint8_t *outData = (uint8_t *)malloc(outFileSize);

    if (outData == NULL)
    {
        printf("malloc failed!\n");

        abort();
        return;
    }

    DecryptLv0Self(outData, inData);

    fwrite(outData, 1, outFileSize, outFile);
    fclose(outFile);

    free(outData);
    free(inData);
}

int main(int argc, char **argv)
{
    if (argc == 5 && !strcmp(argv[1], "lv0gen"))
        lv0gen(argv[2], argv[3], argv[4]);
    else if (argc == 4 && !strcmp(argv[1], "lv0decrypt"))
        lv0decrypt(argv[2], argv[3]);
    else
    {
        printf("lv0gen <inFile> <outFile> <stage2j>\n");
        printf("lv0decrypt <inFile> <outFile>\n");
    }

    return 0;
}